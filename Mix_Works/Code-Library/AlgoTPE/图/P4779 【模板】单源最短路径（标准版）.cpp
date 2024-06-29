/*
题目背景
2018 年 7 月 19 日，某位同学在 NOI Day 1 T1 归程 一题里非常熟练地使用了一个广为人知的算法求最短路。
然后呢？

100→60 ；

Ag→Cu ；

最终，他因此没能与理想的大学达成契约。

小 F 衷心祝愿大家不再重蹈覆辙。
题目描述

给定一个 n  个点，m  条有向边的带非负权图，请你计算从 s  出发，到每个点的距离。

数据保证你能从 s 出发到任意点。
输入格式

第一行为三个正整数 n,m,s 。 第二行起 m 行，每行三个非负整数 ui,vi,wi ，表示从 ui  到 vi 有一条权值为 wi 的有向边。
输出格式

输出一行 n 个空格分隔的非负整数，表示 s 到每个点的距离。
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
typedef long long ll;
const int MAXN = 500100;
const ll inf = 0x7fffffff;
ll n, m, s, u, v, w;

struct edge {
	ll from, to, w;
	edge(ll f, ll _t, ll _w) { from = f; to = _t; w = _w; }
};
vector<edge> adj[MAXN];

struct s_node {
	ll id, n_dis;
	s_node(int i, int d) { id = i; n_dis = d; }
	bool operator < (const s_node &a) const { return n_dis > a.n_dis; }
};

bool vis[MAXN];
ll d[MAXN];
void dijkstra(ll s) {
	for (int i = 1; i <= n; ++i) { vis[i] = false; d[i] = inf; }
	d[s] = 0;
	priority_queue<s_node> Q;
	Q.push(s_node(s, d[s]));
	
	while (!Q.empty()) {
		s_node v = Q.top(); Q.pop();
		if (vis[v.id]) continue; //丢弃已经访问过找到最短路径的结点 
		vis[v.id] = true;
		for (int i = 0; i < adj[v.id].size(); ++i) {
			edge y = adj[v.id][i];
			if (vis[y.to]) continue;
			if (y.w + v.n_dis < d[y.to]) {
				d[y.to] = y.w + v.n_dis;
				Q.push(s_node(y.to, d[y.to]));
			}
		}
	}
	for (int i = 1; i <= n; ++i) {
		if (i == 1) printf("%lld", d[i]);
		else printf(" %lld", d[i]); 
	}
}

int main() {
	scanf("%lld%lld%lld", &n, &m, &s);
	for (int i = 0; i < m; ++i) {
		scanf("%lld%lld%lld", &u, &v, &w);
		adj[u].push_back(edge(u, v, w));
	}
	dijkstra(s);
	return 0;
}


