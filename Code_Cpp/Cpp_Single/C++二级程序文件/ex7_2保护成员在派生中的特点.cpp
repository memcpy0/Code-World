#include <iostream>
using namespace std;
class Base
{
    protected:
	    int data;	
};
class Derived: public Base 
{
	public:
		void fun() {
			data = 1;
		}
};

int main()
{
	Base objB;
	Derived odjD;
    // odjD.data = 2;
	// objB.data = 2;
	objD.fun();
	return 0;
}
