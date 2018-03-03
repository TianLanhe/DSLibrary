#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include "ContainerStack.h"
#include "../list/linked_list/CDLinkList.h"

DSLIB_BEGIN

/*template < typename T >
class LinkStack : public Stack<T, CDLinkList<T>> {

};
   跪了，厉害，不可以用typedef，C++11 使用using  */

template < typename T >
using LinkStack = ContainerStack<T, CDLinkList<T>>;


DSLIB_END

#endif // !LINKED_STACK_H
