#ifndef QUEUE_H
#define QUEUE_H

#include "../Object.h"

DSLIB_BEGIN

template < typename T >
class Queue : public Object<T> {

public:
	virtual reference back() = 0;
	virtual const_reference back() const = 0;
	virtual reference front() = 0;
	virtual const_reference front() const = 0;

	virtual void pop() = 0;
	virtual void push(const_reference) = 0;

	virtual size_type size() const = 0;
	virtual size_type length()const { return size(); }
	virtual bool empty() const { return size() == 0; }
};

DSLIB_END

#endif // !QUEUE_H
