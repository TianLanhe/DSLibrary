#ifndef STACK_H
#define STACK_H

#include "../Object.h"

DSLIB_BEGIN

template < typename T, typename Container >
class Stack : public Object<T> {

public:
	virtual reference top() { return m_container.back(); }
	virtual const_reference top() const { return m_container.back(); }

	virtual void push(const_reference e) { m_container.push_back(e); }
	virtual void pop() { m_container.pop_back(); }

	virtual size_type size() const { return m_container.size(); }
	virtual size_type length()const { return size(); }
	virtual bool empty() const { return m_container.empty(); }

	virtual void swap(Stack& s) { return m_container.swap(s.m_container); }

protected:
	Container m_container;

};

DSLIB_END

#endif // !STACK_H
