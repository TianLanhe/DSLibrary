#ifndef CONTAINER_STACK_H
#define CONTAINER_STACK_H

#include "Stack.h"

DSLIB_BEGIN

template < typename T, typename Container >
class ContainerStack : public Stack<T> {

public:
	virtual reference top() { return m_container.back(); }
	virtual const_reference top() const { return m_container.back(); }

	virtual void push(const_reference e) { m_container.push_back(e); }
	virtual void pop() { m_container.pop_back(); }

	virtual size_type size() const { return m_container.size(); }

	virtual void swap(ContainerStack& s) { return m_container.swap(s.m_container); }

protected:
	Container m_container;

};

DSLIB_END

#endif // !CONTAINER_STACK_H
