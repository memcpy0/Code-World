#include <iostream>
using namespace std;

int max(int, int);

int main()
{
	int num, maxnum;
	cout << "Enter 10 integer number." << endl;
	cin >> num;
	maxnum = num;
	for (int i = 1; i < 10; cin >> num, maxnum = max(maxnum, num), i++);
	cout << "The max number is " << maxnum << endl;
	return 0;
}

int max(int m, int n)
{
	return m>n ? m : n;
}
