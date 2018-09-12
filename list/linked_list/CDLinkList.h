#ifndef CIRCULAR_DOUBLY_LINKED_LIST_H
#define CIRCULAR_DOUBLY_LINKED_LIST_H

#include "DLinkList.h"

DSLIB_BEGIN

template < typename T, typename Alloc = DefaultMemoryManager<DNode<T>> >
class CDLinkList : public DLinkList<T, Alloc> {

public:
	CDLinkList();
	CDLinkList(const CDLinkList<T, Alloc>& obj);
	CDLinkList<T, Alloc>& operator=(CDLinkList<T, Alloc>);
	~CDLinkList();

	virtual size_type remove(size_type);
	virtual void clear();

	virtual void reverse();

	virtual void resize(size_type, const_reference val = T());
	virtual void swap(CDLinkList<T, Alloc>& obj) { return DLinkList<T, Alloc>::swap(obj); }

	virtual void next();
	virtual void prev();

protected:
	DNode<T>* locate(size_type) const;	// ����λ��ѡ��Ͻ����ƶ���ʽ���� m_len / 2 ���������ƶ������������ƶ�

};

template < typename T, typename Alloc > inline
void swap(CDLinkList<T, Alloc>& a, CDLinkList<T, Alloc> b) {
	a.swap(b);
}

template < typename T, typename Alloc >
CDLinkList<T, Alloc>::CDLinkList() {
	m_head->next = m_head;
	m_head->pre = m_head;
}

template < typename T, typename Alloc >
CDLinkList<T, Alloc>::CDLinkList(const CDLinkList<T, Alloc>& obj) {
	m_head->next = m_head;
	m_head->pre = m_head;

	DNode<T> *head = m_head;
	for (DNode<T> *ptrH = obj.m_head->next; ptrH != obj.m_head; ptrH = ptrH->next) {
		head->next = m_alloc.create(ptrH->val, head, m_head);
		CHECK_NO_MEMORY_EXCEPTION(head->next);

		head = head->next;

		++m_len;
	}
	m_head->pre = head;
}

template < typename T, typename Alloc >
CDLinkList<T, Alloc>& CDLinkList<T, Alloc>::operator=(CDLinkList<T, Alloc> obj) {
	swap(obj);

	return *this;
}

template < typename T, typename Alloc >
CDLinkList<T, Alloc>::~CDLinkList() {
	clear();
	// clear ��ֻʣͷ��㣬��ʱ next �� pre �ÿձ��˫������˫����������������
	m_head->next = nullptr;	// Review : �о���һ���ֵֹ�
	m_head->pre = nullptr;
}

template < typename T, typename Alloc >
typename CDLinkList<T, Alloc>::size_type CDLinkList<T, Alloc>::remove(size_type i) {
	size_type ret = DLinkList<T, Alloc>::remove(i);

	// �� CLinkList ��ͬ��˫������ m_cur ָ��ͷ���Ҳ����Ч��
	if (m_cur == m_head)
		m_cur = m_cur->next;

	return ret;
}

template < typename T, typename Alloc >
void CDLinkList<T, Alloc>::clear() {
	while (m_head->next != m_head) {
		DNode<T> *temp = m_head->next;

		m_head->next = temp->next;
		if (temp->next)
			temp->next->pre = m_head;

		// Ϊ���쳣��ȫ�������� while �� m_len = 0 
		--m_len;

		m_alloc.destroy(temp);
	}

	CHECK_STATE_EXCEPTION(m_len == 0);

	// �޸��α��������
	m_cur = nullptr;
	m_step = 0;
}

template < typename T, typename Alloc >
void CDLinkList<T, Alloc>::reverse() {
	DNode<T> *cur = m_head->next;
	DNode<T> *pre = m_head;

	DNode<T> *next;
	while (cur != m_head) {
		next = cur->next;

		cur->pre = cur->next;
		cur->next = pre;

		pre = cur;
		cur = next;
	}
	m_head->pre = m_head->next;
	m_head->next = pre;
}

template < typename T, typename Alloc >
void CDLinkList<T, Alloc>::resize(size_type n, const_reference val) {
	if (n < m_len) {
		DNode<T> *tail = (n == 0 ? m_head : locate(n - 1));
		DNode<T> *node;
		while (tail->next != m_head) {
			node = tail->next;

			tail->next = node->next;
			if (node->next)
				node->next->pre = tail;

			--m_len;

			// ���ܻ�ı��α�
			if (m_cur == node) {
				m_cur = nullptr;
				m_step = 0;
			}

			m_alloc.destroy(node);
		}
	}
	else {
		DLinkList<T, Alloc>::resize(n, val);
	}
}

template < typename T, typename Alloc>
void CDLinkList<T, Alloc>::next() {
	if (end())
		return;

	for (size_type i = 0; i < m_step; ++i) {
		m_cur = m_cur->next;
		if (m_cur == m_head)
			m_cur = m_cur->next;
	}
}

template < typename T, typename Alloc >
void CDLinkList<T, Alloc>::prev() {
	if (end())
		return;

	for (size_type i = 0; i < m_step; ++i) {
		m_cur = m_cur->pre;
		if (m_cur == m_head)
			m_cur = m_cur->pre;
	}
}

template < typename T, typename Alloc >
DNode<T>* CDLinkList<T, Alloc>::locate(size_type pos) const {
	if (pos <= (m_len - 1) / 2) {
		return DLinkList<T, Alloc>::locate(pos);
	}
	else {
		size_type newPos = m_len - pos;
		DNode<T> *head = m_head;

		for (size_type i = 0; i < newPos; ++i)
			head = head->pre;

		return head;
	}
}

DSLIB_END

#endif // !CIRCULAR_DOUBLY_LINKED_LIST_H
