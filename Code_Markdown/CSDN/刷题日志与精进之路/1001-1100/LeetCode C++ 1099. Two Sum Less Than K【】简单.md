<p>Given an array <code>nums</code> of integers and&nbsp;integer <code>k</code>, return the maximum <code>sum</code> such that there exists <code>i &lt; j</code> with <code>nums[i] + nums[j] = sum</code> and <code>sum &lt; k</code>. If no <code>i</code>, <code>j</code> exist satisfying this equation, return <code>-1</code>.</p>
 
<p><strong>Example 1:</strong></p>

```haskell
Input: nums = [34,23,1,24,75,33,54,8], k = 60
Output: 58
Explanation: We can use 34 and 24 to sum 58 which is less than 60.
```

 
<p><strong>Example 2:</strong></p>

```haskell
Input: nums = [10,20,30], k = 15
Output: -1
Explanation: In this case it is not possible to get a pair sum less that 15.
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 100</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 1000</code></li>
	<li><code>1 &lt;= k &lt;= 2000</code></li>
</ul>



题意：给定一个整数数组 `nums` 和整数 `k` ，返回最大和 `sum` ，满足存在 `i < j` 使得 `nums[i] + nums[j] = sum` 且 `sum < k` 。如果没有满足此等式的 `i,j` 存在，则返回 `-1` 。
 
