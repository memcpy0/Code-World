> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Given an integer array <code>nums</code> and an integer <code>k</code>, return <em>the number of pairs</em> <code>(i, j)</code> <em>where</em> <code>i &lt; j</code> <em>such that</em> <code>|nums[i] - nums[j]| == k</code>.</p>

<p>The value of <code>|x|</code> is defined as:</p>

<ul>
	<li><code>x</code> if <code>x &gt;= 0</code>.</li>
	<li><code>-x</code> if <code>x &lt; 0</code>.</li>
</ul>

 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [1,2,2,1], k = 1
Output: 4
Explanation: The pairs with an absolute difference of 1 are:
- [1,2,2,1]
- [1,2,2,1]
- [1,2,2,1]
- [1,2,2,1]
```
<p><strong>Example 2:</strong></p>

```clike
Input: nums = [1,3], k = 3
Output: 0
Explanation: There are no pairs with an absolute difference of 3. 
```
<p><strong>Example 3:</strong></p>

```clike
Input: nums = [3,2,1,5,4], k = 2
Output: 3
Explanation: The pairs with an absolute difference of 2 are:
- [3,2,1,5,4]
- [3,2,1,5,4]
- [3,2,1,5,4]
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 200</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 100</code></li>
	<li><code>1 &lt;= k &lt;= 99</code></li>
</ul>



题意：给你一个整数数组&nbsp;<code>nums</code>&nbsp;和一个整数&nbsp;<code>k</code>&nbsp;，请你返回数对&nbsp;<code>(i, j)</code>&nbsp;的数目，满足&nbsp;<code>i &lt; j</code>&nbsp;且&nbsp;<code>|nums[i] - nums[j]| == k</code>&nbsp;。 
<p><code>|x|</code>&nbsp;的值定义为：</p>
<ul>
	<li>如果&nbsp;<code>x &gt;= 0</code>&nbsp;，那么值为&nbsp;<code>x</code>&nbsp;。</li>
	<li>如果&nbsp;<code>x &lt; 0</code>&nbsp;，那么值为&nbsp;<code>-x</code>&nbsp;。</li>
</ul> 


---
### 解法1 暴力双重循环
很简单的题目，数据范围很小，用双重循环也能过。时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int countKDifference(vector<int>& nums, int k) {
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i)
            for (int j = i + 1; j < n; ++j) 
                if (abs(nums[i] - nums[j]) == k)
                    ++ans;
        return ans;
    }
};
//执行用时：12 ms, 在所有 C++ 提交中击败了87.21% 的用户
//内存消耗：12.1 MB, 在所有 C++ 提交中击败了43.34% 的用户
```
---
### 解法2 哈希表
和[1. 两数之和](https://leetcode-cn.com/problems/two-sum/)差不多的题目、差不多的做法。算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(100)$ ：
```cpp
//C++ version
class Solution {
public:
    int countKDifference(vector<int>& nums, int k) {
        int cnt[102] = {0}, ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i) {
            if (nums[i] - k >= 0) ans += cnt[nums[i] - k];
            if (nums[i] + k <= 100) ans += cnt[nums[i] + k];
            ++cnt[nums[i]]; 
        }
        return ans;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了96.71% 的用户
//内存消耗：11.9 MB, 在所有 C++ 提交中击败了98.40% 的用户
```
