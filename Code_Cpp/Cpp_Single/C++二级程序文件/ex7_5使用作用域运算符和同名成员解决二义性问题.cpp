#include <iostream>
using namespace std;
/* �̳��п��ܻ���ַ��ʳ�Ա���������ݳ�Ա�Ķ����ԣ�����ʹ�����������������ͬ�������ڲ������д */
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
