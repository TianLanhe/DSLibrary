#ifndef STRING_H
#define STRING_H

#include "../Object.h"

DSLIB_BEGIN

class String : public Object<char> {

public:
	String();
	//String(const String&);
	//String(const char*);
	//String(size_type, char);
	~String() { delete[] m_content; }

	String& append(const char*);
	String& append(const String& str) { return append(str.c_str()); }
	String& append(size_type, char);

	//String& assign(const char*);
	//String& assign(const String&);
	//String& assign(size_type, char);

	//int compare(const String&) const;
	//int compare(const char*) const;

	const char* c_str() const { return m_content; }

	//String substr(size_type pos = 0, size_type len = npos) const;

	//size_type copy(char*, size_type, size_type pos = 0) const;

	reference get(size_type);
	const_reference get(size_type) const;
	reference at(size_type i) { return get(i); }
	const_reference at(size_type i) const { return get(i); }
	void set(size_type, const_reference);

	reference operator[](size_type i) { return get(i); }
	const_reference operator[](size_type i) const { return get(i); }

	void clear();

	//String& erase(size_type, size_type);

	//size_type find(const String&, size_type pos = 0)const;
	//size_type find(const char*, size_type pos = 0)const;
	//size_type find(char, size_type pos = 0)const;

	//size_type rfind(const String&, size_type pos = npos)const;
	//size_type rfind(const char*, size_type pos = npos)const;
	//size_type rfind(char, size_type pos = npos)const;

	//size_type find_first_not_of(const String&, size_type pos = 0)const;
	//size_type find_first_not_of(const char*, size_type pos = 0)const;
	//size_type find_first_not_of(char, size_type pos = 0)const;

	//size_type find_first_of(const String&, size_type pos = 0)const;
	//size_type find_first_of(const char*, size_type pos = 0)const;
	//size_type find_first_of(char, size_type pos = 0)const;

	//size_type find_last_not_of(const String&, size_type pos = 0)const;
	//size_type find_last_not_of(const char*, size_type pos = 0)const;
	//size_type find_last_not_of(char, size_type pos = 0)const;

	//size_type find_last_of(const String&, size_type pos = 0)const;
	//size_type find_last_of(const char*, size_type pos = 0)const;
	//size_type find_last_of(char, size_type pos = 0)const;

	//String& insert(size_type, const String&);
	//String& insert(size_type, const char*);
	//String& insert(size_type, size_type, char);

	String& operator+=(const String& str) { return append(str); }
	String& operator+=(const char* arr) { return append(arr); }
	String& operator+=(char ch) { return append(1, ch); }

	//String& operator=(const String&);
	//String& operator=(const char*);
	//String& operator=(char);

	//void pop_back();
	//void push_back();

	//void pop_front();
	//void push_front();	// 复杂度较高，O(n)，慎用

	//String& replace(size_type, size_type, const String&);
	//String& replace(size_type, size_type, const char*);
	//String& replace(size_type, size_type, size_type, char);

	reference back() { return get(size() - 1); }
	const_reference back() const { return get(size() - 1); }
	reference front() { return get(0); }
	const_reference front() const { return get(0); }

	size_type size() const { return m_len; }
	size_type length()const { return size(); }
	bool empty() const { return size() == 0; }

	size_type capacity() const { return m_capacity; }

	//void reserve(size_type);
	void resize(size_type n) { return resize(n, '\0'); }
	void resize(size_type, char) {}

	void shrink_to_fit();

	void swap(String&);

private:
	void grow(size_type n = 0);

public:
	static const size_type npos;

private:
	char *m_content;
	size_type m_len;
	size_type m_capacity;
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

#endif // !STRING_H
