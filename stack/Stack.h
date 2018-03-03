#ifndef STACK_H
#define STACK_H

#include "../Object.h"

DSLIB_BEGIN

template < typename T >
class Stack : public Object<T> {

public:
	virtual reference top() = 0;
	virtual const_reference top() const = 0;

	virtual void push(const_reference e) = 0;
	virtual void pop() = 0;

	virtual size_type size() const = 0;
	virtual size_type length() const { return size(); }
	virtual bool empty() const { return size() == 0; }
};

DSLIB_END

#endif // !STACK_H
