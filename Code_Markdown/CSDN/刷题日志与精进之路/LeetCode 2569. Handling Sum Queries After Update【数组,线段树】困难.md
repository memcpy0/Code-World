> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个下标从 **0** 开始的数组 `nums1` 和 `nums2` ，和一个二维数组 `queries` 表示一些操作。总共有 3 种类型的操作：
1. 操作类型 1 为 `queries[i] = [1, l, r]` 。你需要将 `nums1` 从下标 `l` 到下标 `r` 的所有 `0` 反转成 `1` 或将 `1` 反转成 `0` 。`l` 和 `r` 下标都从 **0** 开始。
2. 操作类型 2 为 `queries[i] = [2, p, 0]` 。对于 `0 <= i < n` 中的所有下标，令 `nums2[i] = nums2[i] + nums1[i] * p` 。
3. 操作类型 3 为 `queries[i] = [3, 0, 0]` 。求 `nums2` 中所有元素的和。

请你返回一个数组，包含所有第三种操作类型的答案。

**示例 1：**
```java
输入：nums1 = [1,0,1], nums2 = [0,0,0], queries = [[1,1,1],[2,1,0],[3,0,0]]
输出：[3]
解释：第一个操作后 nums1 变为 [1,1,1] 。第二个操作后，nums2 变成 [1,1,1] ，所以第三个操作的答案为 3 。所以返回 [3] 。
```
**示例 2：**
```java
输入：nums1 = [1], nums2 = [5], queries = [[2,0,0],[3,0,0]]
输出：[5]
解释：第一个操作后，nums2 保持不变为 [5] ，所以第二个操作的答案是 5 。所以返回 [5] 。``
```
**提示：**
- `1 <= nums1.length,nums2.length <= 10^5`
- `nums1.length = nums2.length`
- `1 <= queries.length <= 10^5`
- `queries[i].length = 3`
- `0 <= l <= r <= nums1.length - 1`
- `0 <= p <= 10^6`
- `0 <= nums1[i] <= 1`
- `0 <= nums2[i] <= 10^9`

---
### 解法 线段树（区间更新和区间查询）
线段树是一个二叉树，每个结点保存数组 $nums$ 在区间 $[s,e]$ 的最小值、最大值或者总和等信息。线段树可以用树也可以用数组（堆式存储）来实现。对于**数组实现**，假设根结点的下标为 $1$ ，如果一个结点在数组的下标为 $node$ ，那么它的左子结点下标为 $node\times 2$ ，右子结点下标为 $\textit{node} \times 2 + 1$ ，线段树可以在 $O(\log N)$ 的时间复杂度内实现**单点修改、区间修改、区间查询**（区间求和，求区间最大值，求区间最小值）等操作，关于线段树的详细描述可以参考「[线段树](https://leetcode.cn/link/?target=https%3A%2F%2Foi-wiki.org%2Fds%2Fseg)」。

区间更新的线段树，需要**借助「[懒惰标记](https://leetcode.cn/problems/handling-sum-queries-after-update/solutions/2356392/geng-xin-shu-zu-hou-chu-li-qiu-he-cha-xu-kv6u/)」来标记当前结点所在区间是否需要更新**。

建树 $build$ 函数：
1. 我们在结点 $node$ 保存数组 $\textit{nums}$ 在区间 $[s,e]$ 的总和。
2. $s = e$ 时，结点 $node$ 是叶子结点，它保存的值等于 $nums[s]$ 。
3. $s<e$ 时，结点 $node$ 的左子结点保存区间 $\Big [ s, \Big \lfloor \dfrac{s + e}{2} \Big \rfloor \Big ]$ 的总和，右子结点保存区间 $\Big [ \Big \lfloor \dfrac{s + e}{2} \Big \rfloor + 1, e \Big ]$ 
 的总和，那么结点 $node$ 保存的值等于它的两个子结点保存的值之和。
    
假设 $nums$ 的大小为 $n$ ，我们规定根结点 $node=1$ 保存区间 $[0, n - 1]$ 的总和，然后自下而上递归地建树。
- 区间修改 $modify$ 函数：当要修改区间 $nums[left⋯right]$ 的值时，==查看当前区间的结点此前是否已经「更新」过==。如果更新过，那么**通过 $pushdown$ 函数将更新标记传递到子结点，对之前的操作进行更新**，同时更新设置每个结点的懒标记 $lazytag$ ，**后续该位置便可以认为无需进行更新操作**。
- 区间查询 $query$ 函数：给定区间 $[left, right]$ 时，也**需要像区间更新操作一样，需要使用 $pushdown$ 函数将更新标记往下传递到子结点**，否则区间本身的数值实际上没有更新，==懒标记只在区间修改或者区间查询时会往下传递，否则只是标记该区间需要更新==。将区间 $[left, right]$ 拆成多个结点对应的区间。
    - 如果结点 $node$ 对应的区间与 $[left, right]$ 相同，可以直接返回该结点的值，即当前区间和。
    - 如果结点 $node$ 对应的区间与 $[left, right]$ 不同，设左子结点对应的区间的右端点为 $m$ ，那么**将区间 $[left, right]$ 沿点 $m$ 拆成两个区间，分别向下传递懒标记，并计算左子结点和右子结点**。
    - 从根结点开始递归地拆分区间 $[left, right]$，边拆分边计算并返回最终结果即可。

本题目中含有三种操作：
- 第一种操作是将给定区间 $[left, right]$ 内的所有数据进行反转，实际为是区间更新，此时我们可以利用线段树进行区间更新，此时需要用到「线段树的区间修改与懒惰标记」。
- 第二种操作是唯一对 $nums_2$ 中的元素进行更新，此时 $\textit{nums}'_2[i] = \textit{nums}_2[i] + \textit{nums}_1[i] \times p$ ，**设数组 $\textit{nums}_2$ 更新之前的和为 $sum$** ，更新之后的和为 $sum′$ 。再假设 $nums_1$ 中总共有 $c$ 个 $1$ ，那么操作2相当于把 $nums_2$ 的元素和增加了 $c \times p$ 。计算过程如下：
$$\begin{aligned} \textit{sum}' &= \sum\limits_{i=0}^{n-1}{\textit{nums}'_2[i]} \\ &= \sum\limits_{i=0}^{n-1}(\textit{nums}_2[i] + \textit{nums}_1[i] \times p) \\ &=\sum\limits_{i=0}^{n-1}\textit{nums}_2[i] + p \times \sum\limits_{i=0}^{n-1}\textit{nums}_1[i] \\ &= \textit{sum} + p \times \sum\limits_{i=0}^{n-1}\textit{nums}_1[i] \end{aligned}$$ 
    根据上述等式可以看到，**每次执行操作二时，实际 $num_2$ 的和会加上 $p$ 倍 $num_1$ 的元素之和，可在每次更新时维护数组 $num_2$ 的和**。由于 $num_1$ 初始化时全部为 $0$ ，经过第一种操作时部分元素会进行反转，因此==只需要用线段树维护区间内 $1$ 的个数，每次进行区间查询即可得到数组 $num_1$ 的元素之和==。
- 第三种操作是求数组 $num_2$ 的元素之和，此时返回操作二中维护的 $num_2$ 的和即可。

根据以上分析，建立区间更新的线段树，可以参考「[线段树的区间修改与懒惰标记](https://leetcode.cn/link/?target=https%3A%2F%2Foi-wiki.org%2Fds%2Fseg%2F%23%E7%BA%BF%E6%AE%B5%E6%A0%91%E7%9A%84%E5%8C%BA%E9%97%B4%E4%BF%AE%E6%94%B9%E4%B8%8E%E6%87%92%E6%83%B0%E6%A0%87%E8%AE%B0)」，当遇到操作一时进行区间更新，遇到操作二时进行区间查询即可。
```cpp
struct SegNode {
    int l, r, sum;
    bool lazyTag;
    SegNode() {
        l = r = sum = 0;
        lazyTag = false;
    }
};
class SegTree {
private:
    vector<SegNode> arr;
public:
    SegTree(vector<int>& nums) {
        int n = nums.size();
        arr = vector<SegNode>(n * 4 + 1);
        build(1, 0, n - 1, nums);
    }
    void build(int id, int l, int r, const vector<int>& nums) {
        arr[id].l = l;
        arr[id].r = r;
        arr[id].lazyTag = false;
        if (l == r) {
            arr[id].sum = nums[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(2 * id, l, mid, nums);
        build(2 * id + 1, mid + 1, r, nums);
        arr[id].sum = arr[2 * id].sum + arr[2 * id + 1].sum;
    }
    // pushdown函数：下传懒标记，将当前区间的修改情况下放到其左右孩子结点
    void pushdown(int x) {
        if (arr[x].lazyTag) {
            arr[2 * x].lazyTag = !arr[2 * x].lazyTag;
            arr[2 * x].sum = arr[2 * x].r - arr[2 * x].l + 1 - arr[2 * x].sum; // 翻转后1的个数
            arr[2 * x + 1].lazyTag = !arr[2 * x + 1].lazyTag;
            arr[2 * x + 1].sum = arr[2 * x + 1].r - arr[2 * x + 1].l + 1 - arr[2 * x + 1].sum;
            arr[x].lazyTag = false;
        }
    }
    /** 区间修改 **/
    void modify(int id, int l, int r) {
        if (arr[id].l >= l && arr[id].r <= r) {
            arr[id].sum = (arr[id].r - arr[id].l + 1) - arr[id].sum;
            arr[id].lazyTag = !arr[id].lazyTag;
            return;
        }
        pushdown(id);
        int mid = (arr[id].l + arr[id].r) >> 1;
        if (arr[2 * id].r >= l) modify(2 * id, l, r);
        if (arr[2 * id + 1].l <= r) modify(2 * id + 1, l, r);
        arr[id].sum = arr[2 * id].sum + arr[2 * id + 1].sum;
    }
    /** 区间查询 **/
    int query(int id, int l, int r) {
        if (arr[id].l >= l && arr[id].r <= r) return arr[id].sum;
        if (arr[id].r < l || arr[id].l > r) return 0;
        pushdown(id);
        int ans = 0;
        if (arr[2 * id].r >= l) ans += query(2 * id, l, r);
        if (arr[2 * id + 1].l <= r) ans += query(2 * id + 1, l, r);
        return ans;
    }
    int sumRange(int left, int right) {
        return query(1, left, right);
    }
    void reverseRange(int left, int right) {
        modify(1, left, right);
    }
};
class Solution {
public:
    vector<long long> handleQuery(vector<int>& nums1, vector<int>& nums2, vector<vector<int>>& queries) {
        int n = nums1.size();
        int m = queries.size();
        SegTree tree(nums1);
        long long sum = accumulate(nums2.begin(), nums2.end(), 0LL);
        vector<long long> ans;
        for (int i = 0; i < m; ++i) {
            if (queries[i][0] == 1) {
                int l = queries[i][1];
                int r = queries[i][2];
                tree.reverseRange(l, r);
            } else if (queries[i][0] == 2) {
                sum += (long long) tree.sumRange(0, n - 1) * queries[i][1];
            } else if (queries[i][0] == 3) {
                ans.emplace_back(sum);
            }
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(m \log n)$ ，其中 $m$ 表示操作 $1$ 与操作 $2$ 的次数之和，$n$ 表示数组的长度。每次遇到操作类型 $1$ 与操作类型 $2$ 时需要更新线段树，线段树每次更新与查询的时间复杂度均为 $O(\log n)$ ，一共最多有 $m$ 次操作，因此总的时间复杂度为 $O(m \log n)$ 。
- 空间复杂度：$O(Cn)$ ，其中 $n$ 表示数组的长度。本题解中线段树采用堆式存储，**假设当前数组的长度为 $n$ ，由于线段树是一棵完全二叉树，此时该树的最大深度为  $\lceil \log n \rceil$ ，则其叶子结点的总数为 $2^{\lceil \log n \rceil}$ ，该树中含有的结点总数为 $2^{\lceil \log n \rceil + 1} - 1$** ，此时可以知道 $2^{\lceil \log n \rceil + 1} - 1 \le 2^{\log n + 2} - 1 \le 4n - 1$ ，因此本题中取 $C=4$ 即可。