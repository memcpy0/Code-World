> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given an integer array `nums` of length `n`, and an integer array `queries` of length `m`.

Return _an array_ `answer` _of length_ `m` _where_ `answer[i]` _is the **maximum** size of a **subsequence** that you can take from_ `nums` _such that the **sum** of its elements is less than or equal to_ `queries[i]`.

A **subsequence** is an array that can be derived from another array by deleting some or no elements without changing the order of the remaining elements.

**Example 1:**
```java
Input: nums = [4,5,2,1], queries = [3,10,21]
Output: [2,3,4]
Explanation: We answer the queries as follows:
- The subsequence [2,1] has a sum less than or equal to 3. It can be proven that 2 is the maximum size of such a subsequence, so answer[0] = 2.
- The subsequence [4,5,1] has a sum less than or equal to 10. It can be proven that 3 is the maximum size of such a subsequence, so answer[1] = 3.
- The subsequence [4,5,2,1] has a sum less than or equal to 21. It can be proven that 4 is the maximum size of such a subsequence, so answer[2] = 4.
```
**Example 2:**
```java
Input: nums = [2,3,4,5], queries = [1]
Output: [0]
Explanation: The empty subsequence is the only subsequence that has a sum less than or equal to 1, so answer[0] = 0.
```
**Constraints:**
-   `n == nums.length`
-   `m == queries.length`
-   `1 <= n, m <= 1000`
-   `1 <= nums[i], queries[i] <= 10^6`

题意：给出一个长度 `n` 的整数数组 `nums` ，和一个长度 `m` 的整数数组 `queries` 。返回一个长度为 `m` 的数组 `answer` ，其中 `answer[i]` 是 `nums` 中 元素之和小于等于 `queries[i]` 的 **子序列** 的 **最大** 长度。**子序列** 是由一个数组删除某些元素（也可以不删除）但不改变剩余元素顺序得到的一个数组。

---
### 解法 贪心+排序+前缀和+二分上界
由题意可知，`nums` 的元素次序对「求子序列的元素和」无影响，因此对 `nums` 从小到大进行**排序**。显然，**要令元素和小于 `queries[i]` 的子序列最长，就要贪心地从最小的几个数开始求和**。使用数组 `ps` 保存 `nums` 的**前缀和**，其中 `ps[i+1]` 表示从 `nums[0]` 到 `nums[i]` 的元素和。遍历 `queries` ，假设当前查询值为 `q` ，使用二分查找获取满足 $ps[i] \gt q$  的最小的 `i` ，那么和小于等于 `q`  的最长子序列长度为 `i−1` 。
```cpp
class Solution {
public:
    vector<int> answerQueries(vector<int>& nums, vector<int>& queries) {
        sort(nums.begin(), nums.end()); // O(nlogn)
        int n = nums.size(), m = queries.size();
        vector<int> ps(n + 1), ans(m);
        for (int i = 0; i < n; ++i) ps[i + 1] = ps[i] + nums[i]; // O(n)
        for (int i = 0; i < m; ++i) {
            int q = queries[i];
            // 找到ps中第一个大于q的值v的位置i
            // v是ps的前缀和,v>q说明到ps[i]的元素和>q,于是到ps[i-1]的元素和<=q
            ans[i] = upper_bound(ps.begin(), ps.begin() + n + 1, q) - ps.begin() - 1;
        }
        return ans;
    }
};
```
- 时间复杂度：$O \big ( (n + m) \times \log n \big )$ ，其中 $n$ 是数组 `nums` 的长度，$m$ 是数组 `queries` 的长度。对 `nums` 进行排序需要 $O(n \log n)$ 的时间，二分查找需要 $O(m\log n)$ 的时间。
- 空间复杂度：$O(n)$ 。返回值不计入空间复杂度。
 