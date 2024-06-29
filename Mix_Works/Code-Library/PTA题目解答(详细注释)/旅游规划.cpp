#include <bits/stdc++.h>
using namespace std; 
/* dijkstra + 第二标尺最短路 */
const int inf = 1000000000, maxv = 510;
int n, m, s, D; //城市个数 公路条数 出发地编号 目的地编号
int G[maxv][maxv], cost[maxv][maxv]; //0 N-1编号 
int c[maxv], d[maxv]; //数字均为整数且不超过500 
bool vis[maxv] = {false}; 

void Dijkstra(int s) {
	fill(d, d + maxv, inf);  //将整个d数组初始化为inf 
	d[s] = 0; //起点距离自己的距离是0 
	fill(c, c + maxv, inf);  //将整个cost数组初始化化为inf
	c[s] = 0; //起点到达自己的花费为0 
	for (int i = 0; i < n; i++) {
		int u = -1, min = inf;
		for (int j = 0; j < n; j++) { //找到未访问顶点中d[v]最小的 
			if (vis[j] == false && d[j] < min) {
				u = j;
				min = d[j]; 
			}
		}
		if (u == -1) return; //其他的顶点与起点S不连通 
		vis[u] = true;
		for (int v = 0; v < n; v++) {
			if (vis[v] == false && G[u][v] != inf) {
				if (d[u] + G[u][v] < d[v]) { //最短路径和花费 
					d[v] = d[u] + G[u][v];
					c[v] = c[u] + cost[u][v];
				} else if (d[u] + G[u][v] == d[v] && c[u] + cost[u][v] < c[v]) {
					c[v] = c[u] + cost[u][v]; //最短距离相同时看能否使c[v]最优 
				} 
			} 
		}
	}
}

int main() {
	fill(G[0], G[0] + maxv * maxv, inf); //初始化G 
	fill(cost[0], cost[0] + maxv * maxv, inf); //初始化cost 
	scanf("%d%d%d%d", &n, &m, &s, &D);
	int c1, c2, p, spend; //城市1、城市2、高速公路长度、收费额
	for (int i = 0; i < m; i++) { //城市1、城市2、高速公路长度、收费额
		scanf("%d%d%d%d", &c1, &c2, &p, &spend);
		G[c1][c2] = G[c2][c1] = p; //记录公路长度 
		cost[c1][c2] = cost[c2][c1] = spend; //记录收费额 
	} 
	Dijkstra(s);
    //输出路径的长度和收费总额
    printf("%d %d\n", d[D], c[D]);
	return 0;  
} 
