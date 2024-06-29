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
		cout << "请输入一个非负数: ";
		cin >> k; 
	} while (k < 0);
	cout << k << " 阶乘是 " << fact(k) << endl; 
}
