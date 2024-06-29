#include <bits/stdc++.h>
using namespace std;
// Trie
void insert(string str) {
	int p = 1;
	for (int i = 0; i < str.size(); ++i) {
		int x = str[i] - 'a';
		if (trie[p][x] == 0) trie[p][x] = ++totle; // why + 上洛谷 
		p = trie[p][x];
	}
	end[p] = true;
}
bool search(string str) {
	int p = 1;
	for (int i = 0; i < str.size(); ++i) {
		p = trie[p][str[i] - 'a'];
		if (p == 0) return false;
	}
	return end[p];
}
// 并查集
void init(void) {
	for (int i = 0; i <= n; ++i) fa[i] = i;
}
int find(int x) {
	if (fa[x] == x) return x;
	return fa[x] = find(fa[x]);
}
void merge(int x, int y) {
	fa[x] = y;
}
// 分块
// 结构体
// 0 | 1 2 3 | 4 5 6 | 7 8 9 Index
// N | 2 3 4 | 5 8 9 | 1 6 6 data
struct Piece { // 索引表结构 
	int l, r; // 左右边界 
	int sum;  // 块表和 
	int add; // 标记 
	Piece(int _l = 0, int _r = 0, int _s = 0, int _a = 0) {
		l = _l, r = _r, sum = _s, add = _a;
	}
	#define l(p) piece[p].l
	#define r(p) piece[p].r
	#define sum(p) piece[p].sum
	#define add(p) piece[p].add 
} piece[SIZE]; 
int n, m;
int len, num;
int data[SIZE], pos[SIZE];
//////// 
void init(void) {
	cin >> n >> m;
	for (int i = 1; i <= n; ++i) cin >> data[i];
}
void buildPiece(void) { // 建块 // 索引表 
	len = sqrt(n); // 每块大小 3
	num = (n % len ? n / len + 1 : n / len); // 块数 3
	for (int i = 1; i <= num; ++i) { // 确定每一块的左右边界 
		l(i) = (i - 1) * len + 1, r(i) = i * len;
	}
	if (r(num) > n) r(num) = n; // 右边界超出实际边界，特判 
	for (int i = 1; i <= num; ++i) 
		for (int j = l(i); j <= r(i); ++j) 
			sum(i) += data[j], pos[j] = i; // 每一块的和 每个数据存在哪个块中 
}
void change(int l, int r, int add) {
	int pl = pos[l], pr = pos[r];
	if (p1 == pr) { // 处于同一块中 
		if (l == l(pl) && r == r(pr)) add(pl) += add; // ???
		else {
			for (int i = l; i <= r; ++i) data[i] += add;
			sum(pl) += add * (r - l + 1);
		}
	} else {
		for (int i = l; i <= r(pl); ++i) data[i] += add; 
		sum(pl) += add * (r(pl) - l + 1);
		for (int i = pl + 1; i <= pr - 1; ++i) add(i) += add, sum(i) += (r(i) - l(i) + 1);	
		for (int i = l(pr); i <= r; ++i) data[i] += add;
		sum(pr) += add * (r - l(pr) + 1);
	}
}			 
int ask(int l, int r) {
	int pl = pos[l], pr = pos[r];
	int val = 0;
	if (pl == pr) {
		if (l == l(pl) && r(pr) == r) val = sum(pl);
		else {
			for (int i = l; i <= r; ++i) val += add(pl) + data[i]; // ???
		}
	} else {
		for (int i = l; i <= r(pl); ++i) ans += 
		for (
		for (int i = 
	}
	return val;
}
////////
// 线段树 
// 邻接矩阵
// 邻接表

// 01背包
// 完全背包
// 多重背包
// 分组背包
// 依赖背包

// 树形DP
// 区间DP
// 四边形不等式
// 广搜
// 深搜
// 迭代加深
// 启发式A*
// 启发式IDA*
// 素数判断
// 埃氏筛法
// gcd
// lcm
// 扩展欧几里得
// 取余公式
// 同余方程
// 矩阵快速幂
// Floyd
// Dijkstra 
// Bellman-Ford
// SPFA
// kruskal
// prim
// 线段树
// 树状数组
// 单步LCA
// 
// 差分
// 离散化
// 哈希
// 拓扑排序
// vector
// string
// queue
// priority_queue
// map
// pair
// 对拍
// 图的遍历 
// deque

#include <bits/stdc++.h>
using namespace std;

const int maxv = 110;
const int inf  = 123456789;
int n, m;

int graph[maxv][maxv];

void floyd() {
	int s = 1;
	for (int k = 1; k <= n; ++k) 
		for (int i = 1; i <= n; ++i) 
			for (int j = 1; j <= n; ++j) 
				if (graph[i][j] > graph[i][k] + graph[k][j]) 
					graph[i][j] = graph[i][k] + graph[k][j];
	printf("%d\n", graph[s][n]); 
}

int main() {
	while (~scanf("%d%d", &n, &m)) {
		if (!n && !m) break;
		for (int i = 1; i <= n; ++i) {
			graph[i][i] = 0;
			for (int j = 1; j <= n; ++j) {
				graph[i][j] = inf;
			}
	}
		while (m--) {
			int a, b;
			scanf("%d%d", &a, &b);
			scanf("%d", &graph[a][b]);
			graph[b][a] = graph[a][b];
		}
		floyd();
	}
}
