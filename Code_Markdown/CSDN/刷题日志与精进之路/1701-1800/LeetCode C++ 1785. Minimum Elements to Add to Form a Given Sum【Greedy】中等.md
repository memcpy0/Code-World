<p>You are given an integer array <code>nums</code> and two integers <code>limit</code> and <code>goal</code>. The array <code>nums</code> has an interesting property that <code>abs(nums[i]) &lt;= limit</code>.</p>

<p>Return <em>the minimum number of elements you need to add to make the sum of the array equal to </em><code>goal</code>. The array must maintain its property that <code>abs(nums[i]) &lt;= limit</code>.</p>

<p>Note that <code>abs(x)</code> equals <code>x</code> if <code>x &gt;= 0</code>, and <code>-x</code> otherwise.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [1,-1,1], limit = 3, goal = -4
Output: 2
Explanation: You can add -2 and -3, then the sum of the array will be 1 - 1 + 1 - 2 - 3 = -4.
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: nums = [1,-10,9,1], limit = 100, goal = 0
Output: 1
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= limit &lt;= 10<sup>6</sup></code></li>
	<li><code>-limit &lt;= nums[i] &lt;= limit</code></li>
	<li><code>-10<sup>9</sup> &lt;= goal &lt;= 10<sup>9</sup></code></li>
</ul>

题意：给你一个整数数组 <code>nums</code> ，和两个整数 <code>limit</code> 与 <code>goal</code> 。数组 <code>nums</code> 有一条重要属性：<code>abs(nums[i]) &lt;= limit</code> 。

<p>返回使数组元素总和等于 <code>goal</code> 所需要向数组中添加的 <strong>最少元素数量</strong> ，添加元素 <strong>不应改变</strong> 数组中 <code>abs(nums[i]) &lt;= limit</code> 这一属性。注意，如果 <code>x &gt;= 0</code> ，那么 <code>abs(x)</code> 等于 <code>x</code> ；否则，等于 <code>-x</code> 。</p>

---
### 解法 贪心
首先求出数组元素和，得到和目标值 `goal` 的差 `diff` 。由于只能添加 `<= abs(limit)` 的值到数组中，我们必须仔细考虑……才怪，一看就知道，如果 `limit` 整除 `diff` ，我们只需要添加 `abs(diff) / abs(limit) = m` 个值，如果有余数就再添加一个值：
```cpp
class Solution {
public:
    int minElements(vector<int>& nums, int limit, int goal) {
        long long sum = 0;
        for (const int &v : nums) sum += v;
        long long diff = abs(goal - sum);
        long long m = diff / abs(limit), n = diff % abs(limit);
        return !n ? m : m + 1;
    }
};
```
运行效率如下：
```cpp
执行用时：112 ms, 在所有 C++ 提交中击败了97.55% 的用户
内存消耗：71.7 MB, 在所有 C++ 提交中击败了28.80% 的用户
```
