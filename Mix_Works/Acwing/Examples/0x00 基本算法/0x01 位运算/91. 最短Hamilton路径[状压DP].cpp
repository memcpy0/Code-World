/*

给定一张 n个点的带权无向图，点从 0∼n−1 标号，求起点 0 到终点 n−1 的最短 Hamilton 路径。

Hamilton 路径的定义是从 0 到 n−1 不重不漏地经过每个点恰好一次。
输入格式

第一行输入整数 n。

接下来 n行每行 n 个整数，其中第 i 行第 j 个整数表示点 i 到 j 的距离（记为 a[i,j]）。

对于任意的 x,y,z，数据保证 a[x,x]=0，a[x,y]=a[y,x] 并且 a[x,y]+a[y,z]≥a[x,z]。
输出格式
输出一个整数，表示最短 Hamilton 路径的长度。

数据范围
1≤n≤20
0≤a[i,j]≤107

输入样例：

5
0 2 4 5 1
2 0 6 5 3
4 6 0 8 3
5 5 8 0 5
1 3 3 5 0

输出样例：
18
*/
// 朴素DFS做法
// (n - 2)!种可能的走法
//int dfsHamilton(int cur, int n) {
//	if () {
//	}
//	for (int i = 0; i < n; ++i) {
//		if (!vis[i]) {
//			vis[i] = true;
//			dfsHamilton(cur + 1, n);
//			vis[i] = false;
//		}
//	}
//	
//}
#include <bits/stdc++.h>
using namespace std;
int n, dp[1 << 20][20], weight[20][20];
// dp[i][j]表示从0开始,不重不漏地遍历状态i中的点,到达j的最短Hamilton路径长度
// 保证是连通图
int hamilton(int n, int weight[20][20]) {
	memset(dp, 0x3f, sizeof(dp));
	dp[1][0] = 0; // 在0这个点,没有走过任何的路程
	int tot = 1 << n;
	for (int i = 1; i < tot; ++i) { // 遍历所有可能的状态
		int curState = i; // 状态i
		for (int j = 0; j < n; ++j) { // 遍历所有的点
			if ((curState >> j) & 1) { // 状态curState中选择了点j
				int prestate = curState ^ (1 << j); // 没有选择j这一点的某个状态中
				for (int k = 0; k < n; ++k) { // 遍历所有的点
					if ((prestate >> k) & 1) // 如果选择了点k
						dp[i][j] = min(dp[i][j], dp[i ^ (1 << j)][k] + weight[k][j]);
				}
			}
		}
	}
	return dp[tot - 1][n - 1]; // 遍历过所有点,最后停在点n-1
}

int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) 
		for (int j = 0; j < n; ++j)
			scanf("%d", &weight[i][j]);
	printf("%d", hamilton(n, weight));
	return 0;	
}

/*
std.cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int f[1 << 20][20], weight[20][20], n;

int hamilton(int n, int weight[20][20]) {
	memset(f, 0x3f, sizeof(f));
	f[1][0] = 0;
	for (int i = 1; i < 1 << n; i++)
		for (int j = 0; j < n; j++) if (i >> j & 1)
			for (int k = 0; k < n; k++) if ((i^1<<j) >> k & 1)
				f[i][j] = min(f[i][j], f[i^1<<j][k]+weight[k][j]);
	return f[(1 << n) - 1][n - 1];
}

int main() {
	cin >> n;
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			scanf("%d", &weight[i][j]);
	cout << hamilton(n, weight) << endl;
}
*/