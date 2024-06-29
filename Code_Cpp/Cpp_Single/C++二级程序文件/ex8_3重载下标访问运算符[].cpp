#include <iostream>
#include <iomanip> 
#include <stdlib.h>
#ifndef IntarrayH
#define IntarrayH
using namespace std;

class IntArray{
	int _size;
	int *data;
	public:
		IntArray(int n);
		~IntArray() {
			delete [] data;
		} 
		int& operator[](int i);
		int size() {
			return _size;
		}
};
#endif

IntArray::IntArray(int n)
{
	if (n < 1) {
		cout << "Error dimension description.";
		exit(1);
	}
	_size = n;
	data = new int[_size];
}
int& IntArray::operator[](int i)
{
	if (i < 1 || i > _size) {
		cout << endl << "Subscript out of range.";
		delete []data;
		exit(2); 
	}
	return data[i-1];       // IntArray的下标从1开始 
} 

int main()
{
	IntArray a(10);
	for (int i=1; i<=a.size(); i++) a[i]=i*10;
	for (int i=1; i<=a.size(); i++) cout << setw(5) << a[i];
	return 0; 
}

