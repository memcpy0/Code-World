#include <iostream>
using namespace std;

void bubble(int v[], int size);

int main()
{
	int vn[] = {53, 3, 7, 5, 31, 16, 9, 75, 29, 36};
	int len = sizeof(vn) / sizeof(int); // 计算数组长度
	for (int i=0; i<len; cout << vn[i] << ',', i++);
	cout << endl << endl;
	bubble(vn, len);
	return 0;
}

void bubble(int v[], int size)
{
	int temp;
	for (int p=1; p<size; p++)
	{
		for (int i = 0; i<size-p; i++)
		{
			if (v[i] > v[i+1]) 
			{
				temp = v[i];
				v[i] = v[i+1];
				v[i+1] = temp;
			}
		}
		for (int i = 0; i<size; cout << v[i] << ",", i++);
		cout << endl;
	}
}
