/*
P3384 【模板】重链剖分
*/
#include <bits/stdc++.h>
using namespace std;
const int maxn = 40005;
int n, m; //n个节点,m个查询
int head[maxn], dist[maxn], cnt; //头结点数组,距离数组 
int father[maxn], depth[maxn]; //父节点,深度数组
int size[maxn], son[maxn], top[maxn]; //子树节点总数,重儿子,所在重链顶端
//树上倍增可以解决的,树链剖分都可以解决,反之则不然 
struct edge {
	int to, w, next;
} e[maxn << 1];
void add(int u, int v, int w) { //顶点编号从1开始 
	e[++cnt].to = v; //cnt为0表示为null 
	e[cnt].w = w;
	e[cnt].next = head[u];
	head[u] = cnt;
}
void dfs1(int u, int f) { //求depth,father,size,son,dist
	size[u] = 1; //u所在的子树节点总数先只有1个
	for (int i = head[u]; i; i = e[i].next) {
		int v = e[i].to;
		if (v == f) continue; //父节点
		depth[v] = depth[u] + 1; //深度 
		father[v] = u;
		//dist[v] = dist[u] + e[i].w; //距离 
		dfs1(v, u);
		size[u] += size[v];
		if (size[v] > size[son[u]]) son[u] = v; //u的重儿子 
	}
}
/*
	  ①
	②   ③
  ④ ⑤ ⑥⑦ 
	   ⑧ 
*/
void dfs2(int u) { //求top,不需要id时
	if (u == son[father[u]]) top[u] = top[father[u]];
	else top[u] = u; //设置根节点的重链顶端
	for (int i = head[u]; i; i = e[i].next) { //随意先搜 
		int v = e[i].to;
		if (v != father[u]) dfs2(v);
	}
} 
int total = 0;
//另外一种搜法(需要id的时候) dfs2(1, 1) 
void dfs2(int u, int t) { //求top,id(节点在线段树中的位置),rev(线段树中相应位置的节点是)
	top[u] = t; 
	id[u] = ++total; //u对应的dfs序下标
	rev[total] = u; //dfs序下标对应的节点u
	if (!son[u]) return; //没有重儿子,是叶子节点 
	dfs2(son[u], t); //沿着重儿子dfs, t是父亲的top[u] 
	for (int i = head[u]; i; i = e[i].next) {
		int v = e[i].to;
		if (v != father[u] && v != son[u]) //dfs那些不是重儿子的节点 
			dfs(v, v);
	} 
} 

//用线段树维护这一个树链剖分的线性序列(下标从1开始)1 3 6 8 7 2 5 4
//查询树上的区间最值,单点更新,区间和值 
int Sum, Max;
void build(int k, int l, int r) { //创建线段树,k表示存储下标,区间[l,r]
	tree[k].l = l, tree[k].r = r;
	if (l == r) {
		tree[k].mx = tree[k].sum = w[rev[l]]; //l:id号,rev[l]节点号 
		return;
	} 
	int mid = (l + r) / 2, lc = k * 2, rc = k * 2 + 1;
	build(lc, l, mid);
	build(rc, mid + 1, r);
	tree[k].mx = max(tree[lc].mx, tree[rc].mx);
	tree[k].sum = tree[lc].sum + tree[rc].sum;
} 
void update(int k, int i, int v) { // 单点修改
	if (tree[k].l == tree[k].r && tree[k].l == i) {
		tree[k].mx = tree[k].sum = v;
		return;
	}
	int mid = (tree[k].l + tree[k].r) / 2, lc = k * 2, rc = k * 2 + 1;
	if (i <= mid) update(lc, i, v);
	else update(rc, i, v);
	tree[k].mx = max(tree[lc].mx, tree[rc].mx);
	tree[k].sum = tree[lc].sum + tree[rc].sum;
} 
void query(int k, int l, int r) { //求区间的最值或和值 
	int Max = -inf, Sum = 0;
	if (tree[k].l >= l && tree[k].r <= r) {
		Max = max(Max, tree[k].mx);
		Sum += tree[k].sum;
		return;
	}
	int mid = (tree[k].l + tree[k].r) / 2, lc = k * 2, rc = k * 2 + 1;
	if (l <= mid) query(lc, l, r);
	if (mid < r) query(rc, l, r); 
}
void ask(int u, int v) { //求u,v之间的最值或和值 1到6而不是6到1 
	while (top[u] != top[v]) { //不在同一条重链上 4和6 top[4]=4,top[6]=1 
		if (depth[top[u]] < depth[top[v]]) swap(u, v); //先查询重链顶端深度大的 
		query(1, id[top[u]], id[u]); //u顶端顶点和u之间 
		u = father[top[u]]; //接着上升到重链顶段的父节点 谁的top深度大谁往上走 
	}
	if (depth[u] > depth[v]) //在同一条重链上
		swap(u, v); //深度小的节点为u,在同一条链上,深度小的一定下标小 
	query(1, id[u], id[v]); //id[1]=1,id[6]=3 
}
void print(int k) {
	if (tree[k].mx) {
		printf("%d [%d, %d]: max=%d, sum=%d\n", k, tree[k].l, tree[k].r, tree[k].mx, tree[k].sum);
		print(k << 1);
		print(k << 1 | 1);
	}
}
int main() {
	int x, y;
	string str;
	cin >> n;
	for (int i = 1; i < n; ++i) { //n-1条边 
		cin >> x >> y;
		add(x, y);
		add(y, x);
		/*
		10
		1 2
		1 3
		2 4
		2 5
		3 6 
		3 7
		3 8
		5 9
		6 10
		*/ 
	}
	for (int i = 1; i <= n; ++i) cin >> w[i]; //20 6 3 12 1 8 16 30 15 5 
	depth[1] = top[1] = 1;
	dfs1(1, 0);
//	for (int i = 1; i <= n; ++i) cout << son[i] << " " << size[i] << endl;
//	cout << endl;
	dfs2(1, 1);
	cout << "树链剖分序列: " << endl;
	for (int i = 1; i <= total; ++i)
		cout << rev[i] << " ";
	cout << endl;
	build(1, 1, total); //创建线段树
	cout << "根据树链剖分序列创建的线段树：" << endl;
	print(1);
	cout << "输入查询数量m: " << endl;
	cin >> m;
	cout << "输入CHANGE u val或者QMAX u v或者QSUM u v" << endl;
	for (int i = 1; i <= m; ++i) {
		cin >> str >> x >> y;
		if (str[0] == 'C') update(1, id[x], y);
		else {
			Sum = 0, Max = -0x3f3f3f3f;
			ask(x, y);
			if (str[1] == 'M') cout << Max << endl;
	 		else cout << Sum << endl;
		}
	}
	return 0;
}
//---------------------------------------------------------------------
//树链剖分求LCA

//---------------------------------------------------------------------
//树链剖分与线段树
//树中的点更新、区间更新、区间查询,可以使用线段树维护处理
 
