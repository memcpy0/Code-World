#ifndef __STRING_H_
#define __STRING_H_
#define MAXCAPACITY 1100

#include <iostream>
#include <cstring>

class String {
protected:
	char *_data;
	int _size; 
	int _capacity;
	void copyFrom(const char *cstr, int b, int e);		//字符数组区间[b,e)复制 
	void expand();										//实现动态扩容
public:
	String(int c, int s, char ch);						//容量为c,规模为s,所有元素初始化为ch  
	String(const char *cstr);							//字符数组整体复制 
	String(const char *cstr, int b, int e);				//字符数组区间复制
	~String();					                        //析构函数, 释放内部空间
	
	int size() const;								    //返回字符串长度
	int capacity() const;								//返回字符串容量 
	bool empty() const; 								//判断字符串是否为空 
	void printStr();									//输出字符串 
	void getline(char delim);							//读入一行字符串 
	//重载部分运算符
	char& operator[] (int r) const;						//直接引用字符串元素 
	String& operator= (const String &str); 				//重载赋值运算符,整体复制字符串	
	bool operator< (const String &a) const; 			//重载<运算符,方便比较字符串大小 
 	friend std::ostream& operator<< (std::ostream &out, const String& str); //用流输出字符串 
};

//字符串类的实现部分
void String::copyFrom(const char *cstr, int b, int e) {	 
	_data = new char[_capacity = 2 * (e - b)];
	_size = 0;
	while (b < e) _data[_size++] = cstr[b++];
}
void String::expand() {
	if (_size < _capacity) return;
	char *oldData = _data;
	_data = new char[_capacity <<= 1];
	for (int i = 0; i < _size; ++i) _data[i] = oldData[i];
	delete [] oldData;
}

String::String(int c = MAXCAPACITY, int s = 0, char ch = 0) {   				
	_data = new char[_capacity = c];
	for (_size = 0; _size < s; _data[_size++] = ch);
}
String::String(const char *cstr) { copyFrom(cstr, 0, strlen(cstr)); }   
String::String(const char *cstr, int b, int e) { copyFrom(cstr, b, e); }
String::~String() { delete [] _data; }

inline int String::size() const { return _size; }
inline int String::capacity() const { return _capacity; }
inline bool String::empty() const { return !_size; }
void String::printStr() {
	for (int i = 0; i < _size; ++i) printf("%c", _data[i]);
}

void String::getline(char delim = '\n') {
	if (_data) delete [] _data;
	char *temp = new char[MAXCAPACITY], ch = getchar();
	int len = 0;
	while (ch != EOF && ch != delim) {
		temp[len++] = ch;
		ch = getchar();
	}
	copyFrom(temp, 0, len);
}

//重载部分运算符
char& String::operator[] (int r) const {  
	return _data[r];
}

String& String::operator= (const String &str) {				
	if (_data) delete [] _data;
	copyFrom(str._data, 0, str.size()); 
	return *this;
}

bool String::operator< (const String &str) const {
	int len = _size < str.size() ? _size : str.size();
	for (int i = 0; i < len; ++i) {
		if (_data[i] < str[i]) return true;
		else if (_data[i] > str[i]) return false;
	}
	return _size < str.size() ? true : false;           
}

std::ostream& operator<< (std::ostream &out, const String& str) {
	for (int i = 0; i < str.size(); ++i) out << str[i];
	return out;
}
#endif
