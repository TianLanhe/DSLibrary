#ifndef QUEUE_STACK_H
#define QUEUE_STACK_H

#include "Stack.h"
#include "../Utility.h"
#include "../queue/LinkQueue.h"		// 顺序表必须指定最大容量，故还是用链式队列吧

DSLIB_BEGIN

template < typename T, typename Queue = LinkQueue<T> >
class QueueStack : public Stack<T> {

public:
	QueueStack();
	QueueStack(const QueueStack<T, Queue>&);
	QueueStack<T, Queue>& operator=(QueueStack<T, Queue>);
	~QueueStack();

	virtual reference top();
	virtual const_reference top() const;

	virtual void push(const_reference e);
	virtual void pop();

	virtual size_type size() const { return m_major->size() + m_secondary->size(); }

	virtual void swap(QueueStack<T, Queue>&);
	
protected:

	void swap(Queue*&, Queue*&);

	Queue *m_major;
	Queue *m_secondary;
};

template < typename T, typename Queue >
void swap(QueueStack<T, Queue>& a, QueueStack<T, Queue>& b) {
	a.swap(b);
}

template < typename T, typename Queue >
QueueStack<T, Queue>::QueueStack() {
	m_major = new Queue();
	CHECK_NO_MEMORY_EXCEPTION(m_major);

	m_secondary = new Queue();
	CHECK_NO_MEMORY_EXCEPTION(m_secondary);
}

template < typename T, typename Queue >
QueueStack<T, Queue>::QueueStack(const QueueStack<T, Queue>& obj) {
	m_major = new Queue(*obj.m_major);
	CHECK_NO_MEMORY_EXCEPTION(m_major);

	m_secondary = new Queue(*obj.m_secondary);
	CHECK_NO_MEMORY_EXCEPTION(m_secondary);
}

template < typename T, typename Queue >
QueueStack<T, Queue>& QueueStack<T, Queue>::operator=(QueueStack<T, Queue> obj) {
	swap(obj);

	return *this;
}

template < typename T, typename Queue >
QueueStack<T, Queue>::~QueueStack() {
	delete m_major;
	delete m_secondary;
}

template < typename T, typename Queue >
typename QueueStack<T, Queue>::reference QueueStack<T, Queue>::top() {
	return m_major->back();
}

template < typename T, typename Queue >
typename QueueStack<T, Queue>::const_reference QueueStack<T, Queue>::top() const {
	return m_major->back();
}

template < typename T, typename Queue >
void QueueStack<T, Queue>::push(const_reference e) {
	CHECK_OPERATION_EXCEPTION(size() != (size_type)(-1));

	m_major->push(e);
}

template < typename T, typename Queue >
void QueueStack<T, Queue>::pop() {
	CHECK_OPERATION_EXCEPTION(size() != 0);

	while (m_major->size() != 1) {
		m_secondary->push(DSLib::move(m_major->front()));	// Review : 这里需要全面实现右值参数才行
		m_major->pop();
	}

	m_major->pop();

	swap(m_major, m_secondary);
}

template < typename T, typename Queue >
void QueueStack<T, Queue>::swap(Queue*& a, Queue*& b) {
	Queue *tmp = a;
	a = b;
	b = tmp;
}

template < typename T, typename Queue >
void QueueStack<T, Queue>::swap(QueueStack<T, Queue>& obj) {
	Queue* temp = m_major;
	m_major = obj.m_major;
	obj.m_major = temp;

	temp = m_secondary;
	m_secondary = obj.m_secondary;
	obj.m_secondary = temp;
	/*m_major->swap(*obj.m_major);
	m_secondary->swap(*obj.m_secondary);*/
}

DSLIB_END

#endif // !QUEUE_STACK_H
