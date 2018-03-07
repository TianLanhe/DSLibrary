#ifndef SINGLE_LINK_LIST_H
#define SINGLE_LINK_LIST_H

#include "LinkList.h"
#include "../../MemoryManager.h"
#include "../../Utility.h"

DSLIB_BEGIN

// ������ڵ����� SNode
template < typename T >
struct SNode {

	SNode() :next(nullptr) { }
	SNode(SNode<T>* const n) :next(n) { }
	SNode(const T& v, SNode<T>* const n = nullptr) :next(n), val(v) { }

	T val;
	SNode<T> *next;
};

template < typename T, typename Alloc = DefaultMemoryManager<SNode<T>> >	// ʹ��Ĭ���ڴ����/���շ�ʽ
class SLinkList : public LinkList<T> {

public:
	SLinkList();
	SLinkList(const SLinkList<T, Alloc>&);
	SLinkList<T, Alloc>& operator=(SLinkList<T, Alloc>);	// ��ֵ������ʹ���� copy-and-swap ������http://blog.csdn.net/xiajun07061225/article/details/7926722#comments
	~SLinkList();

	virtual size_type insert(size_type, const_reference);
	virtual size_type remove(size_type);
	virtual void clear();

	virtual void reverse();

	virtual void resize(size_type, const_reference val = T());

	virtual void swap(SLinkList<T, Alloc>& obj);

	virtual reference get(size_type i);
	virtual const_reference get(size_type i) const;
	virtual void set(size_type i, const_reference val);

	virtual void move(size_type, size_type step = 1);
	virtual void next();
	virtual bool end() const { return m_cur == nullptr; }
	virtual reference current();
	virtual const_reference current() const;

	virtual size_type size() const { return m_len; }

	template < typename Pred >
	size_type find(const T&, Pred) const;
	size_type find(const T& e) const { return find(e, equal()); }

protected:
	SNode<T>* locate(size_type) const;

	SNode<T>* __reverse(SNode<T>*);		// �ݹ�ʵ������װ�õĺ���

	SNode<T> *m_head;
	size_type m_len;

	Alloc m_alloc;

	SNode<T> *m_cur;
	size_type m_step;
};

template < typename T, typename Alloc > inline
void swap(SLinkList<T, Alloc>& a, SLinkList<T, Alloc>& b) {
	a.swap(b);
}

template < typename T, typename Alloc >
SLinkList<T, Alloc>::SLinkList() :m_len(0), m_cur(nullptr), m_step(0) {
	m_head = m_alloc.allocate();
	// m_head = (SNode<T>*)(::operator new(sizeof(SNode<T>)));

	CHECK_NO_MEMORY_EXCEPTION(m_head);

	m_head->next = nullptr;
}

template < typename T, typename Alloc >
SLinkList<T, Alloc>::SLinkList(const SLinkList<T, Alloc>& obj) :m_len(0), m_cur(nullptr), m_step(0) {
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
void SLinkList<T, Alloc>::swap(SLinkList<T, Alloc>& b) {
	SNode<T> *temp = b.m_head;
	b.m_head = m_head;
	m_head = temp;

	temp = m_cur;
	m_cur = b.m_cur;
	b.m_cur = temp;

	size_type tmp = m_len;
	m_len = b.m_len;
	b.m_len = tmp;

	tmp = m_step;
	m_step = b.m_step;
	b.m_step = tmp;
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

	// �޸��α�������ݣ�Ϊ���쳣��ȫ����delete�������
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

		// Ϊ���쳣��ȫ�������� while �� m_len = 0 
		--m_len;

		m_alloc.destroy(temp);
		// delete temp;
	}

	CHECK_STATE_EXCEPTION(m_len == 0);

	// �޸��α��������
	m_cur = nullptr;
	m_step = 0;
}

template < typename T, typename Alloc >
typename SLinkList<T, Alloc>::const_reference SLinkList<T, Alloc>::get(size_type i) const {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return locate(i)->val;
}

template < typename T, typename Alloc >
typename SLinkList<T, Alloc>::reference SLinkList<T, Alloc>::get(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return locate(i)->val;
}

template < typename T, typename Alloc >
void SLinkList<T, Alloc>::set(size_type i, const_reference val) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	locate(i)->val = val;
}

template < typename T, typename Alloc >
void SLinkList<T, Alloc>::move(size_type pos, size_type step) {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);	// ���� move �������ڵ� m_len λ�ã���ʾ�ƶ���ĩβ
	CHECK_PARAMETER_EXCEPTION(step != 0);

	if (pos == m_len) {
		m_cur = nullptr;
		m_step = 0;
	}
	else {
		m_cur = locate(pos);
		m_step = step;
	}
}

template < typename T, typename Alloc >
void SLinkList<T, Alloc>::next() {
	if (end())
		return;

	for (size_type i = 0; i < m_step && m_cur; ++i)
		m_cur = m_cur->next;
}

template < typename T, typename Alloc >
typename SLinkList<T, Alloc>::const_reference SLinkList<T, Alloc>::current() const {
	CHECK_OPERATION_EXCEPTION(m_cur);

	return m_cur->val;
}

template < typename T, typename Alloc >
typename SLinkList<T, Alloc>::reference SLinkList<T, Alloc>::current() {
	CHECK_OPERATION_EXCEPTION(m_cur);

	return m_cur->val;
}

template < typename T, typename Alloc >
SNode<T>* SLinkList<T, Alloc>::__reverse(SNode<T>* node) {
	if (node == nullptr || node->next == nullptr)
		return node;
	else {
		SNode<T>* ret;
		SNode<T>* next = node->next;
		ret = __reverse(node->next);	// ��벿��ת�ã�������ת�ò��ֵĵ�һ����㣬�������ͷ�����ýڵ�����
		next->next = node;				// next ��Ϊ��벿���Ѿ�ת�õ���������һ��
		node->next = nullptr;			// nextָ��ǰ�벿�֣�ǰ�벿��ָ���
		return ret;
	}
}

template < typename T, typename Alloc >
void SLinkList<T, Alloc>::reverse() {
	// ����ת��ѭ��ʵ��
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

	/*	����ת�õݹ�ʵ��
	SNode<T>* ret = __reverse(m_head->next);
	m_head->next = ret; */
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

			// ���ܻ�ı��α�
			if (m_cur == node) {
				m_cur = nullptr;
				m_step = 0;
			}

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

template < typename T, typename Alloc >
template < typename Pred >
typename SLinkList<T, Alloc>::size_type SLinkList<T, Alloc>::find(const T& e, Pred pred) const {
	size_type ret = -1;
	size_type i = 0;
	for (SNode<T>* head = m_head->next; head && i < m_len; head = head->next, ++i) {
		if (pred(head->val, e)) {
			ret = i;
			break;
		}
	}
	return ret;
}

DSLIB_END

#endif // !SINGLE_LINK_LIST_H
