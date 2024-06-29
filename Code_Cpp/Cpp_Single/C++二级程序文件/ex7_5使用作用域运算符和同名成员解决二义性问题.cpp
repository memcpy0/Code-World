#include <iostream>
using namespace std;
/* 继承中可能会出现访问成员函数和数据成员的二义性，可以使用作用域运算符或者同名函数内层变量改写 */
class Base1
{
	public:
		int data;
		void fun() {
			cout << "Member of Base1." << endl;
		}
};
class Base2
{
	public:
		int data; 
		void fun() {
			cout << "Member of Base2." << endl;
		}
};
class Derived: public Base1, public Base2
{
	public:
		int data;
		void fun() {
			cout << "Member of Derived." << endl;
		}
};

int main()
{
	Derived obj;
	obj.data = 1;
	obj.fun();
	obj.Base1::data = 2; 
	obj.Base1::fun();
	obj.Base2::data = 3;
    obj.Base2::fun();
    return 0;
}
