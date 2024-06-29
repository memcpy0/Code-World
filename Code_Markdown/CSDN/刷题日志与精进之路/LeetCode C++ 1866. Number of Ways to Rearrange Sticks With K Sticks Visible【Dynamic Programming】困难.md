<p>There are <code>n</code> uniquely-sized sticks whose lengths are integers from <code>1</code> to <code>n</code>. You want to arrange the sticks such that <strong>exactly</strong> <code>k</code>&nbsp;sticks are <strong>visible</strong> from the left. A stick&nbsp;is <strong>visible</strong> from the left if there are no <strong>longer</strong>&nbsp;sticks to the <strong>left</strong> of it.</p>

<ul>
	<li>For example, if the sticks are arranged <code>[<u>1</u>,<u>3</u>,2,<u>5</u>,4]</code>, then the sticks with lengths <code>1</code>, <code>3</code>, and <code>5</code> are visible from the left.</li>
</ul>

<p>Given <code>n</code> and <code>k</code>, return <em>the <strong>number</strong> of such arrangements</em>. Since the answer may be large, return it <strong>modulo</strong> <code>10<sup>9</sup> + 7</code>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: n = 3, k = 2
Output: 3
Explanation: [1,3,2], [2,3,1], and [2,1,3] are the only arrangements such that exactly 2 sticks are visible.
The visible sticks are underlined.
```

<p><strong>Example 2:</strong></p>

```clike
Input: n = 5, k = 5
Output: 1
Explanation: [1,2,3,4,5] is the only arrangement such that all 5 sticks are visible.
The visible sticks are underlined.
```
 
<p><strong>Example 3:</strong></p>

```clike
Input: n = 20, k = 11
Output: 647427950
Explanation: There are 647427950 (mod 109 + 7) ways to rearrange the sticks such that exactly 11 sticks are visible.
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 1000</code></li>
	<li><code>1 &lt;= k &lt;= n</code></li>
</ul>



题意：有 <code>n</code> 根长度互不相同的木棍，长度为从 <code>1</code> 到 <code>n</code> 的整数。请你将这些木棍排成一排，并满足从左侧 <strong>可以看到</strong>&nbsp;<strong>恰好</strong> <code>k</code> 根木棍。从左侧 <strong>可以看到</strong> 木棍的前提是这个木棍的 <strong>左侧</strong> 不存在比它 <strong>更长的</strong> 木棍。 例如，如果木棍排列为 <code>[<em><strong>1</strong></em>,<em><strong>3</strong></em>,2,<em><strong>5</strong></em>,4]</code> ，那么从左侧可以看到的就是长度分别为 <code>1</code>、<code>3</code> 、<code>5</code> 的木棍。

<p>给你 <code>n</code> 和 <code>k</code> ，返回符合题目要求的排列 <strong>数目</strong> 。由于答案可能很大，请返回对 <code>10<sup>9</sup> + 7</code> <strong>取余 </strong>的结果。</p>
 
---
### 解法 动态规划
我们设 `dp[i][j]` 表示从高度为 `1, 2, ..., i` 的木棍中，高度逐渐递减地插入新的木棍，从左侧看恰好看到 `k` 根木棍的方案数。新插入的木棍高度为 `1` ，当其插入在最前面时，`dp[i][j] += dp[i-1][j-1]` ；当长度为1的棍子不是插入在最前面时，共有 `i - 1 ` 种插法，且它插入何处都是不可视的（除了最左边)，因此 `dp[i][j] += dp[i-1][j] * (i - 1)` 。代码如下：
```cpp
class Solution {
public:
    int rearrangeSticks(int n, int k) {
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1));
        dp[0][0] = 1;
        const int mod = 1e9 + 7;
        int minnk = min(n, k);
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= minnk; ++j)
                dp[i][j] = (dp[i - 1][j - 1] + (dp[i - 1][j] * (i - 1)) % mod) % mod; 
        return dp[n][k];
    }
};
```
运行效率如下：
```cpp
执行用时：280 ms, 在所有 C++ 提交中击败了18.07% 的用户
内存消耗：117.1 MB, 在所有 C++ 提交中击败了11.37% 的用户
```
