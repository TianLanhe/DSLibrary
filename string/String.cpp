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

String::String(const char* arr) {
	CHECK_PARAMETER_EXCEPTION(arr);

	size_type len = strlen(arr);

	m_content = new char[len + 1];
	CHECK_NO_MEMORY_EXCEPTION(m_content);

	strcpy(m_content, arr);
	m_len = len;
	m_capacity = len;
}

String::String(size_type n, char ch) {
	m_content = new char[n + 1];
	CHECK_NO_MEMORY_EXCEPTION(m_content);

	size_type i;
	for (i = 0; i < n; ++i)
		m_content[i] = ch;
	m_content[i] = '\0';

	m_len = n;
	m_capacity = n;
}

String& String::append(const char* arr) {
	CHECK_PARAMETER_EXCEPTION(arr);

	size_type len = strlen(arr);

	if (m_capacity < m_len + len)
		grow(m_len + len);

	strcat(m_content, arr);
	m_len += len;

	return *this;
}

String& String::append(size_type n, char ch) {
	if (n == 0)
		return *this;

	if (m_capacity < m_len + n)
		grow(m_len + n);

	size_type i;
	for (i = 0; i < n; ++i)
		m_content[m_len + i] = ch;
	m_content[m_len + i] = '\0';

	m_len += n;

	return *this;
}

String& String::assign(const char* arr) {
	CHECK_PARAMETER_EXCEPTION(arr);

	size_type len = strlen(arr);

	if (m_capacity < m_len + len)
		grow(m_len + len);

	strcpy(m_content, arr);

	return *this;
}

String& String::assign(size_type n, char ch) {
	if (m_capacity < m_len + n)
		grow(m_len + n);

	size_type i;
	for (i = 0; i < n; ++i)
		m_content[i] = ch;
	m_content[i] = '\0';

	return *this;
}

int String::compare(const char* arr) const {
	CHECK_PARAMETER_EXCEPTION(arr);

	return strcmp(m_content, arr);
}

String String::substr(size_type pos, size_type length) const {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	size_type len = (length > m_len - pos ? m_len - pos : length);

	char *arr = new char[len + 1];
	CHECK_NO_MEMORY_EXCEPTION(arr);

	size_type i;
	for (i = pos; i < len + pos; ++i)
		arr[i] = m_content[i];
	arr[i] = '\0';

	String ret(arr);
	delete[] arr;

	return ret;
}

typename String::size_type String::copy(char* buffer, size_type length, size_type pos) const {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);
	CHECK_PARAMETER_EXCEPTION(buffer);

	size_type len = (length > m_len - pos ? m_len - pos : length);

	size_type i;
	for (i = pos; i < pos + len; ++i)
		buffer[i] = m_content[i];
	buffer[i] = '\0';

	return len;
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

void String::resize(size_type n, char ch) {
	if (m_capacity < n)
		grow(n);

	if (m_len < n) {
		while (m_len < n) {
			m_content[m_len] = ch;
			++m_len;
		}
		m_content[m_len] = '\0';
	}
	else if (m_len > n) {
		m_content[n] = '\0';
		m_len = n;
	}
}

void String::shrink_to_fit() {
	CHECK_STATE_EXCEPTION(m_capacity >= m_len);

	if (m_capacity != m_len) {
		char *temp = m_content;

		m_content = new char[m_len + 1];
		CHECK_NO_MEMORY_EXCEPTION(m_content);

		strcpy(m_content, temp);

		m_capacity = m_len;

		delete[] temp;
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

// 传入的容量不包括'\0'，即要分配的空间为形参+1，若形参为0，则现有容量翻倍
void String::grow(size_type capacity) {
	CHECK_INDEX_OUT_OF_BOUNDS(capacity != (size_type)(-1));

	if (capacity == 0)
		capacity = ((size_type)(-2) / m_capacity > 1 ? m_capacity * 2 : (size_type)(-2));

	if (m_capacity >= capacity)
		return;

	char* temp = m_content;

	m_content = new char[capacity + 1];
	CHECK_NO_MEMORY_EXCEPTION(m_content);

	m_capacity = capacity;

	strcpy(m_content, temp);

	delete[] temp;
}

void swap(String& a, String& b) {
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
