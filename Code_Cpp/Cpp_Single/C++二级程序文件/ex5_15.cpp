#include <iostream>
using namespace std;

int fact(int n)
{
	if (n <= 1) return 1;
	else return n*fact(n-1);
}

int main()
{
	int k;
	do {
		cout << "������һ���Ǹ���: ";
		cin >> k; 
	} while (k < 0);
	cout << k << " �׳��� " << fact(k) << endl; 
}
