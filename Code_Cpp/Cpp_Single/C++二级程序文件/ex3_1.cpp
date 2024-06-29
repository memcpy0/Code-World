#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	cout << "Please input 3 numbers a, b and c";
	double a, b, c;
	cin >> a >> b >> c;
	double sum = a + b + c;
	cout << "a = " << a << '\t' << "b = " << b << '\t' << "c = " << c << '\n';
	cout << "The sum of a, b and c is " << sum << endl;
	return 0;
 } 
