#ifndef LINK_QUEUE_H
#define LINK_QUEUE_H

#include "Queue.h"
#include "../list/linked_list/CDLinkList.h"

DSLIB_BEGIN

template < typename T, typename Container = CDLinkList<T> >
class LinkQueue : public Queue<T> {

public:
	virtual reference back() { return m_container.back(); }
	virtual const_reference back() const { return m_container.back(); }
	virtual reference front() { return m_container.front(); }
	virtual const_reference front() const { return m_container.front(); }

	virtual void pop() { m_container.pop_front(); }
	virtual void push(const_reference e) { m_container.push_back(e); }

	virtual size_type size() const { return m_container.size(); }

	virtual void swap(LinkQueue<T, Container>& b) { m_container.swap(b.m_container); }

private:
	Container m_container;

};

template < typename T, typename Container = CDLinkList<T> >
void swap(LinkQueue<T, Container>& a, LinkQueue<T, Container>& b) {
	a.swap(b);
}

DSLIB_END

#endif // !LINK_QUEUE_H
