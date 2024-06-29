#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	double radius, girth, area;
	cin >> radius;
	girth = 2 * 3.14159 * radius;  // 圆的周长表达式
	area = radius * radius * 3.14159;  // 圆的面积 
	cout << "radius = " << radius << endl;  
	cout << "girth = " << girth << endl;
	cout << "area = " << area << endl; 
	return 0;
 } 
