#include <iostream>
#include <cmath>
using namespace std;

int main()
{
	const double err = 1e-6;
	int sign = 1;
	double pi = 0.0;
	double item = 1.0;
	double d = 1.0;
	while (fabs(item) >= err)
	{
		pi += item;
		d += 2.0;
		sign = -sign;
		item = sign * (1/d);
	}
	pi *= 4;
	cout << "PI = " << pi << endl;
	return 0;
}
