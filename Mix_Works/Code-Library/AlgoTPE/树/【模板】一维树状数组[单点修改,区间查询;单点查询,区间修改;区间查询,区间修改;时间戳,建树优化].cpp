#include <bits/stdc++.h>
using namespace std;

#define lowbit ((x) & -(x))
int tree[MAXN];
inline void update(int i, int x) { //单点修改
    for (int pos = i; pos < MAXN; pos += lowbit(pos))
        tree[pos] += x;
}
inline int query(int n) //求前n项和
{
    int ans = 0;
    for (int pos = n; pos; pos -= lowbit(pos))
        ans += tree[pos];
    return ans;
}
inline int query(int a, int b) //区间查询
{
    return query(b) - query(a - 1);
}
//初始化的时候，我们只需要update每个点的初始值即可
//------------------------------------------------------- 
//单点修改+区间查询(特例是单点查询)
//计算出x的二进制表示中从右往左第一个1代表的值
#define lowbit ((x) & -(x)) 
//更新数组tree[],ak=ak+d
void add(int k, int d) { //单点修改原数组的同时,维护了原数组的区间和信息
	while (k <= n) {  //注意更新到tree[n]为止
		tree[k] += d; //修改和ak有关的tree[]
		k += lowbit(k);
	}
}
//求和sum=a1+a2+...+ak
int sum(int k) {
	int sum = 0;
	while (k > 0) { //不对tree[0]进行操作
		sum += tree[k];
		k -= lowbit[k];
	}
	return sum;
}
int queryRange(int l, int r) { //区间查询(单点查询是区间查询的特例)
	return sum(r) - sum(l - 1);
}
//------------------------------------------------------
//单点查询+区间修改(特例是单点修改)
#define lowbit(x) ((x) & (-x))
typedef long long ll;
const int maxn = 5e5 + 10;
ll tree[maxn], n; 
void add(int i, ll d) { //单点修改差分数组的同时,维护了差分数组的区间和信息
	while (i <= n) {
		tree[i] += d;
		i += lowbit(i);
	}
}
int sum(int i) {	//前缀和查询(单点查询) 
	int ret = 0;
	while (i) {
		ret += tree[i];
		i -= lowbit(i);
	}
	return ret;
}
void addRange(int l, int r, ll v) { //区间修改(单点修改是区间修改的特例)
	add(l, v);   
	add(r + 1, -v);
}

int main() { 
	//...
	memset(tree, 0, sizeof(tree));
	ll pre = 0, now; 
	for (int i = 1; i <= n; ++i) {
		scanf("%lld", &now);
		add(i, now - pre);	//维护差分数组 
		pre = now;
	}
	//...
    return 0;
}  
//------------------------------------------------------
//区间查询(特例是单点查询)+区间修改(特例是单点修改)
#define lowbit(x) ((x) & (-x))
const int maxn = 5e5 + 10;
int tree1[maxn], tree2[maxn], n; //t1维护\sum d_i, t2维护\sum d_i\times i
void add(int i, int d1) { //单点修改两个差分数组的同时,维护了两个差分数组的区间和信息
	int d2 = i * d1;
	while (i <= n) {
		tree1[i] += d1, tree2[i] += d2;
		i += lowbit(i);
	}
} 
int sum(int *tree, int i) { //指定树状数组进行前缀和查询(对tree1使用sum即为单点查询)  
	int ret = 0;
	while (i) {
		ret += tree[i];
		i -= lowbit(i);
	}
	return ret;
}
void addRange(int l, int r, int v) { //区间修改(单点修改是区间修改的特例)
	add(l, v);       
	add(r + 1, -v);
}
int queryRange(int l, int r) { //区间查询
	return (r + l) * sum(tree1, r) - sum(tree2, r) //前缀和sum[r]
	- (l * sum(tree1, l - 1) - sum(tree2, l - 1)); //前缀和sum[l-1] 
}
//------------------------------------------------------
//建树：每一个节点的值是由所有与自己直接相连的儿子的值求和得到的。
//因此可以倒着考虑贡献，即每次确定完儿子的值后，用自己的值更新自
//己的直接父亲。
// O(n)建树
void init() {
  for (int i = 1; i <= n; ++i) {
    t[i] += a[i];
    int j = i + lowbit(i);
    if (j <= n) t[j] += t[i];
  }
}
//------------------------------------------------------
/*
时间戳优化：
对付多组数据很常见的技巧。如果每次输入新数据时，都暴力清空树状数组，就可能会造成超时。
因此使用tag标记，存储当前节点上次使用时间（即最近一次是被第几组数据使用）。
每次操作时判断这个位置tag中的时间和当前时间是否相同，
就可以判断这个位置应该是 0 还是数组内的值。
*/
int tag[maxn], tree[maxn], curtag = 0;
void inc() { ++curtag; }
void add(int i, int d) {
	while (i <= n) {
		if (tag[i] != curtag) tree[i] = 0;
		tree[i] += d, tag[i] = curtag;
		i += lowbit(i);
	}
}
int sum(int i) {
	int ret = 0;
	while (i) {
		if (tag[i] == curtag) ret += t[i];
		i -= lowbit(i);
	}
	return ret;
}
//每次利用输入数据改变tree[],然后不清空而是inc 
