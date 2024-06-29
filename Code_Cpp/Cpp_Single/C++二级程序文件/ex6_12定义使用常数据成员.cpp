#include <iostream>
using namespace std;

class MyClass
{
	public:
		MyClass(int i, int j);
		void Print() const { cout << a << ", " << b << endl;}
	private:
		const int a;
		int b;
};
MyClass::MyClass(int i, int j):a(i), b(j) { }

int main()
{
	MyClass obj(1, 2);
	obj.Print();
	return 0;
}
