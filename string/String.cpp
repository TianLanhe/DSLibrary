#include "String.h"
#include <cstring>
#include <iostream>

using namespace DSLib;
using namespace std;

typename const String::size_type String::npos = (String::size_type)(-1);

class Forward : public Locator {
public:
	Forward(size_type len):Locator(len){ }
	size_type operator()(size_type i) { return i;}
};

class Backward : public Locator {
public:
	Backward(size_type len):Locator(len){ }
	size_type operator()(size_type i) { return m_len - 1 - i;}
};

//////////////////////////////////////////////

String::String() : m_len(0), m_capacity(0) {
	m_content = new char[1];
	CHECK_NO_MEMORY_EXCEPTION(m_content);

	m_content[0] = '\0';
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

	if (m_capacity < len)
		grow(len);

	strcpy(m_content, arr);

	return *this;
}

String& String::assign(size_type n, char ch) {
	if (m_capacity < n)
		grow(n);

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
		buffer[i-pos] = m_content[i];
	buffer[i-pos] = '\0';

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

String& String::remove(size_type pos, size_type len){
	CHECK_INDEX_OUT_OF_BOUNDS(pos < m_len);

	if(len > m_len - pos)
		len = m_len - pos;

	for (size_type i = pos + len; i <= m_len; ++i){
		m_content[i - len] = m_content[i];
	}

	m_len -= len;

	return *this;
}

String& String::insert(size_type pos, const char* str){
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);
	CHECK_PARAMETER_EXCEPTION(str);

	size_type strLen = strlen(str);

	if(m_capacity < m_len + strLen)
		grow(m_len + strLen)

	for(size_type i = m_len; i >= pos; --i)
		m_content[i + strLen] = m_content[i];
	for(size_type i = 0; i < strLen; ++i)
		m_content[i + pos] = str[i];

	m_len += strLen;

	return *this;
}

String& String::insert(size_type pos, size_type n, char ch){
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	if(m_capacity < m_len + n)
		grow(m_len + n)

	for(size_type i = m_len; i >= pos; --i)
		m_content[i + n] = m_content[i];
	for(size_type i = 0; i < n; ++i)
		m_content[i + pos] = ch;

	m_len += n;

	return *this;
}

String& String::replace(size_type pos, size_type len, const char* str){
	remove(pos,len);
	return insert(pos,str);
}

String& String::replace(size_type pos, size_type len, size_type n, char ch){
	remove(pos,len);
	return insert(pos,n,ch);
}

size_type String::find(const char* str, size_type pos = 0) const {
	CHECK_PARAMETER_EXCEPTION(str);
	CHECK_INDEX_OUT_OF_BOUNDS(pos < m_len);

	return kmp_find(str,pos,Forward());
}

size_type String::find(char ch, size_type pos = 0) const {
	char arr[2] = { ch, '\0' };
	return find(arr,pos);
}

size_type String::rfind(const char* str, size_type pos = npos) const {
	CHECK_PARAMETER_EXCEPTION(str);
	CHECK_INDEX_OUT_OF_BOUNDS(pos < m_len || pos == npos);

	return kmp_find(str,pos,Backward());
}

size_type String::rfind(char, size_type pos = npos) const {
	char arr[2] = { ch, '\0' };
	return rfind(arr,pos);
}

void String::reserve(size_type n){
	if(m_capacity < n)
		grow(n);
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

size_type forward(size_type i){
	return i;
}

size_type backward(size_type i){
	return m_len - i -1;
}

void String::getNext(size_type* next,const char* substr,const Locator& locator){
	next[0] = npos;
	size_type i = 0;
	size_type j = npos;
	while(i < strlen(substr) - 1){
		if(j == npos || substr[locator(i)] == substr[locator(j)]){
			++i, ++j;
			if(substr[locator(i)] == substr[locator(j)])
				next[i] == next[j];
			else
				next[i] == j;
		}else{
			j = next[j];
		}
	}
}

size_type String::kmp_find(const char* substr,size_type startPos,const Locator& locator){
	CHECK_PARAMETER_EXCEPTION(substr && getIndex);
	CHECK_INDEX_OUT_OF_BOUNDS(startPos < m_len || startPos == npos);

	size_type substrLen = strlen(substr);

	if(substrLen == 0)
		return 0;

	size_type *next = new size_type[strlen(substr)];
	getNext(next,substr,locator);

	size_type i = startPos;
	size_type j = 0;
	while(i < m_len && j < strLen){
		if(j == npos || m_content[locator(i)] == substr[locator(j)]){
			++i;
			++j;
		}else{
			j = next[j];
		}
	}

	delete[] next;

	return (j == strLen ? i - substrLen : npos);
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

ostream& operator<< (ostream& os, const string& str){
	if(os){
		os << str.c_str();
	}
	return os;
}

bool isspace(char ch){
	return ch == '\t' || ch == '\r' || ch == '\n' || ch == ' ';
}

istream& operator>> (istream& is, string& str){
	if(is){
		char ch;
		while(is >> ch){
			if(isspace(ch))
				break;
			str.append(1,ch);
		}
	}
	return is;
}
