<p>Given a string <code>s</code>, find <em>the longest palindromic <strong>subsequence</strong>'s length in</em> <code>s</code>.</p>

<p>A <strong>subsequence</strong> is a sequence that can be derived from another sequence by deleting some or no elements without changing the order of the remaining elements.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: s = "bbbab"
Output: 4
Explanation: One possible longest palindromic subsequence is "bbbb".
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: s = "cbbd"
Output: 2
Explanation: One possible longest palindromic subsequence is "bb".
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 1000</code></li>
	<li><code>s</code> consists only of lowercase English letters.</li>
</ul>
 
题意：给你一个字符串 `s` ，找出其中最长的回文子序列，并返回该序列的长度。
 
---
### 解法 动态规划
「区间DP」问题的基本流程是：
- 从小到大枚举区间长度 `len` ；
- 枚举区间左端点 `l` ，根据区间长度 `len` 计算出区间右端点 `r = l + len - l` ；
- 通过状态转移方程计算 `dp[l][r]` 的值。

本题就是最长回文子序列问题，是一道经典的「区间DP」问题——因为**在给定一个回文串/序列的前提下，在左右两侧分别添加两个新的字符，再判断两字符是否相同，就可以得知新串/序列是否为回文串/序列**。即，由小区间的回文状态可以推导出大区间的回文状态。

如果从图论的角度来看，**任何一个长度为 $n$ 的回文序列，必然由长度为 $n - 1$ 或者长度为 $n - 2$ 的回文序列转移而来**。即存在有向边，由长度为 $n - 1$ 或 $n - 2$ 的较短的回文序列**指向**长度为 $n$ 的较长的回文序列。更准确的说是，「**两个具有公共回文部分的回文序列间存在拓扑序**」，这些拓扑序最终构成一个**有向无环图**。

此处我们定义 `dp[l][r]` 为字符串区间 `[l, r]` 中的最长回文子序列长度。考虑 `dp[l][r]` 状态转移时，由于**没有限制**回文序列必须选择 `s[l]` 或 `s[r]` ，我们对边界字符 `s[l], s[r]` 分情况讨论，最终的 `dp[l][r]` 需要在这三者中取**最大值**：
- 形成的回文子序列可能包含 `s[l]` 、但一定不包含 `s[r]` ，此时 `dp[l][r] = dp[l][r - 1]` ；
- 形成的回文子序列可能包含 `s[r]` 、但一定不包含 `s[l]` ，此时 `dp[l][r] = dp[l + 1][r]` ；
- 形成的回文子序列可能包含 `s[l], s[r]` 、或者不包含 `s[l], s[r]` ，根据 `s[l]` 是否等于 `s[r]` ：相等时一定同时包含 `s[l], s[r]` ，`dp[l][r] = dp[l + 1][r - 1] + 2` ；不等时不包含 `s[l], s[r]` ，`dp[l][r] = dp[l + 1][r - 1]` ，或者不同时包含——考虑前两种情况。

通过分类讨论，我们「不漏」地考虑了所有情况（**不能保证「不重」**，只是对于最值问题，某些状态重复参与比较不会影响结果的正确性），得到了DP转移方程。但是也不要忘记**基本情况**，特判长度为 `1`（和 `2` 也行，当且仅当两字符相等时为回文）时必然为回文。

代码如下，时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(n^2)$ 。注意，由于转移方程由更小的区间指向更大的区间，在二维数组中表现出的就是**由左边和下边转移向上边和右边**。下面的代码从二维矩阵 `dp[][]` 的右下角向上、向右转移（**思考一下为什么没有特判长度为 `2` 的情况**——此时 `dp[i + 1][j - 1]` 为 `0` ，相等则加 `2` 、否则加 `0` ；又从 `dp[i][j - 1], dp[i + 1][j]` 转移过来，于是不等时 `dp[i][j]` 为 `1` ）：
```cpp
//C++ version
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        if (s.size() <= 1) return s.size();
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n));
        for (int i = n - 1; i >= 0; --i) {
            for (int j = i; j < n; ++j) {
                dp[i][j] = (i == j ? 1 : //长度为1时特判
                    max(dp[i + 1][j - 1] + (s[i] == s[j] ? 2 : 0), //长度不为1时 //可能都包含,或都不包含
                        max(dp[i + 1][j], dp[i][j - 1])) //不包含s[i]或s[j]
                );
            }
        }
        return dp[0][n - 1];
    }
};
//执行用时：144 ms, 在所有 C++ 提交中击败了14.70% 的用户
//内存消耗：71.2 MB, 在所有 C++ 提交中击败了66.45% 的用户
```
通过使用「**滚动数组**」，可以优化空间复杂度到 $O(n)$ 。上述代码的升级版如下所示，我们发现空间优化的效率是惊人的（**思考一下为什么这里也没有特判长度为 `2` 的情况**）：
```cpp
//C++ version
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        if (s.size() <= 1) return s.size();
        int n = s.size();
        vector<vector<int>> dp(2, vector<int>(n));
        for (int i = n - 1; i >= 0; --i) {
            for (int j = i; j < n; ++j) {
                dp[i & 1][j] = (i == j ? 1 : 
                    max(dp[!(i & 1)][j - 1] + (s[i] == s[j] ? 2 : 0),
                        max(dp[!(i & 1)][j], dp[i & 1][j - 1]))
                );
            }
        }
        return dp[0][n - 1];
    }
};
//执行用时：140 ms, 在所有 C++ 提交中击败了15.97% 的用户
//内存消耗：6.6 MB, 在所有 C++ 提交中击败了96.69% 的用户
```
自然，我们也可以从左往右转移。下面的代码从二维矩阵 `dp[][]` 的主对角线向右转移。**为了简洁明了，使用了「区间DP」的经典步骤**，只是这样写就难以滚动优化。此处**显式特判了长度为 `2` 的基本情况**，也可以不特判、和上述做法一致（即相等时 `dp[i][j]` 为 `0 + 2` ，不等时转移过来为 `1` ）：
```java
//Java version
class Solution {
    public int longestPalindromeSubseq(String s) {
        int n = s.length();
        char[] cs = s.toCharArray();
        int[][] dp = new int[n][n];
        for (int len = 1; len <= n; ++len) {
            for (int l = 0; l + len - 1 < n; ++l) { //右端点在范围内
                int r = l + len - 1;
                if (len == 1) dp[l][r] = 1;
                else if (len == 2) dp[l][r] = (cs[l] == cs[r]) ? 2 : 1; //注意特判
                else dp[l][r] = Math.max(
                    dp[l + 1][r - 1] + (cs[l] == cs[r] ? 2 : 0),
                    Math.max(dp[l + 1][r], dp[l][r - 1])
                );
            }
        }
        return dp[0][n - 1];
    }
}
//执行用时：36 ms, 在所有 Java 提交中击败了54.56% 的用户
//内存消耗：48.2 MB, 在所有 Java 提交中击败了78.30% 的用户
```


