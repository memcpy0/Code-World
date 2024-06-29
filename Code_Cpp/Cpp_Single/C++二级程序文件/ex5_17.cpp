#include <iostream>
using namespace std;

int i = 0;
int main()
{
	int i = 1;
	cout << "i = " << i << endl;
	{
		int i = 2;
		cout << "i = " << i << endl;
		{
			i += 1;
			cout << "i = " << i << endl;
		}
		cout << "i = " << i << endl;
	}
	cout << "i = " << i << endl;
	return 0; 
}
