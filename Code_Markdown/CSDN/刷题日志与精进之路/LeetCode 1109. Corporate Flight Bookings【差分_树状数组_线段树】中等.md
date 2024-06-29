> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>There are <code>n</code> flights that are labeled from <code>1</code> to <code>n</code>.</p>

<p>You are given an array of flight bookings <code>bookings</code>, where <code>bookings[i] = [first<sub>i</sub>, last<sub>i</sub>, seats<sub>i</sub>]</code> represents a booking for flights <code>first<sub>i</sub></code> through <code>last<sub>i</sub></code> (<strong>inclusive</strong>) with <code>seats<sub>i</sub></code> seats reserved for <strong>each flight</strong> in the range.</p>

<p>Return <em>an array </em><code>answer</code><em> of length </em><code>n</code><em>, where </em><code>answer[i]</code><em> is the total number of seats reserved for flight </em><code>i</code>.</p>

<p><strong>Example 1:</strong></p>

```clike
Input: bookings = [[1,2,10],[2,3,20],[2,5,25]], n = 5
Output: [10,55,45,25,25]
Explanation:
Flight labels:        1   2   3   4   5
Booking 1 reserved:  10  10
Booking 2 reserved:      20  20
Booking 3 reserved:      25  25  25  25
Total seats:         10  55  45  25  25
Hence, answer = [10,55,45,25,25] 
```
 

<p><strong>Example 2:</strong></p>

```clike
Input: bookings = [[1,2,10],[2,2,15]], n = 2
Output: [10,25]
Explanation:
Flight labels:        1   2
Booking 1 reserved:  10  10
Booking 2 reserved:      15
Total seats:         10  25
Hence, answer = [10,25]
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 2 * 10<sup>4</sup></code></li>
	<li><code>1 &lt;= bookings.length &lt;= 2 * 10<sup>4</sup></code></li>
	<li><code>bookings[i].length == 3</code></li>
	<li><code>1 &lt;= first<sub>i</sub> &lt;= last<sub>i</sub> &lt;= n</code></li>
	<li><code>1 &lt;= seats<sub>i</sub> &lt;= 10<sup>4</sup></code></li>
</ul>



题意：这里有&nbsp;<code>n</code>&nbsp;个航班，它们分别从 <code>1</code> 到 <code>n</code> 进行编号。有一份航班预订表&nbsp;<code>bookings</code> ，表中第&nbsp;<code>i</code>&nbsp;条预订记录&nbsp;<code>bookings[i] = [first<sub>i</sub>, last<sub>i</sub>, seats<sub>i</sub>]</code>&nbsp;意味着在从 <code>first<sub>i</sub></code>&nbsp;到 <code>last<sub>i</sub></code> （<strong>包含</strong> <code>first<sub>i</sub></code> 和 <code>last<sub>i</sub></code> ）的 <strong>每个航班</strong> 上预订了 <code>seats<sub>i</sub></code>&nbsp;个座位。请你返回一个长度为 <code>n</code> 的数组&nbsp;<code>answer</code>，其中 <code>answer[i]</code> 是航班 <code>i</code> 上预订的座位总数。</p>

---
涉及到「区间」相关问题时，需要仔细考虑问题属于哪一类，该用什么方式解决：
- 数组不变，区间查询：**「前缀和」**、树状数组、线段树；
- 数组单点修改、区间查询：**「树状数组」**、线段树；
- 数组区间修改：**「差分」**，加上单点查询：**「线段树」**、树状数组
- 数组区间修改、区间查询：**「线段树」**、树状数组

树状数组本是针对**单点修改、区间查询**问题的，但通过一定的操作和转化，比如对原数组差分，就能使用树状数组，解决**区间修改、单点查询**问题；使用多个树状数组来维护多个指标，也能解决**区间修改、区间查询**中特定的问题，或者实现**类似线段树的持久化标记**。只是这两者都不大具有一般性，因此要谨慎使用。

###  解法1 差分
原始数组 `answer[]` ，`answer[i]` 表示航班 `i` 上预订的座位总数，对应的差分数组 `d[]` ，`d[0] = answer[0], d[i] = answer[i] - answer[i - 1]` ，从而可以将每个 `bookings[i]` 都转换为 `d[firsti] += seatsi, d[lasti + 1] -= seatsi` 。最后 $O(n)$ 从前往后恢复数组 `answer[]` 即可。整个算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        int m = bookings.size(), sum = 0;
        vector<int> d(n + 1, 0), ans;
        for (const vector<int>& book : bookings) d[book[0] - 1] += book[2], d[book[1]] -= book[2];
        for (int i = 0; i < n; ++i) {
            sum += d[i];
            ans.push_back(sum);
        }
        return ans;
    }
};
//执行用时：236 ms, 在所有 C++ 提交中击败了69.65% 的用户
//内存消耗：67.9 MB, 在所有 C++ 提交中击败了29.05% 的用户
```
---
### 解法2 树状数组
虽然此处有点水土不服，但是通过维护原数组的差分数组、进行区间修改，最后构造答案时对每个下标做**单点查询**操作，也能解决这一题。令 `bookings` 数组长度为 `m` ，原数组长度为 `n` ，则时间复杂度为 $O(m\log n +n\log n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
const int maxn = 2e4 + 100;
#define lowbit(x) ((x) & (-(x)))
int tree[maxn];
void add(int i, int d) {
    while (i < maxn) { tree[i] += d; i += lowbit(i); }
}
int sum(int i) {
    int ans = 0;
    while (i) { ans += tree[i]; i -= lowbit(i); }
    return ans;
}
class Solution {
public:
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        vector<int> ans;
        memset(tree, 0, sizeof(tree)); //注意清空数组
        for (const vector<int>& book : bookings) add(book[0], book[2]), add(book[1] + 1, -book[2]);
        for (int i = 0; i < n; ++i) ans.push_back(sum(i + 1));
        return ans;
    }
};
//执行用时：180 ms, 在所有 C++ 提交中击败了73.43% 的用户
//内存消耗：67.3 MB, 在所有 C++ 提交中击败了31.77% 的用户
```
---
### 解法3 线段树
这里涉及到了「区间修改」操作，因此必须使用懒标记。查询、（单点或区间）修改时下推懒标记、进行实际修改，区间修改时直接标记相应区间。整体时空复杂度和树状数组几乎一致，只是常数相当大：
```cpp
//C++ version
class Solution {
private:
    struct node {
        long long l, r, v, lz;
        node() {}
        node(int _l, int _r) : l(_l), r(_r), v(0), lz(0) {}
    };
    static const int maxn = 2e4 + 10;
    node tree[maxn * 4];
    void init() { 
        for (int i = 0; i < maxn * 4; ++i) 
            tree[i].l = tree[i].r = tree[i].v = tree[i].lz = 0;
    }
    void lazy(int k, int v) { //懒标记
        tree[k].v += v;  //更新值
        tree[k].lz += v; //做懒标记
    }
    void pushdown(int k) { //下传懒标记并进行实际修改
        lazy(k << 1, tree[k].lz);      //传递懒标记给左孩子
        lazy(k << 1 | 1, tree[k].lz); //传递懒标记给右孩子
        tree[k].lz = 0;                //清除自己的懒标记
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
public:
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        vector<int> ans; 
        build(1, 1, n);
        for (const vector<int>& book : bookings) updateRange(1, book[0], book[1], book[2]);
        for (int i = 0; i < n; ++i) ans.push_back(query(1, i + 1, i + 1)); 
        return ans;
    }
};
//执行用时：272 ms, 在所有 C++ 提交中击败了5.84% 的用户
//内存消耗：69.8 MB, 在所有 C++ 提交中击败了25.48% 的用户
```
