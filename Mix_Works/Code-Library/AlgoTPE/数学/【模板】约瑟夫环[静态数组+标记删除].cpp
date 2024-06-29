#include <iostream>
using namespace std;

int main() {
	int n, m;
	cin >> n >> m;
	int a[n];
	for (int i = 0; i < n; ++i) a[i] = i + 1;
	int kth = 1, cnt = n; //kth为第几个, cnt确定数组实际大小
	while (cnt > 1) {
		for (int i = 0; i < n; ++i) { //没有实际删除元素, n不变 
			if (a[i] != 0 && kth++ % m == 0) { //用0标记删除; 不是0就代表是现存的元素
				a[i] = 0; //标记删除
				--cnt; //现存元素数目-1
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
//	int kth = 1, cnt = n; //kth为第几个, cnt为实际的数组元素数量
//	while (cnt > 1) {
//		for (int i = 0; i < n; ++i) {
//			if (a[i] != 0 && kth++ % m == 0) { //kth从1-m, 然后不可能直接取余m
//				a[i] = 0; 
//				--cnt; 
//				kth = 1; //kth又变回1 
//			}
//		}
//	}
//	int i = 0; 
//	while (i < n && a[i] == 0) ++i;
//	cout << a[i];
//	return 0;
//}
