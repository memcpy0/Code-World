#include <iostream>
#include <algorithm>
using namespace std;

int x;  // x 全局变量，默认为静态static，系统自动赋初值0 
int main() 
{
	int y; // y 局部变量，默认为auto，无确定初值 
    y = x + 2;
	cout << "y = " << y << endl;
	
	int a = 1, b = 5;
	if (b >= 3) {
		int a = 3;  // 声明的局部变量遮掩了上层变量 
		b = 2;
		cout << "local int a = " << a << endl;
	} 
	cout << "int a = " << a << endl;
	cout << "int b = " << b << endl;
	return 0; 
} 
