<p>Given an integer array <code>instructions</code>, you are asked to create a sorted array from the elements in <code>instructions</code>. You start with an empty container <code>nums</code>. For each element from <strong>left to right</strong> in <code>instructions</code>, insert it into <code>nums</code>. The <strong>cost</strong> of each insertion is the <b>minimum</b> of the following:</p>

- The number of elements currently in <code>nums</code> that are <strong>strictly less than</strong> <code>instructions[i]</code>.</li>
- The number of elements currently in <code>nums</code> that are <strong>strictly greater than</strong> <code>instructions[i]</code>.

<p>For example, if inserting element <code>3</code> into <code>nums = [1,2,3,5]</code>, the <strong>cost</strong> of insertion is <code>min(2, 1)</code> (elements <code>1</code> and <code>2</code> are less than <code>3</code>, element <code>5</code> is greater than <code>3</code>) and <code>nums</code> will become <code>[1,2,3,3,5]</code>.</p>

<p>Return <em>the <strong>total cost</strong> to insert all elements from </em><code>instructions</code><em> into </em><code>nums</code>. Since the answer may be large, return it <strong>modulo</strong> <code>10<sup>9</sup> + 7</code></p>

**Example 1:**

```swift
Input: instructions = [1,5,6,2]
Output: 1
Explanation: Begin with nums = [].
Insert 1 with cost min(0, 0) = 0, now nums = [1].
Insert 5 with cost min(1, 0) = 0, now nums = [1,5].
Insert 6 with cost min(2, 0) = 0, now nums = [1,5,6].
Insert 2 with cost min(1, 2) = 1, now nums = [1,2,5,6].
The total cost is 0 + 0 + 0 + 1 = 1.
```

**Example 2:**

```swift
Input: instructions = [1,2,3,6,5,4]
Output: 3
Explanation: Begin with nums = [].
Insert 1 with cost min(0, 0) = 0, now nums = [1].
Insert 2 with cost min(1, 0) = 0, now nums = [1,2].
Insert 3 with cost min(2, 0) = 0, now nums = [1,2,3].
Insert 6 with cost min(3, 0) = 0, now nums = [1,2,3,6].
Insert 5 with cost min(3, 1) = 1, now nums = [1,2,3,5,6].
Insert 4 with cost min(3, 2) = 2, now nums = [1,2,3,4,5,6].
The total cost is 0 + 0 + 0 + 0 + 1 + 2 = 3.
```

**Example 3:**

```swift
Input: instructions = [1,3,3,3,2,4,2,1,2]
Output: 4
Explanation: Begin with nums = [].
Insert 1 with cost min(0, 0) = 0, now nums = [1].
Insert 3 with cost min(1, 0) = 0, now nums = [1,3].
Insert 3 with cost min(1, 0) = 0, now nums = [1,3,3].
Insert 3 with cost min(1, 0) = 0, now nums = [1,3,3,3].
Insert 2 with cost min(1, 3) = 1, now nums = [1,2,3,3,3].
Insert 4 with cost min(5, 0) = 0, now nums = [1,2,3,3,3,4].
​​​​​​​Insert 2 with cost min(1, 4) = 1, now nums = [1,2,2,3,3,3,4].
​​​​​​​Insert 1 with cost min(0, 6) = 0, now nums = [1,1,2,2,3,3,3,4].
​​​​​​​Insert 2 with cost min(2, 4) = 2, now nums = [1,1,2,2,2,3,3,3,4].
The total cost is 0 + 0 + 0 + 0 + 1 + 0 + 1 + 0 + 2 = 4.
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= instructions.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= instructions[i] &lt;= 10<sup>5</sup></code></li>
</ul>

题意：给出一个整数数组&nbsp;<code>instructions</code>&nbsp;，根据&nbsp;<code>instructions</code>&nbsp;中的元素创建一个有序数组。一开始有一个空的数组&nbsp;<code>nums</code>&nbsp;，需要&nbsp;<strong>从左到右</strong>&nbsp;遍历&nbsp;<code>instructions</code>&nbsp;中的元素，将它们依次插入&nbsp;<code>nums</code>&nbsp;数组中。每一次插入操作的&nbsp;<strong>代价</strong>&nbsp;是以下两者的 <strong>较小值</strong>：

<ul>
	<li><code>nums</code>&nbsp;中 <strong>严格小于&nbsp;</strong>&nbsp;<code>instructions[i]</code>&nbsp;的数字数目。</li>
	<li><code>nums</code>&nbsp;中 <strong>严格大于&nbsp;</strong>&nbsp;<code>instructions[i]</code>&nbsp;的数字数目。</li>
</ul>

<p>比方说，如果要将&nbsp;<code>3</code> 插入到&nbsp;<code>nums = [1,2,3,5]</code>&nbsp;，那么插入操作的&nbsp;<strong>代价</strong>&nbsp;为&nbsp;<code>min(2, 1)</code> (元素&nbsp;<code>1</code>&nbsp;和&nbsp;&nbsp;<code>2</code>&nbsp;小于&nbsp;<code>3</code>&nbsp;，元素&nbsp;<code>5</code>&nbsp;大于&nbsp;<code>3</code>&nbsp;），插入后&nbsp;<code>nums</code> 变成&nbsp;<code>[1,2,3,3,5]</code>&nbsp;。</p>

<p>返回将&nbsp;<code>instructions</code>&nbsp;中所有元素依次插入&nbsp;<code>nums</code>&nbsp;后的 <strong>总最小代价&nbsp;</strong>。由于答案会很大，请将它对&nbsp;<code>10<sup>9</sup> + 7</code>&nbsp;<b>取余</b>&nbsp;后返回。</p>

---
### 解法1 树状数组（+离散化）
使用普通的一维树状数组（**单点修改+区间查询**），维护 `arr[]` 的各个区间和的信息，其中 `arr[i]` 指的是当前 `nums` 中 `i` 的个数：
```cpp
const int maxn = 1e5 + 10;
int lowbit(int x) { return x & (-x); }
//arr[i]为nums中i的个数
//树状数组维护arr[],求前缀和sum[0:i)即为nums中此时严格<i的数字数目
int range[maxn]; //tree[maxn]
void add(int i, int d) { //单点修改
    while (i < maxn) {
        range[i] += d;
        i += lowbit(i);
    }
}
int sum(int i) { //前缀和
    int ans = 0;
    while (i) {
        ans += range[i];
        i -= lowbit(i);
    }
    return ans;
} 

class Solution {
public:
    int createSortedArray(vector<int>& instructions) {
        memset(range, 0, sizeof(range));
        int n = instructions.size(), ans = 0, mod = 1e9 + 7;
        for (int i = 0; i < n; ++i) {
            int v = instructions[i];
            int lt = sum(v - 1), gt = i - sum(v);
            // cout << lt <<", " << gt << endl;
            int mincost = min(lt, gt);
            ans = (ans + mincost) % mod;
            add(v, 1); //多了一个v,++arr[v],从而修改整个树状数组
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：580 ms, 在所有 C++ 提交中击败了65.36% 的用户
内存消耗：112.1 MB, 在所有 C++ 提交中击败了98.55% 的用户
```
如果数据规模不变、但数值范围更大一些，比如<code>1 &lt;= instructions[i] &lt;= 10<sup>9</sup></code>，就需要使用**离散化**，且分配排名时要**从1开始**：
```cpp
//...
class Solution {
public:
    int createSortedArray(vector<int>& instructions) {
        memset(range, 0, sizeof(range));
        int n = instructions.size(), ans = 0, mod = 1e9 + 7;
        vector<int> tmp(instructions), ins;
        sort(tmp.begin(), tmp.end()); //排序
        int len = unique(tmp.begin(), tmp.end()) - tmp.begin(); //去重
        for (const int &v : instructions) //离散化(从1开始分配排名)
            ins.push_back(lower_bound(tmp.begin(), tmp.end(), v) - tmp.begin() + 1);
        
        for (int i = 0; i < n; ++i) {
            int v = ins[i];
            //...
        }
        return ans;
    }
};
```
效率的损失不可避免：
```cpp
执行用时：1620 ms, 在所有 C++ 提交中击败了8.12% 的用户
内存消耗：129.5 MB, 在所有 C++ 提交中击败了31.38% 的用户
```

---
### 解法2 线段树
树状数组能够实现的，线段树都可以实现，只是线段树占用的空间更大，查询需要的常数时间也大，运行效率要慢于树状数组：
```cpp
struct kv { int v, p; };
const int maxn = 1e5 + 10;
class Solution {
private:
    int segtree[maxn * 4]; //线段树
    int lazy[maxn * 4]; //懒标记
    void down(int root, int ln, int rn) {
        if (lazy[root]) {
            lazy[root << 1] += lazy[root];
            lazy[root << 1 | 1] += lazy[root];
            segtree[root << 1] += lazy[root] * ln;
            segtree[root << 1 | 1] += lazy[root] * rn;
            lazy[root] = 0;
        }
    }
    void build(int l, int r, int root) { //清0,建树;当前区间[l,r],当前节点root
        if (l >= r) { //(同时代表值v)
            segtree[root] = 0;
            return;
        }
        int m = l + (r - l) / 2;
        build(l, m, root << 1);
        build(m + 1, r, root << 1 | 1);
        segtree[root] = segtree[root << 1] + segtree[root << 1 | 1];
        return;
    }
    void add(int x, int c, int l, int r, int root) { //单点修改; 当前区间[l,r],当前节点
        if (l >= r) {
            segtree[root] += c; 
            return;
        }
        int m = l + (r - l) / 2;
        if (x <= m) add(x, c, l, m, root << 1);
        else add(x, c, m + 1, r, root << 1 | 1);
        segtree[root] += c; //修改经过的路径值
        return;
    }
    void update(int x, int y, int c, int l, int r, int root) {
        if (x <= l && y >= r) {
            segtree[root] += (r - l + 1) * c;
            lazy[root] += c;
            return;
        }
        int m = l + (r - l) / 2;
        down(root, m - l + 1, r - m);
        if (x <= m) update(x, y, c, l, m, root << l);
        if (y < m) update(x, y, c, m + 1, r, root << 1 | 1);
        segtree[root] = segtree[root << 1] + segtree[root << 1 | 1];
    }
    int query(int x, int y, int l, int r, int root) {
        if (x <= l && y >= r) return segtree[root];
        int ans = 0;
        int m = l + (r - l) / 2;
        down(root, m - l + 1, r - m);
        if (x <= m) ans += query(x, y, l, m, root << 1);
        if (y > m) ans += query(x, y, m + 1, r, root << 1 | 1);
        segtree[root] = segtree[root << 1] + segtree[root << 1 | 1];
        return ans;
    }
public:
    int createSortedArray(vector<int>& instructions) {
        int n = instructions.size(), ans = 0, mod = 1e9 + 7;
        build(0, 100000, 1);
        for (int i = 0; i < n; ++i) { //遍历所有原数组的下标对应的值,有序插入时的代价
            ans = (ans + min(query(0, instructions[i] - 1, 0, 100000, 1), query(instructions[i] + 1, 100000, 0, 100000, 1))) % mod;
            add(instructions[i], 1, 0, 100000, 1); //先查询,后更新
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：1220 ms, 在所有 C++ 提交中击败了14.16% 的用户
内存消耗：115.1 MB, 在所有 C++ 提交中击败了47.65% 的用户
```

---
### 解法3 归并排序
从左往右将 `instructions[]` 中的元素不断**有序插入** `nums[]` 中，从而创建有序数组 `nums[]` ，这一过程看起来和**归并排序**很相似——左右子树递归返回后，左子树数组有序（相当于 `nums[]` ，当然了，右子树数组也有序），接下来的归并过程，实际上相当于**将右子树数组中的元素有序插入到左子树数组中**，因此我们可以顺便统计**对于右子树数组中的每个元素，有序插入时左子树数组中小于和大于该元素值的元素个数**。问题在于，在归并过程中不方便统计**小于该元素值的个数**，因为  `instructions[]` 中存在重复元素。

更好的做法是——在归并过程之前使用一个循环，对**右子树数组的每个元素**，寻找左子树数组中第一个大于等于它的位置 `lowerBound ` 和第一个大于它的位置 `upperBound` ，于是**在插入时左子树数组中小于该元素值的个数**为 `lowerBound - 1` ，**在插入时左子树数组中大于该元素值的个数**为 `m - upperBound + 1` 。实际代码如下：
```cpp
struct kv { int v, p; };
const int maxn = 1e5 + 10;
class Solution {
private:
    kv ins[maxn]; //记录元素值和其在原数组中的下标
    int Left[maxn] = {0}, Right[maxn] = {0}; //记录原数组中的下标对应的值,在有序插入时小于或者大于该值的数字个数
    kv tmp[maxn]; //归并时的暂存数组
    void mergeSort(int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSort(l, m);
        mergeSort(m + 1, r);
        int i = l, j = m + 1, k = l, lowerBound = l, upperBound = l;
        //在递归返回的左右子树数组有序时,记录右子树数组中每个节点在左子树中小于和大于该点的数目
        for (int t = m + 1; t <= r; ++t) {
            while (lowerBound <= m && ins[lowerBound].v < ins[t].v) ++lowerBound; //ins[lowerBound].v>=ins[t].v
            while (upperBound <= m && ins[upperBound].v <= ins[t].v) ++upperBound; //ins[upperBound].v>ins[t].v
            Left[ins[t].p] += (lowerBound - l); //左子树中小于ins[t].v的个数
            Right[ins[t].p] += (m - upperBound + 1); //左子树中大于ins[t].v的个数
        }
        while (i <= m && j <= r) {
            if (ins[i].v <= ins[j].v) tmp[k++] = ins[i++]; //左子树的元素值<=右子树的元素值
            else tmp[k++] = ins[j++]; //复制右子树的值:下标
        }
        while (i <= m) tmp[k++] = ins[i++];
        while (j <= r) tmp[k++] = ins[j++];
        for (int x = l; x <= r; ++x) ins[x] = tmp[x];
    }
public:
    int createSortedArray(vector<int>& instructions) {
        int n = instructions.size(), ans = 0, mod = 1e9 + 7;
        for (int i = 0; i < n; ++i) {
            ins[i].v = instructions[i];
            ins[i].p = i; //记录原数组中的值:下标映射
        }
        mergeSort(0, n - 1);
        for (int i = 0; i < n; ++i) //遍历所有原数组的下标对应的值,有序插入时的代价 
            ans = (ans + min(Left[i], Right[i])) % mod;
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：760 ms, 在所有 C++ 提交中击败了36.21% 的用户
内存消耗：114.2 MB, 在所有 C++ 提交中击败了60.61% 的用户
```
