#ifndef STRING_H
#define STRING_H

#include "../Object.h"

#include <iostream>

DSLIB_BEGIN

class String : public Object<char> {

public:
	String();
	String(const String& str) :String(str.c_str()) { }
	String(const char*);
	String(size_type, char);
	~String() { delete[] m_content; }

	String& append(const char*);
	String& append(const String& str) { return append(str.c_str()); }
	String& append(size_type, char);

	String& assign(const String& str) { return assign(str.c_str()); }
	String& assign(const char*);
	String& assign(size_type, char);

	int compare(const String& str) const { return compare(str.c_str()); }
	int compare(const char*) const;

	const char* c_str() const { return m_content; }

	String substr(size_type pos = 0, size_type len = npos) const;

	size_type copy(char*, size_type, size_type pos = 0) const;

	reference get(size_type);
	const_reference get(size_type) const;
	reference at(size_type i) { return get(i); }
	const_reference at(size_type i) const { return get(i); }
	void set(size_type, const_reference);

	reference operator[](size_type i) { return get(i); }
	const_reference operator[](size_type i) const { return get(i); }

	void clear();

	String& remove(size_type pos = 0, size_type len = npos);

	String& remove(const String& str, size_type count = npos);// { return remove(str.c_str(), count); }
	//String& remove(const char*, size_type count = npos);
	// 因为这个方法(const char*,size_type)会跟(size_type,size_type)形参匹配冲突，所以只能去掉这个

	String& rremove(const String& str, size_type count = npos) { return rremove(str.c_str(), count); }
	String& rremove(const char*, size_type count = npos);

	size_type find(const String& str, size_type pos = 0) const { return find(str.c_str(), pos); }
	size_type find(const char*, size_type pos = 0) const;
	size_type find(char, size_type pos = 0) const;

	size_type rfind(const String& str, size_type pos = npos) const { return rfind(str.c_str(), pos); }
	size_type rfind(const char*, size_type pos = npos) const;
	size_type rfind(char, size_type pos = npos) const;

	size_type find_first_not_of(const String& str, size_type pos = 0) const { return find_first_not_of(str.c_str(), pos); }
	size_type find_first_not_of(const char*, size_type pos = 0) const;
	size_type find_first_not_of(char, size_type pos = 0) const;

	size_type find_first_of(const String& str, size_type pos = 0) const { return find_first_of(str.c_str(), pos); }
	size_type find_first_of(const char*, size_type pos = 0)const;
	size_type find_first_of(char, size_type pos = 0)const;

	size_type find_last_not_of(const String& str, size_type pos = npos) const { return find_last_not_of(str.c_str(), pos); }
	size_type find_last_not_of(const char*, size_type pos = npos)const;
	size_type find_last_not_of(char, size_type pos = npos)const;

	size_type find_last_of(const String& str, size_type pos = npos) const { return find_last_of(str.c_str(), pos); }
	size_type find_last_of(const char*, size_type pos = npos)const;
	size_type find_last_of(char, size_type pos = npos)const;

	String& insert(size_type pos, const String& str) { return insert(pos, str.c_str()); }
	String& insert(size_type, const char*);
	String& insert(size_type, size_type, char);

	String& operator+=(const String& str) { return append(str); }
	String& operator+=(const char* arr) { return append(arr); }
	String& operator+=(char ch) { return append(1, ch); }

	String& operator=(const String& str) { return assign(str); }
	String& operator=(const char* arr) { return assign(arr); }
	String& operator=(char ch) { return assign(1, ch); }

	void pop_back() { remove(size() - 1, 1); }
	void push_back(char ch) { insert(size(), 1, ch); }

	void pop_front() { remove(0, 1); }
	void push_front(char ch) { insert(0, 1, ch); }	// 复杂度较高，O(n)，慎用

	String& replace(size_type pos, size_type len, const String& str) { return replace(pos, len, str.c_str()); }
	String& replace(size_type, size_type, const char*);
	String& replace(size_type, size_type, size_type, char);

	String& replace(const char*, const char*, size_type count = npos);
	String& replace(const char* arr, const String& str, size_type count = npos) { return replace(arr, str.c_str(), count); }
	String& replace(const String& str, const char* arr, size_type count = npos) { return replace(str.c_str(), arr, count); }
	String& replace(const String& str, const String& str2, size_type count = npos) { return replace(str.c_str(), str2.c_str(), count); }
	String& replace(const String& str, size_type n, char ch, size_type count = npos) { return replace(str.c_str(), n, ch, count); }
	String& replace(const char*, size_type, char, size_type count = npos);

	String& rreplace(const char*, const char*, size_type count = npos);
	String& rreplace(const char* arr, const String& str, size_type count = npos) { return rreplace(arr, str.c_str(), count); }
	String& rreplace(const String& str, const char* arr, size_type count = npos) { return rreplace(str.c_str(), arr, count); }
	String& rreplace(const String& str, const String& str2, size_type count = npos) { return rreplace(str.c_str(), str2.c_str(), count); }
	String& rreplace(const String& str, size_type n, char ch, size_type count = npos) { return rreplace(str.c_str(), n, ch); }
	String& rreplace(const char*, size_type, char, size_type count = npos);

	reference back() { return get(size() - 1); }
	const_reference back() const { return get(size() - 1); }
	reference front() { return get(0); }
	const_reference front() const { return get(0); }

	size_type size() const { return m_len; }
	size_type length()const { return size(); }
	bool empty() const { return size() == 0; }

	size_type capacity() const { return m_capacity; }

	void reserve(size_type);
	void resize(size_type, char ch = '\0');

	void shrink_to_fit();

	void swap(String&);

	String& trim();

	bool startWith(const String& str) const { return startWith(str.c_str()); }
	bool startWith(const char*) const;

	bool endOf(const String& str) const { return endOf(str.c_str()); }
	bool endOf(const char*) const;


public:
	class Locator {
	public:
		typedef String::size_type size_type;
		Locator(size_type len) :m_len(len) { }
		virtual size_type operator()(size_type) const = 0;
	protected:
		size_type m_len;
	};

private:
	void grow(size_type n = 0);		// 传入的容量不包括'\0'，即要分配的空间为形参+1，若形参为0，则现有容量翻倍

	void getNext(size_type*, const char*, const Locator&) const;
	size_type kmp_find(const char*, size_type, const Locator&, const Locator&) const;

public:
	static const size_type npos;

private:
	char *m_content;
	size_type m_len;
	size_type m_capacity;	// 不计算最后的'\0'，亦即最大容量为(size_t)(-2)
};

void swap(String& a, String& b);

String operator+(const char*, const String&);
String operator+(const String&, const String&);
String operator+(const String&, const char*);
String operator+(const String&, char);
String operator+(char, const String&);

bool operator==(const char*, const String&);
bool operator==(const String&, const String&);
bool operator==(const String&, const char*);

bool operator!=(const String&, const String&);
bool operator!=(const String&, const char*);
bool operator!=(const char*, const String&);

bool operator<(const char*, const String&);
bool operator<(const String&, const String&);
bool operator<(const String&, const char*);

bool operator<=(const String&, const String&);
bool operator<=(const String&, const char*);
bool operator<=(const char*, const String&);

bool operator>(const String&, const String&);
bool operator>(const String&, const char*);
bool operator>(const char*, const String&);

bool operator>=(const String&, const String&);
bool operator>=(const String&, const char*);
bool operator>=(const char*, const String&);

DSLIB_END

std::ostream& operator<< (std::ostream&, const DSLib::String&);
std::istream& operator >> (std::istream&, DSLib::String&);

#endif // !STRING_H
