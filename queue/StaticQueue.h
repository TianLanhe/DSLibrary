#ifndef STATIC_QUEUE_H
#define STATIC_QUEUE_H

#include "Queue.h"
#include "../Allocator.h"

DSLIB_BEGIN

template < typename T, size_t N >
class StaticQueue : public Queue<T> {

public:
	StaticQueue();
	StaticQueue(const StaticQueue<T, N>&);
	StaticQueue<T, N>& operator=(StaticQueue<T, N>);
	~StaticQueue();

	virtual reference back();
	virtual const_reference back() const;
	virtual reference front();
	virtual const_reference front() const;

	virtual void push(const_reference);
	virtual void pop();

	virtual size_type size() const { return (m_rear + N + 1 - m_front) % (N + 1); }

	virtual void swap(StaticQueue<T, N>& obj);

private:

	size_type nextIndex(size_type pos) { return (pos + 1) % (N + 1); }

	T *m_arr;

	size_type m_front;
	size_type m_rear;

	Allocator<T> m_alloc;	// 使用 allocator 分离内存的分配与对象的构造

};

template < typename T, size_t N >
void swap(StaticQueue<T, N>& a, StaticQueue<T, N>& b) {
	a.swap(b);
}

template < typename T, size_t N >
StaticQueue<T, N>::StaticQueue() {
	m_arr = m_alloc.allocate(N + 1);

	CHECK_NO_MEMORY_EXCEPTION(m_arr);

	m_front = m_rear = 0;
}

template < typename T, size_t N >
StaticQueue<T, N>::StaticQueue(const StaticQueue<T, N>& obj) {
	m_arr = m_alloc.allocate(N + 1);
	CHECK_NO_MEMORY_EXCEPTION(m_arr);
	m_front = m_rear = 0;

	for (size_type index = obj.m_front; index != obj.m_rear; index = nextIndex(index)) {
		m_alloc.construct(&m_arr[m_rear], m_arr[index]);
		m_rear = nextIndex(m_rear);
	}
}

template < typename T, size_t N >
StaticQueue<T, N>& StaticQueue<T, N>::operator=(StaticQueue<T, N> obj) {
	swap(obj);

	return *this;
}

template < typename T, size_t N >
StaticQueue<T, N>::~StaticQueue() {
	while (size() > 0)
		pop();

	m_alloc.deallocate(m_arr);
}

template < typename T, size_t N >
typename StaticQueue<T, N>::reference StaticQueue<T, N>::back() {
	CHECK_OPERATION_EXCEPTION(m_rear != m_front);

	return m_arr[m_rear - 1];
}

template < typename T, size_t N >
typename StaticQueue<T, N>::const_reference StaticQueue<T, N>::back() const {
	CHECK_OPERATION_EXCEPTION(m_rear != m_front);

	return m_arr[m_rear - 1];
}

template < typename T, size_t N >
typename StaticQueue<T, N>::reference StaticQueue<T, N>::front() {
	CHECK_OPERATION_EXCEPTION(m_rear != m_front);

	return m_arr[m_front];
}

template < typename T, size_t N >
typename StaticQueue<T, N>::const_reference StaticQueue<T, N>::front() const {
	CHECK_OPERATION_EXCEPTION(m_rear != m_front);

	return m_arr[m_front];
}

template < typename T, size_t N >
void StaticQueue<T, N>::push(const_reference e) {
	CHECK_OPERATION_EXCEPTION(nextIndex(m_rear) != m_front);

	m_alloc.construct(&m_arr[m_rear], e);
	m_rear = nextIndex(m_rear);
}

template < typename T, size_t N >
void StaticQueue<T, N>::pop() {
	CHECK_OPERATION_EXCEPTION(m_rear != m_front);

	m_alloc.destroy(&m_arr[m_front]);
	m_front = nextIndex(m_front);
}

template < typename T, size_t N >
void StaticQueue<T, N>::swap(StaticQueue<T, N>& obj) {
	T *tmp = m_arr;
	m_arr = obj.m_arr;
	obj.m_arr = tmp;

	int temp = m_front;
	m_front = obj.m_front;
	obj.m_front = temp;

	temp = m_rear;
	m_rear = obj.m_rear;
	obj.m_rear = temp;
}

DSLIB_END

#endif // !STATIC_QUEUE_H
