#ifndef ARRAY_H
#define ARRAY_H

#include "../Object.h"

DSLIB_BEGIN

template < typename T >
class Array : public Object<T> {

public:
	virtual reference get(size_type) = 0;
	virtual const_reference get(size_type) const = 0;
	virtual reference at(size_type i) { return get(i); }
	virtual const_reference at(size_type i) const { return get(i); }
	virtual void set(size_type, const_reference) = 0;

	virtual reference front() { return get(0); }
	virtual const_reference front() const { return get(0); }
	virtual reference back() { return get(size() - 1); }
	virtual const_reference back() const { return get(size() - 1); }
	
	reference operator[](size_type i) { return at(i); }
	const_reference operator[](size_type i) const { return at(i); }

	virtual void fill(const_reference) = 0;

	virtual size_type size() const = 0;
	virtual size_type length()const { return size(); }
	virtual bool empty() const { return size() == 0; }
};

DSLIB_END

#endif // !ARRAY_H
