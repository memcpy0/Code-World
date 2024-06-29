<p>You are given an integer array <code>nums</code> and an integer <code>x</code>. In one operation, you can either remove the leftmost or the rightmost element from the array <code>nums</code> and subtract its value from <code>x</code>. Note that this <strong>modifies</strong> the array for future operations.</p>

<p>Return <em>the <strong>minimum number</strong> of operations to reduce </em><code>x</code> <em>to <strong>exactly</strong></em> <code>0</code> <em>if it's possible</em><em>, otherwise, return </em><code>-1</code>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: nums = [1,1,4,2,3], x = 5
Output: 2
Explanation: The optimal solution is to remove the last two elements to reduce x to zero.
```

 

<p><strong>Example 2:</strong></p>

```clike
Input: nums = [5,6,7,8,9], x = 4
Output: -1 
```

<p><strong>Example 3:</strong></p>

```clike
Input: nums = [3,2,20,1,1,3], x = 10
Output: 5
Explanation: The optimal solution is to remove the last three elements and the first two elements (5 operations in total) to reduce x to zero.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= nums[i] &lt;= 10<sup>4</sup></code></li>
	<li><code>1 &lt;= x &lt;= 10<sup>9</sup></code></li>
</ul>

题意：给你一个整数数组 `nums` 和一个整数 `x` 。每一次操作时，你应当移除数组 `nums` 最左边或最右边的元素，然后从 `x` 中减去该元素的值。请注意，需要 **修改** 数组以供接下来的操作使用。

如果可以将 `x` **恰好** 减到 `0` ，返回 **最小操作数** ；否则，返回 `-1` 。 

---
### 解法 贪心+滑动窗口
令数组元素和为 `sum` ，`x` 要减去数组左右两边的数，减到 `0` 为止。即数组中间部分需要存在和为 `sum - x` 的子数组。于是，可以使用滑动窗口，寻找最长的和为 `sum - x` 的子数组。
```cpp
class Solution {
public:
    int minOperations(vector<int>& nums, int x) {
        int n = nums.size(), lo = 0, maxLen = 0, cursum = 0, target = accumulate(nums.begin(), nums.end(), 0) - x;
        if (target < 0) return -1;
        if (target == 0) return n;
        for (int i = 0; i < n; ++i) {
            cursum += nums[i]; //nums[i]大于0,这种写法才没有问题
            while (cursum > target) cursum -= nums[lo++];
            if (cursum == target) maxLen = max(maxLen, i - lo + 1);
        }
        return maxLen == 0 ? -1 : n - maxLen;
    }
};
```
运行效率如下：
```cpp
执行用时：160 ms, 在所有 C++ 提交中击败了92.25% 的用户
内存消耗：96.2 MB, 在所有 C++ 提交中击败了35.86% 的用户
```
