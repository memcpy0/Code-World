#include <iostream>
#include "complex.h"
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	complex<double> c1(2, 1);
	complex<int> c2;
//	c2.func(c1);
	cout << imag(c1) << real(c1);
	
	cout << c1 << endl;
	cout << c2 << endl;
	
	c2 = c1 + 5;
	c2 = 7 + c1;
	c2 = c1 + c2;
	
	c2 += c1;
	c2 += 3;
	c2 = -c1;
	
	cout << (c1 == c2) << endl;
	cout << (c1 != c2) << endl;
	cout << conj(c1) << endl;
	return 0;
}