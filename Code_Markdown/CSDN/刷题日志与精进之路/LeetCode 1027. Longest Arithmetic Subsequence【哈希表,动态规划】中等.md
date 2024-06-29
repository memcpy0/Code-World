> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array `nums` of integers, return _the length of the longest arithmetic subsequence in_ `nums`.

**Note** that:
-   A **subsequence** is an array that can be derived from another array by deleting some or no elements without changing the order of the remaining elements.
-   A sequence `seq` is arithmetic if `seq[i + 1] - seq[i]` are all the same value (for `0 <= i < seq.length - 1`).

**Example 1:**
```java
Input: nums = [3,6,9,12]
Output: 4
Explanation:  The whole array is an arithmetic sequence with steps of length = 3.
```
**Example 2:**
```java
Input: nums = [9,4,7,2,10]
Output: 3
Explanation:  The longest arithmetic subsequence is [4,7,10].
```
**Example 3:**
```java
Input: nums = [20,1,15,3,10,5,8]
Output: 4
Explanation:  The longest arithmetic subsequence is [20,15,10,5].
```
**Constraints:**
-   `2 <= nums.length <= 1000`
-   `0 <= nums[i] <= 500`

题意：给你一个整数数组 `nums`，返回 `nums` 中最长等差子序列的**长度**。

---
### 解法1 没有优化的动态规划
由于至少两个元素才能定义等差数列的公差，所以可以定义状态 `dp[i][j]` 为「以 `A[i], A[j]` 为最后两个元素的等差数列」的最长长度。最后两个数确定，前一个元素就确定为 `target = 2 * A[i] - A[j]` 。我们只需要找到 **`i` 前面最靠近 `i` 的 `target` 的位置**即可：
$$dp[i][j] = dp[\mathrm{idx}_{target}][i] + 1$$
这种做法需要三重循环。先循环确定 $i$ ，再确定 $j$ ，最后找 $target$ 。效率比较低。

---
### 解法2 动态规划+哈希表
通过观察，我们发现，内部的第三重循环可以使用哈希表来优化。通过一个哈希表来记录「**每个在 `i` 之前的数出现的最后下标**」，就可以在 $O(1)$ 时间找到 `target` 的下标。

特别要注意的是，由于哈希表必须记录的是「在前一个数之前的数」的下标，所以必须先访问前一个数，再记录其「值：位置」的键值对。
```cpp
class Solution {
public:
    int longestArithSeqLength(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> dp(n, vector<int>(n, 2)); // dp[i][j]表示以nums[i],nums[j]结尾的最长等差数列的长度,初始化为2
        unordered_map<int, int> rec;
        int ans = 2;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int target = 2 * nums[i] - nums[j];
                if (rec.find(target) != rec.end()) dp[i][j] = dp[rec[target]][i] + 1;
                ans = max(dp[i][j], ans); 
            }
            rec[nums[i]] = i;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$
- 空间复杂度：$O(n^2)$

这种做法可在数值范围广时使用。

---
### 解法3 值域形式的动态规划
由于数据范围不大，所以可以将哈希表和状态数组合二为一。我们设 **$dp[i][d]$ 表示以元素 $nums[i]$ 结尾、公差为 $d$ 的等差数列的最大长度**。某种程度上说，也表示以元素 $nums[i] - d, nums[i]$ 结尾的等差数列的最大长度。不过此处的定义中，**等差数列可以只有一个元素**，因此将 $dp$ 数组的所有元素都初始化为 $1$ 。

假设现在有一个子序列元素 $nums[i]$ ，考虑它能否加入到 $nums[j]$ 结尾的等差数列**后**，先计算 $d = nums[i] - nums[j]$ ，然后令 $dp[i][d] = \max(dp[i][d],\ dp[j][d] + 1)$ 即可。在此期间维护一个最大值作为结果。
> 关于 $d$ 的范围：由于 $0 \le nums[i] \le 500$ ，综合递增和递减的两个极端，得到 $d$ 的范围 $-500\le d \le 500$ ，我们需要 $d$ 作为下标，而现在 $d$ 又可能是负值，对此统一增加一个偏移量 $500$ ， 把负的抵消掉，$0\le d'\le1000$ 。

```cpp
class Solution {
public:
    int longestArithSeqLength(vector<int>& nums) {
        int n = nums.size();
        int dp[n + 1][1020];
        int ans = 0;
        for (int i = 0; i < n; ++i) { // dp[i][j]表示以nums[i]结尾,公差为j的最长等差子序列的长度
            for (int j = 0; j < 1020; ++j) dp[i][j] = 1; // 都为1
            for (int j = 0; j < i; ++j) {
                int diff = nums[i] - nums[j] + 500;
                dp[i][diff] = max(dp[i][diff], dp[j][diff] + 1);
                ans = max(ans, dp[i][diff]);
            }
        }
        return ans;
    }
};
```
 