#include <iostream>
using namespace std;

int main()
{
	int iArray[10];
	iArray[0] = 1;
	iArray[1] = 1;
	for (int i=2; i<10; i++)
		iArray[i] = iArray[i-2] + iArray[i-1];
	for (int j=0; j<10; j++) cout << iArray[j] << ' ';
	return 0;
}
