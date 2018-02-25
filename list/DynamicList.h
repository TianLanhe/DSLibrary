#ifndef DYNAMIC_SEQ_LIST_H
#define DYNAMIC_SEQ_LIST_H

#include "SeqList.h"

DSLIB_BEGIN

#define DEFAULT_DYNAMIC_SEQ_LIST_LENGTH 1

template < typename T >
class DynamicList : public SeqList<T> {

public:
	DynamicList(size_type i = DEFAULT_DYNAMIC_SEQ_LIST_LENGTH);
	DynamicList(const DynamicList<T>& stl);
	DynamicList<T>& operator=(const DynamicList<T>& stl);
	~DynamicList();

	virtual size_type insert(size_type n, const_reference e);

	virtual size_type capacity() { return m_capacity; }

	void resize(size_type, const_reference val = T()); // Review
	void reserve(size_type n) {		// Review : 多次reallocate导致性能下降 
		while (m_capacity < n)	
			grow();
	}

private:
	void grow();

protected:
	size_type m_capacity;
};

template<typename T>
DynamicList<T>::DynamicList(size_type i) : m_capacity(i) {
	if (i == 0)
		m_capacity = 1;
	m_arr = new T*[m_capacity];
	CHECK_NO_MEMORY_EXCEPTION(m_arr);
}

template<typename T>
DynamicList<T>::DynamicList(const DynamicList<T>& dl) {
	if (dl.m_len == 0) {
		m_capacity = 1;
		m_arr = new T*[m_capacity];
		CHECK_NO_MEMORY_EXCEPTION(m_arr);
	}
	else {
		m_capacity = dl.m_len;
		m_arr = new T*[m_capacity];
		CHECK_NO_MEMORY_EXCEPTION(m_arr);

		for (size_type i = 0; i < dl.m_len; ++i) {
			m_arr[i] = new T(*dl.m_arr[i]);
			CHECK_NO_MEMORY_EXCEPTION(m_arr[i]);
		}
	}
	m_len = dl.m_len;
}

template<typename T>
DynamicList<T>& DynamicList<T>::operator=(const DynamicList<T>& dl) {
	if (this != &dl) {
		if (dl.m_len > m_capacity) {
			clear();
			delete[] m_arr;

			m_arr = new T*[dl.m_len];
			CHECK_NO_MEMORY_EXCEPTION(m_arr);
			m_capacity = dl.m_len;

			for (size_type i = 0; i < dl.m_len; ++i)
				m_arr[i] = new T(*dl.m_arr[i]);
		}
		else {
			size_type i;
			for (i = 0; i < m_len && i < dl.m_len; ++i)
				*m_arr[i] = *dl.m_arr[i];

			while (i < dl.m_len) {
				m_arr[i] = new T(*dl.m_arr[i]);
				++i;
			}

			while (i < m_len) {
				delete m_arr[i];
				++i;
			}
		}
		m_len = dl.m_len;
	}
	return *this;
}

template<typename T>
DynamicList<T>::~DynamicList() {
	size_type i = m_len;
	while (i-- > 0) {
		delete m_arr[i];
	}
	delete[] m_arr;
}

template<typename T>
typename DynamicList<T>::size_type DynamicList<T>::insert(size_type n, const_reference e) {
	CHECK_INDEX_OUT_OF_BOUNDS(n <= m_len);

	if (m_len == m_capacity)
		grow();

	for (size_type i = m_len; i > n; --i) {
		m_arr[i] = m_arr[i - 1];
	}

	m_arr[n] = new T(e);
	CHECK_NO_MEMORY_EXCEPTION(m_arr[n]);
	++m_len;

	return n;
}

template<typename T>
void DynamicList<T>::grow() {
	iterator *temp = m_arr;

	m_arr = new T*[m_capacity * 2];
	m_capacity *= 2;

	for (size_type i = 0; i < m_len; ++i)
		m_arr[i] = temp[i];
	delete[] temp;
}

template<typename T>
void DynamicList<T>::resize(size_type n, const_reference val) {
	if (m_len < n) {
		for (size_type i = m_len; i < n; ++i)
			insert(i, val);
	}
	else if (m_len > n) {
		for (size_type i = m_len - 1; i >= n; --i)
			remove(i);
	}
}

DSLIB_END

#endif // !DYNAMIC_SEQ_LIST_H
