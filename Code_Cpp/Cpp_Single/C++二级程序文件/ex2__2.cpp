#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	double radius, girth, area;
	cin >> radius;
	girth = 2 * 3.14159 * radius;  // Բ���ܳ����ʽ
	area = radius * radius * 3.14159;  // Բ����� 
	cout << "radius = " << radius << endl;  
	cout << "girth = " << girth << endl;
	cout << "area = " << area << endl; 
	return 0;
 } 
