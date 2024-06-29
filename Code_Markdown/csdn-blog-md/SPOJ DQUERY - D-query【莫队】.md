题目链接：https://www.spoj.com/problems/DQUERY/

---
Given a sequence of `n` numbers `a1, a2, ..., an` and a number of d-queries. A d-query is a pair `(i, j) (1 ≤ i ≤ j ≤ n)` . For each d-query `(i, j)` , you have to return the number of distinct elements in the subsequence `ai, ai+1, ..., aj` .

**Input**
- Line 1: `n (1 ≤ n ≤ 30000)` .
- Line 2: `n` numbers `a1, a2, ..., an (1 ≤ ai ≤ 10^6)` .
-  Line 3: `q (1 ≤ q ≤ 200000)` , the number of d-queries.
- In the next `q` lines, each line contains `2` numbers `i, j` representing a d-query `(1 ≤ i ≤ j ≤ n)` .

**Output**
- For each d-query `(i, j)` , print the number of distinct elements in the subsequence `ai, ai+1, ..., aj` in a single line.

**Example**
```cpp
Input
5
1 1 2 1 3
3
1 5
2 4
3 5

Output
3
2
3 
```

题意：给出一个数列，给出 `q` 次区间询问 `[l, r]` ，求出区间中不相同的数有多少个。

---
思路：普通莫队+奇偶化排序。代码如下：
```cpp 
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
```
提交：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200831162432820.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

