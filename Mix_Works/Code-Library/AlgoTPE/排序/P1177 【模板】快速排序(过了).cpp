#include <iostream>
#include <algorithm>
using namespace std;

int n, a[100001];
void qsort(int l, int r) { //ʹ�ö���
	int mid = a[(l + r) / 2];
	int i = l, j = r;
	do {
		while (a[i] < mid) ++i; //�������߱��м��������
		while (a[j] > mid) --j; //�����Ұ�߱��м���С����
		if (i <= j) { //����i <= j 
			swap(a[i], a[j]); //��������, ��a[i]<=mid, a[j]>=mid 
			++i; //ǿ�Ƽ�1��һ 
			--j;
		}
	} while (i <= j); //����ע��Ҫ��=
	
	if (l < j) qsort(l, j); //�ݹ�������벿�� //�и��жϣ����� 
	if (i < r) qsort(i, r); //�ݹ������Ұ벿��
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; ++i) cin >> a[i];
	qsort(1, n);
	for (int i = 1; i <= n; ++i)
		cout << a[i] << " ";
	return 0;
} 
