#include <iostream>
using namespace std;
int n, m;
void remove(int a[], int t) { //ɾ������a���±�Ϊt���� 
	if (t < 0 || t >= n) return;
	for (int i = t; i < n - 1; ++i)
		a[i] = a[i + 1]; 
}
//��ӡ����, �������Ժ�ʵ�� 
void print(int a[]) { for (int i = 0; i < n; ++i) printf("%d ", a[i]); }


int main() {
	cin >> n >> m;
	int arr[n];
	//��ʼ��Ϊ1-n
	for (int i = 0; i < n; ++i) arr[i] = i + 1;
	//���������, ����Ҫ������һ��һ��������ȥ 
 	int pos = 0; //��ʼ��Ϊ0����Ϊ�����0��ʼ; ȡ�������õ�0-(n-1)�����ķ�Χ 
	while (n > 1) {
		pos = (pos + m - 1) % n; //�õ�pos����ĵ�m-1�������±�, ȡ�಻�ϱ仯��n 
		print(arr); printf(" | a[%d] = %d\n", pos, arr[pos]); 
		remove(arr, pos); //ɾ�������; posָ���λ�����ڷŵ�����һ���� 
		--n; //����һ���� 
	}
	cout << arr[0]; 
	return 0;	
}
