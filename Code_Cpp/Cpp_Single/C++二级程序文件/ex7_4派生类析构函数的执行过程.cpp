#include <iostream>

using namespace std;

class Base
{
	public:
		Base():b1(0), b2(0) {	}
		Base(int i, int j);
	    ~Base();
	    void Print() const { cout << b1 << ", " << b2 << ", ";}
    private:
    	int b1, b2;
};
Base::Base(int i, int j):b1(i), b2(j)
{
	cout << "Base's constructor called." << b1 << ", " << b2 <<endl;
} 
Base::~Base()
{
	cout << "Base's deconstructor called." << b1 << ", " << b2 << endl; 
}

class Derived:public Base
{
	public:
		Derived():d(0) { }
		Derived(int i, int j, int k);
		~Derived();
		void Print() const;
	private:
		int d;
}; 
Derived::Derived(int i, int j, int k):Base(i, j), d(k)
{
	cout << "Derived's constructor called." << d << endl;
}
Derived::~Derived()
{
	cout << "Derived's destructor called." << d << endl;
}
void Derived::Print() const
{
	Base::Print();
	cout << d << endl;
}  
int main()
{
	Derived objD1(1, 2, 3), objD2(-4, -5, -6);
	objD1.Print();
	objD2.Print();
	return 0;
}









 
