#include <bits/stdc++.h> 
/* Kruskal + 邻接表 + 集合S */ 
using namespace std;
struct node {
	int v, w; //v为边的目标, w为边权 
	node(int _v, int _w): v(_v), w(_w) { } 
};
const int inf = 1000000000, maxv = 1010; //1000个最大城镇 
int n, m, d[maxv]; //n个结点和m条边; 顶点与集合S的最短距离 
bool vis[maxv] = {false}; //标记数组, 标记顶点加入了集合S
vector<node> Adj[maxv]; //邻接表 

int prim() { //默认1号为初始点, 返回MST的边权和 
	fill(d, d + maxv, inf); //将整个d数组赋值为inf
	d[1] = 0; //只有1号结点的到集合S的距离为0, 其余全部为inf 
	int ans = 0; //MST的边权和
	for (int i = 0; i < n; i++) { //循环n次
		int u = -1, min = inf; //u使得d[u]最小, min存放最小的d[u]
		for (int j = 1; j <= n; j++) {
			if (vis[j] == false && d[j] < min) {
				u = j;
				min = d[j];
			}
		} 
		//找不到小于inf的d[u], 剩下的顶点和集合S不相通
		if (u == -1) return -1;
		vis[u] = true;
		ans += d[u]; //将与集合S距离最小的边加入最小生成树 
	    for (int i = 0; i < Adj[u].size(); i++) { //u可达v
	    	int v = Adj[u][i].v; //通过邻接表直接获得u能达到的顶点v 
	    	//v未访问, 以u为中介点可以使得v离集合S更近
			if (vis[v] == false && Adj[u][i].w < d[v]) {
				d[v] = Adj[u][i].w; //将G[u][v]赋值给d[v] 
			} 
		}
	}
	return ans; //返回MST的边权之和 
} 

int main() {
	scanf("%d%d", &n, &m);
	int u, v, w; 
	for (int i = 1; i <= m; i++)  {//点的编号从1开始  
		scanf("%d%d%d", &u, &v, &w);
		Adj[u].push_back(node(v, w)); //无向图 
		Adj[v].push_back(node(u, w));  
	}
	printf("%d\n", prim()); 
	return 0;
}
