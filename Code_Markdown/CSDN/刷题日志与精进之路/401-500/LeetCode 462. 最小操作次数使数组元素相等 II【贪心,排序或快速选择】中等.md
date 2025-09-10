> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的整数数组 `nums` ，返回使所有数组元素相等需要的最小操作数。

在一次操作中，你可以使数组中的一个元素加 `1` 或者减 `1` 。

**示例 1：**
```js
输入：nums = [1,2,3]
输出：2
解释：
只需要两次操作（每次操作指南使一个元素加 1 或减 1）：
[1,2,3]  =>  [2,2,3]  =>  [2,2,2]
```
**示例 2：**
```js
输入：nums = [1,10,2,9]
输出：16
```
**提示：**
- `n == nums.length`
- `1 <= nums.length <= 10^5`
- `-10^9 <= nums[i] <= 10^9`

---
题目集合：
- [453. 最小操作次数使数组元素相等](https://leetcode.cn/problems/minimum-moves-to-equal-array-elements/)
- [456. 最小操作次数使数组元素相等 II](https://leetcode.cn/problems/minimum-moves-to-equal-array-elements-ii/)
- [2448. 使数组相等的最小开销](https://leetcode.cn/problems/minimum-cost-to-make-array-equal/)
### 解法1 数学+排序
每次可以将一个数加一或者减一，使得所有数组元素相等。凭借直觉可知，**将所有数组元素向中间靠拢，所需要的操作次数最少**。下面进行证明。

假设数组元素都变成 $x$ 时，所需的移动数最少，那么 $x$ 需要满足什么性质呢？

为了简化讨论，我们先假定数组长度 $n$ 是偶数。我们将数组 $\textit{nums}$ 从小到大进行排序，然后**将数组进行首尾配对**，从而划分为多个**数对**，并将这些数对组成**区间**：$$[\textit{nums}_0, \textit{nums}_{n-1}], [\textit{nums}_1, \textit{nums}_{n-2}], ...,[\textit{nums}_{\frac{n}{2} - 1}, \textit{nums}_{\frac{n}{2}}]$$
结论：**当 $x$ 同时位于以上区间内时，所需的移动数最少**，总移动数为 $$\sum_{i=0}^{\frac{n}{2} - 1} (\textit{nums}_{n-1-i} - \textit{nums}_i)$$ 
> 证明：对于某一个区间 $[\textit{nums}_i, \textit{nums}_{n - 1 -i}]$ ，元素变为 $x$ ，**该区间对应的数对**所需要的移动数为 $$|\textit{nums}_{n - 1 - i} - x| + |\textit{nums}_i - x| \ge |\textit{nums}_{n - 1 - i} - x - (\textit{nums}_i - x)| = \textit{nums}_{n - 1 - i} - \textit{nums}_i$$ ，当且仅当 $x\in [\textit{nums}_i, \textit{nums}_{n - 1 -i}]$ 时**等号成立**。
> 
> 在上述区间中，**后一个区间是前一个区间的子集**，因此只要 $x \in [\textit{nums}_{\frac{n}{2} - 1}, \textit{nums}_{\frac{n}{2}}]$ 就满足要求。

当 $n$ 为奇数时，我们将排序后的数组中间的元素 $\textit{nums}_{\lfloor \frac{n}{2} \rfloor}$ 当成区间 $[\textit{nums}_{\lfloor \frac{n}{2} \rfloor}, \textit{nums}_{\lfloor \frac{n}{2} \rfloor}]$ 看待，则 $x \in [\textit{nums}_{\lfloor \frac{n}{2} \rfloor}, \textit{nums}_{\lfloor \frac{n}{2} \rfloor}]$ 即 $x= \textit{nums}_{\lfloor \frac{n}{2} \rfloor}$ 时，所需的移动数最少。
 
综上所述，所有元素都变成 $\textit{nums}_{\lfloor \frac{n}{2} \rfloor}$ 时，所需的移动数最少。
```cpp
class Solution {
public:
    int minMoves2(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), ans = 0, x = nums[n / 2];
        for (int i = 0; i < n; ++i) ans += abs(nums[i] - x);
        // int i = 0, j = nums.size() - 1, ans = 0;
        // while (i < j) ans += nums[j--] + nums[i++];
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log n)$，其中 $n$ 是数组 $\textit{nums}$ 的长度。排序需要 $O(n\log n)$ 的时间。
- 空间复杂度：$O(\log n)$ 。排序需要 $O(\log n)$ 的递归栈空间。

---
### 解法2 快速选择
根据方法一的推导，$x$ 取数组 $\textit{nums}$ 第 $\lfloor \frac{n}{2} \rfloor$ 小元素（从 $0$ 开始计数）时，所需要的移动数最少。求解数组第 $k$ 小元素可以使用快速选择算法。
```cpp
class Solution {
public:
    int quickSelect(vector<int>& nums, int left, int right, int index) {
        int q = randomPartition(nums, left, right);
        if (q == index) {
            return nums[q];
        } else {
            return q < index ? quickSelect(nums, q + 1, right, index) : quickSelect(nums, left, q - 1, index);
        }
    }

    inline int randomPartition(vector<int>& nums, int left, int right) {
        int i = rand() % (right - left + 1) + left;
        swap(nums[i], nums[right]);
        return partition(nums, left, right);
    }

    inline int partition(vector<int>& nums, int left, int right) {
        int x = nums[right], i = left - 1;
        for (int j = left; j < right; ++j) {
            if (nums[j] <= x) {
                swap(nums[++i], nums[j]);
            }
        }
        swap(nums[i + 1], nums[right]);
        return i + 1;
    }

    int minMoves2(vector<int>& nums) {
        srand(time(0));
        int n = nums.size(), x = quickSelect(nums, 0, n - 1, n / 2), ret = 0;
        for (int i = 0; i < n; ++i) {
            ret += abs(nums[i] - x);
        }
        return ret;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是数组 $\textit{nums}$ 的长度。快速选择算法的平均时间复杂度为 $O(n)$ 。
- 空间复杂度：$O(\log n)$ 。递归栈的平均占用空间为 $O(\log n)$ 。