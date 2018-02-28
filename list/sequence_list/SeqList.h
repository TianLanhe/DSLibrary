#ifndef SEQ_LIST_H
#define SEQ_LIST_H

#include "../List.h"
#include "../../Exception.h"
#include "../../Allocator.h"
#include "../../Utility.h"

DSLIB_BEGIN

template < typename T>
class SeqList : public List <T> {

public:
	SeqList() :m_len(0), m_arr(nullptr) {}
	SeqList(size_type capacity);
	virtual ~SeqList();

	virtual size_type insert(size_type, const_reference);
	virtual void clear();
	virtual size_type remove(size_type i);

	virtual reference get(size_type);
	virtual const_reference get(size_type) const;
	virtual void set(size_type, const_reference);
	reference operator[](size_type i) { return get(i); }
	const_reference operator[](size_type i) const { return get(i); }

	virtual void reverse();

	virtual size_type size() const { return m_len; }

	virtual size_type capacity() = 0;

protected:

	virtual void swap(SeqList<T>&);

	T *m_arr;
	size_type m_len;

	Allocator<T> m_alloc;	// 使用 allocator 分离内存的分配与对象的构造
};

template<typename T>
SeqList<T>::SeqList(size_type capacity) :m_len(0) {
	m_arr = m_alloc.allocate(capacity);
	// m_arr = new T*[capacity];
	CHECK_NO_MEMORY_EXCEPTION(m_arr);
}

template<typename T>
SeqList<T>::~SeqList() {
	clear();
	m_alloc.deallocate(m_arr);
}

template<typename T>
typename SeqList<T>::size_type SeqList<T>::insert(size_type n, const_reference val) {
	CHECK_INDEX_OUT_OF_BOUNDS(n <= m_len && (m_len + 1) <= capacity());

	for (size_type i = m_len; i > n; --i) {
		m_arr[i] = m_arr[i - 1];
	}

	m_alloc.construct(&m_arr[n], val);
	// m_arr[n] = new T(e);
	++m_len;

	return n;
}

template<typename T>
typename SeqList<T>::size_type SeqList<T>::remove(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	m_alloc.destroy(&m_arr[i]);
	// delete m_arr[i];
	for (size_type p = i; p < m_len - 1; ++p) {
		m_arr[p] = m_arr[p + 1];
	}
	--m_len;

	return i;
}

template<typename T>
void SeqList<T>::reverse() {
	for (size_type i = 0; i < m_len / 2; ++i) {
		T temp(DSLib::move(m_arr[i]));
		m_arr[i] = DSLib::move(m_arr[m_len - i - i]);
		m_arr[m_len - i - i] = DSLib::move(temp);
	}
}

template<typename T>
void SeqList<T>::clear() {
	while (m_len > 0) {
		m_alloc.destroy(&m_arr[m_len - 1]);
		// delete m_arr[i];
		--m_len;
	}
}

template<typename T>
void SeqList<T>::set(size_type i, const_reference e) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	m_arr[i] = e;
}

template<typename T>
typename SeqList<T>::reference SeqList<T>::get(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return m_arr[i];
}

template<typename T>
typename SeqList<T>::const_reference SeqList<T>::get(size_type i) const {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return m_arr[i];
}

template<typename T>
void SeqList<T>::swap(SeqList<T>& obj) {
	T* temp = m_arr;
	m_arr = obj.m_arr;
	obj.m_arr = temp;

	size_type tmp = m_len;
	m_len = obj.m_len;
	obj.m_len = tmp;
}

DSLIB_END

#endif // !SEQ_LIST_H
