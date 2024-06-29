**Description**
There are N beads which of the same shape and size, but with different weights. N is an odd number and the beads are labeled as 1, 2, ..., N. Your task is to find the bead whose weight is median (the ((N+1)/2)th among all beads). The following comparison has been performed on some pairs of beads:
A scale is given to compare the weights of beads. We can determine which one is heavier than the other between two beads. As the result, we now know that some beads are heavier than others. We are going to remove some beads which cannot have the medium weight.

For example, the following results show which bead is heavier after M comparisons where M=4 and N=5.
1.	Bead 2 is heavier than Bead 1.
2.	Bead 4 is heavier than Bead 3.
3.	Bead 5 is heavier than Bead 1.
4.	Bead 4 is heavier than Bead 2.

From the above results, though we cannot determine exactly which is the median bead, we know that Bead 1 and Bead 4 can never have the median weight: Beads 2, 4, 5 are heavier than Bead 1, and Beads 1, 2, 3 are lighter than Bead 4. Therefore, we can remove these two beads.

Write a program to count the number of beads which cannot have the median weight.

**Input**
The first line of the input file contains a single integer t (1 <= t <= 11), the number of test cases, followed by the input data for each test case. The input for each test case will be as follows:
The first line of input data contains an integer N (1 <= N <= 99) denoting the number of beads, and M denoting the number of pairs of beads compared. In each of the next M lines, two numbers are given where the first bead is heavier than the second bead.

**Output**
There should be one line per test case. Print the number of beads which can never have the medium weight.

**Sample Input**
```cpp
1
5 4
2 1
4 3
5 1
4 2
```
**Sample Output**
```cpp
2
```
**Source**
[Tehran Sharif 2004 Preliminary](http://poj.org/searchproblem?field=source&key=Tehran+Sharif+2004+Preliminary)


题意：给出一些珠子之间的重量大小关系，求出有多少颗珠子的重量，不可能是所有珠子重量的中位数。


---
### 解法 传递闭包
根据偏序关系建图，如果第 $i$ 号珠子重于第 $j$ 号珠子，即 $w_i > w_j$ ，则由 $i$ 到 $j$ 连接一条有向边。然后求传递闭包，即对每个点都统计「**其能到达多少个点、有多少点能到达它**」，如果这两个数中有一个大于 `n / 2` ，那这颗珠子的重量就不可能是中位数。

感觉用DFS也可以，对每个点DFS，同时统计它能到达哪些点、哪些点可由它达到……不过代码还是传递闭包更简单：
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

![在这里插入图片描述](https://img-blog.csdnimg.cn/ddedf8c66cdf4032afb27815e63e8b24.png)

