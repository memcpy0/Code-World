#include <iostream>
using namespace std;

const int Rows = 4;
const int Cols = 4;

int main()
{
	int sum;
	int matrix[Rows][Cols] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	
	for (int row = 0; row < Rows; row++) {
		for(int col = 0; col < Cols; cout.width(5), cout<<matrix[row][col]<<" ", col++); 
		cout<<endl;
	}
	// 行和 
	for (int row = 0; row < Rows; row++) 
	{
		sum = 0;
		for(int col=0; col<Cols; sum += matrix[row][col], col++);
		cout<<"Sum of the "<<row+1<<" row: "<< sum << endl;
	}
	// 列和 
	for (int col = 0; col < Cols; col++)
	{
		sum = 0;
	    for(int row=0; row<Rows; sum += matrix[row][col], row++);
		cout<<"Sum of the "<<col+1<<" col: "<< sum << endl;
	}
	sum = 0;
	for (int row = 0; row < Rows; sum+=matrix[row][row], row++);
	cout << "Sum of the main diagonal: " << sum << endl;
	
	sum = 0;
	for (int row = 0; row < Rows; sum+=matrix[row][Rows-1-row], row++);
	cout << "Sum of the opposite diagonal: " << sum << endl;
	
	return 0;
}
