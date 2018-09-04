#ifndef STATIC_ARRAY_H
#define STATIC_ARRAY_H

#include "Array.h"

DSLIB_BEGIN

template < typename T, size_t N >
class StaticArray : public Array<T> {

public:
	virtual reference get(size_type);
	virtual const_reference get(size_type) const;
	virtual void set(size_type, const_reference);

	virtual void fill(const_reference);

	virtual size_type size() const { return N; }

protected:
	T m_space[N];		// 直接赋值或拷贝构造会复制数组，不需要额外重载赋值操作符和拷贝构造

};

template<typename T, size_t N>
typename StaticArray<T, N>::reference StaticArray<T, N>::get(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < N);

	return m_space[i];
}

template<typename T, size_t N>
typename StaticArray<T, N>::const_reference StaticArray<T, N>::get(size_type i) const {
	CHECK_INDEX_OUT_OF_BOUNDS(i < N);

	return m_space[i];
}

template<typename T, size_t N>
void StaticArray<T, N>::set(size_type i, const_reference val) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < N);

	m_space[i] = val;
}

template<typename T, size_t N>
void StaticArray<T, N>::fill(const_reference val) {
	for (size_type i = 0; i<N; ++i) {
		m_space[i] = val;
	}
}

DSLIB_END

#endif // !STATIC_ARRAY_H
