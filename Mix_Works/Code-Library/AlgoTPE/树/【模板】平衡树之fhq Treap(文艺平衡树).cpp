#include <bits/stdc++.h>
using namespace std;

//fhq Treap如何实现区间操作
/*
要操作的区间是[l,r]
那么我们在fhq Treap中把这一段直接split拆出来进行操作然后合并回去
按大小分裂的写法: 把树拆成两棵树,其中一棵树的大小等于给定的大小,剩下部分在另外一棵树中
具体步骤:
把fhq Treap按照大小l-1拆分成x和y
再把y按大小r-l+1拆分成y和z
此时y树就是我们要操作的区间的平衡树,对它进行操作就好了
最后把xyz合并回去
 
如何解决文艺平衡树问题？
使用线段树中常见的概念：懒标记(lazy Tag)
每当我们想要对一个区间进行翻转时,我们就对这个区间维护懒标记:如果原来没有懒标记
就打上;如果原来有懒标记,就去掉,因为翻转两次等于没有翻转.
别忘了写懒标记的下传操作,如果当前结点的左右子树在后续代码中有被更改的风险的话,就下传,以防万一 
*/ 
const int MAXN = 1e5 + 10;
struct node {
	int l, r;
	int val, key;
	int size;
	bool reverse; //懒标记,证明这个区间是否被翻转 
} fhq[MAXN];
int cnt, root;
#include <random>
std::mt19937 rnd(233);

inline int newNode(int val) {
	fhq[++cnt].val = val;
	fhq[cnt].key = rnd();
	fhq[cnt].size = 1;
	return cnt;
}
inline void sizeUpdate(int now) {
	fhq[now].size = fhq[fhq[now].l].size + fhq[fhq[now].r].size + 1;
}
//懒标记的下传
inline void pushdown(int now) {
	//让左右子树都翻转
	swap(fhq[now].l, fhq[now].r);
	fhq[fhq[now].l].reverse ^= 1; //false=true;true=false
	fhq[fhq[now].r].reverse ^= 1;
	fhq[now].reverse = false;     //把当前结点的懒标记去掉等于false 
}

inline void split(int now, int siz, int &x, int &y) {
	if (!now) x = y = 0;
	else {
		if (fhq[now].reverse) pushdown(now);
		if (fhq[fhq[now].l].size < siz) {
			x = now; //根结点一定是x 
			split(fhq[now].r, siz - fhq[fhq[now].l].size - 1, fhq[now].r, y); //当前结点+左子树 
		} else {
			y = now;
			split(fhq[now].l, siz, x, fhq[now].l);
		}
	}
}

inline int merge(int x, int y) {
	if (!x || !y) return x + y;
	if (fhq[x].key < fhq[y].key) {
		if (fhq[x].reverse) pushdown(x);
		fhq[x].r = merge(fhq[x].r, y); //y的值大于等于x的值,因此用右子树进行合并 
		sizeUpdate(x);
		return x;
	}
	else {
		if (fhq[y].reverse) pushdown(y);
		fhq[y].l = merge(x, fhq[y].l); //x的值小于等于y的值,因此用y的右子树进行合并 
		sizeUpdate(y);
		return y;
	}	
}

void reverse(int l, int r) {
	int x, y, z;
	split(root, l - 1, x, y);
	split(y, r - l + 1, y, z);
	//此时y处就是我们要翻转的区间
	fhq[y].reverse ^= 1;
	root = merge(merge(x, y), z);
}

void inorder(int now) {
	if (!now) return;
	if (fhq[now].reverse) pushdown(now);
	inorder(fhq[now].l);
	printf("%d ", fhq[now].val);
	inorder(fhq[now].r);
}

int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= n; ++i) { 
		root = merge(root, newNode(i));
	}
	while (m--) {
		int l, r;
		cin >> l >> r;
		reverse(l, r);
	}
	ldr(root);
    return 0;
}
