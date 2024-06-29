#include <iostream>
using namespace std;

class Base1
{
	public:
		Base1(int i):b1(i) {
			cout << "Constructor of Base1." << endl;
		}
		void Print() const { cout << b1 << endl; }
	private:
		int b1; 
};
class Base2
{
	public:
		Base2(int i):b2(i) {
			cout << "Constructor of Base2." << endl;
		} 
		void Print() const{ cout << b2 << endl;}
	private:
		int b2;
};
class Base3
{
	public:
		Base3():b3(0) {
			cout << "Default constructor of Base3." << endl;
		} 
		void Print() const {cout << b3 << endl;}
	private:
		int b3;
};
class Member
{
	public:
		Member(int i):m(i) {
			cout << "Constructor of Member." << endl; }
		int GetM() const { return m; }
	private:
		int m;
};
class Derived:public Base2, public Base1, public Base3
{
	public:
	    Derived(int i, int j, int k, int l);
		void Print() const; 
	private:
		int d;
		Member mem;
}; 
Derived::Derived(int i, int j, int k, int l):Base1(i), Base2(j), mem(k), d(l)
{	
	cout << "Constructor of Derived." << endl;
}
void Derived::Print() const
{
	Base1::Print();
	Base2::Print();
	Base3::Print();
	cout << mem.GetM() << endl;
	cout << d << endl;
}
int main()
{
	Derived obj(1, 2, 3, 4);
	obj.Print();
	return 0;
}
