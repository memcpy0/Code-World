#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
/*
珂朵莉树的思想在于随机数据下的区间赋值操作很可能让大量元素变为同一个数。
所以我们以三元组<l,r,v>的形式保存数据（区间 [l,r] 中的元素的值都是v）：
*/ 
struct node {
	ll l, r;
	mutable ll v; //mutable要写不然可能会CE 
	node(ll l, ll r, ll v) : l(l), r(r), v(v) { } //构造函数
	bool operator<(const node &o) const { return l < o.l; } //重载小于运算符
};
set<node> kdl; //珂朵莉树
/*
要把结构体放进set里需要重载小于运算符，set会保证内部元素有序（插入、删除和查询的时间复杂度都是 
O(logn)。
而mutable使得当整个结构体为const时，标为mutable的成员仍可变（因为可能有区间加等操作）。

然而，我们进行区间操作时并不总是那么幸运，可能会把原本连续的区间断开。
我们需要一个函数来实现“断开”的操作，把<l,r,v>断成<l,pos-1,v>和<pos,r,v>：
*/
auto split(ll pos) { //set<node>::iterator //支持C++14可用
	auto it = kdl.lower_bound(node(pos, 0, 0)); //寻找左端点大于等于pos的第一个结点
	//如果不支持C++11，auto需要改为set<node>::iterator 
	if (it != kdl.end() && it->l == pos) //如果已经存在以pos为左端点的结点，直接返回
		return it; 
	it--; //否则往前一个结点
	ll l = it->l, r = it->r, v = it->v;
	kdl.erase(it); //删除该结点
	kdl.insert(node(l, pos - 1, v)); //插入<l, pos - 1, v>和<pos, r, v>
	return kdl.insert(node(pos, r, v)).first; //返回以pos开头的那个结点的迭代器
	//insert默认返回值是一个pair,第一个成员是我们想要的 
}
//珂朵莉树的精髓在于区间赋值。而区间赋值操作的写法也极其简单
void assign(ll l, ll r, ll v) {
	auto end = split(r + 1), begin = split(l); //顺序不能颠倒,否则可能RE
	kdl.erase(begin, end); //清除一系列结点
	kdl.insert(node(l, r, v)); //插入新的结点	
/*
无非就是把范围内的节点全部删除，然后换上新的（范围较大的）节点而已。只是需要注意求end和begin的顺序不能颠倒，
因为split(end)可能把begin原来所在的节点断开。
*/
}
//区间加（挨个加）
void add(ll l, ll r, ll v) {
	auto end = split(r + 1);
	for (auto it = split(l); it != end; ++it)
		it->v += v;
}
//求区间k大值（直接到vector中排下序）
ll kth(ll l, ll r, ll k) {
	auto end = split(r + 1);
	vector<pair<ll, ll>> v; //这个pair中存储结点的值和区间长度
	for (auto it = split(l); it != end; ++it) 
		v.push_back(make_pair(it->v, it->r - it->l + 1));
	sort(v.begin(), v.end()); //直接按照结点的值的大小排下序
	for (int i = 0; i < v.size(); ++i) { //然后挨个丢出来，直到丢出k个元素为止
		k -= v[i].second;
		if (k <= 0)
			return v[i].first;
	}
}
//求区间n次方和（用快速幂直接求）
ll sum_of_pow(ll l, ll r, ll x, ll y) {
	ll tot = 0;
	auto end = split(r + 1);
	for (auto it = split(l); it != end; ++it) 
		tot = (tot + fastPow(it->v, x, y) * (it->r - it->l + 1)) % y; //快速幂
	return tot; 
}

int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

