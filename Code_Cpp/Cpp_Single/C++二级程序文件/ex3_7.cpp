#include <iostream>
#include <algorithm>
using namespace std;

int main() 
{
	unsigned int n;
	do {
		cout << "Please input a number(0 to end):";
		cin >> n;
		cout << "Your number is:" << n << '\n'; 
	} while (n != 0);
	return 0;
}
