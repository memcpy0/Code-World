**类别**	
综合

**时间限制**	
2S

**内存限制**	
256Kb

**问题描述**	
两个寻宝者找到一个宝藏，里面包含n件物品，每件物品的价值分别是W[0]，W[1]，…W[n-1]。
SumA代表寻宝者A所获物品价值总和，SumB代表寻宝者B所获物品价值总和，请问怎么分配才能使得两人所获物品价值总和差距最小，即两人所获物品价值总和之差的绝对值|SumA - SumB|最小。

**输入说明**	
输入数据由两行构成：
第一行为一个正整数n，表示物品个数，其中0<n<=200。
第二行有n个正整数，分别代表每件物品的价值W[i]，其中0<W[i]<=200。

**输出说明**	
对于每组数据，输出一个整数|SumA-SumB|，表示两人所获物品价值总和之差的最小值。

**输入样例**	
```cpp
4
1 2 3 4
```
**输出样例**	
```cpp
0
```

---
### 解法 01背包
要使两人所获物品价值总和差距最小，只需使一人在 $n$ 件物品中选出部分物品，并使其价值总和最接近 $\dfrac{\sum {w_i} } {2}$ 。因此，可使用01背包解决：
```cpp
#include <bits/stdc++.h>
using namespace std;
int m, n, a[1000], tot = 0;
int dp[100000];
int main() {
	cin >> n;
	for (int i = 1; i <= n; ++i) { cin >> a[i]; tot += a[i]; }
	int bound = tot / 2;
	for (int i = 1; i <= n; ++i)
		for (int j = bound; j >= a[i]; --j)
			dp[j] = max(dp[j], dp[j - a[i]] + a[i]);
	cout << (tot - dp[bound] - dp[bound]) << endl;
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/9d2f3a6953f14466aaf97851ac205262.png)

