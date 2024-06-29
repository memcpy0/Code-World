#include <bits/stdc++.h>
using namespace std;
//P3377洛谷 
const int MAXN = 1e5 + 10;
struct node {
	int l, r, fa;
	int val, dist;
} ltt[MAXN];
#define ls(x) ltt[x].l
#define rs(x) ltt[x].r

inline int merge(int x, int y) {
	if (!x || !y) return x + y;
	if (ltt[x].val > ltt[y].val || (ltt[x].val == ltt[y].val && x > y))
		swap(x, y);
	rs(x) = merge(rs(x), y);
	ltt[rs(x)].fa = x; //x的右儿子的父亲一定是x,但是右儿子可能会变
	if (ltt[ls(x)].dist < ltt[rs(x)].dist)
		swap(ls(x), rs(x));
	ltt[x].dist = ltt[rs(x)].dist + 1;
	return x; 
}
int find(int x, int y) { return ltt[x].fa == x ? x : ltt[x].fa = find(ltt[x].fa); }
inline void pop(int x) { //一个堆的堆顶
	ltt[x].val = -1;
	ltt[ls(x)].fa = ls(x);
	ltt[rs(x)].fa = rs(x); 
	ltt[x].fa = merge(ls(x), rs(x));
}

int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	ltt[0].dist = -1;
	for (int i = 1; i <= n; ++i) {
		scanf("%d", &ltt[i].val);
		ltt[i].fa = i;
	}
	while (m--) {
		int opt, x, y;
		scanf("%d%d", &opt, &x);
		if (opt == 1) {
			scanf("%d", &y);
			if (ltt[x].val == -1 || ltt[y].val == -1) 
				continue;
			int fx = find(x), fy = find(y);
			if (fx == fy) continue;
			ltt[fx].fa = ltt[fy].fa = merge(fx, fy);	
		} else {
			if (ltt[x].val == -1 printf("-1");
			else {
				int fx = find(x);
				printf("%d", ltt[fx].val);
				pop(fx);
			}	
		}
	}
    return 0;
}
/*
中，左偏树还需要套一个并查集，因为需要知道一个数在哪个左偏树里面，用并查集可以方便快速地获取一个数所在堆的堆顶结点编号

这里可以用路径压缩，虽然路径压缩会破坏树的结果，不过破坏的是并查集的树结构，和左偏树没有关系，当然，还需要注意一些细节。
*/

