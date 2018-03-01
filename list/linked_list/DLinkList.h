#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include "LinkList.h"

DSLIB_BEGIN

// 双向链表节点类型 DNode
template < typename T >
struct DNode {

	DNode() :next(nullptr), pre(nullptr) { }
	DNode(DNode<T>* const n，DNode<T>* const p) :next(n), pre(p) { }
	DNode(const T& v, DNode<T>* const n = nullptr, DNode<T>* const p = nullptr) :next(n), val(v), pre(p) { }

	T val;
	DNode<T> *next;
	DNode<T> *pre;
};

template < typename T, typename
	class DLinkList {


	protected:
		virtual SNode<T>* locate(size_type) const = 0;
};

DSLIB_END

#endif // DOUBLE_LINKED_LIST_H
