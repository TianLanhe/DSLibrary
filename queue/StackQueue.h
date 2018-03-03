#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include "Queue.h"
#include "../Utility.h"
#include "../stack/SeqStack.h"	// �����룬�����������������˳����

DSLIB_BEGIN

template < typename T, typename Stack = SeqStack<T> >
class StackQueue : public Queue<T> {

public:
	virtual reference back();		// Review : ȡ��β���Ӷȸ�һ�㣬��֪����û�и��õİ취
	virtual const_reference back() const;
	virtual reference front();
	virtual const_reference front() const;

	virtual void push(const_reference);
	virtual void pop();

	virtual size_type size() const { return m_in.size() + m_out.size(); }

private:
	void transfer(Stack& a, Stack& b, size_type n) const;			// ��a��n��ջ��Ԫ��ת�Ƶ�b��
	void transfer() const { transfer(m_in, m_out, m_in.size()); }		// ������ջ������Ԫ��ת�Ƶ�b��

	mutable Stack m_in;		// Review����û�в��� mutable �ķ���
	mutable Stack m_out;

};

template < typename T, typename Stack >
typename StackQueue<T, Stack>::reference StackQueue<T, Stack>::back() {
	CHECK_OPERATION_EXCEPTION(size() != 0);

	if (m_in.size() != 0)		// �������ջ�������ݣ���ջ��Ԫ��Ϊ��βԪ��
		return m_in.top();

	Stack tmpStack;				// �������ջû�������ˣ����˳�ջ��ջ��Ԫ��Ϊ��βԪ�أ���ʱ���Ӷȸ�һ��
	transfer(m_out, tmpStack, m_out.size() - 1);

	reference ret = m_out.top();

	transfer(tmpStack, m_out, tmpStack.size());

	return ret;
}

template < typename T, typename Stack >
typename StackQueue<T, Stack>::const_reference StackQueue<T, Stack>::back() const {
	CHECK_OPERATION_EXCEPTION(size() != 0);

	if (m_in.size() != 0)		// �������ջ�������ݣ���ջ��Ԫ��Ϊ��βԪ��
		return m_in.top();

	Stack tmpStack;				// �������ջû�������ˣ����˳�ջ��ջ��Ԫ��Ϊ��βԪ�أ���ʱ���Ӷȸ�һ��
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
	CHECK_OPERATION_EXCEPTION(size() != (size_type)(-1));	// ע���������ջ���ܳ��Ȳ��ܳ�������޷���������

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
		b.push(DSLib::move(a.top()));	// Review : ������Ҫȫ��ʵ����ֵ��������
		a.pop();
	}
}


DSLIB_END

#endif // !STACK_QUEUE_H
