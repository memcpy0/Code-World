#include <iostream>
using namespace std;

int main() {
	int n, m;
	cin >> n >> m;
	int a[n];
	for (int i = 0; i < n; ++i) a[i] = i + 1;
	int kth = 1, cnt = n; //kthΪ�ڼ���, cntȷ������ʵ�ʴ�С
	while (cnt > 1) {
		for (int i = 0; i < n; ++i) { //û��ʵ��ɾ��Ԫ��, n���� 
			if (a[i] != 0 && kth++ % m == 0) { //��0���ɾ��; ����0�ʹ������ִ��Ԫ��
				a[i] = 0; //���ɾ��
				--cnt; //�ִ�Ԫ����Ŀ-1
			}
		}
	}
	int i = 0;
	while (i < n && a[i] == 0) ++i;
	cout << a[i]; 
	return 0;
}
//#include <iostream>
//using namespace std;
//
//int main() {
//	int n, m;
//	cin >> n >> m;
//	int a[n];
//	for (int i = 0; i < n; ++i) a[i] = i + 1;
//	int kth = 1, cnt = n; //kthΪ�ڼ���, cntΪʵ�ʵ�����Ԫ������
//	while (cnt > 1) {
//		for (int i = 0; i < n; ++i) {
//			if (a[i] != 0 && kth++ % m == 0) { //kth��1-m, Ȼ�󲻿���ֱ��ȡ��m
//				a[i] = 0; 
//				--cnt; 
//				kth = 1; //kth�ֱ��1 
//			}
//		}
//	}
//	int i = 0; 
//	while (i < n && a[i] == 0) ++i;
//	cout << a[i];
//	return 0;
//}
