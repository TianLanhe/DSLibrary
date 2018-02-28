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

// 实现链表不同内存分配方式的allocator
// 默认是向堆申请内存，静态链表是向已有的静态内存池中申请内存
template < typename T >
class DefaultMemoryManager {
public:
	T* allocate() { return (T*)::operator new(sizeof(T)); }
	void deallocate(T* ptr) { ::operator delete(ptr); }

	template < typename... Args >
	T* create(Args&&... args) { return new T(DSLib::forward<Args>(args)...); }
	void destroy(T* ptr) { delete ptr; }
};

// 实现静态链表的内存分配方式，相当于一个小的静态内存池
// 通过将内存池中的空闲内存链起来进行管理，分配/回收时取下/添加一块内存
template < typename T, size_t N >
class StaticMemoryManager {
public:
	StaticMemoryManager();

	T* allocate();
	void deallocate(T*);

	template < typename... Args >
	T* create(Args&&... args) {
		T *ret = allocate();
		if (ret)
			new(ret) T(DSLib::forward<Args>(args)...);
		return ret;
	}

	void destroy(T* ptr) {
		ptr->~T();
		deallocate(ptr);
	}

private:
	StaticMemoryManager(const StaticMemoryManager<T, N>&);
	StaticMemoryManager<T, N>& operator=(const StaticMemoryManager<T, N>&);

private:

	union Block {
		unsigned char memory[sizeof(T)];
		Block* next;
	};

	Block* m_head;
	Block m_space[N];
};

template < typename T, size_t N >
StaticMemoryManager<T, N>::StaticMemoryManager() {
	m_head = m_space;

	Block* node = m_head;
	for (size_t i = 1; i < N; ++i) {
		node->next = &m_space[i];
		node = node->next;
	}
	node->next = nullptr;
}

template < typename T, size_t N >
T* StaticMemoryManager<T, N>::allocate() {
	if (m_head == nullptr)
		return nullptr;

	Block *block = m_head;
	m_head = m_head->next;

	return reinterpret_cast<T*>(block);
}

template < typename T, size_t N >
void StaticMemoryManager<T, N>::deallocate(T* ptr) {
	Block *block = reinterpret_cast<Block*>(ptr);

	if (m_head == nullptr) {
		m_head = block;
	}
	else {
		block->next = m_head->next;
		m_head->next = block;
	}
}

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

	size_type i;
	for (i = 0; i < m_step && m_cur; ++i)
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
