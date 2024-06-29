#include <iostream>
using namespace std;

int main()
{
	int intOne;
	//创建和使用引用
	int& rInt = intOne;
	intOne = 5;
	cout << "intOne:" << intOne << endl;
	cout << "rInt:" << rInt << endl;
	rInt = 7;
	cout << "intOne:" << intOne << endl;
	cout << "rInt:" << rInt << endl;
	return 0; 
 } 
