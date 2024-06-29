<p>Given two arrays <code>nums1</code>&nbsp;and <code><font face="monospace">nums2</font></code><font face="monospace">.</font></p>

<p>Return the maximum dot product&nbsp;between&nbsp;<strong>non-empty</strong> subsequences of nums1 and nums2 with the same length.</p>

<p>A subsequence of a array is a new array which is formed from the original array by deleting some (can be none) of the characters without disturbing the relative positions of the remaining characters. (ie,&nbsp;<code>[2,3,5]</code>&nbsp;is a subsequence of&nbsp;<code>[1,2,3,4,5]</code>&nbsp;while <code>[1,5,3]</code>&nbsp;is not).</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: nums1 = [2,1,-2,5], nums2 = [3,0,-6]
Output: 18
Explanation: Take subsequence [2,-2] from nums1 and subsequence [3,-6] from nums2.
Their dot product is (2*3 + (-2)*(-6)) = 18. 
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: nums1 = [3,-2], nums2 = [2,-6,7]
Output: 21
Explanation: Take subsequence [3] from nums1 and subsequence [7] from nums2.
Their dot product is (3*7) = 21. 
```
 
<p><strong>Example 3:</strong></p>

```cpp
Input: nums1 = [-1,-1], nums2 = [1,1]
Output: -1
Explanation: Take subsequence [-1] from nums1 and subsequence [1] from nums2.
Their dot product is -1. 
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums1.length, nums2.length &lt;= 500</code></li>
	<li><code>-1000 &lt;= nums1[i], nums2[i] &lt;= 1000</code></li>
</ul>

题意：给你两个数组&nbsp;<code>nums1</code>&nbsp;和&nbsp;<code>nums2</code>&nbsp;。请你返回 <code>nums1</code> 和 <code>nums2</code> 中两个长度相同的 <strong>非空</strong> 子序列的最大点积。</p>

<p>数组的非空子序列是通过删除原数组中某些元素（可能一个也不删除）后剩余数字组成的序列，但不能改变数字间相对顺序。比方说，<code>[2,3,5]</code>&nbsp;是&nbsp;<code>[1,2,3,4,5]</code>&nbsp;的一个子序列而&nbsp;<code>[1,5,3]</code>&nbsp;不是。</p>
 
---
### 解法 动态规划
和LCS差不多的思路，这里分三种情况动态规划：不包含 `nums1[i]` 、不包含 `nums2[j]` 、包含 `nums1[i], nums2[j]`（如果之前的最大点积为负数，则需要重新积累）。
```cpp
class Solution {
public:
    int maxDotProduct(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        //dp[i][j]表示nums1[0..i]与nums2[0..j]最大的点积
        vector<vector<int>> dp(m, vector<int>(n, INT_MIN));
        dp[0][0] = nums1[0] * nums2[0];
        //初始状态
        for (int j = 1; j < n; ++j) dp[0][j] = max(dp[0][j - 1], nums1[0] * nums2[j]);
        for (int i = 1; i < m; ++i) dp[i][0] = max(dp[i - 1][0], nums2[0] * nums1[i]);
        //动态规划
        for (int i = 1; i < m; ++i)
            for (int j = 1; j < n; ++j) 
                dp[i][j] = max(max(dp[i - 1][j], dp[i][j - 1]),
                (dp[i - 1][j - 1] > 0 ? dp[i - 1][j - 1] : 0) + nums1[i] * nums2[j]); 
        return dp[m - 1][n - 1];
    }
};
```
运行效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了75.80% 的用户
内存消耗：12.7 MB, 在所有 C++ 提交中击败了63.70% 的用户
```
