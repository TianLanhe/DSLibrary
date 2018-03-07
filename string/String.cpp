#include "String.h"
#include "cstring"

using namespace DSLib;
using namespace std;

typename const String::size_type String::npos = (String::size_type)(-1);

String::String() {
	m_content = new char[1];
	CHECK_NO_MEMORY_EXCEPTION(m_content);

	m_content[0] = '\0';
	m_capacity = 0;
	m_len = 0;
}

String& String::append(const char* arr) {
	CHECK_PARAMETER_EXCEPTION(arr);

	size_type len = strlen(arr);

	if (m_capacity < m_len + len + 1)
		grow(m_len + len + 1);

	strcat(m_content, arr);
	m_len += len;

	return *this;
}

String& String::append(size_type n, char ch) {
	if (n == 0)
		return *this;

	if (m_capacity < m_len + n + 1)
		grow(m_len + n + 1);

	size_type i;
	for (i = 0; i < n; ++i)
		m_content[m_len + i] = ch;
	m_content[m_len + i] = '\0';

	m_len += n;

	return *this;
}

typename String::reference String::get(size_type i) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return m_content[i];
}

typename String::const_reference String::get(size_type i) const {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	return m_content[i];
}

void String::set(size_type i, const_reference e) {
	CHECK_INDEX_OUT_OF_BOUNDS(i < m_len);

	m_content[i] = e;
}

void String::clear() {
	m_len = 0;
	m_content[0] = '\0';
}

void String::shrink_to_fit() {
	CHECK_STATE_EXCEPTION(m_capacity > m_len);

	if (m_capacity != m_len + 1) {
		char *newArr = new char[m_len + 1];

		CHECK_NO_MEMORY_EXCEPTION(newArr);
		strcpy(newArr, m_content);

		delete[] m_content;
		m_content = newArr;

		m_capacity = m_len + 1;
	}
}

void String::swap(String& obj) {
	char* temp = m_content;
	m_content = obj.m_content;
	obj.m_content = temp;

	size_type tmp = m_len;
	m_len = obj.m_len;
	obj.m_len = tmp;

	tmp = m_capacity;
	m_capacity = obj.m_capacity;
	obj.m_capacity = tmp;
}

void String::grow(size_type capacity) {
	if (capacity == 0)
		capacity = m_capacity * 2;

	if (m_capacity >= capacity)
		return;

	char* temp = m_content;

	m_content = new char[capacity];
	CHECK_NO_MEMORY_EXCEPTION(m_content);

	m_capacity = capacity;

	strcpy(m_content, temp);

	delete[] temp;
}

void DSLib::swap(String& a, String& b) {
	a.swap(b);
}

String DSLib::operator+(const char* arr, const String& str) {
	CHECK_PARAMETER_EXCEPTION(arr);

	return (String(str).append(arr));
}

String DSLib::operator+(char c, const String& str) {
	char arr[] = { c,'\0' };
	return arr + str;
}

String DSLib::operator+(const String& a, const String& b) { return a.c_str() + b; }
String DSLib::operator+(const String& str, const char* arr) { return arr + str; }
String DSLib::operator+(const String& str, char c) { return c + str; }

bool DSLib::operator==(const char* arr, const String& str) {
	CHECK_PARAMETER_EXCEPTION(arr);

	return (strcmp(arr, str.c_str()) == 0);
}

bool DSLib::operator==(const String& a, const String& b) { return a.c_str() == b; }
bool DSLib::operator==(const String& str, const char* arr) { return arr == str; }

bool DSLib::operator!=(const String& a, const String& b) { return !(a == b); }
bool DSLib::operator!=(const String& a, const char* b) { return !(a == b); }
bool DSLib::operator!=(const char* a, const String& b) { return !(a == b); }

bool DSLib::operator<(const char* arr, const String& str) {
	CHECK_PARAMETER_EXCEPTION(arr);

	return (strcmp(arr, str.c_str()) < 0);
}

bool DSLib::operator<(const String& a, const String& b) { return a.c_str() < b; }
bool DSLib::operator<(const String& str, const char* arr) { return arr < str; }

bool DSLib::operator<=(const String& a, const String& b) { return a == b || a < b; }
bool DSLib::operator<=(const String& a, const char* b) { return a == b || a < b; }
bool DSLib::operator<=(const char* a, const String& b) { return a == b || a < b; }

bool DSLib::operator>(const String& a, const String& b) { return b < a; }
bool DSLib::operator>(const String& a, const char* b) { return b < a; }
bool DSLib::operator>(const char* a, const String& b) { return b < a; }

bool DSLib::operator>=(const String& a, const String& b) { return !(a < b); }
bool DSLib::operator>=(const String& a, const char* b) { return !(a < b); }
bool DSLib::operator>=(const char* a, const String& b) { return !(a < b); }
