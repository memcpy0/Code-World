#include <iostream>
using namespace std;

void hanoi(int n, char x, char y, char z) //n�����ӣ�x,y,z��������
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
	cout << "�����뺺ŵ�����ӵ�������";
	cin >> n;
	hanoi(n, 'A', 'B', 'C'); 
	return 0;
 } 
