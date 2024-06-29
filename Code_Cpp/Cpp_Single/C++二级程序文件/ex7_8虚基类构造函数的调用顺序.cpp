#include <iostream>
using namespace std;

class Base
{
	public:
		Base(char i) {
			cout << "Base's cons." << i << endl;
		}
		~Base() {
			cout << "Base's des." << endl;
		}
};
class Derived11: virtual public Base
{
	public:
		Derived11(char i, char j):Base(i) {
			cout << "Derived11's cons." << j << endl;
		}
		~Derived11() {
			cout << "Derived11's des." << endl;
		}
};
class Derived12: virtual public Base
{
	public:
        Derived12(char i, char j):Base(i) {
        	cout << "Derived12's des." << j << endl;
		}
		~Derived12() {
			cout << "Derived12's des." << endl;
		}
};
class Derived2:public Derived11, public Derived12 
{ 
    public:
    	Derived2(char i, char j, char k, char l, char m, char n):Derived12(k, l), Derived11(i, j), Base(i), aa(m)
    	{
    		cout << "Derived2's cons." << n << endl; 
		}
		~Derived2() {
			cout << "Derived2's des." << endl;
		}
    private:
    	Base aa;
}; 

int main()
{
    Derived2 obj('a', 'b', 'c', 'd', 'e', 'f');
	return 0;	
}
