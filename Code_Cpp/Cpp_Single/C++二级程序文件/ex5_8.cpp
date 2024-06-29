#include <iostream>
using namespace std;

void swap_refer(int& x, int& y)
{
   	int temp = x;
   	x = y;
   	y = temp;
}

void swap_point(int *x, int *y)
{
    int temp = *x;
	*x = *y;
	*y = temp;	
}

int main()
{
    int a = 2, b = 3;
	swap_point(&a, &b);
	cout << a << " " << b << endl;
	swap_refer(a, b);
	cout << a << " " << b << endl;
	return 0;	
}
