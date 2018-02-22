#ifndef LIST_H
#define LIST_H

#include "../Object.h"

DSLIB_BEGIN

template <typename T>
class List : public Object<T> {

public:
	virtual size_type insert(size_type, const_reference) = 0;
	virtual size_type remove(size_type) = 0;
	virtual void clear() = 0;

	virtual reference get(size_type) = 0;
	virtual const_reference get(size_type) const = 0;
	virtual reference at(size_type i) { return get(i); }
	virtual const_reference at(size_type i) const { return get(i); }
	virtual void set(size_type, const_reference) = 0;

	virtual reference back() { return get(size() - 1); }
	virtual const_reference back() const { return get(size() - 1); }
	virtual reference front() { return get(0); }
	virtual const_reference front() const { return get(0); }

	virtual void push_back(const_reference e) { insert(size(), e); }
	virtual void push_front(const_reference e) { insert(0, e); }
	virtual void pop_back() { remove(size() - 1); }
	virtual void pop_front() { remove(0); }

	virtual void reverse() = 0;

	virtual size_type size() const = 0;
	virtual size_type length()const { return size(); }
	virtual bool empty() const { return size() == 0; }

};

DSLIB_END

#endif // !LIST_H
