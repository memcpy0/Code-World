#include <iostream>
#include <cstdlib>  //使用库函数exit
using namespace std;
int main()
{
	int arraySize;
	int *array;          //用于保存数组首地址 
	cout << "Please input the size of the array.";
	cin >> arraySize;
	array = new int[arraySize];
	if (array == NULL) 
	{
		cout << "Cannot allocate more memory, exit the program.\n";
		exit(1);
    } 
    for (int i=0; i < arraySize; array[i] = i*i, i++);
    for (int i=0; i < arraySize; cout << array[i] << " ", i++); 
	cout << endl;
	delete [] array;
	return 0; 
 } 
