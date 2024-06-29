#include <cstdio>
#include <queue>
#include <vector> 
using namespace std;
/* 邻接矩阵版本 */
const int maxv = 50; 
const int inf = 1000;
int nv, ne, G[maxv][maxv];
bool vis[maxv] = {false}, inq[maxv] = {false};

void CreateGraph() {
	int u, v;
	for (int i = 0; i < nv; i++) 
		for (int j = 0; j < nv; j++) 
			G[i][j] = inf;
	for (int i = 0; i < ne; i++) {
		scanf("%d%d", &u, &v); //0到N-1编号 
		G[u][v] = 1; //无向图 
		G[v][u] = 1; 
	}
}

vector<int> comp;
void DFS(int u) {
	vis[u] = true;
	comp.push_back(u);
	for (int v = 0; v < nv; v++) {
		if (vis[v] == false && G[u][v] != inf) {
			DFS(v);
		}
	} 
}
void DFSTrave() {
	for (int i = 0; i < nv; i++) {
		if (vis[i] == false) {
			comp.clear(); 
			DFS(i);
			printf("{");
			for (int j = 0; j < comp.size(); j++) { 
				printf(" %d", comp[j]);
			} 
			printf(" }\n");
		}
	}
}

void BFS(int u) {
	queue<int> q;
	q.push(u);
	inq[u] = true; //u已经加入队列
	while (!q.empty()) {
		int u = q.front(); q.pop();
	    comp.push_back(u);
		for (int v = 0; v < nv; v++) { //如果u的邻接点v未曾加入过队列 
			if (inq[v] == false && G[u][v] != inf) {
				q.push(v); 
				inq[v] = true; //标记v已经加入过队列 
			}
		}
	} 
}
void BFSTrave() {
	for (int u = 0; u < nv; u++) {
		if (inq[u] == false) { //u未曾加入过队列 
			comp.clear(); 
			BFS(u); //遍历u在的连通块 
			printf("{");
			for (int j = 0; j < comp.size(); j++) { 
				printf(" %d", comp[j]);
			} 
			printf(" }\n");
		}
	} 
}

int main() {
	scanf("%d%d", &nv, &ne);
	CreateGraph();
	DFSTrave();
	BFSTrave();
	return 0;
}
