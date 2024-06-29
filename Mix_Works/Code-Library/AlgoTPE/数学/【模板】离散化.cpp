#include <iostream>
using namespace std;
const int MAXN = 100;
int main() {
	int idx[MAXN], L[MAXN];
	memcpy(idx, A, sizeof(A)); //复制
	sort(idx, idx + n); //从小到大排序 //从大到小加一个greater<int>()
	int len = unique(idx, idx + n) - idx; //去重 //得到不重复元素数组的长度/数量 
	for (int i = 0  i < n; ++i)
		L[i] = lower_bound(idx, idx + len, A[i]) - idx + 1; //二分查找, +1是为了让排名从1开始
		
	return 0;
}
