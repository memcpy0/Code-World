> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given a <strong>0-indexed</strong> integer array <code>nums</code>. For each index <code>i</code> (<code>1 &lt;= i &lt;= nums.length - 2</code>) the <strong>beauty</strong> of <code>nums[i]</code> equals:</p>

<ul>
	<li><code>2</code>, if <code>nums[j] &lt; nums[i] &lt; nums[k]</code>, for <strong>all</strong> <code>0 &lt;= j &lt; i</code> and for <strong>all</strong> <code>i &lt; k &lt;= nums.length - 1</code>.</li>
	<li><code>1</code>, if <code>nums[i - 1] &lt; nums[i] &lt; nums[i + 1]</code>, and the previous condition is not satisfied.</li>
	<li><code>0</code>, if none of the previous conditions holds.</li>
</ul>

<p>Return<em> the <strong>sum of beauty</strong> of all </em><code>nums[i]</code><em> where </em><code>1 &lt;= i &lt;= nums.length - 2</code>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [1,2,3]
Output: 2
Explanation: For each index i in the range 1 <= i <= 1:
- The beauty of nums[1] equals 2. 
```
<p><strong>Example 2:</strong></p>

```clike
Input: nums = [2,4,6,4]
Output: 1
Explanation: For each index i in the range 1 <= i <= 2:
- The beauty of nums[1] equals 1.
- The beauty of nums[2] equals 0.
```
 
<p><strong>Example 3:</strong></p>
 
```clike
Input: nums = [3,2,1]
Output: 0
Explanation: For each index i in the range 1 <= i <= 1:
- The beauty of nums[1] equals 0.
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>3 &lt;= nums.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= nums[i] &lt;= 10<sup>5</sup></code></li>
</ul>

题意：给你一个下标从 <strong>0</strong> 开始的整数数组 <code>nums</code> 。对于每个下标 <code>i</code>（<code>1 &lt;= i &lt;= nums.length - 2</code>），<code>nums[i]</code> 的 <strong>美丽值</strong> 等于：</p>

<ul>
	<li><code>2</code>，对于所有 <code>0 &lt;= j &lt; i</code> 且 <code>i &lt; k &lt;= nums.length - 1</code> ，满足 <code>nums[j] &lt; nums[i] &lt; nums[k]</code></li>
	<li><code>1</code>，如果满足 <code>nums[i - 1] &lt; nums[i] &lt; nums[i + 1]</code> ，且不满足前面的条件</li>
	<li><code>0</code>，如果上述条件全部不满足</li>
</ul>

返回符合 <code>1 &lt;= i &lt;= nums.length - 2</code> 的所有<em> </em><code>nums[i]</code><em> </em>的 <strong>美丽值的总和</strong> 。 

---
### 解法 递推
本题中，需要判断当前元素 `nums[i]` 是否大于所有前面的元素、且小于所有后面的元素。考虑到数据范围，使用暴力解法（即双重循环）绝对会超时。

因此，可以使用 `suffix/prefix` 数组，其中 `prefix[i]` 记录 `[0, i - 1]` 区间的最大值，`suffix[i]` 记录 `[i + 1, n - 1]` 区间的最小值。先通过递推求出这两个数组。然后只需一重循环，即可解决本题。

具体代码如下所示。整个算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int sumOfBeauties(vector<int>& nums) {
        int n = nums.size(), ans = 0;
        vector<int> prefix(n), suffix(n); //prefix[i]表示[0,i)之间的最大值,suffix[i]表示(i,n-1]之间的最小值
        prefix[0] = 0, suffix[n - 1] = 100001;
        for (int i = 1; i < n; ++i) prefix[i] = max(nums[i - 1], prefix[i - 1]);
        for (int i = n - 2; i >= 0; --i) suffix[i] = min(nums[i + 1], suffix[i + 1]);
        for (int i = 1; i < n - 1; ++i) {
            if (nums[i] > prefix[i] && nums[i] < suffix[i]) ans += 2;
            else if (nums[i] > nums[i - 1] && nums[i] < nums[i + 1]) ++ans;
        }
        return ans;
    }   
};
//执行用时：140 ms, 在所有 C++ 提交中击败了34.81% 的用户
//内存消耗：86.5 MB, 在所有 C++ 提交中击败了22.21% 的用户
```
求 `prefix/suffix` 两个数组的循环可以合并在一起：
```cpp
//C++ version
class Solution {
public:
    int sumOfBeauties(vector<int>& nums) {
        int n = nums.size(), ans = 0;
        vector<int> prefix(n), suffix(n); //prefix[i]表示[0,i)之间的最大值,suffix[i]表示(i,n-1]之间的最小值
        prefix[0] = 0, suffix[n - 1] = 100001;
        for (int i = 1; i < n; ++i) { //合并起来
            prefix[i] = max(nums[i - 1], prefix[i - 1]);
            suffix[n - i - 1] = min(nums[n - i], suffix[n - i]);
        }
        for (int i = 1; i < n - 1; ++i) {
            if (nums[i] > prefix[i] && nums[i] < suffix[i]) ans += 2;
            else if (nums[i] > nums[i - 1] && nums[i] < nums[i + 1]) ++ans;
        }
        return ans;
    }   
};
//执行用时：128 ms, 在所有 C++ 提交中击败了65.55% 的用户
//内存消耗：86.3 MB, 在所有 C++ 提交中击败了43.46% 的用户
```
