/*
��Ŀ����
2018 �� 7 �� 19 �գ�ĳλͬѧ�� NOI Day 1 T1 ��� һ����ǳ�������ʹ����һ����Ϊ��֪���㷨�����·��
Ȼ���أ�

100��60 ��

Ag��Cu ��

���գ������û��������Ĵ�ѧ�����Լ��

С F ����ףԸ��Ҳ����ص����ޡ�
��Ŀ����

����һ�� n  ���㣬m  ������ߵĴ��Ǹ�Ȩͼ���������� s  ��������ÿ����ľ��롣

���ݱ�֤���ܴ� s ����������㡣
�����ʽ

��һ��Ϊ���������� n,m,s �� �ڶ����� m �У�ÿ�������Ǹ����� ui,vi,wi ����ʾ�� ui  �� vi ��һ��ȨֵΪ wi ������ߡ�
�����ʽ

���һ�� n ���ո�ָ��ķǸ���������ʾ s ��ÿ����ľ��롣
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
		if (vis[v.id]) continue; //�����Ѿ����ʹ��ҵ����·���Ľ�� 
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


