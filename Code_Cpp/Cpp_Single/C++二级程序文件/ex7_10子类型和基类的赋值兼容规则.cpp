#include <iostream>
using namespace std;

class Base
{
	public:
		void Who() {
			cout << "class Base." << endl;
 	}
};
class Derived1: public Base
{
	public:
		void Who() {
			cout << "class Derived1." << endl;
		}
};
class Derived2: public Base
{
	public:
		void Who() {
			cout << "class Derived2." << endl;
		}
};
int main()
{
	Base obj1, *p;
	Derived1 obj2;
	Derived2 obj3;
	p = &obj1;
	p->Who();
	p = &obj2;
	p->Who();
	p = &obj3;
	p->Who();
	obj2.Who();
	obj3.Who();
	
	return 0;
}
