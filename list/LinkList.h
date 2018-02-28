#ifndef LINK_LIST_H
#define LINK_LIST_H

#include "List.h"

DSLIB_BEGIN

// 单链表节点类型 SNode
template < typename T >
struct SNode {

	SNode() :next(nullptr) { }
	SNode(SNode<T>* const n) :next(n) { }
	SNode(const T& v, SNode<T>* const n = nullptr) :next(n), val(v) { }

	T val;
	SNode<T> *next;
};

template < typename T >
class LinkList : public List<T> {

public:
	LinkList() :m_len(0), m_cur(nullptr), m_step(0) { }

	virtual size_type insert(size_type, const_reference) = 0;
	virtual size_type remove(size_type) = 0;
	virtual void clear() = 0;

	virtual void reverse() = 0;

	virtual void resize(size_type, const_reference val = T()) = 0;

	virtual reference get(size_type i);
	virtual const_reference get(size_type i) const;
	virtual void set(size_type i, const_reference val);

	virtual void move(size_type, size_type step = 1);
	virtual void next();
	virtual bool end() const { return m_cur == nullptr; }
	virtual reference current();
	virtual const_reference current() const;

	virtual size_type size() const { return m_len; }

	virtual void swap(LinkList<T>&);

protected:
	virtual SNode<T>* locate(size_type) const = 0;

protected:
	size_type m_len;

	SNode<T>* m_cur;
	size_type m_step;
};

template < typename T >
typename LinkList<T>::const_reference LinkList<T>::get(size_type i) const {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return locate(i)->val;
}

template < typename T >
typename LinkList<T>::reference LinkList<T>::get(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return locate(i)->val;
}

template < typename T >
void LinkList<T>::set(size_type i, const_reference val) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	locate(i)->val = val;
}

template < typename T >
void LinkList<T>::move(size_type pos, size_type step) {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);	// 允许 move 到不存在的 m_len 位置，表示移动到末尾
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

template < typename T >
void LinkList<T>::next() {
	if (end())
		return;

	for (size_type i = 0; i < m_step && m_cur; ++i)
		m_cur = m_cur->next;
}

template < typename T >
typename LinkList<T>::const_reference LinkList<T>::current() const {
	if (m_cur == nullptr)
		THROW_EXCEPTION(InvalidOperationException, "No value at current position!");

	return m_cur->val;
}

template < typename T >
typename LinkList<T>::reference LinkList<T>::current() {
	if (m_cur == nullptr)
		THROW_EXCEPTION(InvalidOperationException, "No value at current position!");

	return m_cur->val;
}

template < typename T >
void LinkList<T>::swap(LinkList<T>& b) {
	SNode<T> *temp = m_cur;
	m_cur = b.m_cur;
	b.m_cur = temp;

	LinkList<T>::size_type tmp = m_len;
	m_len = b.m_len;
	b.m_len = tmp;

	tmp = m_step;
	m_step = b.m_step;
	b.m_step = tmp;
}

DSLIB_END

#endif // !LINK_LIST_H
