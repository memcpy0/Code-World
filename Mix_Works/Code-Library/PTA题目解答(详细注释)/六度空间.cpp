#include <bits/stdc++.h>
using namespace std;
const int maxv = 1010;
struct node {
	int v, layer; //顶点编号, 顶点层号 
	node(int _v, int _l): v(_v), layer(_l) { }
};
vector<node> Adj[maxv]; //结点从1到N编号
bool inq[maxv] = {false};
int nums[maxv] = {0}; //nums[i]表示与结点i距离不超过6的结点数, 包括自己 
int n, m;

void BFS(int u) {
    queue<node> q;
    q.push(node(u, 0)); //将初始点入队
	inq[u] = true; 
	while (!q.empty()) {
		node top = q.front(); q.pop();
		int v = top.v; //队首顶点的编号 
		if (top.layer <= 6) nums[u]++; 
		else break; //超过六度退出 
		for (int i = 0; i < Adj[v].size(); i++) {
			node next = Adj[v][i]; //从v出发能到达的顶点next
			next.layer = top.layer + 1;
			if (inq[next.v] == false) {
				q.push(next); //将k入队 
				inq[next.v] = true; //标记v已加入队列 
			}
		}
	} 
}

void BFSTrave() {
    fill(nums, nums + maxv, 0); 
	for (int i = 1; i <= n; i++) {
		fill(inq, inq + maxv, 0); //每一次将之置0 
		BFS(i); //对每个结点输出与该结点距离不超过6的结点数占结点总数的百分比
		printf("%d: %.2lf%%\n", i, nums[i] * 100.0 / n);
	} 
}


int main() {
	int u, v;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &u, &v);
		Adj[u].push_back(node(v, 0));
		Adj[v].push_back(node(u, 0)); //无向图 
	}
	BFSTrave();
	return 0;
}

