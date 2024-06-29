#include <iostream>
using namespace std;

int main()
{
	int x = 37;
	int *p = &x;
	cout << "Line 1: x = " << x << endl;
	cout << "Line 3: *p = " << *p << ", x =" << x << endl;
	*p = 58;
	cout << "Line 5: *p = " << *p << ", x = " << x << endl;
	cout << "Line 6: Address of p = " << &p << endl;
	cout << "Line 7: Value of p = " << p << endl;
	cout << "Line 8: Value of the memory location pointed to by *p = " << *p << endl;
	cout << "Line 9: Address of x = " << &x << endl;
	cout << "Line 10: Value of x = " << x << endl;
	return 0;
 } 
