<p>Given an integer array <code>nums</code> <strong>(0-indexed)</strong> and two integers <code>target</code> and <code>start</code>, find an index <code>i</code> such that <code>nums[i] == target</code> and <code>abs(i - start)</code> is <strong>minimized</strong>. Note that&nbsp;<code>abs(x)</code>&nbsp;is the absolute value of <code>x</code>.</p>

<p>Return <code>abs(i - start)</code>.</p>

<p>It is <strong>guaranteed</strong> that <code>target</code> exists in <code>nums</code>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [1,2,3,4,5], target = 5, start = 3
Output: 1
Explanation: nums[4] = 5 is the only value equal to target, so the answer is abs(4 - 3) = 1. 
```

 

<p><strong>Example 2:</strong></p>

```clike
Input: nums = [1], target = 1, start = 0
Output: 0
Explanation: nums[0] = 1 is the only value equal to target, so the answer is abs(0 - 0) = 1. 
```

<p><strong>Example 3:</strong></p>

```clike
Input: nums = [1,1,1,1,1,1,1,1,1,1], target = 1, start = 0
Output: 0
Explanation: Every value of nums is 1, but nums[0] minimizes abs(i - start), which is abs(0 - 0) = 0.
```

 
<p><strong>Constraints:</strong></p> 
<ul>
	<li><code>1 &lt;= nums.length &lt;= 1000</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 10<sup>4</sup></code></li>
	<li><code>0 &lt;= start &lt; nums.length</code></li>
	<li><code>target</code> is in <code>nums</code>.</li>
</ul>

题意： 给你一个整数数组 <code>nums</code> （下标 <strong>从 0 开始</strong> 计数）以及两个整数 <code>target</code> 和 <code>start</code> ，找出一个下标 <code>i</code> ，满足 <code>nums[i] == target</code> 且 <code>abs(i - start)</code> <strong>最小化</strong> 。注意：<code>abs(x)</code> 表示 <code>x</code> 的绝对值。 返回 <code>abs(i - start)</code> 。</p>

---
### 解法1 直接遍历
从头开始遍历整个数组：
```cpp
class Solution {
public:
    int getMinDistance(vector<int>& nums, int target, int start) {
        if (nums[start] == target) return 0;
        int n = nums.size(), ans = INT_MAX;
        for (int i = 0; i < n; ++i) 
            if (nums[i] == target && abs(i - start) < ans) ans = abs(i - start);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了50.00% 的用户
内存消耗：9.5 MB, 在所有 C++ 提交中击败了50.00% 的用户
```
---
### 解法2 从 `start` 向两边遍历
```cpp
class Solution {
public:
    int getMinDistance(vector<int>& nums, int target, int start) {
        if (nums[start] == target) return 0;
        int n = nums.size(), i = 1;
        while (true) { //target必然存在于nums中,所以不会死循环
            if (start + i < n && nums[start + i] == target) return i;
            if (start - i >= 0 && nums[start - i] == target) return i;
            ++i;
        }
        return n;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了89.26% 的用户
内存消耗：9.4 MB, 在所有 C++ 提交中击败了81.38% 的用户
```
