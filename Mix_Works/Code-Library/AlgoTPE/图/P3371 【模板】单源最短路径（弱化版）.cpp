/*
题目背景

本题测试数据为随机数据，在考试中可能会出现构造数据让SPFA不通过，如有需要请移步 P4779。
题目描述

如题，给出一个有向图，请输出从某一点出发到所有点的最短路径长度。
输入格式

第一行包含三个整数 n,m,s ，分别表示点的个数、有向边的个数、出发点的编号。

接下来 mmm 行每行包含三个整数 u,v,w ，表示一条 u→v  的，长度为 w  的边。
输出格式

输出一行 n  个整数，第 i 个表示 s 到第 i 个点的最短路径，若不能到达则输出  2^{31}-1。
输入输出样例
输入 #1

4 6 1
1 2 2
2 3 2
2 4 1
1 3 5
3 4 3
1 4 4

输出 #1

0 2 4 3
*/
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 40500;
const int inf = 0x3f3f3f3f;
struct node {
	int from, to, w;
	node(int f, int t, int _w) { from = f; to = t, w = _w; }
};
vector<node> adj[MAXN];
int n, m, s, u, v, w;

void dijkstra(int s) {
	bool vis[MAXN];
	int d[MAXN];
	for (int i = 0; i < MAXN; ++i) { vis[i] = false; d[i] = inf; } 
   
	d[s] = 0;
	for (int i = 1; i <= n; ++i) {
		int minDist = inf, v;
		for (int j = 1; j <= n; ++j) {
			if (vis[j] == false && d[j] < minDist) {
				minDist = d[j];
				v = j;
			}
		}
		if (minDist == inf) break;
		vis[v] = true;
		for (int k = 0; k < adj[v].size(); ++k) {
			int u = adj[v][k].to;
			if (vis[u] == false && adj[v][k].w + d[v] < d[u]) {
				d[u] = adj[v][k].w + d[v];
			}
		}
	}
	for (int i = 1; i <= n; ++i) {
		if (i == 1) printf("%d", d[i] == inf ? 2147483647: d[i]);
		else printf(" %d", d[i] == inf ? 2147483647 : d[i]);
	}
}

int main() {
	scanf("%d%d%d", &n, &m, &s);
	for (int i = 0; i < m; ++i) {
	 	scanf("%d%d%d", &u, &v, &w);
		adj[u].push_back(node(u, v, w));
	}
	dijkstra(s);	 
	return 0;
}

