#include <iostream>
#include <algorithm>
using namespace std;

int x;  // x ȫ�ֱ�����Ĭ��Ϊ��̬static��ϵͳ�Զ�����ֵ0 
int main() 
{
	int y; // y �ֲ�������Ĭ��Ϊauto����ȷ����ֵ 
    y = x + 2;
	cout << "y = " << y << endl;
	
	int a = 1, b = 5;
	if (b >= 3) {
		int a = 3;  // �����ľֲ������������ϲ���� 
		b = 2;
		cout << "local int a = " << a << endl;
	} 
	cout << "int a = " << a << endl;
	cout << "int b = " << b << endl;
	return 0; 
} 
