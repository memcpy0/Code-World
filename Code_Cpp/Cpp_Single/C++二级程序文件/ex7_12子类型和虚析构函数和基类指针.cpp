#include <iostream>
#include <cstring>
using namespace std;

class AA {
	int i;
	public:
		AA(int n):i(n) {	};
		virtual ~AA() {	cout << "~AA被调用!" << endl;}
};

class BB:public AA{
	char *p;
	public:
		BB(int n, char *s):AA(n), p(s){
			if (!s) p=NULL;
			else {
				p=new char[strlen(s)]+1;
				strcpy(p, s);
			}
		}
		~BB() {
			// 通常只要派生类中对析构函数进行了专门的定义, 基类析构函数就应该改为虚函数 
			delete [] p; cout<<"~BB被调用！"<<endl; 
		}
};

int main()
{
	AA *p = new BB(5, NULL);
	delete p; // 用指向基类的指针析构派生类的对象 
	return 0;
}
