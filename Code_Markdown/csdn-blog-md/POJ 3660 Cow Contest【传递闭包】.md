**Description**
N (1 ≤ N ≤ 100) cows, conveniently numbered 1..N, are participating in a programming contest. As we all know, some cows code better than others. Each cow has a certain constant skill rating that is unique among the competitors.

The contest is conducted in several head-to-head rounds, each between two cows. If cow A has a greater skill level than cow B (1 ≤ A ≤ N; 1 ≤ B ≤ N; A ≠ B), then cow A will always beat cow B.

Farmer John is trying to rank the cows by skill level. Given a list the results of M (1 ≤ M ≤ 4,500) two-cow rounds, determine the number of cows whose ranks can be precisely determined from the results. It is guaranteed that the results of the rounds will not be contradictory.

**Input**
Line 1: Two space-separated integers: N and M
Lines 2..M+1: Each line contains two space-separated integers that describe the competitors and results (the first integer, A, is the winner) of a single round of competition: A and B

**Output**
Line 1: A single integer representing the number of cows whose ranks can be determined
　

**Sample Input**
```cpp
5 5
4 3
4 2
3 2
1 2
2 5
```
**Sample Output**
```cpp
2
```
**Source**
[USACO 2008 January Silver](http://poj.org/searchproblem?field=source&key=USACO+2008+January+Silver)

题意：有许多牛，给出它们在编程比赛上的胜负关系，求出有多少只牛的排名无法确定。

---
### 解法 传递闭包
数据范围这么小，用传递闭包是最简单的。对于每个点，如果「其可达顶点的数目」和「可达它的顶点数目」两者之和为 `n - 1` ，就可以确定其排名了。

```cpp
#include <cstdio>
using namespace std;
const int maxn = 102;
int t, n, m, a, b;
bool g[maxn][maxn];

int main() {
	scanf("%d", &t);
	while (t--) {
		for (int i = 0; i < maxn; ++i)
			for (int j = 0; j < maxn; ++j)
				g[i][j] = false; //初始化
		scanf("%d%d", &n, &m);
		for (int i = 0; i < m; ++i) {
			scanf("%d%d", &a, &b);
			g[a][b] = true;
		}
		for (int k = 1; k <= n; ++k)
			for (int i = 1; i <= n; ++i)
				for (int j = 1; j <= n; ++j)
					if (g[i][k] && g[k][j])
						g[i][j] = 1;
		int cnt = 0;
		//to:检查顶点i可达多少个顶点
		//from:有多少个顶点可达i
		//如果有一个数>n/2,顶点的重量就不可能成为中位数
		for (int i = 1; i <= n; ++i) {
			int from = 0, to = 0;
			for (int j = 1; j <= n; ++j) {
				from += g[j][i];
				to += g[i][j];
			}
			cnt += (from > n / 2 || to > n / 2);
		}
		printf("%d\n", cnt);
	}
	return 0;
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/58ea9396fb884d69a640c8c0c2c6342f.png)


