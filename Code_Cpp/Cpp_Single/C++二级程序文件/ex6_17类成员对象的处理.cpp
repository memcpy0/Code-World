#include <iostream>
using namespace std;

class Counter
{
	public:
		Counter():val(0) { cout << "Default Constructor of Counter."<< endl;}
		Counter(int x):val(x) { cout << "Constructor of Counter:" << val << endl;}
		~Counter() {cout << "Destructor of Counter:" << val << endl;}
	private:
		int val;
};
class Example
{
	public:
		Example():val(0) {cout<<"Default Constructor of Example" << endl;}
		Example(int x):c2(x), val(x) {cout << "Constructor of Example:" << val << endl;}
		~Example() {cout << "Destructor of Example:" << val << endl;}
		void Print() {cout << "value=" << val << endl;}
	private:
		Counter c1, c2;
		int val;
};
int main()
{
	Example e1, e2(4);
	e2.Print();
	return 0;
}
