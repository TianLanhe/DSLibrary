#ifndef STATIC_CIRCULAR_SINGLE_LINK_LIST_H
#define STATIC_CIRCULAR_SINGLE_LINK_LIST_H

#include "CLinkList.h"

DSLIB_BEGIN

template < typename T, size_t N >
class StaticCLinkList : public CLinkList<T, StaticMemoryManager<SNode<T>, N + 1> > {

public:
	size_type capacity() const { return N; }

};

DSLIB_END

#endif // !STATIC_CIRCULAR_SINGLE_LINK_LIST_H
