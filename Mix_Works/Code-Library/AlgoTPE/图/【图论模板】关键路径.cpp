#include <bits/stdc++.h>
using namespace std;

/*
A  B   a1 = 6
A  C   a2 = 4
A  D   a3 = 5
B  E   a4 = 1
C  E   a5 = 1
D  H   a6 = 2
E  F   a7 = 9
E  G   a8 = 7
H  I   a9 = 4
F  I   a10= 2
G  I   a11= 4
-------------
9 11
0  1  6
0  2  4
0  3  5
1  4  1
2  4  1
3  7  2
4  5  9
4  6  7
7  8  4
5  8  2
6  8  4
*/
const int MAXN = 101;
struct node {
	int to, w;
};
int M, N; 		//M个顶点0~M-1,N条边 
vector<node> G[MAXN];
int *indegrees, *etv, *ltv;  //事件的最早发生时间,最晚发生时间 
stack<int> topOrder;  //存储拓扑排序的序列 

bool TopologicalSort() {
	stack<int> vex; 
	for (int i = 0; i < M; ++i) if (!indegrees[i]) vex.push(i); //压入零入度的顶点 
	while (!vex.empty()) {
		int u = vex.top(); vex.pop();
		topOrder.push(u);	//将u加入拓扑序列  
		for (int i = 0; i < G[u].size(); ++i) {
			int v = G[u][i].to, w = G[u][i].w;  //u的i号后续结点编号为v 
			if (!(--indegrees[v])) vex.push(v);
			//用etv[u]更新u的所有后继结点v 
			if (etv[u] + w > etv[v]) etv[v] = etv[u] + w;
		}
	}
	if (topOrder.size() != M) return false;
	return true;
}

int CriticalPath() { //返回关键路径的长度  
	etv = (int*)malloc(M * sizeof(int));
	ltv = (int*)malloc(M * sizeof(int));
	//memset(etv, 0, sizeof(etv));
	//memset(ltv, 0, sizeof(ltv));
	fill(etv, etv + M, 0);
	if (TopologicalSort() == false) {
		cout << "输入的不是有向无环图!" << endl; 
		return -1; //非DAG,返回-1
	}
	//int begin = topOrder[0], end = topOrder[top]; //得到源点和汇点
	int maxLength = 0; //etv数组的最大值就是关键路径的长度,也是汇点的最早/最迟开始时间
	for (int i = 0; i < M; ++i) if (etv[i] > maxLength) maxLength = etv[i]; 
	fill(ltv, ltv + M, maxLength);
	//以topOrder出栈顺序作为逆拓扑排序,求ltv数组 
	while (!topOrder.empty()) {
		int u = topOrder.top(); topOrder.pop();
		for (int i = 0; i < G[u].size(); ++i) {
			int v = G[u][i].to, w = G[u][i].w; //u的后续结点v
			//用u的所有后续结点v的ltv值更新ltv[u] 
			if (ltv[v] - w < ltv[u]) ltv[u] = ltv[v] - w; 
		}
	}
	//遍历邻接表的所有边,计算活动的最早开始时间和最迟开始时间
	int ete, lte;
	for (int u = 0; u < M; ++u) {
		for (int i = 0; i < G[u].size(); ++i) {
			int v = G[u][i].to, w = G[u][i].w;
			ete = etv[u], lte = ltv[v] - w;
			if (ete == lte) cout << char(u + 'A') << " -> " << char(v + 'A') << ", w: " << w << endl;
		}
	}
	return maxLength;
}

int main() {
	cin >> M >> N;  //输入顶点数和边数 
	indegrees = (int*)malloc(M * sizeof(int));
	fill(indegrees, indegrees + M, 0);
	for (int i = 0; i < N; ++i) {
		int u, v, w;	//有向边<u,v>,权值为w 
		cin >> u >> v >> w;
		++indegrees[v]; //v的入度+1 
		G[u].push_back(node{v, w});
	}
	cout << CriticalPath() << endl;  
	return 0;
}
//动态规划求关键路径
int *dp, *choice;
dp = new int[numVertexes];
choice = new int[numVertexes];
fill(choice, choice + numVertexes, -1);
void dynamicProgramming() {
	fill(dp, dp + numVertexes, 0);
	int maxLength = 0, v = -1;
	for (int i = 0; i < numVertexes; ++i) {
		if (dp[i] == 0) dp[i] = DP(i);
		if (dp[i] > maxLength) { v = i, maxLength = dp[i]; }
	}
	printf("%d\n", maxLength);
	printPath(v);
}

int DP(int i) {
	if (dp[i] > 0) return dp[i];
	for (int j = 0; j < numVertexes; ++j) {
		if (G[i][j] != INFINITY) {
			int temp = DP(j) + G[i][j];
			if (temp > dp[i]) {
				dp[i] = temp;
				choice[i] = j;
			}
		}
	}
	return dp[i];
}
void printPath(int i) {
	printf("%c", 'A' + i);
	while (choice[i] != -1) {
		i = choice[i];
		printf("->%c", 'A' + i);
	}
}