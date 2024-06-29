#include <iostream>
using namespace std;

void myfunc(int x = 0, int y = 1)
{
	cout << "x: " << x << " y: " << y << endl;
}

int main()
{
    myfunc(2, 3);
	myfunc(2);
	myfunc();
	return 0;	
}
