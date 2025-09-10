> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的整数数组 `nums` 和一个 **非负** 整数 `k` 。

在一步操作中，你可以执行下述指令：

- 在范围 `[0, nums.length - 1]` 中选择一个 **此前没有选过** 的下标 `i` 。
- 将 `nums[i]` 替换为范围 `[nums[i] - k, nums[i] + k]` 内的任一整数。

数组的 **美丽值** 定义为数组中由相等元素组成的最长子序列的长度。

对数组 `nums` 执行上述操作任意次后，返回数组可能取得的 **最大** 美丽值。

**注意：**你 **只** 能对每个下标执行 **一次** 此操作。

数组的 **子序列** 定义是：经由原数组删除一些元素（也可能不删除）得到的一个新数组，且在此过程中剩余元素的顺序不发生改变。

**示例 1：**

**输入：**nums = [4,6,1,2], k = 2
**输出：**3
**解释：**在这个示例中，我们执行下述操作：
- 选择下标 1 ，将其替换为 4（从范围 [4,8] 中选出），此时 nums = [4,4,1,2] 。
- 选择下标 3 ，将其替换为 4（从范围 [0,4] 中选出），此时 nums = [4,4,1,4] 。
执行上述操作后，数组的美丽值是 3（子序列由下标 0 、1 、3 对应的元素组成）。
可以证明 3 是我们可以得到的由相等元素组成的最长子序列长度。

**示例 2：**

**输入：**nums = [1,1,1,1], k = 10
**输出：**4
**解释：**在这个示例中，我们无需执行任何操作。
数组 nums 的美丽值是 4（整个数组）。

**提示：**

- `1 <= nums.length <= 10^5`
- `0 <= nums[i], k <= 10^5`

由于选的是子序列，且子序列的元素都相等，所以元素顺序对答案没有影响，可以先对数组排序。

由于替换操作替换的是一个连续范围内的数，所以排序后，选出的子序列必然也是一段连续子数组。

那么问题变成：「找最长的连续子数组，其最大值减最小值不超过 2k2k2k」，只要子数组满足这个要求，其中的元素都可以变成同一个数。

这个问题可以用 同向双指针 解决。枚举 nums[right]\textit{nums}[\textit{right}]nums[right] 作为子数组的最后一个数，一旦 nums[right]−nums[left]>2k\textit{nums}[\textit{right}]-\textit{nums}[\textit{left}]>2knums[right]−nums[left]>2k，就移动左端点。

right−left+1\textit{right}-\textit{left}+1right−left+1 是子数组的长度，取所有长度最大值，即为答案。

Python3
Go
class Solution:
    def maximumBeauty(self, nums: List[int], k: int) -> int:
        nums.sort()
        ans = left = 0
        for right, x in enumerate(nums):
            while x - nums[left] > k * 2:
                left += 1
            ans = max(ans, right - left + 1)
        return ans
复杂度分析
时间复杂度：O(nlog⁡n)\mathcal{O}(n\log n)O(nlogn)，其中 nnn 为 nums\textit{nums}nums 的长度。瓶颈在排序上。
空间复杂度：O(1)\mathcal{O}(1)O(1)。忽略排序的栈开销，仅用到若干额外变量。



```java
class SegmentTree {
private:
    struct node { // v 表示在 [v-k,v+k] 中的元素个数全部+1
        int lazy = 0;
        int maxcnt = 0; // 某区间的最大的元素个数
    };
    node tree[300010 * 4];
public:
    void pushup(int root) {
        tree[root].maxcnt = max(tree[root << 1].maxcnt, tree[root << 1 | 1].maxcnt);
    }
    void pushdown(int root) {
        if (tree[root].lazy) { // 该区间所有元素的个数+1
            int left = root << 1, right = root << 1 | 1;
            tree[left].lazy += tree[root].lazy;
            tree[left].maxcnt += tree[root].lazy;
            tree[right].lazy += tree[root].lazy;
            tree[right].maxcnt += tree[root].lazy;
            tree[root].lazy = 0;
        }
    }
    void modify(int root, int l, int r, int s, int e, int v) {
        if (s <= l && r <= e) {
            tree[root].lazy += v;
            tree[root].maxcnt += v;
            return;
        }
        pushdown(root);
        int mid = (l + r) >> 1;
        if (s <= mid) modify(root << 1, l, mid, s, e, v);
        if (mid < e) modify(root << 1 | 1, mid + 1, r, s, e, v);
        pushup(root);
    }
    int query(int root, int l, int r, int s, int e) {
        if (s <= l && r <= e) {
            return tree[root].maxcnt;
        }
        pushdown(root);
        int mid = (l + r) >> 1;
        int ans = 0;
        if (s <= mid) ans = max(ans, query(root << 1, l, mid, s, e));
        if (mid < e) ans = max(ans, query(root << 1 | 1, mid + 1, r, s, e));
        pushup(root);
        return ans;
    }
};
class Solution {
private:
    SegmentTree tree;
public:
    int maximumBeauty(vector<int>& nums, int k) {
        for (int i : nums) {
            tree.modify(1, 0, 300001, i + 100000 - k, i + 100000 + k, 1);
        }
        return tree.query(1, 0, 300001, 0, 300001);
    }  
};
```
O(nlog n) 超过5%

从上面可以看到，是区间修改+区间查询（所有叶子中的最大值）

于是可以用差分+前缀和，找到最大的前缀和即可。
差分修改区间，给某个区间+1，前缀和恢复 每个点的值 ，超过 37.5% O(n), O(n) O(max(nums)) 与值域有关

数组实现的话就和值域有关了，不能算作 O(n)。可以用 map 实现，时间复杂度 O(nlogn)。
```cpp
class Solution {
public:
    int maximumBeauty(vector<int>& nums, int k) {
        int diff[300010]; memset(diff, 0, sizeof(diff));
        for (int i : nums) ++diff[i + 100000 - k], --diff[i + 100001 + k];
        int ans = 0, sum = 0;
        for (int i = 0; i < 300010; ++i) {
            sum += diff[i];
            // if (sum) cout << sum << endl;
            ans = max(ans, sum);
        }
        return ans;
    }
}; 
```