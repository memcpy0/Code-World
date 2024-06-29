#include <iostream>
using namespace std;

void hanoi(int n, char x, char y, char z) //n个盘子，x,y,z三根柱子
{
	if (n > 0) {
		hanoi(n-1, y, z);
		cout << "Move from " << x
		hanoi(n-1, x, y); 
	}
} 

int main()
{
	int n;
	cout << "请输入汉诺塔盘子的数量：";
	cin >> n;
	hanoi(n, 'A', 'B', 'C'); 
	return 0;
 } 
