#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "Utility.h"

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

private:	// 内存池不允许复制或赋值
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

#endif // !MEMORY_MANAGER_H
