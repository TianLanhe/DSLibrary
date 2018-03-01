#ifndef STATIC_CIRCULAR_DOUBLY_LINK_LIST_H
#define STATIC_CIRCULAR_DOUBLY_LINK_LIST_H

#include "CDLinkList.h"

DSLIB_BEGIN

template < typename T, size_t N >
class StaticCDLinkList : public CDLinkList<T, StaticMemoryManager<DNode<T>, N + 1> > {

public:
	size_type capacity() const { return N; }

};

DSLIB_END

#endif // !STATIC_CIRCULAR_DOUBLY_LINK_LIST_H
