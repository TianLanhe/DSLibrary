#ifndef LINK_LIST_H
#define LINK_LIST_H

#include "../List.h"

DSLIB_BEGIN

template < typename T >
class LinkList : public List<T> {

public:
	virtual size_type insert(size_type, const_reference) = 0;
	virtual size_type remove(size_type) = 0;
	virtual void clear() = 0;

	virtual void reverse() = 0;

	virtual void resize(size_type, const_reference val = T()) = 0;

	virtual reference get(size_type i) = 0;
	virtual const_reference get(size_type i) const = 0;
	virtual void set(size_type i, const_reference val) = 0;

	virtual void move(size_type, size_type step = 1) = 0;
	virtual void next() = 0;
	virtual bool end() const = 0;
	virtual reference current() = 0;
	virtual const_reference current() const = 0;

	virtual size_type size() const = 0;
};

DSLIB_END

#endif // !LINK_LIST_H
