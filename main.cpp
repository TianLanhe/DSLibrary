#include <iostream>
#include "Allocator.h"
#include "list/DynamicList.h"
#include "list/SLinkList.h"
#include "list/StaticSLinkList.h"
#include <list>

using namespace std;
using namespace DSLib;

class Test
{
public:
	Test() { cout << "Test()" << endl; }
	Test(const Test& t) { cout << "copy" << endl; }
	Test& operator=(const Test&t) { cout << "operator=" << endl; return *this; }
	~Test() { cout << "~Test()" << endl; }

	void display() { cout << "Test" << endl; }

};

int index = 0;
ostream& operator<<(ostream& out, const Test& t) {
	cout << index;
	return cout;
}

class T {
public:
	Test t;
	void *arrr[10];
	void func() const {
		const Test* th = &t;
		th++;
	}
	void display() {
		cout << "yes" << endl;
	}
	~T() { cout << "descontrutor" << endl; }

	T() { cout << "constructor" << endl; }
};

class A {
	int *a;

public:

	A() :a(new int(0)) {}
	explicit A(int i) :a(new int(i)) {}

	A(A& obj) :a(new int) {
		*a = *(obj.a);
	}

	A(A&& obj) {
		a = obj.a;
		obj.a = nullptr;
	}

	A& operator=(A obj) {
		int *tmp = a;
		a = obj.a;
		obj.a = tmp;
		return *this;
	}

	~A() {
		cout << "deconstructor:";
		if (a)
			cout << a << ' ' << *a << endl;
		else
			cout << "nullptr" << endl;

		if (a)
			delete a;
	}

	void display() {
		cout << *a << endl;
	}

	void set(int a) {
		*(this->a) = a;
	}

	A operator+(const A& b) {
		int tmp = *a + *b.a;
		A ret;
		ret.set(tmp);
		return ret;
	}
};

class Bad {
public:
	Bad() { throw 0; }
};

class C {
public:
	void clear() {
		display();
	}

	virtual void display() { cout << "In C" << endl; }

	~C() {
		clear();
	}
};

class C2 : public C {
public:
	void display() { cout << "In C2" << endl; }

	~C2() {
		clear();
	}
};

int main() {
	/*SLinkList<Test> l;
	Test t;

	cout << (l.size() == 0);

	for(int i=0;i<5;++i)
		l.push_back(t);

	cout << (l.size() == 5);

	l.clear();

	cout << (l.size() == 0);*/

	/*SLinkList<int> intL;

	for (SLinkList<int>::size_type i = 0; i < 5; ++i) {
		intL.push_back(i);
		intL.set(i, i*i);
	}

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;

	intL.set(3, 10);

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;

	intL.insert(4, 100);

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;

	intL.reverse();

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;

	intL.clear();
	intL.reverse();

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;

	intL.push_back(0);
	intL.reverse();

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;

	intL.resize(10, 123);

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;

	intL.resize(3);

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;
	
	intL.resize(0);

	for (intL.move(0); !intL.end(); intL.next())
		cout << intL.current() << endl;
	cout << endl;*/

	StaticSLinkList<int, 5> n;
	
	n.resize(4);

	for (n.move(0); !n.end(); n.next())
		cout << n.current() << endl;
	cout << endl;

	n.insert(1, 2);

	for (n.move(0); !n.end(); n.next())
		cout << n.current() << endl;
	cout << endl;

	return 0;
}
