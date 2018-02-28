#ifndef STATIC_SINGLE_LINK_LIST_H
#define STATIC_SINGLE_LINK_LIST_H

#include "SLinkList.h"

DSLIB_BEGIN

template < typename T, size_t N >
class StaticSLinkList : public SLinkList<T, StaticMemoryManager<SNode<T>,N+1> > {

public:
	size_type capacity() { return N; }

};

DSLIB_END

#endif // !STATIC_SINGLE_LINK_LIST_H
