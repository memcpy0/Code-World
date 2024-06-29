> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。



<p>An integer array is called arithmetic if it consists of <strong>at least three elements</strong> and if the difference between any two consecutive elements is the same.</p>

<ul>
	<li>For example, <code>[1,3,5,7,9]</code>, <code>[7,7,7,7]</code>, and <code>[3,-1,-5,-9]</code> are arithmetic sequences.</li>
</ul>

<p>Given an integer array <code>nums</code>, return <em>the number of arithmetic <strong>subarrays</strong> of</em> <code>nums</code>.</p>

<p>A <strong>subarray</strong> is a contiguous subsequence of the array.</p>
 
<p><strong>Example 1:</strong></p>

```java
Input: nums = [1,2,3,4]
Output: 3
Explanation: We have 3 arithmetic slices in nums: [1, 2, 3], [2, 3, 4] and [1,2,3,4] itself.
```
<p><strong>Example 2:</strong></p>

```java 
Input: nums = [1]
Output: 0
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 5000</code></li>
	<li><code>-1000 &lt;= nums[i] &lt;= 1000</code></li>
</ul>


题意：如果一个数列 <strong>至少有三个元素</strong> ，并且任意两个相邻元素之差相同，则称该数列为等差数列。给你一个整数数组 <code>nums</code> ，返回数组 <code>nums</code> 中所有为等差数组的 <strong>子数组</strong> 个数。</p> 

---
### 解法1 双指针
这一做法是从暴力做法中优化得到的。很容易想到，我们可以双重循环所有的子数组区间 `[i, j]` ，判断 `nums[i], ..., nums[j]` 是否是等差数列，是则计数，算法时间复杂度为 $O(n^3)$ 。不过这样做太低效了，我们可以进一步优化。

由于**等差数组的子数组也是等差数组**，因此**在确定了起始位置 `i` 后，只要公差一致，指针 `j` 就可以向后面尽可能的扩展**，从而得到以 `i` 为起始位置、公差 `d = nums[i + 1] - nums[i]` 的最长等差子数组区间 `[i, j]` ，令区间长度为 `len = j - i + 1` ，其中符合条件的等差子数组的数量为：
$$cnt = \sum^{len}_{k=3} countArrayWithLength(k)$$

其中函数 `int countArrayWithLength(int k)` 求的是长度为 `k` 的等差子数组的数量，`k` 的变化区间为 `[3, len]` 。显然，随着 `k` 的增大，等差子数组的数量 `cnt` 逐渐减少，且实质上是一个 **「首项为 `1` 、末项为 `len - 3 + 1` 、公差为 `1` 的等差数列的求和结果」**。因此等差子数组的总数加上 `cnt` ，然后 `i` 指针跳转到 `j` 处，重复上述过程。


最后的代码如下。时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp 
//C++ version
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        if (nums.size() <= 2) return 0;
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n - 2; ) {
            int j = i, d = nums[i + 1] - nums[i];
            while (j + 1 < n && nums[j + 1] - nums[j] == d) ++j;
            int len = j - i + 1, a0 = 1, an = len - 3 + 1;
            //符合条件(长度>=3且<=len)的等差子数组的数量
            int cnt = (a0 + an) * an / 2;
            ans += cnt;
            i = j;
        } 
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：7.2 MB, 在所有 C++ 提交中击败了48.81% 的用户
```
 
---
### 解法2 动态规划
设数组 `dp[]` ，其中的  `dp[i]` 代表以 `nums[i]` 为结尾的等差子数组的数目，本题答案为所有 `dp[i]` 之和。容易得到递推公式：
$$\begin{cases}
dp[i] = dp[i - 1] +1 \quad &if\ nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2] \\
dp[i] = 0				\quad &else.
\end{cases}
$$

原理很简单，在等差子数组 `arr` 之后添加一个元素 `nums[i]` ：
- 可行时，等差子数组的总个数将新增 `arr` 中等差子数组的个数再加一，即 `nums[i]` 之前的等差子数组（长度大于等于 `3` ）均向后延伸到 `nums[i]` ，`nums[i]` 还和 `nums[i - 2], nums[i - 1]` 组成新的等差子数组；
- 不可行时，以 `nums[i]` 结尾的等差子数组的数目为零。

代码如下。时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        if (nums.size() <= 2) return 0;
        int n = nums.size(), ans = 0;
        vector<int> dp(n); //dp[i]为以nums[i]结尾的等差数组的数目
        for (int i = 2; i < n; ++i) {
            if (nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2]) {
                dp[i] = dp[i - 1] + 1;
                ans += dp[i];
            } 
        } 
        return ans;
   }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了44.58% 的用户
//内存消耗：7.2 MB, 在所有 C++ 提交中击败了41.60% 的用户
```
由于 `dp[i]` 只和前一个状态 `dp[i - 1]` 有关，而且不用记录下 `dp[i - 1]` ，我们可以优化掉 `dp[]` 而代之以一个状态变量 `dp` 。时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        if (nums.size() <= 2) return 0;
        int n = nums.size(), ans = 0, dp = 0;
        for (int i = 2; i < n; ++i) {
            if (nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2]) {
                ++dp; //dp[i]=dp[i-1]+1;
                ans += dp;
            } else dp = 0;
        } 
        return ans;
   }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：7.1 MB, 在所有 C++ 提交中击败了59.52% 的用户
```
