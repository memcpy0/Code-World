> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Given a <strong>0-indexed</strong> integer array <code>nums</code> of size <code>n</code>, find the <strong>maximum difference</strong> between <code>nums[i]</code> and <code>nums[j]</code> (i.e., <code>nums[j] - nums[i]</code>), such that <code>0 &lt;= i &lt; j &lt; n</code> and <code>nums[i] &lt; nums[j]</code>.</p>

<p>Return <em>the <strong>maximum difference</strong>. </em>If no such <code>i</code> and <code>j</code> exists, return <code>-1</code>.</p>


<p><strong>Example 1:</strong></p>

<pre><strong>Input:</strong> nums = [7,<strong><u>1</u></strong>,<strong><u>5</u></strong>,4]
<strong>Output:</strong> 4
<strong>Explanation:</strong>
The maximum difference occurs with i = 1 and j = 2, nums[j] - nums[i] = 5 - 1 = 4.
Note that with i = 1 and j = 0, the difference nums[j] - nums[i] = 7 - 1 = 6, but i &gt; j, so it is not valid.
</pre>

<p><strong>Example 2:</strong></p>

<pre><strong>Input:</strong> nums = [9,4,3,2]
<strong>Output:</strong> -1
<strong>Explanation:</strong>
There is no i and j such that i &lt; j and nums[i] &lt; nums[j].
</pre>

<p><strong>Example 3:</strong></p>

<pre><strong>Input:</strong> nums = [<strong><u>1</u></strong>,5,2,<strong><u>10</u></strong>]
<strong>Output:</strong> 9
<strong>Explanation:</strong>
The maximum difference occurs with i = 0 and j = 3, nums[j] - nums[i] = 10 - 1 = 9.
</pre>


<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == nums.length</code></li>
	<li><code>2 &lt;= n &lt;= 1000</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 10<sup>9</sup></code></li>
</ul>


题意：给你一个下标从 <strong>0</strong> 开始的整数数组 <code>nums</code> ，该数组的大小为 <code>n</code> ，请你计算 <code>nums[j] - nums[i]</code> 能求得的 <strong>最大差值 </strong>，其中 <code>0 &lt;= i &lt; j &lt; n</code> 且 <code>nums[i] &lt; nums[j]</code> 。

<p>返回 <strong>最大差值</strong> 。如果不存在满足要求的 <code>i</code> 和 <code>j</code> ，返回 <code>-1</code> 。</p>
 
---
### 解法1 暴力
直接暴力双重循环。对每个 `nums[i]` ，都与其后的每个 `nums[j]` 比较并计算差值。算法的时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int maximumDifference(vector<int>& nums) {
        int ans = -1;
        for (int i = 0, n = nums.size(); i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if (nums[i] < nums[j])
                    ans = max(ans, nums[j] - nums[i]);
        return ans;
    }
};
//执行用时：24 ms, 在所有 C++ 提交中击败了40.26% 的用户
//内存消耗：8.2 MB, 在所有 C++ 提交中击败了7.19% 的用户
```
---
### 解法2 前缀最小值
在遍历数组时不断更新前缀最小值，如果前缀最小值小于 `nums[i]` ，我们就得到了答案的一个候选。算法时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int maximumDifference(vector<int>& nums) {
        int ans = -1, premin = nums[0];
        for (int i = 1, n = nums.size(); i < n; ++i) {
            if (premin < nums[i]) ans = max(ans, nums[i] - premin);
            else premin = nums[i];
        }
        return ans;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了93.17% 的用户
//内存消耗：8.1 MB, 在所有 C++ 提交中击败了36.52% 的用户
```
