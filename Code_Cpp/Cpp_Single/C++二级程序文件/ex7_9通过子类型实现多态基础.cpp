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
	cb.Print();  // ʵ���ϵ��õ�����Base�е�print����, ����������Ķ�����Ϊ����Ķ�����ʹ��, ����ֻ��ʹ�ôӻ���̳еĳ�Ա 
}

int main()
{
	Derived obj;
	fun(obj);
	return 0;
}
