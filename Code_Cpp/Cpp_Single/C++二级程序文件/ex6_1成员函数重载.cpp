#include <iostream>
using namespace std;

class Test
{
	public:
		void SetVal(int x);         // 成员函数重载 
		void SetVal(int x, int y);   
		int Add(int x, int y);      // 成员函数重载 
		int Add(int x);
		int Add();
		int GetX() { return X;}     // 内联函数 
		int GetY() { return Y;}     // 内联函数 
	private:
		int X, Y;
};
void Test::SetVal(int x, int y)
{
	X = x;
	Y = y;
}
void Test::SetVal(int x)
{
	X = x;
	Y = x*x;
}
int Test::Add(int x, int y)
{
	X = x;
	Y = y;
	return X+Y;
 } 
int Test::Add(int x)
{
	X = Y = x;
	return X+Y;
}
int Test::Add()
{
	return X+Y;
}

int main()
{
	Test obj1, obj2;
	obj1.SetVal(10, 20); // 10 20
	obj2.SetVal(4);      // 4 16
	cout << "obj1=" << obj1.GetX() << ',' << obj1.GetY() << endl;
	cout << "obj2=" << obj2.GetX() << ',' << obj2.GetY() << endl;
	int i=obj1.Add();
	int j=obj2.Add(3, 9);
	int k=obj2.Add(5);
	cout << i << endl << j << endl << k << endl;
	return 0;
}








