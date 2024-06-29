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

STRING& STRING::operator=(const STRING& s)  // ����=Ϊ��Ա����, �ı��һ������,�������� 
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
ostream& operator<<(ostream& os, const STRING &str) // ֻ������<<Ϊ�ǳ�Ա���� 
{
	if (str.p != NULL) os<<str.p;
	return os;
}

int main()
{
	STRING s1("This is a string."), s2;
	cout << "s1��s2�ĳ�ֵ��" << endl;
	cout << "s1: " << s1 << endl;
	cout << "s2: " << s2 << endl;
	s2 = s1;
	cout << "��s1��ֵ��s2��" << endl;
	cout << "s1: " << s1 << endl;
	cout << "s2: " << s2 << endl;
    s1.replace(8, 'A');
	cout << "��s1�е��ַ�a�滻ΪA֮��" << endl;  // Ϊ��ʹ��ֵ��s2������Ϊs1�ĸ���, ����ר�����ظ�ֵ����� 
	cout << "s1: " << s1 << endl;
	cout << "s2: " << s2 << endl; 
	s2 = "XYZ"; 
	cout << "��C�ַ�����ֵ��s2��" << endl;
	cout << "s2: " << s2 << endl;
	return 0; 
}
