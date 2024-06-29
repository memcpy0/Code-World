#include <iostream>
using namespace std;

class Test
{
	public:
		Test();
		Test(int i, int j);
		~Test() {
			cout << "Destructor called: " << X << ", " << Y << endl;
		}
		void Set(int i, int j);
		void Print() {
			cout << X << ", " << Y << endl;
		}
	private:
		int X, Y;
};
Test::Test():X(0), Y(0) { cout << "Default constructor called." << endl;}
Test::Test(int i, int j):X(i), Y(j) 
{
	cout << "Constructor called:" << X << "," << Y << endl;
}
void Test::Set(int i, int j)
{
	X = i; 
	Y = j;
}
int main()
{
	Test *p1, *p2, *p3;
	p1 = new Test(1, 2);   
	p2 = new Test(3, 4);
	p3 = new Test[2];
	p3[0].Set(5, 6);
	p3[1].Set(7, 8);
	p1->Print();
	p2->Print();
	p3[0].Print();
	p3[1].Print();
	delete p1;
	delete p2;
	delete [] p3;
	return 0;
}













