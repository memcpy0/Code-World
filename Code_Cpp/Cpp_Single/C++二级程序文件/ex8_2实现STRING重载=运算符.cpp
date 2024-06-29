#include <iostream>
#include <cstring>
#include <string>
using namespace std;

class STRING {
	char *p;
	public:
		STRING(const char *c=NULL);
	    ~STRING() {
		    if (p!=NULL) delete[] p;
		} 
		STRING(const STRING& s);
		friend ostream& operator<<(ostream& os, const STRING& str);
		void replace(int pos, char c);
		STRING& operator=(const STRING &); 
}; 
STRING::STRING(const char *c)
{
	if (c == NULL) p = NULL;
	else {
		p = new char[strlen(c)+1];
		strcpy(p, c);
	}
}
STRING::STRING(const STRING& s)
{
	if (s.p == NULL) p = NULL;
	else {
		p = new char[strlen(s.p)+1];
		strcpy(p, s.p);
	}
}
void STRING::replace(int pos, char c)
{
	if (pos < strlen(p)) p[pos] = c;
}

STRING& STRING::operator=(const STRING& s)  // 重载=为成员函数, 改变第一操作数,返回引用 
{
	if(&s == this) return *this;
	if (p) delete[] p;
	if (s.p == NULL) p = NULL;
	else {
        p = new char[strlen(s.p)+1];
        strcpy(p, s.p);
	} 
	return *this;
}
ostream& operator<<(ostream& os, const STRING &str) // 只能重载<<为非成员函数 
{
	if (str.p != NULL) os<<str.p;
	return os;
}

int main()
{
	STRING s1("This is a string."), s2;
	cout << "s1和s2的初值：" << endl;
	cout << "s1: " << s1 << endl;
	cout << "s2: " << s2 << endl;
	s2 = s1;
	cout << "将s1赋值给s2后：" << endl;
	cout << "s1: " << s1 << endl;
	cout << "s2: " << s2 << endl;
    s1.replace(8, 'A');
	cout << "将s1中的字符a替换为A之后：" << endl;  // 为了使赋值后s2真正成为s1的副本, 必须专门重载赋值运算符 
	cout << "s1: " << s1 << endl;
	cout << "s2: " << s2 << endl; 
	s2 = "XYZ"; 
	cout << "将C字符串赋值给s2后：" << endl;
	cout << "s2: " << s2 << endl;
	return 0; 
}
