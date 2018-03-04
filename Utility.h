#ifndef UTILITY_H
#define UTILITY_H

namespace DSLib {

	// 移除类型引用
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

	// 移除类型指针
	template < typename T >
	struct remove_pointer {
		typedef T type;
	};

	template < typename T >
	struct remove_pointer <T*> {
		typedef T type;
	};

	// move 函数，实现右值转换
	template < typename T > inline
	typename remove_reference<T>::type&& move(T&& obj) {
		return static_cast<remove_reference<decltype(obj)>::type&&> (obj);
	}

	// forward 函数，实现完美转发
	template < typename T > inline
	T&& forward(typename remove_reference<T>::type& t) {
		return static_cast<T&&>(t);
	}

	// 小于 functor
	struct less {
		template < typename T1, typename T2 >
		bool operator()(const T1& t1, const T2& t2) { return t1 < t2; }
	};

	// 相等 functor
	struct equal {
		template < typename T1, typename T2 >
		bool operator()(const T1& t1, const T2& t2) { return t1 == t2; }
	};

}

#endif // !UTILITY_H
