#include <bits/stdc++.h>
using namespace std;
const int maxn = 101001;
int n, a[maxn];
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
uniform_int_distribution<int> ud; // 均匀随机分布 

int PartitionRandom(int a[], int i, int j) {
	if (i >= j) return i;
	ud.param(uniform_int_distribution<>::param_type{i, j});
	int p = ud(generator);
	swap(a[p], a[i]); 
	int t = a[i];
	while (i < j) {
		while (i < j && a[j] >= t) --j;
		a[i] = a[j];
		while (i < j && a[i] <= t) ++i;
		a[j] = a[i]; 
	}
	a[i] = t;
	return i;
}
void QuickSortRandom(int a[], int l, int r) {
	if (l < r) {
		int mid = PartitionRandom(a, l, r);
		QuickSortRandom(a, l, mid - 1);
		QuickSortRandom(a, mid + 1, r);	
	}
}

int main() {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) 
		scanf("%d", &a[i]);
	QuickSortRandom(a, 0, n - 1);
	for (int i = 0; i < n; ++i) {
		if (i != 0) printf(" "); 
		printf("%d", a[i]); 
	}
	return 0;
}
