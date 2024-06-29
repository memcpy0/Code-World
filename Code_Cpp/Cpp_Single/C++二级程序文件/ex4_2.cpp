#include <iostream>
using namespace std;

int main()
{
	int vn[5] = {13, 44, -12, 39, 1};
	int avg=0, max, min; 
	max = min = vn[0];
	for (int i=0; i<5; i++) 
	{
		avg += vn[i];
		if (vn[i] < min) min = vn[i];
		else if (vn[i] > max) max = vn[i];
	}
	cout << "平均值是:" << avg/5 << '\n' << "最大值是:" << max << '\n' << "最小值是:" << min << endl;
	return 0; 
}
