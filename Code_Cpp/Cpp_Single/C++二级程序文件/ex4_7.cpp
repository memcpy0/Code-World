#include <iostream>
using namespace std;

int main()
{
	int *iptr, iArray[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	double *fptr, fArray[10] = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
	iptr = iArray;
	fptr = fArray;
	for (int x = 0; x < 10; x++)
		cout<<iptr+x<<" "<< *(iptr+x) <<"\t"<<fptr+x<<" "<<*(fptr+x)<<endl;
	return 0;
}
