<p>You are given an integer array <code>nums</code> consisting of <code>2 * n</code> integers.</p>

<p>You need to divide <code>nums</code> into <code>n</code> pairs such that:</p>

<ul>
	<li>Each element belongs to <strong>exactly one</strong> pair.</li>
	<li>The elements present in a pair are <strong>equal</strong>.</li>
</ul>

<p>Return <code>true</code> <em>if nums can be divided into</em> <code>n</code> <em>pairs, otherwise return</em> <code>false</code>.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: nums = [3,2,3,2,2,2]
Output: true
Explanation: 
There are 6 elements in nums, so they should be divided into 6 / 2 = 3 pairs.
If nums is divided into the pairs (2, 2), (3, 3), and (2, 2), it will satisfy all the conditions.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: nums = [1,2,3,4]
Output: false
Explanation: 
There is no way to divide nums into 4 / 2 = 2 pairs such that the pairs satisfy every condition.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>nums.length == 2 * n</code></li>
	<li><code>1 &lt;= n &lt;= 500</code></li>
	<li><code>1 &lt;= nums[i] &lt;= 500</code></li>
</ul>

题意：给出一个长为 $2 \times n$ 的整数数组 `nums` ，需要将 `nums` 划分成 $n$ 个数对，满足：每个元素 **只属于一个** 数对；同一数对中的元素 **相等** 。如果可以将 `nums` 划分成 $n$ 个数对，返回 `true` ，否则返回 `false` 。

---
### 解法 哈希计数
这就是道计数题，我们对 $[1, 500]$ 范围内的任一数字 $x$ ，计算 `nums[]` 中值等于 $x$ 的元素数目，当且仅当其数目为偶数时，具有相等值的元素 $x$ 可以完全分成多个数对。具体实现上，可以使用整型数组进行哈希计数，然后判断每种元素是否为偶数个：
```java
// Java Version
class Solution {
    public boolean divideArray(int[] nums) {
        int[] cnt = new int[510];
        for (int v : nums) ++cnt[v];
        for (int i = 1; i <= 500; ++i)
            if ((cnt[i] & 1) == 1) return false;
        return true;
    }
}
// 执行用时：1 ms, 在所有 Java 提交中击败了99.93% 的用户
// 内存消耗：41.1 MB, 在所有 Java 提交中击败了95.66% 的用户
```
