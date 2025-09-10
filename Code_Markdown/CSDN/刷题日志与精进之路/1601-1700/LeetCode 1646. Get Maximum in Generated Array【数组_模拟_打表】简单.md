> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given an integer <code>n</code>. An array <code>nums</code> of length <code>n + 1</code> is generated in the following way:</p>

<ul>
	<li><code>nums[0] = 0</code></li>
	<li><code>nums[1] = 1</code></li>
	<li><code>nums[2 * i] = nums[i]</code> when <code>2 &lt;= 2 * i &lt;= n</code></li>
	<li><code>nums[2 * i + 1] = nums[i] + nums[i + 1]</code> when <code>2 &lt;= 2 * i + 1 &lt;= n</code></li>
</ul>

<p>Return<strong> </strong><em>the <strong>maximum</strong> integer in the array </em><code>nums</code>​​​.</p>
 
<p><strong>Example 1:</strong></p>

```python
Input: n = 7
Output: 3
Explanation: According to the given rules:
  nums[0] = 0
  nums[1] = 1
  nums[(1 * 2) = 2] = nums[1] = 1
  nums[(1 * 2) + 1 = 3] = nums[1] + nums[2] = 1 + 1 = 2
  nums[(2 * 2) = 4] = nums[2] = 1
  nums[(2 * 2) + 1 = 5] = nums[2] + nums[3] = 1 + 2 = 3
  nums[(3 * 2) = 6] = nums[3] = 2
  nums[(3 * 2) + 1 = 7] = nums[3] + nums[4] = 2 + 1 = 3
Hence, nums = [0,1,1,2,1,3,2,3], and the maximum is 3. 
```

<p><strong>Example 2:</strong></p>

```python
Input: n = 2
Output: 1
Explanation: According to the given rules, the maximum between nums[0], nums[1], and nums[2] is 1. 
```

<p><strong>Example 3:</strong></p>

```python
Input: n = 3
Output: 2
Explanation: According to the given rules, the maximum between nums[0], nums[1], nums[2], and nums[3] is 2.
```
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= n &lt;= 100</code></li>
</ul>

题意：给你一个整数 <code>n</code> 。按下述规则生成一个长度为 <code>n + 1</code> 的数组 <code>nums</code> ： 
- <code>nums[0] = 0</code> 
- <code>nums[1] = 1</code> 
- 当 <code>2 &lt;= 2 * i &lt;= n</code> 时，<code>nums[2 * i] = nums[i]</code> 
- 当 <code>2 &lt;= 2 * i + 1 &lt;= n</code> 时，<code>nums[2 * i + 1] = nums[i] + nums[i + 1]</code> 

返回生成数组 <code>nums</code> 中的 <strong>最大</strong> 值。

---
### 解法1 模拟
按照题意进行模拟，生成 `nums` 数组，顺便求出最大值。时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ ：
```cpp
class Solution {
public:
    int getMaximumGenerated(int n) {
        if (n <= 1) return n;
        int arr[101] = {0, 1}, ans = 1;
        for (int i = 2; i <= n; ++i) {
            arr[i] = (i & 1) ? arr[i >> 1] + arr[(i >> 1) + 1] : arr[i >> 1];
            ans = max(arr[i], ans);
        }
        return ans;  
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了83.41% 的用户
```

---
### 解法2 打表
利用数据范围，直接打表。时间复杂度为 $O(1)$ ，空间复杂度为 $O(n)$ ：
```cpp
class Solution {
private:
    int ans[101] = {0,1,1,2,2,3,3,3,3,4,4,5,5,5,5,5,5,5,5,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,11,11,11,11,11,11,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,15,15,18,18,18,18,18,18,18,18,19,19,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21};
public:
    int getMaximumGenerated(int n) { 
        return ans[n];  
    }
}; 
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了92.05% 的用户
```

