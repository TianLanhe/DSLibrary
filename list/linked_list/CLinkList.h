#ifndef CIRCULAR_SINGLE_LINK_LIST_H
#define CIRCULAR_SINGLE_LINK_LIST_H

#include "SLinkList.h"

DSLIB_BEGIN

template < typename T, typename Alloc = DefaultMemoryManager<SNode<T>> >
class CLinkList : public SLinkList<T, Alloc> {

public:
	CLinkList() { m_head->next = m_head; }
	CLinkList(const CLinkList<T, Alloc>& obj);
	CLinkList<T, Alloc>& operator=(CLinkList<T, Alloc>);
	~CLinkList();

	virtual size_type remove(size_type);
	virtual void clear();

	virtual void reverse();

	virtual void resize(size_type, const_reference val = T());
	virtual void swap(CLinkList<T, Alloc>& obj) { return SLinkList<T, Alloc>::swap(obj); }

	virtual void next();

};

template < typename T, typename Alloc > inline
void swap(CLinkList<T, Alloc>& a, CLinkList<T, Alloc> b) {
	a.swap(b);
}

template < typename T, typename Alloc >
CLinkList<T, Alloc>::CLinkList(const CLinkList<T, Alloc>& obj) {
	m_head->next = m_head;

	SNode<T> *head = m_head;
	for (SNode<T> *ptrH = obj.m_head->next; ptrH != obj.m_head; ptrH = ptrH->next) {
		head->next = m_alloc.create(ptrH->val,m_head);
		CHECK_NO_MEMORY_EXCEPTION(head->next);

		head = head->next;

		++m_len;
	}
}

template < typename T, typename Alloc >
CLinkList<T, Alloc>& CLinkList<T, Alloc>::operator=(CLinkList<T, Alloc> obj) {
	swap(obj);

	return *this;
}

template < typename T, typename Alloc >
CLinkList<T, Alloc>::~CLinkList() {
	clear();
							// clear 后只剩头结点，此时 next 置空变成单链表交由单链表析构函数销毁
	m_head->next = nullptr;	// Review : 感觉这一步怪怪的
}

template < typename T, typename Alloc >
typename CLinkList<T, Alloc>::size_type CLinkList<T, Alloc>::remove(size_type i) {
	size_type ret = SLinkList<T, Alloc>::remove(i);

	if (m_cur == m_head) {
		if (m_len == 0)
			m_cur = nullptr;
		else
			m_cur = m_cur->next;
	}

	return ret;
}

template < typename T, typename Alloc >
void CLinkList<T, Alloc>::clear() {
	while (m_head->next != m_head) {
		SNode<T> *temp = m_head->next;
		m_head->next = temp->next;

		// 为了异常安全，不能在 while 外 m_len = 0 
		--m_len;

		m_alloc.destroy(temp);
	}

	CHECK_STATE_EXCEPTION(m_len == 0);

	// 修改游标相关数据
	m_cur = nullptr;
	m_step = 0;
}

template < typename T, typename Alloc >
void CLinkList<T, Alloc>::reverse() {
	SNode<T> *cur = m_head->next;
	SNode<T> *pre = m_head;

	SNode<T> *temp;
	while (cur != m_head) {
		temp = cur->next;
		cur->next = pre;
		pre = cur;
		cur = temp;
	}

	m_head->next = pre;
}

template < typename T, typename Alloc >
void CLinkList<T, Alloc>::resize(size_type n, const_reference val) {
	if (n < m_len) {
		SNode<T> *tail = (n == 0 ? m_head : locate(n - 1));
		SNode<T> *node;
		while (tail->next != m_head) {
			node = tail->next;
			tail->next = node->next;

			--m_len;

			// 可能会改变游标
			if (m_cur == node) {
				m_cur = nullptr;
				m_step = 0;
			}

			m_alloc.destroy(node);
		}
	}
	else {
		SLinkList<T, Alloc>::resize(n, val);
	}
}

template < typename T, typename Alloc>
void CLinkList<T, Alloc>::next() {
	if (end())
		return;

	for (size_type i = 0; i < m_step; ++i) {
		m_cur = m_cur->next;
		if (m_cur == m_head)
			m_cur = m_cur->next;
	}
}

DSLIB_END

#endif // !CIRCULAR_SINGLE_LINK_LIST_H
