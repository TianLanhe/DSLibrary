#ifndef STATIC_SEQ_LIST_H
#define STSTIC_SEQ_LIST_H

#include "SeqList.h"

DSLIB_BEGIN

template < typename T, size_t N >
class StaticList : public SeqList<T> {

public:
	StaticList() : SeqList<T>(N) { }
	StaticList(const StaticList<T, N>& stl);
	StaticList<T, N>& operator=(StaticList<T, N> stl);

	virtual size_type capacity() { return N; }

	virtual void swap(StaticList<T, N>&);
};

template < typename T, size_t N >
void swap(StaticList<T, N>& a, StaticList<T, N> b) {
	a.swap(b);
}

template<typename T, size_t N>
StaticList<T, N>::StaticList(const StaticList<T, N>& obj) :SeqList<T>(obj.m_len) {
	for (size_type i = 0; i < obj.m_len; ++i) {
		m_alloc.construct(&m_arr[i], obj.m_arr[i]);
		++m_len;
	}
}

template<typename T, size_t N>
StaticList<T, N>& StaticList<T, N>::operator=(StaticList<T, N> sql) {
	swap(sql);

	return *this;
}

template<typename T, size_t N>
void StaticList<T, N>::swap(StaticList<T, N>& obj) {
	SeqList<T>::swap(obj);
}

DSLIB_END

#endif // !STATIC_SEQ_LIST_H
