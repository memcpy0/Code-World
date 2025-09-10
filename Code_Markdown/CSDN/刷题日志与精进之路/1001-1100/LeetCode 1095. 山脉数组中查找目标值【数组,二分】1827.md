> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310131605964.png)

（这是一个 **交互式问题** ）

给你一个 **山脉数组** `mountainArr`，请你返回能够使得 `mountainArr.get(index)` **等于** `target` **最小** 的下标 `index` 值。

如果不存在这样的下标 `index`，就请返回 `-1`。

何为山脉数组？如果数组 `A` 是一个山脉数组的话，那它满足如下条件：

**首先**，`A.length >= 3`

**其次**，在 `0 < i < A.length - 1` 条件下，存在 `i` 使得：
- `A[0] < A[1] < ... A[i-1] < A[i]`
- `A[i] > A[i+1] > ... > A[A.length - 1]`

你将 **不能直接访问该山脉数组**，必须通过 `MountainArray` 接口来获取数据：
- `MountainArray.get(k)` - 会返回数组中索引为`k` 的元素（下标从 0 开始）
- `MountainArray.length()` - 会返回该数组的长度

**注意：**
对 `MountainArray.get` 发起超过 `100` 次调用的提交将被视为错误答案。此外，任何试图规避判题系统的解决方案都将会导致比赛资格被取消。

为了帮助大家更好地理解交互式问题，我们准备了一个样例 “**答案**”：[https://leetcode-cn.com/playground/RKhe3ave](https://leetcode-cn.com/playground/RKhe3ave)，请注意这 **不是一个正确答案**。

**示例 1：**
```js
输入：array = [1,2,3,4,5,3,1], target = 3
输出：2
解释：3 在数组中出现了两次，下标分别为 2 和 5，我们返回最小的下标 2。
```
**示例 2：**
```js
输入：array = [0,1,2,4,2,1], target = 3
输出：-1
解释：3 在数组中没有出现，返回 -1。
```
**提示：**
- `3 <= mountain_arr.length() <= 10000`
- `0 <= target <= 10^9`
- `0 <= mountain_arr.get(index) <= 10^9`

---
### 解法 三次二分
显然，如果山脉数组是一个单调递增或者单调递减的序列，那么我们可以通过二分法迅速找到目标值。

而现在题目中**有一个单调递增序列（峰值左边）和一个单调递减序列（峰值右边）**，我们只是不知道两个序列的分割点，即峰值在哪里。所以我们**第一步应该首先找到峰值**。

而峰值也可以使用**二分法**（或者三分法，对 $l, r$ 找到两个三分点 $lmid, rmid$ ）寻找：
- 对于一个范围 $[i, j]$ ，我们可以先找到范围 $[i, j]$ 中间连续的两个点 $mid$ 与 $mid + 1$ 。
- 如果 $mountainArr.get(mid + 1) > mountainArr.get(mid)$ ，那么可以知道峰值在范围 $[mid + 1, j]$ 内；
- 如果 $mountainArr.get(mid + 1) < mountainArr.get(mid)$ ，那么可以知道峰值在范围 $[i, mid]$ 内。
- 通过这样的方法，我们可以在 $O(\log n)$ 的时间内找到峰值所处的下标。
    ![](https://assets.leetcode-cn.com/solution-static/1095/1095_fig1.png)

这个方法的正确性在于**我们二分的目标是相邻位置数的差值**，我们每次判断的是 $mountainArr.get(mid + 1) - mountainArr.get(mid)$ 与 $0$ 的大小关系。这个差值组成的数组保证了**单调递增的部分差值均为正数，单调递减的部分差值均为负数**，整个数组呈现 `[正数,正数,正数,...,负数,负数]` 这样前半部分均为正数，后半部分均为负数的性质，满足单调性（**二段性**），因此我们可以使用二分查找。

以示例 1 为例，我们对整个数组进行差分，即除了第一个数每个数都减去前一个数得到新的数组，最终我们得到 $[1, 1, 1, 1, -2, -2]$ ，整个差分数组满足单调性，可以应用二分法。

接下来，只需要使用二分法在单调序列中找到目标值即可，注意二分法要使用两次，为了编码简洁可以将二分法封装成函数。
1. 先使用二分法找到数组的峰值。
2. 在峰值左边使用二分法寻找目标值。
3. 如果峰值左边没有目标值，那么使用二分法在峰值右边寻找目标值。
```cpp
class Solution {
private:
    int binarySearch(MountainArray &mountain, int target, int l, int r, int key(int)) {
        target = key(target);
        while (l <= r) {
            int m = l + r >> 1;
            int cur = key(mountain.get(m));
            if (cur == target) return m;
            else if (cur < target) l = m + 1;
            else r = m - 1; 
        }
        return -1;
    }
public:
    int findInMountainArray(int target, MountainArray &mountainArr) {
        int l = 0, r = mountainArr.length() - 1;
        while (l < r) {
            int m = l + r >> 1;
            if (mountainArr.get(m) < mountainArr.get(m + 1)) l = m + 1; // 在右边
            else r = m;
        }
        int peak = l;
        int index = binarySearch(mountainArr, target, 0, peak, 
            [](int x) -> int { return x; });
        if (index != -1) return index;
        return binarySearch(mountainArr, target, peak + 1, mountainArr.length() - 1, 
            [](int x) -> int { return -x; });
    }
};
```
复杂度分析：
- 时间复杂度：$O(\log n)$
- 空间复杂度：$O(1)$
