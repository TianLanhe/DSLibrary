#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include "Queue.h"
#include "../Utility.h"
#include "../stack/SeqStack.h"	// 想了想，如无特殊情况还是用顺序表吧

DSLIB_BEGIN

template < typename T, typename Stack = SeqStack<T> >
class StackQueue : public Queue<T> {

public:
	virtual reference back();		// Review : 取队尾复杂度高一点，不知道有没有更好的办法
	virtual const_reference back() const;
	virtual reference front();
	virtual const_reference front() const;

	virtual void push(const_reference);
	virtual void pop();

	virtual size_type size() const { return m_in.size() + m_out.size(); }

private:
	void transfer(Stack& a, Stack& b, size_type n) const;			// 从a将n个栈顶元素转移到b中
	void transfer() const { transfer(m_in, m_out, m_in.size()); }		// 将进入栈的所有元素转移到b中

	mutable Stack m_in;		// Review：有没有不用 mutable 的方法
	mutable Stack m_out;

};

template < typename T, typename Stack >
typename StackQueue<T, Stack>::reference StackQueue<T, Stack>::back() {
	CHECK_OPERATION_EXCEPTION(size() != 0);

	if (m_in.size() != 0)		// 如果进入栈还有数据，则栈顶元素为队尾元素
		return m_in.top();

	Stack tmpStack;				// 如果进入栈没有数据了，则退出栈的栈底元素为队尾元素，这时候复杂度高一点
	transfer(m_out, tmpStack, m_out.size() - 1);

	reference ret = m_out.top();

	transfer(tmpStack, m_out, tmpStack.size());

	return ret;
}

template < typename T, typename Stack >
typename StackQueue<T, Stack>::const_reference StackQueue<T, Stack>::back() const {
	CHECK_OPERATION_EXCEPTION(size() != 0);

	if (m_in.size() != 0)		// 如果进入栈还有数据，则栈顶元素为队尾元素
		return m_in.top();

	Stack tmpStack;				// 如果进入栈没有数据了，则退出栈的栈底元素为队尾元素，这时候复杂度高一点
	transfer(m_out, tmpStack, m_out.size() - 1);

	const_reference ret = m_out.top();

	transfer(tmpStack, m_out, tmpStack.size());

	return ret;
}

template < typename T, typename Stack >
typename StackQueue<T, Stack>::reference StackQueue<T, Stack>::front() {
	CHECK_OPERATION_EXCEPTION(size() != 0);

	if (m_out.size() == 0)
		transfer();

	return m_out.top();
}

template < typename T, typename Stack >
typename StackQueue<T, Stack>::const_reference StackQueue<T, Stack>::front() const {
	CHECK_OPERATION_EXCEPTION(size() != 0);

	if (m_out.size() == 0)
		transfer();

	return m_out.top();
}

template < typename T, typename Stack >
void StackQueue<T, Stack>::push(const_reference e) {
	CHECK_OPERATION_EXCEPTION(size() != (size_type)(-1));	// 注意测试两个栈的总长度不能超过最大无符号整型数

	m_in.push(e);
}

template < typename T, typename Stack >
void StackQueue<T, Stack>::pop() {
	CHECK_OPERATION_EXCEPTION(size() != 0);

	if (m_out.size() == 0)
		transfer();

	m_out.pop();
}

template < typename T, typename Stack >
void StackQueue<T, Stack>::transfer(Stack& a, Stack& b, size_type n) const {
	while (n-- > 0) {
		b.push(DSLib::move(a.top()));	// Review : 这里需要全面实现右值参数才行
		a.pop();
	}
}


DSLIB_END

#endif // !STACK_QUEUE_H
