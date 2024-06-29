非递归线段树，因为张昆玮在统计的力量中介绍了这种数据结构，常常被称为zkw线段树，是一种代码较短、常数较小的线段树写法。
普通的线段树，是从上到下处理的。因为对于普通线段树，我们很容易定位根节点，却不容易定位叶子节点。
为了后面方便，本文统一对节点采用1-index，对线段采用0-index但是，一种特殊情况下，线段树的叶子节点很好定位，那就是这棵二叉树是满二叉树时：
这时我们发现叶子节点的编号非常有规律，设一共有  个叶子节点，若某叶子节点所代表的退化线段为  ，则其编号  。
不仅如此，这棵树还有大量很好的性质：
整棵树一共有  个节点一共有  层，（从上往下数）第  层有  个节点，且该层线段长度为  
若某节点的编号为  ，其父节点编号为  ，子节点编号为  和  。若两节点编号分别为  和  ，则它们是兄弟节点等价于  。
除根结点外，编号为偶数的节点都是某个节点的左节点，编号为奇数的节点都是某个节点的右节点。
要构建出这样一棵满二叉树，需要让  是  的整数幂。
如果  不是  的整数幂怎么办呢？很简单，往后补  使它成为  的整数幂即可。
于是，我们可以写出一个非常简短的建树函数（以维护和信息为例）：
int n, tree[1 << __lg(MAXN) + 2]; // 其实普通线段树也只需要开这么大空间
void build(auto bg, auto ed) // 这里为了泛用性传的是迭代器
{
    n = 1 << __lg(ed - bg) + 1; // 补成2的整数幂
    for (int i = 0; i < ed - bg; ++i)
        tree[i + n] = *(bg + i);
    for (int i = n - 1; i > 0; --i)
        tree[i] = tree[i << 1] + tree[i << 1 | 1];
}
然后我们还可以写出一个更简短的单点修改函数：void update(int x, int d)
{
    for (int i = x + n; i; i >>= 1) 
        tree[i] += d;
}
区间查询时，我们把查询  这个闭区间的信息换成查询  这个开区间的信息。然后从下往上，一层一层地缩小区间范围。观察上图发现，不断上移的过程，就是不断把端点及其兄弟排除出查询区间的过程。而如果查询区间的左端点是父节点的左儿子 / 右端点是父节点的右儿子，说明它的兄弟应当是答案的一部分。最后在查询的区间为空区间（两端点为兄弟节点）时，退出循环。int query(int l, int r)
{
    int ans = 0;
    for (l += n - 1, r += n + 1; l ^ r ^ 1; l >>= 1, r >>= 1)
    {
        if (~l & 1) ans += tree[l ^ 1]; // 左端点是左儿子
        if (r & 1) ans += tree[r ^ 1]; // 右端点是右儿子
    }
    return ans;
}
因为我们把闭区间查询改成了开区间查询，如果原数据是0-index的话就会出现-1这样的端点。为了避免这种情况，建议原容器用1-index存放并稍微开大一点。区间修改可能稍微麻烦一点点。在非递归的情况下，标记下传是比较困难的，所以我们不下传标记，而是将标记永久化。具体地，和上文中区间查询类似，当左端点是左儿子/右端点是右儿子时，我们对它的兄弟进行修改并打上标记，表示这棵子树中每个节点都要修改。但单纯这样是不够的，因为上述修改还会波及到这些节点的各级祖先。所以我们需要在途中根据实际修改的区间长度来更新各级祖先的值，而且这种操作需要一路上推到根节点。void update(int l, int r, int d)
{
    int len = 1, cntl = 0, cntr = 0; // cntl、cntr是左、右两边分别实际修改的区间长度
    for (l += n - 1, r += n + 1; l ^ r ^ 1; l >>= 1, r >>= 1, len <<= 1)
    {
        tree[l] += cntl * d, tree[r] += cntr * d;
        if (~l & 1) tree[l ^ 1] += d * len, mark[l ^ 1] += d, cntl += len;
        if (r & 1) tree[r ^ 1] += d * len, mark[r ^ 1] += d, cntr += len;
    }
    for (; l; l >>= 1, r >>= 1)
        tree[l] += cntl * d, tree[r] += cntr * d;
}
在有区间修改存在时，区间查询也需要考虑标记的影响。所以除了加上端点的兄弟节点的信息，沿途中遇到的标记也对答案有相应的贡献， 依赖于实际查询的区间长度，这同样也需要上推到根节点。int query(int l, int r)
{
    int ans = 0, len = 1, cntl = 0, cntr = 0;
    for (l += n - 1, r += n + 1; l ^ r ^ 1; l >>= 1, r >>= 1, len <<= 1)
    {
        ans += cntl * mark[l] + cntr * mark[r];
        if (~l & 1) ans += tree[l ^ 1], cntl += len;
        if (r & 1) ans += tree[r ^ 1], cntr += len;
    }
    for (; l; l >>= 1, r >>= 1)
        ans += cntl * mark[l] + cntr * mark[r];
    return ans;
} 

0x00 Preface说到线段树大家都不陌生，一个用于维护区间信息的  数据结构。Zkw线段树是一个改良版的线段树。其功能与传统线段树相同，也是用于维护区间信息。但是Zkw线段树有很多优点：1. 代码简短；2. 纯天然非递归；3. 常数小（尤其在差分区间更新时），因此受到很多竞赛党的喜爱。由于涉及内容较多，Zkw线段树将(预计)分两篇文章介绍。本篇文章主要focus on zkw线段树的原理 / 基础功能。P.S. 如果您想学线段树可又无法完全理解那么这篇文章十分适合您学习一个更加简单的线段树实现0x01 Basis为了辅助大家理解Zkw线段树，我们先看一下传统线段树是如何存储信息 / 建树的：不难看出，第  个结点存储的信息是第  个结点的前半段或后半段，并且叶子结点都是单点数据。传统建树的方法：从根(当前)结点开始递归如果  那么存下当前端点信息，然后分别递归进入左子树和右子树；否则存下当前端点信息并输入当前结点信息，退出当前递归。维护当前结点  的信息（即从左儿子和右儿子收集信息，如果是求和就把两个结点值相加赋给父结点）Talk is not that helpful ... 这里是普通线段树的建树代码:inline void Maintain(int rt) {
    // 这里以区间和为例
    tree[rt].sum = tree[rt<<1].sum + tree[rt<<1|1].sum;
}
void Build(int rt, int l,int r) {
    tree[rt].l = l;
    tree[rt].r = r;
    // l==r -> 到达叶子结点
    if(l == r) {
        tree[rt].sum = read(); // 读入数据
        return;
    }
    int mid = (l+r) >> 1; // 求出l,r的中点
    Build(rt<<1, l, mid); // 建立左区间(儿子)
    Build(rt<<1|1, mid+1, r); // 建立右区间(儿子)
    Maintain(rt); // 收集儿子们的信息给爹地
}
说了这么多，一句话总结：普通线段树是自顶向下一直到叶子（信息输入）然后在一层一层退回到根结点（信息收集）。那么就有一位大神这么问：“既然可以从顶向下，那么能不能自底向上呢？”。答案是“完全O吉拔K”。因此诞生了Zkw线段树（麻吉亚巴库内）。Zkw线段树建树的方式就是首先输入叶子结点的信息然后再一路向上传递信息，直到根结点。这时问题又来了....Where is the first leaf？？？我怎么找到第一个叶子在哪？假设我们的单点数量（叶子数量）正好是  ，那么我们手里就握着一个满二叉树了，这样我们就能轻松地计算出来第一个结点的位置是：  。但是如果不是满二叉树怎么办呢？没有关系，现在的电脑内存不是问题！直接开成满二叉树就好啦~~。这样一来，第一个叶子结点的位置就是：  (见代码下的解释)，也就是比  大的最小的  。找到叶子结点之后，直接输入叶子结点信息，然后一个一个结点上传信息到父亲结点。于是我们得到了这样的代码：inline void Maintain(int x) {
    tree[x] = tree[lson(x)] + tree[rson(x)];
}

inline void Build() {
    for(M=1;M<N;M<<=1);
    for(int i=M+1;i<=M+N;++i) scanf("%d", &tree[i]);
    for(int i=M-1;i;--i) Maintain(i);
}
Update on 10.06:感谢 @Dilthey提示。NOTICE：看到评论中很多朋友问为什么要在  处开始输入。我这里统一解释一下，评论中我所说的也有问题(这里说声抱歉，昨天一天都在路上，没有时间思考....)，以文章此处为标准好啦。建议先看完下一节（区间和计算）再来看此处有助于理解。从  开始是为了进行区间查询。假设我们查询的区间就是  ，这时为了进行查询我们会将  转换为  ，看上去没有区别，其实是有区别的。由于位于  上的数字是否能被统计上与左端点位置相关（  ），如果从  开始输入会导致查询时统计不到位于  上的信息，因为  初始位置就是第一个叶子的位置了(  )... 但是如果换成  开始，查询时  的位置依旧是  ，但是第一个叶子的位置在  上，这样就能够统计到那个叶子上的信息啦。因此要从  开始输入信息。这样一来，代码一下缩短了好多！三行建树，就是这么简洁。连左右儿子的位置也不需要存了呢~是不是美滋滋(不是，因为空间复杂度变高了好多....TAT不过萌大奶！内存这么大！（滑稽)0x02 Single-Point Modification由于区间修改有一些不同的操作，本篇先介绍最简单的单点修改。由于单点修改仅需要修改叶子结点，因此Zkw线段树的单点更新比传统线段树高到不知.....咳咳，非常简单。由于我们能够通过简单的加法计算找到叶子结点，我们只需要(Simply)修改掉叶子结点的值，然后一路向上，更新到根结点。例如：如果修改了上图  的值，我们的更新路径为：（Graphviz用得不好不会画这样的....手绘了2333）是不是很森破？代码酱在这里哟：inline void Update(int pos,int v) {
    pos += M;
    tree[pos] = v;
    for(pos>>=1;pos;pos>>=1) Maintain(pos);
}
短，真短，真鸡鹅短！非常轻松愉快地完成了Update操作。并且Zkw线段树在Update上的效率要比普通线段树优秀，因为Zkw线段树寻找叶子的复杂度是  ，Removed(而传统线段树是  。所以如果某个题有频繁的单点修改，Zkw线段树会是一个非常好的选择。)感谢 @Schureed 提供的信息：事实上传统线段树在建树时可以记录下叶子结点位置，因此也可以做到  的叶子查找。0x03 Range Sum Query在仅存在单点更新的情况下，RMQ和区间和查询方法一样，因此这里依旧以区间和查询为例介绍区间操作。当遇到区间和查询时，问题双来了....传统线段树通过递归查询需要加和的区间最后统计所有的和，但是Zkw线段树....没法从顶上找到需要加和的区间啊QwQ....怎么办呢？但是换个方向思考...从底向上，查询区间为  ，我们只能知道当前区间是否在查询区间内，即：如果当前查询区间左端点  所指向结点是线段树某个结点的左儿子且  (即右端点指向结点不是左端点指向结点的兄弟)，那么它的兄弟结点  必然在查询区间内。同理，如果  所指向结点的兄弟结点不是  ，那么它的兄弟结点必然包含在查询区间内。如图：因此我们实际能够查询到的是  。没有关系，我们只需要查询 那么就相当于查询  啦~~多么智慧。为了避免重复查询，当  和  是兄弟结点时，我们就需要停止统计啦。代码如下。那么区间和查询的代码如下：inline int Sum(int l,int r) {
    int ans = 0;
    // l=l+M-1->将查询区间改为L-1，r=r+M+1->将查询区间改为R+1
    // l^r^1 -> 相当于判断l与r是否是兄弟节点
    for(l=l+M-1,r=r+M+1;l^r^1;l>>=1,r>>=1) {
        if(~l&1) // l % 2 == 0 即l是l/2的左儿子
            ans += tree[l^1];
        if(r&1) // r % 2 == 1 即r是r/2的右儿子
            ans += tree[r^1];
    }
    return ans;
}
Very Very森破，容易理解对不对？QwQ。人类的智慧！！Zkw大神的智慧！！Orz这样我们就能在非常短的篇幅内写出一个优秀的线段树啦~Zkw线段树（一）至此结束啦。

//----------------------------------------------------------------------------------------------------------
#include <bits/stdc++.h>
using namespace std;
const int maxn = 100005, inf = 0x3f3f3f3f; //线段树是二叉搜索树,平衡树 
int n = 10, a[maxn] = {5, 3, 7, 2, 12, 1, 6, 4, 8, 15};
struct node { //结点 
	int l, r, mx; //[l,r]左右区间端点,mx表示[l,r]区间最值 
} tree[maxn * 4]; //树节点存储数组

void build(int k, int l, int r) { //创建线段树,k表示存储下标,区间[l,r]
	tree[k].l = l, tree[k].r = r; 
	if (l == r) {
		tree[k].mx = a[l];
		return;
	}
	int mid = (l + r) / 2; //划分点
	int lc = k * 2, rc = k * 2 + 1; //左右孩子存储点
	build(lc, l, mid);
	build(rc, mid + 1, r);
	tree[k].mx = max(tree[lc].mx, tree[rc].mx); //结点的最大值等于左右孩子最值的最大值 
} 
void update(int k, int i, int v) { //单点修改a[i]为v
	if (tree[k].l == tree[k].r && tree[k].l == i) { //找到a[i]
		tree[k].mx = v;
		return;
	} 
	int mid = (tree[k].l + tree[k].r) / 2;
	int lc = k * 2, rc = k * 2 + 1;
	if (i <= mid) update(lc, i, v); //到左子树单点更新 
	else update(rc, i, v); //到右子树单点更新 
	tree[k].mx = max(tree[lc].mx, tree[rc].mx); //返回时修改更新最值 
}
int query(int k, int l, int r) { //区间查询[l,r]的最值 
	if (tree[k].l >= l && tree[k].r <= r) //找到被覆盖的一部分区间
		return tree[k].mx;
	int mid = (tree[k].l + tree[k].r) / 2;
	int lc = k * 2, rc = k * 2 + 1;
	int maxV = -inf; //局部变量
	if (l <= mid) maxV = max(maxV, query(lc, l, r)); //到左子树区间查询
	if (r > mid) maxV = max(maxV, query(rc, l, r)); //到右子树区间查询
	return maxV;	
} 
void print(int k) {
	if (tree[k].mx) {
		printf("%d [%d, %d] -> %d\n", k, tree[k].l, tree[k].r, tree[k].mx);
		print(k << 1);
		print(k << 1 | 1);
	}
}  
int main() {
	int i, v, l, r;
//	cin >> n;
//	for (int i = 1; i <= n; ++i) cin >> a[i];
	build(1, 1, n); //创建线段树
	print(1);
	cout << "输入查询最值的区间[l,r]:" << endl;
	cin >> l >> r;
	cout << query(1, l, r) << endl; //求区间[l,r]的最值
	cout << "输入修改元素的下标和值i, v:" << endl;
	cin >> i >> v;
	update(1, i, v); //将a[i]修改更新为v
	cout << "输入查询最值的区间[l,r]:" << endl;
	cin >> l >> r;
	cout << query(1, l, r) << endl; //求区间[l,r]的最值 
	return 0;
} 
/*
1.区间更新 
①对[l,r]进行区间更新
如果当前结点的区间被查询区间[l,r]覆盖，仅对该结点进行更新（值），并做懒标记，表示该节点被
更新过，对该节点的子节点则不再进行更新，随后向上更新最值 
②区间更新时，过程中若当前节点带有（以前的）懒标记，需要下传懒标记给子节点，更新子节点 
并清除当前节点懒标记，继续更新  
2.区间查询（带有懒标记） 
在查询的过程中，若当前节点带有（以前的）懒标记，需要下传懒标记给子节点，更新子节点 
并清除当前节点懒标记，继续查询，其他操作都一样 
*/
//最值+区间修改（=v）+区间查询线段树
#include <bits/stdc++.h>
using namespace std;
const int maxn = 100005, inf = 0x3f3f3f3f; //线段树是二叉搜索树,平衡树 
int n = 10, a[maxn] = {5, 3, 7, 2, 12, 1, 6, 4, 8, 15};
struct node { //结点 
	int l, r, mx, lz; //[l,r]左右区间端点,mx表示[l,r]区间最值,lz懒标记 
} tree[maxn * 4]; //树节点存储数组
void lazy(int k, int v) {
	tree[k].mx = v; //更新最值
	tree[k].lz = v; //做懒标记 
} 
void pushdown(int k) { //向下传递懒标记 
	lazy(2 * k, tree[k].lz); //传递给左孩子
	lazy(2 * k + 1, tree[k].lz); //传递给右孩子
	tree[k].lz = -1; //清除自己的懒标记 
}
void build(int k, int l, int r) { //创建线段树,k表示存储下标,区间[l,r]
	tree[k].l = l, tree[k].r = r, tree[k].lz = -1; //初始化懒标记 
	if (l == r) {
		tree[k].mx = a[l];
		return;
	}
	int mid = (l + r) / 2; //划分点
	int lc = k * 2, rc = k * 2 + 1; //左右孩子存储点
	build(lc, l, mid);
	build(rc, mid + 1, r);
	tree[k].mx = max(tree[lc].mx, tree[rc].mx); //结点的最大值等于左右孩子最值的最大值 
} 
//update和updateRange完全可以合并,只是单点修改没有懒标记才独立一个方法
void update(int k, int i, int v) { //单点修改a[i]为v
	if (tree[k].l == tree[k].r && tree[k].l == i) { //找到a[i]
		tree[k].mx = v;
		return;
	} 
	if (tree[k].lz != -1) pushdown(k); //访问到以前的懒标记,下移
	int mid = (tree[k].l + tree[k].r) / 2;
	int lc = k * 2, rc = k * 2 + 1;
	if (i <= mid) update(lc, i, v); //到左子树单点更新 
	else update(rc, i, v); //到右子树单点更新 
	tree[k].mx = max(tree[lc].mx, tree[rc].mx); //返回时修改更新最值 
}
void updateRange(int k, int l, int r, int v) { //将区间[l,r]修改更新为v  //改增加v时lazy()懒标记就不是这么写
	if (tree[k].l >= l && tree[k].r <= r) { //找到被覆盖的一部分区间
		lazy(k, v); //更新并做懒标记
		return; 
	} 
	if (tree[k].lz != -1) pushdown(k); //访问到以前的懒标记,下移
	int mid = (tree[k].l + tree[k].r) / 2;
	int lc = k * 2, rc = k * 2 + 1;
	if (l <= mid) updateRange(lc, l, r, v); //到左子树区间更新
	if (r > mid) updateRange(rc, l, r, v); //到右子树区间更新 
	tree[k].mx = max(tree[lc].mx, tree[rc].mx); //返回时修改更新最值 
} 
int query(int k, int l, int r) { //区间查询[l,r]的最值 
	if (tree[k].l >= l && tree[k].r <= r) //找到被覆盖的一部分区间
		return tree[k].mx;
	if (tree[k].lz != -1) pushdown(k); //懒标记下移 
	int mid = (tree[k].l + tree[k].r) / 2;
	int lc = k * 2, rc = k * 2 + 1;
	int maxV = -inf; //局部变量
	if (l <= mid) maxV = max(maxV, query(lc, l, r)); //到左子树区间查询
	if (r > mid) maxV = max(maxV, query(rc, l, r)); //到右子树区间查询
	return maxV;
} 
void print(int k) {
	if (tree[k].mx) {
		printf("%d [%d, %d] -> %d\n", k, tree[k].l, tree[k].r, tree[k].mx);
		print(k << 1);
		print(k << 1 | 1);
	}
}  
int main() {
	int i, v, l, r;
//	cin >> n;
//	for (int i = 1; i <= n; ++i) cin >> a[i];
	build(1, 1, n); //创建线段树
	print(1);
	cout << "输入查询最值的区间[l,r]:" << endl;
	cin >> l >> r;
	cout << query(1, l, r) << endl; //求区间[l,r]的最值
	
	cout << "输入区间更新的区间[l,r]和值v:" << endl;
	cin >> l >> r >> v;
	updateRange(1, l, r, v); //将区间更新为v 
//	cout << query(1, l, r) << endl; //求区间[l,r]的最值
	
	cout << "输入修改元素的下标和值i, v:" << endl;
	cin >> i >> v;
	update(1, i, v); //将a[i]修改更新为v
	
	cout << "输入查询最值的区间[l,r]:" << endl;
	cin >> l >> r;
	cout << query(1, l, r) << endl; //求区间[l,r]的最值 
	return 0;
} 


//和值+区间修改（+v）+区间查询线段树：有很多问题！！！
struct node {
        long long l, r, v, lz;
        node(int _l = 0, int _r = 0) : l(_l), r(_r), v(0), lz(0) {}
};
static const int maxn = 2e4 + 10;
node tree[maxn * 4];
void lazy(int k, int v) { //懒标记
    tree[k].v += v;  //更新值
    tree[k].lz += v; //做懒标记
}
void pushdown(int k) { //下传懒标记并进行实际修改
    lazy(k << 1, tree[k].lz);     //传递懒标记给左孩子
    lazy(k << 1 | 1, tree[k].lz); //传递懒标记给右孩子
    tree[k].lz = 0;               //清除自己的懒标记
}
void pushup(int k) { //上推值
    tree[k].v = tree[k << 1].v + tree[k << 1 | 1].v; //内部节点的值为孩子节点值的和
}
void build(int k, int l, int r) { //创建线段树
    tree[k] = node(l, r);  //初始化
    if (l == r) return;    //叶子节点,即原始的数组值,此处为0
    int mid = l + r >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
}
void updateRange(int k, int l, int r, int v) { //区间[l,r]加v
    if (tree[k].l >= l && tree[k].r <= r) { //做懒标记,不进行实际区间修改
        lazy(k, v);
        return;
    }
    if (tree[k].lz) pushdown(k); //存在懒标记时下推
    int mid = tree[k].l + tree[k].r >> 1;
    if (l <= mid) updateRange(k << 1, l, r, v);
    if (r > mid) updateRange(k << 1 | 1, l, r, v);
    pushup(k); //修改后上推值
    // tree[k].v = tree[k << 1].v + tree[k << 1 | 1].v; //内部节点的值为孩子节点值的和
}
int query(int k, int l, int r) { //区间查询[l,r]的和值,也可用于单点查询
    if (tree[k].l >= l && tree[k].r <= r) return tree[k].v;
    if (tree[k].lz) pushdown(k); //存在懒标记时下推
    int mid = tree[k].l + tree[k].r >> 1;
    int ans = 0;
    if (l <= mid) ans += query(k << 1, l, r);
    if (r > mid) ans += query(k << 1 | 1, l, r);
    return ans;
}