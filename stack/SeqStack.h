#ifndef SEQUENCE_STACK_H
#define SEQUENCE_STACK_H

#include "Stack.h"
#include "../list/sequence_list/DynamicList.h"

DSLIB_BEGIN

template < typename T>
using SeqStack = Stack<T, DynamicList<T>>;


DSLIB_END

#endif // !SEQUENCE_STACK_H
