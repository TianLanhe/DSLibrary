#include "String.h"
#include <cstring>
#include <iostream>

using namespace DSLib;
using namespace std;

typename const String::size_type String::npos = (String::size_type)(-1);

class Forward : public String::Locator {
public:
	Forward(size_type len) :Locator(len) { }
	size_type operator()(size_type i) const { return i; }
};

class Backward : public String::Locator {
public:
	Backward(size_type len) :Locator(len) { }
	size_type operator()(size_type i) const { return m_len - 1 - i; }
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
	if (ch == '\0')
		n = 0;

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
	if (n == 0 || ch == '\0')
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
	m_len = len;

	return *this;
}

String& String::assign(size_type n, char ch) {
	if (n == 0 || ch == '\0') {
		clear();
		return *this;
	}

	if (m_capacity < n)
		grow(n);

	size_type i;
	for (i = 0; i < n; ++i)
		m_content[i] = ch;
	m_content[i] = '\0';
	m_len = n;

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
		arr[i - pos] = m_content[i];
	arr[i - pos] = '\0';

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
		buffer[i - pos] = m_content[i];
	//buffer[i - pos] = '\0';	不要加结束符，只需要复制内容即可

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

String& String::remove(size_type pos, size_type len) {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	if (len > m_len - pos)
		len = m_len - pos;

	for (size_type i = pos + len; i <= m_len; ++i) {
		m_content[i - len] = m_content[i];
	}

	m_len -= len;

	return *this;
}

String& String::remove(const String& str,size_type count){
	size_type len = str.size();

	if(len == 0)
		return *this;

	size_type index = 0;
	while(count-- > 0 && (index = find(str,index)) != npos){
		remove(index,len);
	}

	return *this;
}

String& String::rremove(const char* str,size_type count){
	CHECK_PARAMETER_EXCEPTION(str);

	size_type len = strlen(str);

	if(len == 0)
		return *this;

	size_type index = npos;
	while(count-- > 0 && (index = rfind(str,index)) != npos){
		remove(index,len);
	}

	return *this;
}

String& String::insert(size_type pos, const char* str) {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);
	CHECK_PARAMETER_EXCEPTION(str);

	size_type strLen = strlen(str);

	if (m_capacity < m_len + strLen)
		grow(m_len + strLen);

	// for (size_type i = m_len; i >= pos; --i)	不可以这样，若pos等于0，这 i>=0 恒成立
	size_type i = m_len + 1;
	while (i-- > pos) {
		m_content[i + strLen] = m_content[i];
	}

	for (size_type i = 0; i < strLen; ++i)
		m_content[i + pos] = str[i];

	m_len += strLen;

	return *this;
}

String& String::insert(size_type pos, size_type n, char ch) {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	if (n == 0 || ch == '\0')
		return *this;

	if (m_capacity < m_len + n)
		grow(m_len + n);

	size_type i = m_len + 1;
	while (i-- > pos) {
		m_content[i + n] = m_content[i];
	}

	for (size_type i = 0; i < n; ++i)
		m_content[i + pos] = ch;

	m_len += n;

	return *this;
}

String& String::replace(size_type pos, size_type len, const char* str) {
	remove(pos, len);
	return insert(pos, str);
}

String& String::replace(size_type pos, size_type len, size_type n, char ch) {
	if (ch == '\0')
		return remove(pos);

	remove(pos, len);
	return insert(pos, n, ch);
}

String& String::replace(const char* oldStr,const char* newStr,size_type count){
	CHECK_PARAMETER_EXCEPTION(oldStr && newStr);

	size_type len = strlen(oldStr);

	if(len == 0)
		return *this;

	size_type newStrLen = strlen(newStr);
	size_type index = 0;
	while(count-- > 0 && (index = find(oldStr,index)) != npos){
		replace(index,len,newStr);
		index += newStrLen;
	}

	return *this;
}

String& String::replace(const char* oldStr,size_type n,char ch,size_type count){
	CHECK_PARAMETER_EXCEPTION(oldStr);

	size_type len = strlen(oldStr);

	if(len == 0)
		return *this;

	size_type index = 0;
	while(count-- > 0 && (index = find(oldStr,index)) != npos){
		replace(index,len,n,ch);
		index += n;
	}

	return *this;
}

String& String::rreplace(const char* oldStr,const char* newStr,size_type count){
	CHECK_PARAMETER_EXCEPTION(oldStr && newStr);

	size_type len = strlen(oldStr);

	if(len == 0)
		return *this;

	size_type newStrLen = strlen(newStr);
	size_type index = npos;
	while(count-- > 0 && (index = rfind(oldStr,index)) != npos){
		replace(index,len,newStr);
		
		if (index == 0)
			break;
		else
			--index;
	}

	return *this;
}

String& String::rreplace(const char* oldStr,size_type n,char ch,size_type count){
	CHECK_PARAMETER_EXCEPTION(oldStr);

	size_type len = strlen(oldStr);

	if(len == 0)
		return *this;

	size_type index = npos;
	while(count-- > 0 && (index = rfind(oldStr,index)) != npos){
		replace(index,len,n,ch);

		if (index == 0)
			break;
		else
			--index;
	}

	return *this;
}

String::size_type String::find(const char* str, size_type pos) const {
	CHECK_PARAMETER_EXCEPTION(str);
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	return kmp_find(str, pos, Forward(m_len), Forward(strlen(str)));
}

String::size_type String::find(char ch, size_type pos) const {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	for (size_type i = pos; i < m_len; ++i)
		if (m_content[i] == ch)
			return i;
	return npos;
}

String::size_type String::rfind(const char* str, size_type pos) const {
	CHECK_PARAMETER_EXCEPTION(str);
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len || pos == npos);

	size_type ret = kmp_find(str, pos, Backward(m_len), Backward(strlen(str)));	// 逆向KMP查找会返回最后一个字符所在的位置，得手动退回到第一个字符的位置
	if (ret != npos && strlen(str) != 0)
		ret += 1 - strlen(str);

	return ret;
}

String::size_type String::rfind(char ch, size_type pos) const {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len || pos == npos);

	size_type i = (pos >= m_len ? m_len : pos + 1);
	while (i-- > 0) {
		if (m_content[i] == ch)
			return i;
	}
	return npos;
}

String::size_type String::find_first_not_of(const char* str, size_type pos) const {
	CHECK_PARAMETER_EXCEPTION(str);
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	size_type len = strlen(str);

	size_type j;
	for (size_type i = pos; i < m_len; ++i) {
		for (j = 0; j < len; ++j)
			if (m_content[i] == str[j])
				break;
		if (j == len)
			return i;
	}

	return npos;
}

String::size_type String::find_first_not_of(char ch, size_type pos) const {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	for (size_type i = pos; i < m_len; ++i)
		if (m_content[i] != ch)
			return i;
	return npos;
}

String::size_type String::find_first_of(const char* str, size_type pos)const {
	CHECK_PARAMETER_EXCEPTION(str);
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len);

	size_type len = strlen(str);

	if (len == 0)
		return npos;

	size_type j;
	for (size_type i = pos; i < m_len; ++i)
		for (j = 0; j < len; ++j)
			if (m_content[i] == str[j])
				return i;

	return npos;
}

String::size_type String::find_first_of(char ch, size_type pos)const {
	return find(ch, pos);
}

String::size_type String::find_last_not_of(const char* str, size_type pos)const {
	CHECK_PARAMETER_EXCEPTION(str);
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len || pos == npos);

	size_type len = strlen(str);

	size_type i = (pos == npos ? m_len : pos + 1);
	size_type j;
	while (i-- > 0) {
		for (j = 0; j < len; ++j)
			if (m_content[i] == str[j])
				break;
		if (j == len)
			return i;
	}
	return npos;
}

String::size_type String::find_last_not_of(char ch, size_type pos)const {
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len || pos == npos);

	size_type i = (pos == npos ? m_len : pos + 1);
	while (i-- > 0) {
		if (m_content[i] != ch)
			return i;
	}
	return npos;
}

String::size_type String::find_last_of(const char* str, size_type pos)const {
	CHECK_PARAMETER_EXCEPTION(str);
	CHECK_INDEX_OUT_OF_BOUNDS(pos <= m_len || pos == npos);

	size_type len = strlen(str);

	if (len == 0)
		return npos;

	size_type i = (pos == npos ? m_len : pos + 1);
	size_type j;
	while (i-- > 0)
		for (j = 0; j < len; ++j)
			if (m_content[i] == str[j])
				return i;

	return npos;
}

String::size_type String::find_last_of(char ch, size_type pos)const {
	return rfind(ch, pos);
}

void String::reserve(size_type n) {
	if (m_capacity < n)
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

void String::getNext(size_type* next, const char* substr, const Locator& locator) const {
	next[0] = npos;
	size_type i = 0;
	size_type j = npos;
	while (i < strlen(substr) - 1) {
		if (j == npos || substr[locator(i)] == substr[locator(j)]) {
			++i, ++j;
			if (substr[locator(i)] == substr[locator(j)])
				next[i] = next[j];
			else
				next[i] = j;
		}
		else {
			j = next[j];
		}
	}
}

String::size_type String::kmp_find(const char* substr, size_type startPos, const Locator& contentlocator, const Locator& locator) const {
	CHECK_PARAMETER_EXCEPTION(substr);
	CHECK_INDEX_OUT_OF_BOUNDS(startPos <= m_len || startPos == npos);

	size_type substrLen = strlen(substr);

	if (substrLen == 0)
		return startPos > m_len ? m_len : startPos;

	if (startPos == npos)
		startPos = m_len - 1;

	size_type *next = new size_type[strlen(substr)];
	getNext(next, substr, locator);

	size_type i = contentlocator(startPos);
	size_type j = 0;
	while (i < m_len && (j < substrLen || j == npos)) {
		if (j == npos || m_content[contentlocator(i)] == substr[locator(j)]) {
			++i;
			++j;
		}
		else {
			j = next[j];
		}
	}

	delete[] next;

	return (j == substrLen ? contentlocator(i - substrLen) : npos);
}

void swap(String& a, String& b) {
	a.swap(b);
}

String& String::trim() {
	size_type start = find_first_not_of(" \t\n\r");

	if (start == npos) {
		clear();
		return *this;
	}

	size_type end = find_last_not_of(" \t\r\n");

	return start == 0 ? remove(end+1) : assign(substr(start, end - start+1));
}

bool String::startWith(const char* str) const {
	CHECK_PARAMETER_EXCEPTION(str);

	if (strlen(str) > m_len)
		return false;

	for (size_type i = 0; i < m_len && i < strlen(str); ++i) {
		if (m_content[i] != str[i])
			return false;
	}
	return true;
}

bool String::endOf(const char* str) const {
	CHECK_PARAMETER_EXCEPTION(str);

	size_type len = strlen(str);

	if (len > m_len)
		return false;

	for (size_type i = 0; i < m_len && i < len; ++i) {
		if (m_content[m_len - 1 - i] != str[len - 1 - i])
			return false;
	}
	return true;
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

ostream& operator<< (ostream& os, const String& str) {
	if (os) {
		os << str.c_str();
	}
	return os;
}

bool isspace(char ch) {
	return ch == '\t' || ch == '\r' || ch == '\n' || ch == ' ';
}

istream& operator >> (istream& is, String& str) {
	if (is) {
		char ch;
		while (is >> ch) {
			if (isspace(ch))
				break;
			str.append(1, ch);
		}
	}
	return is;
}
