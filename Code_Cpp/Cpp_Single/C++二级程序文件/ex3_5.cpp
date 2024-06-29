#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	const double pi = 3.14159;
	for (int r = 5; r > 0; cout << "R = " << r << '\t' << "S = " << pi * r * r << endl, r--);
	return 0;
}
