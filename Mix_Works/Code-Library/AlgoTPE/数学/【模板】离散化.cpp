#include <iostream>
using namespace std;
const int MAXN = 100;
int main() {
	int idx[MAXN], L[MAXN];
	memcpy(idx, A, sizeof(A)); //����
	sort(idx, idx + n); //��С�������� //�Ӵ�С��һ��greater<int>()
	int len = unique(idx, idx + n) - idx; //ȥ�� //�õ����ظ�Ԫ������ĳ���/���� 
	for (int i = 0  i < n; ++i)
		L[i] = lower_bound(idx, idx + len, A[i]) - idx + 1; //���ֲ���, +1��Ϊ����������1��ʼ
		
	return 0;
}
