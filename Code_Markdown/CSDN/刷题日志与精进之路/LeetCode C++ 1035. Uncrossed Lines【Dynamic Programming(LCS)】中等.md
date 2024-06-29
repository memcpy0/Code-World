<p>We write the integers of <code>nums1</code> and <code>nums2</code>&nbsp;(in the order they are given) on two separate horizontal lines.</p>

<p>Now, we may draw <em>connecting lines</em>: a straight line connecting two numbers <code>nums1[i]</code> and <code>nums2[j]</code>&nbsp;such that:</p>

<ul>
	<li><code>nums1[i] == nums2[j]</code>;</li>
	<li>The line we draw does not intersect any other connecting (non-horizontal) line.</li>
</ul>

<p>Note that a connecting lines cannot intersect even at the endpoints:&nbsp;each number can only belong to one connecting line.</p>

<p>Return the maximum number of connecting lines we can draw in this way.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 100px; height: 72px;" src="https://assets.leetcode.com/uploads/2019/04/26/142.png" alt="">
 

```clike
Input: nums1 = [1,4,2], nums2 = [1,2,4]
Output: 2
Explanation: We can draw 2 uncrossed lines as in the diagram.
We cannot draw 3 uncrossed lines, because the line from nums1[1]=4 to nums2[2]=4 will intersect the line from nums1[2]=2 to nums2[1]=2.
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: nums1 = [2,5,1,2,5], nums2 = [10,5,2,1,5,2]
Output: 3
```

<p><strong>Example 3:</strong></p>

```clike
Input: nums1 = [1,3,7,1,7,5], nums2 = [1,9,2,5,1]
Output: 2
```

<p><strong>Note:</strong></p>

<ol>
	<li><code>1 &lt;= nums1.length &lt;= 500</code></li>
	<li><code>1 &lt;= nums2.length &lt;= 500</code></li>
	<li><code><font face="monospace">1 &lt;= nums1[i], nums2[i] &lt;= 2000</font></code></li>
</ol>




题意：在两条独立的水平线上按给定的顺序写下 `nums1` 和 `nums2` 中的整数。现在，可以绘制一些连接两个数字 `nums1[i]` 和 `nums2[j]` 的直线，这些直线需要同时满足满足：
- `nums1[i] == nums2[j]`
 -  且绘制的直线不与任何其他连线（非水平线）相交。

请注意，连线即使在端点也不能相交：每个数字只能属于一条连线。以这种方法绘制线条，并返回可以绘制的最大连线数。

---
### 解法 动态规划
转换为求LCS的问题，公共子序列能够保证他们的连线不相交。
```cpp
class Solution {
public:
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {  
        int m = nums1.size(), n = nums2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (nums1[i] == nums2[j]) dp[i + 1][j + 1] = dp[i][j] + 1;
                else dp[i + 1][j + 1] = max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
        return  dp[m][n];
    }
};
```
运行效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了59.90% 的用户
内存消耗：12.6 MB, 在所有 C++ 提交中击败了39.35% 的用户
```

