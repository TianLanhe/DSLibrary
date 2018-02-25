#ifndef UTILITY_H
#define UTILITY_H

namespace DSLib {

	// �Ƴ���������
	template < typename T >
	struct remove_reference {
		typedef T type;
	};

	template < typename T >
	struct remove_reference <T&> {
		typedef T type;
	};

	template < typename T >
	struct remove_reference <T&&> {
		typedef T type;
	};

	// �Ƴ�����ָ��
	template < typename T >
	struct remove_pointer {
		typedef T type;
	};

	template < typename T >
	struct remove_pointer <T*> {
		typedef T type;
	};

	// move ������ʵ����ֵת��
	template < typename T > inline
	typename remove_reference<T>::type&& move(T&& obj) {
		return static_cast<remove_reference<decltype(obj)>::type&&> (obj);
	}

	// forward ������ʵ������ת��
	template < typename T > inline
	T&& forward(typename remove_reference<T>::type& t) {
		return static_cast<T&&>(t);
	}

}

#endif // !UTILITY_H
