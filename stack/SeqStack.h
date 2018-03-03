#ifndef SEQUENCE_STACK_H
#define SEQUENCE_STACK_H

#include "ContainerStack.h"
#include "../list/sequence_list/DynamicList.h"

DSLIB_BEGIN

template < typename T >
using SeqStack = ContainerStack<T, DynamicList<T>>;


DSLIB_END

#endif // !SEQUENCE_STACK_H
