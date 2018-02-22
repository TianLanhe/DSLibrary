#include <iostream>
#include <vector>
#include "list/DynamicList.h"
#include <array>
#include <list>
#include "array/DynamicArray.h"
#include "array/StaticArray.h"

using namespace std;
using namespace DSLib;

template<typename T>
void find(const Object<T> obj, const T& val) {
	
}

class Test
{
public:
	Test() { cout << "Test" << endl; }
	Test(const Test& t) { cout << "copy" << endl; }
	Test& operator=(const Test&t) { cout << "operator=" << endl; return *this; }
	~Test() { cout << "~Test" << endl; }

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
private:
	T() {}
};

int main() {
	StaticArray<array<int,4>,0> a;

	cout << a.size() << endl;

	for (StaticArray<array<int, 4>, 4>::size_type i = 0; i < a.size(); ++i)
		cout << a[i].size() << endl;

	//a[3] = array<int, 4>();

	for (StaticArray<array<int, 4>, 4>::size_type i = 0; i < a.size(); ++i)
		cout << a[i].size() << endl;

	for (StaticArray<array<int, 4>, 4>::size_type i = 0; i < a.size(); ++i) {
		for (StaticArray<int, 4 > ::size_type j = 0; j < a[i].size(); ++j) {
			cout << a[i][j] << ' ';
		}
		cout << endl;
	}

	return 0;
}
