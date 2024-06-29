#include <bits/stdc++.h>
using namespace std;
//无向图的割边和割点
const int maxn = 1000 + 5;
int n, m, head[maxn], cnt;
struct edge {
	int to, next;
} e[maxn << 1];
int low[maxn], dfn[maxn], num = 0, root;
void add(int u, int v) {
	e[++cnt].next = head[u];
	e[cnt].to = v;
	head[u] = cnt;
}
/*
7 7
1 4
1 2
2 3
3 5
5 7
5 6
6 4
5--7是割边
*/
void tarjan_for_undigraph(int u, int fa) {
	dfn[u] = low[u] = ++num;
	int count = 0;
	for (int i = head[u]; i; i = e[i].next) {
		int v = e[i].to;
		if (v == fa) continue;
		if (!dfn[v]) { //没有访问过 
			tarjan_for_undigraph(v, u);
			low[u] = min(low[u], low[v]); //看子节点能否回退到已经访问过的节点 
			//割边判断法则:
			//无向边(x,y)是割边,当且仅当DFS搜索树上存在x的一个子节点y,满足:
			//low[y]>dfn[x]
			//割点判断法则:
			//若x不是根节点,则x是割点,当且仅当DFS搜索树上存在x的一个子节点y,满足:
			//low[y]>=dfn[x]
			//若x是根节点,则x是割点,当且仅当DFS搜索树上存在至少两个子节点,满足
			//low[y]>=dfn[x] 
			if (low[v] > dfn[u]) cout << u << "--" << v << "是割边" << endl;
			if (low[v] >= dfn[u]) ++count;
		} else low[u] = min(low[u], dfn[v]); //回退边(指向已经访问过的节点) 
	}
	if ((u != root && count >= 1) || count > 1) cout << u << "是割点" << endl; 
} 
/*
有向图的强连通分量:
1) DFS节点,第一次访问节点x时,将x入栈,dfn[x]=low[x]=++num;
2) 遍历x的所有邻接点y: (检查是否在栈中,是为了避免两个有向图连通分量都存在一个边指向同一个节点,此时只有在栈中才算是被这次访问过 
用栈的意义在于保存连通分量节点) 
	若y没有被访问,则递归访问y,返回时更新low[x]=min(low[x], low[y])
	若y已被访问且在栈中,则令low[x]=min(low[x],dfn[y])
3) x回溯之前,判断如果low[x]=dfn[x],则不断从栈中弹出节点,直到x出栈时停止,弹出的节点就是
一个连通分量 
5 8
1 2
1 3
3 2
3 4
3 5
4 1
4 5
5 1
*/
int ins[maxn];
stack<int> s;
void tarjan_for_digraph(int u) {
	dfn[u] = low[u] = ++num;
	ins[u] = true; //是否在栈中 
	s.push(u); //存入栈中 
	cout << u << endl;
	for (int i = head[u]; i; i = e[i].next) {
		int v = e[i].to;
		//不用检查是否是父节点 
		if (!dfn[v]) { //没有访问过 
			tarjan_for_digraph(v);
			low[u] = min(low[u], low[v]); //看子节点能否回退到已经访问过的节点 
		} else if (ins[v]) low[u] = min(low[u], dfn[v]); //回退边(指向已经访问过的节点) 
	}
	if (low[u] == dfn[u]) {
		int v;
		cout << "连通分量: ";
		do {
			v = s.top(); s.pop();
			cout << v << " ";
			ins[v] = false; 
		} while (v != u);
		cout << endl;
	}
	//这里不能写ins[u] = false; 不然万一又有顶点连接到u,发现u竟然不在栈中！！！
	//示例：
//	9 11 
//	1 2
//	2 4
//	4 5
//	2 3
//	5 3
//	7 1
//	5 7
//	5 8
//	8 9
//	9 7
//  6 5
//	4
//	1 7
//	1 3
//	5 6
//	8 9
} 
void init() { 
	cnt = num = 0;
	for (int i = 0; i < maxn; ++i) head[i] = low[i] = dfn[i] = ins[i] = 0;
	for (int i = 0; i < maxn * 2; ++i) e[i].next = 0; 
}
int main() {
	while (cin >> n >> m) {
		init();
		int u, v;
		while (m--) {
			cin >> u >> v;
			//add(u, v), add(v, u); //无向图
			add(u, v); //有向图 
		}
		for (int i = 1; i <= n; ++i) {	
			if (!dfn[i]) {
				//root = i; //无向图割点 
				tarjan_for_digraph(1);
			}
		}
	}
	return 0;
}


