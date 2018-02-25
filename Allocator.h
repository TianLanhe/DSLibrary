#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "Utility.h"
#include "Exception.h"

namespace DSLib {

	template<typename T>
	class Allocator {

	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;

	public:

		pointer allocate(size_type);	// 与标准库不同，这里省略了hint参数，抛自己的异常
		void deallocate(pointer) {		// 与标准库不同，标准库分配有对齐操作，回收时需要知道空间大小以对对齐操作进行处理
			::operator delete(ptr);
		}

		template <typename... Args>		// 与标准库不同，只能构造和系统和模板相同的类型
		void construct(T* p, Args&&... args) {
			::operator new((void*)p) T(DSLib::forward<Args>(args)...);
		}

		void destroy(T* p) {
			p->~T();
		}

		pointer address(reference x) const { return &x; }
		const_pointer address(const_reference x) const { return &x; }
	};

	template<typename T>
	typename Allocator<T>::pointer Allocator<T>::allocate(size_type count) {
		void* ret = nullptr;
		
		if (count == 0)
			return ret;

		if (size_type(-1) / sizeof(T) < count)
			THROW_EXCEPTION(NoMemoryException, "no enough memory to allocate in Allocator!");

		ret = ::operator new(count * sizeof(T));
		if (ret == nullptr)
			THROW_EXCEPTION(NoMemoryException, "allocate error in Allocator!");

		return ret;
	}
}

#endif // !ALLOCATOR_H
