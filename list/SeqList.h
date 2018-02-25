#ifndef SEQ_LIST_H
#define SEQ_LIST_H

#include "List.h"
#include "../Exception.h"

DSLIB_BEGIN

template < typename T>
class SeqList : public List <T> {

public:
	SeqList() :m_len(0), m_arr(nullptr) {}

	virtual size_type insert(size_type, const_reference) = 0;
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
	iterator *m_arr;	// 为了能及时地remove时真正删除对象，这里用指针数组存，删除时才可以析构掉
	size_type m_len;
};

template<typename T>
typename SeqList<T>::size_type SeqList<T>::remove(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	delete m_arr[i];
	for (size_type p = i; p < m_len - 1; ++p) {
		m_arr[p] = m_arr[p + 1];
	}
	--m_len;

	return i;
}

template<typename T>
void SeqList<T>::reverse() {
	for (size_type i = 0; i < m_len / 2; ++i) {
		iterator temp = m_arr[i];
		m_arr[i] = m_arr[m_len - 1 - i];
		m_arr[m_len - 1 - i] = temp;
	}
}

template<typename T>
void SeqList<T>::clear() {
	size_type i = m_len;
	while (i-- > 0) {
		delete m_arr[i];
	}
	m_len = 0;
}

template<typename T>
void SeqList<T>::set(size_type i, const_reference e) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	*m_arr[i] = e;
}

template<typename T>
typename SeqList<T>::reference SeqList<T>::get(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return *m_arr[i];
}

template<typename T>
typename SeqList<T>::const_reference SeqList<T>::get(size_type i) const {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return *m_arr[i];
}

DSLIB_END

#endif // !SEQ_LIST_H
