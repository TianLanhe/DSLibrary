#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include "LinkList.h"
#include "../../MemoryManager.h"

DSLIB_BEGIN

// 双向链表节点类型 DNode
template < typename T >
struct DNode {

	DNode() :next(nullptr), pre(nullptr) { }
	DNode(DNode<T>* const p, DNode<T>* const n) :next(n), pre(p) { }
	DNode(const T& v, DNode<T>* const p = nullptr, DNode<T>* const n = nullptr) :next(n), val(v), pre(p) { }

	T val;
	DNode<T> *next;
	DNode<T> *pre;
};

template < typename T, typename Alloc = DefaultMemoryManager<DNode<T>> >
class DLinkList : public LinkList<T> {

public:
	DLinkList();
	DLinkList(const DLinkList<T, Alloc>&);
	DLinkList<T, Alloc>& operator=(DLinkList<T, Alloc>);	// 赋值操作符使用了 copy-and-swap 技术，http://blog.csdn.net/xiajun07061225/article/details/7926722#comments
	~DLinkList();

	virtual size_type insert(size_type, const_reference);
	virtual size_type remove(size_type);
	virtual void clear();

	virtual void reverse();

	virtual void resize(size_type, const_reference val = T());

	virtual void swap(DLinkList<T, Alloc>& obj);

	virtual reference get(size_type i);
	virtual const_reference get(size_type i) const;
	virtual void set(size_type i, const_reference val);

	virtual void move(size_type, size_type step = 1);
	virtual void next();
	virtual void prev();
	virtual bool end() const { return m_cur == nullptr || m_cur == m_head; }
	virtual reference current();
	virtual const_reference current() const;

	virtual size_type size() const { return m_len; }

protected:
	virtual DNode<T>* locate(size_type) const;

	DNode<T> *m_head;
	size_type m_len;

	Alloc m_alloc;

	DNode<T> *m_cur;
	size_type m_step;
};

template < typename T, typename Alloc > inline
void swap(DLinkList<T, Alloc>& a, DLinkList<T, Alloc>& b) {
	a.swap(b);
}

template < typename T, typename Alloc >
DLinkList<T, Alloc>::DLinkList() :m_len(0), m_cur(nullptr), m_step(0) {
	m_head = m_alloc.allocate();
	// m_head = (SNode<T>*)(::operator new(sizeof(SNode<T>)));

	CHECK_NO_MEMORY_EXCEPTION(m_head);

	m_head->next = nullptr;
	m_head->pre = nullptr;
}

template < typename T, typename Alloc >
DLinkList<T, Alloc>::DLinkList(const DLinkList<T, Alloc>& obj) :m_len(0), m_cur(nullptr), m_step(0) {
	m_head = m_alloc.allocate();
	// m_head = (SNode<T>*)(::operator new(sizeof(SNode<T>)));
	CHECK_NO_MEMORY_EXCEPTION(m_head);
	m_head->next = nullptr;
	m_head->pre = nullptr;

	DNode<T> *head = m_head;
	for (DNode<T> *ptrH = obj.m_head->next; ptrH; ptrH = ptrH->next) {
		head->next = m_alloc.create(ptrH->val, head);
		CHECK_NO_MEMORY_EXCEPTION(head->next);

		head = head->next;

		++m_len;
	}
}

template < typename T, typename Alloc >
DLinkList<T, Alloc>& DLinkList<T, Alloc>::operator=(DLinkList<T, Alloc> obj) {
	swap(obj);

	return *this;
}

template < typename T, typename Alloc >
DLinkList<T, Alloc>::~DLinkList() {
	clear();

	m_alloc.deallocate(m_head);
}

template < typename T, typename Alloc >
DNode<T>* DLinkList<T, Alloc>::locate(size_type pos) const {
	DNode<T> *head = m_head->next;

	for (size_type i = 0; i < pos; ++i)
		head = head->next;

	return head;
}

template < typename T, typename Alloc >
void DLinkList<T, Alloc>::swap(DLinkList<T, Alloc>& b) {
	DNode<T> *temp = b.m_head;
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
typename DLinkList<T, Alloc>::size_type DLinkList<T, Alloc>::insert(size_type i, const_reference val) {
	CHECK_INDEX_OUT_OF_BOUNDS(i <= m_len);

	DNode<T> *pre = (i == 0 ? m_head : locate(i - 1));

	DNode<T> *newNode = m_alloc.create(val, pre, pre->next);
	CHECK_NO_MEMORY_EXCEPTION(newNode);

	if (pre->next)
		pre->next->pre = newNode;

	pre->next = newNode;


	++m_len;

	return i;
}

template < typename T, typename Alloc >
typename DLinkList<T, Alloc>::size_type DLinkList<T, Alloc>::remove(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	DNode<T> *pre = (i == 0 ? m_head : locate(i - 1));
	DNode<T> *del = pre->next;
	DNode<T> *next = del->next;

	pre->next = del->next;

	if (next)
		next->pre = del->pre;

	--m_len;

	// 修改游标相关数据，为了异常安全，把delete放在最后
	if (m_cur == del) {
		m_cur = del->next;
	}

	m_alloc.destroy(del);

	return i;
}

template < typename T, typename Alloc >
void DLinkList<T, Alloc>::clear() {
	while (m_head->next) {
		DNode<T> *temp = m_head->next;
		m_head->next = temp->next;

		// 为了异常安全，不能在 while 外 m_len = 0 ,且每删除一个要把两个指针都修改好
		// 不能为了图方便只顾修改 next 而让 pre 悬空
		if (temp->next)
			temp->next->pre = m_head;

		--m_len;

		m_alloc.destroy(temp);
	}

	CHECK_STATE_EXCEPTION(m_len == 0);

	// 修改游标相关数据
	m_cur = nullptr;
	m_step = 0;
}

template < typename T, typename Alloc >
void DLinkList<T, Alloc>::reverse() {
	DNode<T> *cur = m_head->next;
	DNode<T> *pre = nullptr;

	DNode<T> *temp;
	while (cur) {
		temp = cur->next;

		cur->pre = cur->next;
		cur->next = pre;

		pre = cur;
		cur = temp;
	}

	m_head->next = pre;
	pre->pre = m_head;
}

template < typename T, typename Alloc >
void DLinkList<T, Alloc>::resize(size_type n, const_reference val) {
	if (n < m_len) {
		DNode<T> *tail = (n == 0 ? m_head : locate(n - 1));
		DNode<T> *node;
		while (tail->next) {
			node = tail->next;

			tail->next = node->next;
			if (node->next)
				node->next->pre = tail;

			--m_len;

			// 可能会改变游标
			if (m_cur == node) {
				m_cur = nullptr;
				m_step = 0;
			}

			m_alloc.destroy(node);
		}
	}
	else if (n > m_len) {
		DNode<T> *node;
		DNode<T> *tail = (m_len == 0 ? m_head : locate(m_len - 1));
		while (m_len < n) {
			node = m_alloc.create(val, tail, tail->next);
			CHECK_NO_MEMORY_EXCEPTION(node);

			tail->next = node;
			tail = node;

			++m_len;
		}
	}
}

template < typename T, typename Alloc >
typename DLinkList<T, Alloc>::const_reference DLinkList<T, Alloc>::get(size_type i) const {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return locate(i)->val;
}

template < typename T, typename Alloc >
typename DLinkList<T, Alloc>::reference DLinkList<T, Alloc>::get(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return locate(i)->val;
}

template < typename T, typename Alloc >
void DLinkList<T, Alloc>::set(size_type i, const_reference val) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	locate(i)->val = val;
}

template < typename T, typename Alloc >
void DLinkList<T, Alloc>::move(size_type pos, size_type step) {
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

template < typename T, typename Alloc >
void DLinkList<T, Alloc>::next() {
	if (end())
		return;

	for (size_type i = 0; i < m_step && m_cur; ++i)
		m_cur = m_cur->next;
}

template < typename T, typename Alloc >
void DLinkList<T, Alloc>::prev() {
	if (end())
		return;

	for (size_type i = 0; i < m_step && m_cur; ++i)
		m_cur = m_cur->pre;
}

template < typename T, typename Alloc >
typename DLinkList<T, Alloc>::const_reference DLinkList<T, Alloc>::current() const {
	CHECK_OPERATION_EXCEPTION(m_cur && m_cur != m_head);

	return m_cur->val;
}

template < typename T, typename Alloc >
typename DLinkList<T, Alloc>::reference DLinkList<T, Alloc>::current() {
	CHECK_OPERATION_EXCEPTION(m_cur && m_cur != m_head);

	return m_cur->val;
}

DSLIB_END

#endif // DOUBLE_LINKED_LIST_H
