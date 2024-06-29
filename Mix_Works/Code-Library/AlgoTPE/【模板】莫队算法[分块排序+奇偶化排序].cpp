#include <bits/stdc++.h>
using namespace std;
/*莫队算法是一种可以解决大部分区间离线问题的离线算法
主要优化是分块
莫队的思想
其实很简单：通过挪动区间的方式按某种顺序离线处理区间查询操作
有三个关键词：挪动区间、按某种顺序、离线处理

时间复杂度为 O(n\sqrt{n}),也是一种解决问题的好方法
莫队很好写 

假设有这样的一道题:对于一个数列,每次给出一个询问[L,R],求它的区间和
强制用莫队做
开一个数组存储数列:
0 1   2  3  4  5   6  7  8  9 index   
  3   8  1  2  7   4  9  0  6 val
假设知道[2,5]区间的和是18,求[2,6]区间的和,怎样做?
用[2,5]区间的和加上第六项的值即可,答案:18+4=22
那[2,4]区间的和怎么求？
用[2,5]区间的和减去数列第5项的值即可,答案为18-7=11
于是[3,6]区间的和就可以用[2,5]区间和减去第二项的值再加上第六项的值求出.
其他区间依次类推

对当前区间[L,R],分别讨论所有四种情况:
1. 加上当前区间左边一格的贡献：Add(--L); //加上L左边一格的值
2. 加上当前区间右边一格的贡献：Add(++R); //加上R右边一格的值
3. 减去当前区间最左一格的贡献：Sub(L++); //减去L这一格的值
4. 减去当前区间最右一格的贡献：Sub(R--); //减去R这一格的值

区间长度变化的情况如上！所有的区间都可以从当前已知区间的结果扩展出来！

但是这是远远不够的，对于一个n项的数列,假设这样询问m次：
[1,2], [n-1,n], [1,2], [n-1,n] ...
时间复杂度为O(mn)
莫队算法要满足的必要条件是我必须O(1)移动相邻区间,否则复杂度很是爆炸。

但是对于同样的这些询问，如果是以下的顺序:
[1,2], [1,2] ... [n-1,n], [n-1,n] ... 
时间复杂度瞬间到了O(m+n)
询问顺序对莫队算法的时间复杂度有着很大的影响！为了解决这一问题，需要使用分块！

具体方法如下：首先分块,块大小就是普通的\sqrt{n},然后对所有的询问进行排序
排序的规则如下：对于一个询问[L,R],对L按照其所在块排序,对于L在同一块的,按照R进行排序 
排序后,遍历所有询问,然后用刚刚所说的方法得出各个询问的答案记录下来, 最后按照原顺序输出答案即可! 

把[1,n]分成\sqrt{n}块

我们先把这些区间按照左端点l所在的块从左往右排序
再把l所在块相同的区间按r从小到大排序
*/

const int maxn = 5e4 + 5;
int a[maxn];	//记录所有的数 
int pos[maxn]; //a数列中的第几项是第几块的 
int ans[maxn]; //记录答案 
//询问
struct Q {
	int l, r, k;	//询问的区间[l,r], 第几个询问 
} q[maxn];
//奇偶化排序 
const int MAXN = 1e5 + 10;
int sqr = sqrt(n);
struct Q {
	int l, r, id;
	bool operator<(const Q &b) const { //重载<运算符,奇偶化排序
		//只需要知道每个元素归属于哪个块,块大小为sqrt(n),所以直接l/sqrt(n)即可
		if (l / sqr != b.l / sqr) return l < b.l;
		if (l / sqr & 1) //奇数块
			return r < b.r;
		return r > b.r;
	}
} Q[MAXN];

int main() {
	int n, m;
	cin >> n >> m;
	//分块
	int size = sqrt(n); //块的大小 
	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
		pos[i] = i / size; //分块 
	}
	//询问
	for (int i = 0; i < m; ++i) {
		cin >> q[i].l >> q[i].r;
		q[i].k = i;
	}
	//所有的询问的顺序进行排序
	sort(q, q + m, [](Q x, Q y) { return pos[x.l] == pos[y.l] ? x.r < y.r : pos[x.l] < pos[y.l] }); //同一个块按照r排序,否则按照块顺序排
	int l = 1, r = 0; //当前所知的闭区间[l,r]
	//遍历所有的询问 
	for (int i = 0; i < m; ++i) {
		while (q[i].l < l) Add(--l);
		while (q[i].r > r) Add(++r);
		while (q[i].l > l) Sub(l++);
		while (q[i].r < r) Sub(r--);
		//记录答案 
		ans[q[i].k] = res;
	}
    return 0;
}

//----------------------莫队+奇偶化排序----------------------------------------------
const int MAXN = 30005, MAXQ = 200005, MAXM = 1000005;
int sq;
struct query // 把询问以结构体方式保存
{
    int l, r, id;
    bool operator<(const query &o) const // 重载<运算符，奇偶化排序
    {
        // 这里只需要知道每个元素归属哪个块，而块的大小都是sqrt(n)，所以可以直接用l/sq
        if (l / sq != o.l / sq) 
            return l < o.l;
        if (l / sq & 1)
            return r < o.r;
        return r > o.r;
    }
} Q[MAXQ];
int A[MAXN], ans[MAXQ], Cnt[MAXM], cur, l = 1, r = 0;
inline void add(int p)
{
    if (Cnt[A[p]] == 0)
        cur++;
    Cnt[A[p]]++;
}
inline void del(int p)
{
    Cnt[A[p]]--;
    if (Cnt[A[p]] == 0)
        cur--;
}
int main()
{
    int n = read();
    sq = sqrt(n);
    for (int i = 1; i <= n; ++i)
        A[i] = read();
    int q = read();
    for (int i = 0; i < q; ++i)
        Q[i].l = read(), Q[i].r = read(), Q[i].id = i; // 把询问离线下来
    sort(Q, Q + q); // 排序
    for (int i = 0; i < q; ++i)
    {
        while (l < Q[i].l)
            del(l++);
        while (l > Q[i].l)
            add(--l);
        while (r < Q[i].r)
            add(++r);
        while (r > Q[i].r)
            del(r--);
        ans[Q[i].id] = cur; // 储存答案
    }
    for (int i = 0; i < q; ++i)
        printf("%d\n", ans[i]); // 按编号顺序输出
    return 0;
}
//------------------------------------------------------------------
//题目链接：https://www.spoj.com/problems/DQUERY/
//其实可以取消pos数组记录块号的 
#include <bits/stdc++.h>
using namespace std;
const int maxn = 3e4 + 10, maxq = 2e5 + 10, maxm = 1000005;
int a[maxn], cnt[maxm], n, q, sqr;
struct Q {
	int l, r, id;
	bool operator<(const Q &b) const {
		if (l / sqr != b.l / sqr) //先按照左边界的块号从小到大排序 
			return l < b.l;
		else if (l / sqr & 1) //块号相等时,如果是奇数块,按照右边界r从小到大排序 
			return r < b.r;
		return r > b.r;	//偶数块 
	}
} query[maxq];
int ans[maxq];
int res = 0;
void Add(int n) {
	if (cnt[a[n]] == 0) ++res;
	++cnt[a[n]];	
}
void Sub(int n) {
	--cnt[a[n]];
	if (cnt[a[n]] == 0) --res;
}

int main() { 
	scanf("%d", &n);
	sqr = sqrt(n);
	for (int i = 1; i <= n; ++i) 
		scanf("%d", &a[i]); 
	scanf("%d", &q);
	for (int i = 0; i < q; ++i) {
		scanf("%d%d", &query[i].l, &query[i].r);
		query[i].id = i;
	}
	sort(query, query + q);
	int l = 1, r = 0;
	for (int i = 0; i < q; ++i) {
		while (l > query[i].l) Add(--l);
		while (r < query[i].r) Add(++r);
		while (l < query[i].l) Sub(l++);
		while (r > query[i].r) Sub(r--);
		ans[query[i].id] = res;
	}
    for (int i = 0; i < q; ++i)
        printf("%d\n", ans[i]); // 按编号顺序输出
    return 0;
}
