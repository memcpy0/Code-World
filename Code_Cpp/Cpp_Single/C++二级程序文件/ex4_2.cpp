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
	cout << "ƽ��ֵ��:" << avg/5 << '\n' << "���ֵ��:" << max << '\n' << "��Сֵ��:" << min << endl;
	return 0; 
}
