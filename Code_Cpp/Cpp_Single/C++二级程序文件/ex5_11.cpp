#include <iostream>
using namespace std;

int neg(const int& val)
{
	return -val;
}

int main()
{
	const int v = 10;
	cout << v << " negate is " << neg(v) << endl;
	return 0;
}
