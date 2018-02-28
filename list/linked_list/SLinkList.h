#ifndef SINGLE_LINK_LIST_H
#define SINGLE_LINK_LIST_H

#include "LinkList.h"
#include "../MemoryManager.h"

DSLIB_BEGIN

template < typename T, typename Alloc = DefaultMemoryManager<SNode<T>> >	// 使用默认内存分配/回收方式
class SLinkList : public LinkList<T> {

public:
	SLinkList();
	SLinkList(const SLinkList<T, Alloc>&);
	SLinkList<T, Alloc>& operator=(SLinkList<T, Alloc>);	// 赋值操作符使用了 copy-and-swap 技术，http://blog.csdn.net/xiajun07061225/article/details/7926722#comments
	~SLinkList();

	virtual size_type insert(size_type, const_reference);
	virtual size_type remove(size_type);
	virtual void clear();

	virtual void reverse();

	virtual void swap(SLinkList<T, Alloc>& obj);

	virtual void resize(size_type, const_reference val = T());

protected:
	SNode<T>* locate(size_type) const;

	SNode<T> *m_head;
	Alloc m_alloc;
};

template < typename T, typename Alloc > inline
void swap(SLinkList<T, Alloc>& a, SLinkList<T, Alloc>& b) {
	a.swap(b);
}

template < typename T, typename Alloc >
SLinkList<T, Alloc>::SLinkList() {
	m_head = m_alloc.allocate();
	// m_head = (SNode<T>*)(::operator new(sizeof(SNode<T>)));

	CHECK_NO_MEMORY_EXCEPTION(m_head);

	m_head->next = nullptr;
}

template < typename T, typename Alloc >
SLinkList<T, Alloc>::SLinkList(const SLinkList<T, Alloc>& obj) {
	m_head = m_alloc.allocate();
	// m_head = (SNode<T>*)(::operator new(sizeof(SNode<T>)));
	CHECK_NO_MEMORY_EXCEPTION(m_head);
	m_head->next = nullptr;

	SNode<T> *head = m_head;
	for (SNode<T> *ptrH = obj.m_head->next; ptrH; ptrH = ptrH->next) {
		head->next = m_alloc.create(ptrH->val);
		// head->next = new SNode<T>(ptrH->val);
		CHECK_NO_MEMORY_EXCEPTION(head->next);

		head = head->next;

		++m_len;
	}
}

template < typename T, typename Alloc >
SLinkList<T, Alloc>& SLinkList<T, Alloc>::operator=(SLinkList<T, Alloc> obj) {
	swap(obj);

	return *this;
}

template < typename T, typename Alloc >
SLinkList<T, Alloc>::~SLinkList() {
	clear();

	m_alloc.deallocate(m_head);
	// ::operator delete(m_head);
}

template < typename T, typename Alloc >
void SLinkList<T, Alloc>::swap(SLinkList<T, Alloc>& obj) {
	SNode<T> *temp = obj.m_head;
	obj.m_head = m_head;
	m_head = temp;

	LinkList<T>::swap(obj);
}

template < typename T, typename Alloc >
SNode<T>* SLinkList<T, Alloc>::locate(size_type pos) const {
	SNode<T> *head = m_head->next;

	for (size_type i = 0; i < pos; ++i)
		head = head->next;

	return head;
}

template < typename T, typename Alloc >
typename SLinkList<T, Alloc>::size_type SLinkList<T, Alloc>::insert(size_type i, const_reference val) {
	CHECK_INDEX_OUT_OF_BOUNDS(i <= m_len);

	SNode<T> *pre = (i == 0 ? m_head : locate(i - 1));

	SNode<T> *newNode = m_alloc.create(val, pre->next);
	// SNode<T> *newNode = new SNode<T>(val, pre->next);
	CHECK_NO_MEMORY_EXCEPTION(newNode);

	pre->next = newNode;

	++m_len;

	return i;
}

template < typename T, typename Alloc >
typename SLinkList<T, Alloc>::size_type SLinkList<T, Alloc>::remove(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	SNode<T> *pre = (i == 0 ? m_head : locate(i - 1));

	SNode<T> *del = pre->next;
	pre->next = del->next;

	--m_len;

	// 修改游标相关数据，为了异常安全，把delete放在最后
	if (m_cur == del) {
		m_cur = del->next;
	}

	m_alloc.destroy(del);
	// delete del;

	return i;
}

template < typename T, typename Alloc >
void SLinkList<T, Alloc>::clear() {
	while (m_head->next) {
		SNode<T> *temp = m_head->next;
		m_head->next = temp->next;

		// 为了异常安全，不能在 while 外 m_len = 0 
		--m_len;

		m_alloc.destroy(temp);
		// delete temp;
	}

	CHECK_STATE_EXCEPTION(m_len == 0);

	// 修改游标相关数据
	m_cur = nullptr;
	m_step = 0;
}

template < typename T, typename Alloc >
void SLinkList<T, Alloc>::reverse() {
	SNode<T> *cur = m_head->next;
	SNode<T> *pre = nullptr;

	SNode<T> *temp;
	while (cur) {
		temp = cur->next;
		cur->next = pre;
		pre = cur;
		cur = temp;
	}

	m_head->next = pre;
}

template < typename T, typename Alloc >
void SLinkList<T, Alloc>::resize(size_type n, const_reference val) {
	if (n < m_len) {
		SNode<T> *tail = (n == 0 ? m_head : locate(n - 1));
		SNode<T> *node;
		while (tail->next) {
			node = tail->next;
			tail->next = node->next;

			--m_len;

			m_alloc.destroy(node);
			// delete node;
		}
	}
	else if (n > m_len) {
		SNode<T> *node;
		SNode<T> *tail = (m_len == 0 ? m_head : locate(m_len - 1));
		while (m_len < n) {
			node = m_alloc.create(val);
			// node = new SNode<T>(val);
			CHECK_NO_MEMORY_EXCEPTION(node);

			node->next = tail->next;
			tail->next = node;
			tail = node;

			++m_len;
		}
	}
}

DSLIB_END

#endif // !SINGLE_LINK_LIST_H
