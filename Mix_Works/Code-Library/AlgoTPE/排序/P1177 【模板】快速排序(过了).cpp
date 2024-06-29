#include <iostream>
#include <algorithm>
using namespace std;

int n, a[100001];
void qsort(int l, int r) { //使用二分
	int mid = a[(l + r) / 2];
	int i = l, j = r;
	do {
		while (a[i] < mid) ++i; //查找左半边比中间数大的数
		while (a[j] > mid) --j; //查找右半边比中间数小的数
		if (i <= j) { //满足i <= j 
			swap(a[i], a[j]); //交换过后, 有a[i]<=mid, a[j]>=mid 
			++i; //强制加1减一 
			--j;
		}
	} while (i <= j); //这里注意要有=
	
	if (l < j) qsort(l, j); //递归搜索左半部分 //有个判断！！！ 
	if (i < r) qsort(i, r); //递归搜索右半部分
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; ++i) cin >> a[i];
	qsort(1, n);
	for (int i = 1; i <= n; ++i)
		cout << a[i] << " ";
	return 0;
} 
