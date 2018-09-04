#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "Array.h"

DSLIB_BEGIN

template < typename T >
class DynamicArray : public Array<T> {

public:
	explicit DynamicArray(size_type i = 0);
	DynamicArray(const DynamicArray<T>& da);
	DynamicArray<T>& operator=(const DynamicArray<T>& da);
	~DynamicArray();

	virtual reference get(size_type i);
	virtual const_reference get(size_type) const;
	virtual void set(size_type, const_reference);

	virtual void fill(const_reference);

	virtual size_type size() const { return m_len; }

protected:
	void grow(size_type);

protected:
	iterator m_arr;
	size_type m_len;

};

template<typename T>
DynamicArray<T>::DynamicArray(size_type i) {
	m_len = i;

	m_arr = new T[m_len];
	CHECK_NO_MEMORY_EXCEPTION(m_arr);
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& obj) {
	m_len = obj.m_len;

	m_arr = new T[m_len];
	CHECK_NO_MEMORY_EXCEPTION(m_arr);
	for (size_type p = 0; p < m_len; ++p) {
		m_arr[p] = obj.m_arr[p];
	}
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& da) {
	if (this != &da) {
		iterator temp = new T[da.m_len];
		CHECK_NO_MEMORY_EXCEPTION(temp);

		for (size_type p = 0; p < da.m_len; ++p) {
			temp[p] = da.m_arr[p];
		}

		iterator tmp = m_arr;
		m_arr = temp;
		temp = tmp;

		m_len = da.m_len;

		delete[] temp;
	}
	return *this;
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
	delete[] m_arr;
}

template<typename T>
T& DynamicArray<T>::get(size_type i) {
	if (i >= m_len)
		grow(i + 1);

	return m_arr[i];
}

template<typename T>
const T& DynamicArray<T>::get(size_type i) const {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return m_arr[i];
}

template<typename T>
void DynamicArray<T>::set(size_type i, const_reference val) {
	if (i >= m_len)
		grow(i + 1);

	m_arr[i] = val;
}

template<typename T>
void DynamicArray<T>::fill(const_reference val) {
	for (size_type i = 0; i < m_len; ++i) {
		m_arr[i] = val;
	}
}

template<typename T>
void DynamicArray<T>::grow(size_type newLen) {
	if (newLen <= m_len)
		return;

	iterator temp = new T[newLen];
	CHECK_NO_MEMORY_EXCEPTION(temp);

	for (size_type i = 0; i < m_len; ++i) {
		temp[i] = m_arr[i];
	}

	iterator tmp = m_arr;
	m_arr = temp;
	temp = tmp;

	m_len = newLen;

	delete[] temp;
}

DSLIB_END

#endif // !DYNAMIC_ARRAY_H
