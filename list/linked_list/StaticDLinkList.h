#ifndef STATIC_DOUBLY_LINKED_LIST_H
#define STATIC_DOUBLY_LINKED_LIST_H

#include "DLinkList.h"

DSLIB_BEGIN

template < typename T, size_t N >
class StaticDLinkList : public DLinkList<T, StaticMemoryManager<DNode<T>, N + 1>> {

public:
	size_type capacity() const { return N; }

};

DSLIB_END

#endif // !STATIC_DOUBLY_LINKED_LIST_H
