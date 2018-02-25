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

		pointer allocate(size_type);	// ���׼�ⲻͬ������ʡ����hint���������Լ����쳣
		void deallocate(pointer) {		// ���׼�ⲻͬ����׼������ж������������ʱ��Ҫ֪���ռ��С�ԶԶ���������д���
			::operator delete(ptr);
		}

		template <typename... Args>		// ���׼�ⲻͬ��ֻ�ܹ����ϵͳ��ģ����ͬ������
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
