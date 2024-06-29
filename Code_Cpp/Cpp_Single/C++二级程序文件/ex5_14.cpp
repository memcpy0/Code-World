#include <iostream>
using namespace std;

inline int myMax(int x, int y)
{
	return x > y ? x : y; 
} 

int main()
{
	cout << myMax(3, 5) << endl;
	return 0;
}
