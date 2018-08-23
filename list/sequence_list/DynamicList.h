#ifndef DYNAMIC_SEQ_LIST_H
#define DYNAMIC_SEQ_LIST_H

#include "SeqList.h"

DSLIB_BEGIN

#define DEFAULT_DYNAMIC_SEQ_LIST_CAPACITY 1

template < typename T >
class DynamicList : public SeqList<T> {

public:
	explicit DynamicList(size_type i = DEFAULT_DYNAMIC_SEQ_LIST_CAPACITY) : m_capacity(i == 0 ? 1 : i), SeqList<T>(i == 0 ? 1 : i) { }
	DynamicList(const DynamicList<T>& stl);
	DynamicList<T>& operator=(const DynamicList<T>& stl);

	virtual size_type insert(size_type n, const_reference e);

	virtual size_type capacity() const { return m_capacity; }

	void resize(size_type, const_reference val = T());
	void reserve(size_type n) { grow(n); }

	virtual void swap(DynamicList<T>& obj);

private:
	void grow(size_type n = 0);

protected:
	size_type m_capacity;
};

template<typename T>
void swap(DynamicList<T>& a, DynamicList<T>& b) {
	a.swap(b);
}

template<typename T>
DynamicList<T>::DynamicList(const DynamicList<T>& dl) {
	if (dl.m_len == 0) {
		m_capacity = 1;
		m_arr = m_alloc.allocate(m_capacity);
		// m_arr = new T*[m_capacity];
		CHECK_NO_MEMORY_EXCEPTION(m_arr);
	}
	else {
		m_capacity = dl.m_len;
		m_arr = m_alloc.allocate(m_capacity);
		// m_arr = new T*[m_capacity];
		CHECK_NO_MEMORY_EXCEPTION(m_arr);

		for (size_type i = 0; i < dl.m_len; ++i) {
			m_alloc.construct(&m_arr[i], dl.m_arr[i]);
			// m_arr[i] = new T(*dl.m_arr[i]);
		}
	}
	m_len = dl.m_len;
}

template<typename T>
DynamicList<T>& DynamicList<T>::operator=(const DynamicList<T>& dl) {
	if (this != &dl) {
		if (dl.m_len > m_capacity) {
			clear();
			m_alloc.deallocate(m_arr);

			m_arr = m_alloc.allocate(dl.m_len);
			// m_arr = new T*[dl.m_len];
			CHECK_NO_MEMORY_EXCEPTION(m_arr);
			m_capacity = dl.m_len;

			for (size_type i = 0; i < dl.m_len; ++i)
				m_alloc.construct(&m_arr[i], dl.m_arr[i]);
			// m_arr[i] = new T(*dl.m_arr[i]);
		}
		else {
			size_type i;
			for (i = 0; i < m_len && i < dl.m_len; ++i)
				m_arr[i] = dl.m_arr[i];

			while (i < dl.m_len) {
				m_alloc.construct(&m_arr[i], dl.m_arr[i]);
				// m_arr[i] = new T(*dl.m_arr[i]);
				++i;
			}

			while (i < m_len) {
				m_alloc.destroy(&m_arr[i]);
				// delete m_arr[i];
				++i;
			}
		}
		m_len = dl.m_len;
	}
	return *this;
}

template<typename T>
typename DynamicList<T>::size_type DynamicList<T>::insert(size_type n, const_reference e) {
	CHECK_INDEX_OUT_OF_BOUNDS(n <= m_len);

	if (m_len == m_capacity)
		grow();

	return SeqList<T>::insert(n, e);
}

template<typename T>
void DynamicList<T>::grow(size_type capacity) {
	if (capacity == 0)
		capacity = m_capacity * 2;

	if (m_capacity >= capacity)
		return;

	T* temp = m_arr;

	m_arr = m_alloc.allocate(capacity);
	// m_arr = new T*[capacity];
	CHECK_NO_MEMORY_EXCEPTION(m_arr);
	m_capacity = capacity;

	for (size_type i = 0; i < m_len; ++i) {
		m_alloc.construct(&m_arr[i], DSLib::move(temp[i]));
		// m_arr[i] = temp[i];
		m_alloc.destroy(&temp[i]);
	}

	m_alloc.deallocate(temp);
	// delete[] temp;
}

template<typename T>
void DynamicList<T>::resize(size_type n, const_reference val) {
	if (m_capacity < n)
		grow(n);

	if (m_len < n) {
		while (m_len < n) {
			m_alloc.construct(&m_arr[m_len], val);
			++m_len;
		}
	}
	else if (m_len > n) {
		for (size_type i = m_len - 1; i >= n; --i) {
			m_alloc.destroy(&m_arr[i]);
			--m_len;
		}
	}
}

template<typename T>
void DynamicList<T>::swap(DynamicList<T>& obj){
	SeqList<T>::swap(obj);

	size_type temp;
	temp = m_capacity;
	m_capacity = obj.m_capacity;
	obj.m_capacity = temp;
}

DSLIB_END

#endif // !DYNAMIC_SEQ_LIST_H
