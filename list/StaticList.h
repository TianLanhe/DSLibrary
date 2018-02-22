#ifndef STATIC_SEQ_LIST_H
#define STSTIC_SEQ_LIST_H

#include "SeqList.h"

DSLIB_BEGIN

template < typename T, size_t N >
class StaticList : public SeqList<T> {

public:
	StaticList() { m_arr = m_space; }
	StaticList(const StaticList<T, N>& stl);
	StaticList<T, N>& operator=(const StaticList<T, N>& stl);
	~StaticList();

	virtual size_type insert(size_type n, const_reference e) {
		CHECK_INDEX_OUT_OF_BOUNDS(n <= m_len && (m_len + 1) <= N);

		for (size_type i = m_len; i > n; --i) {
			m_arr[i] = m_arr[i - 1];
		}

		m_arr[n] = new T(e);
		CHECK_NO_MEMORY_EXCEPTION(m_arr[n]);
		++m_len;

		return n;
	}

	virtual size_type capacity() { return N; }

protected:
	T *m_space[N];
};

template<typename T, size_t N>
StaticList<T, N>::StaticList(const StaticList<T, N>& stl) {
	m_arr = m_space;
	for (size_type i = 0; i < stl.m_len; ++i) {
		m_arr[i] = new T(*stl.m_arr[i]);
		CHECK_NO_MEMORY_EXCEPTION(m_arr[i]);
	}
	m_len = stl.m_len;
}


template<typename T, size_t N>
StaticList<T, N>& StaticList<T, N>::operator=(const StaticList<T, N>& sql) {
	if (this != &sql) {
		for (size_type i = 0; i < sql.m_len; ++i) {
			*m_arr[i] = *sql.m_arr[i];
		}

		for (size_type i = sql.m_len; i < m_len; ++i) {
			delete m_arr[i];
		}
		m_len = sql.m_len;
	}
	return *this;
}

template<typename T, size_t N>
StaticList<T, N>::~StaticList() {
	size_type i = m_len;
	while (i-- > 0) {
		delete m_arr[i];
	}
}

DSLIB_END

#endif // !STATIC_SEQ_LIST_H
