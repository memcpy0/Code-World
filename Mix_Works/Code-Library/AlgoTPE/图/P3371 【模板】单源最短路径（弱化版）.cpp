/*
��Ŀ����

�����������Ϊ������ݣ��ڿ����п��ܻ���ֹ���������SPFA��ͨ����������Ҫ���Ʋ� P4779��
��Ŀ����

���⣬����һ������ͼ���������ĳһ����������е�����·�����ȡ�
�����ʽ

��һ�а����������� n,m,s ���ֱ��ʾ��ĸ���������ߵĸ�����������ı�š�

������ mmm ��ÿ�а����������� u,v,w ����ʾһ�� u��v  �ģ�����Ϊ w  �ıߡ�
�����ʽ

���һ�� n  ���������� i ����ʾ s ���� i ��������·���������ܵ��������  2^{31}-1��
�����������
���� #1

4 6 1
1 2 2
2 3 2
2 4 1
1 3 5
3 4 3
1 4 4

��� #1

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

