#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int a, b, max;
	cout << "Please input two integers";
	cin >> a >> b;
//	if (a > b) max = a;
//	else max = b;
	cout << "The max is " << (a > b ? a : b) << endl; 
	return 0;
}
