#include <iostream>
using namespace std;

class Base
{
    public:
    	void Print() const { cout << "Base::Print() called." << endl;}
}; 
class Derived:public Base
{
	public:
		void Print() const { cout<< "Derived::Print() called." << endl;}
}; 
void fun(Base &cb)
{
	cb.Print();  // 实际上调用的是类Base中的print函数, 公共派生类的对象作为基类的对象来使用, 它就只能使用从基类继承的成员 
}

int main()
{
	Derived obj;
	fun(obj);
	return 0;
}
