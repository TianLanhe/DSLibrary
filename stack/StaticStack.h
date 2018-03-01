#ifndef STATIC_STACK_H
#define STATIC_STACK_H

#include "Stack.h"
#include "../list/sequence_list/StaticList.h"

DSLIB_BEGIN

template < typename T, size_t N>
using StaticStack = Stack<T, StaticList<T, N>>;


DSLIB_END

#endif // !STATIC_STACK_H
