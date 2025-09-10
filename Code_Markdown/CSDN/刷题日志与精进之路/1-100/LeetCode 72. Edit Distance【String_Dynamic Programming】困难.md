<p>Given two strings <code>word1</code> and <code>word2</code>, return <em>the minimum number of operations required to convert <code>word1</code> to <code>word2</code></em>.</p>

<p>You have the following three operations permitted on a word:</p>

<ul>
	<li>Insert a character</li>
	<li>Delete a character</li>
	<li>Replace a character</li>
</ul>

 
<p><strong>Example 1:</strong></p>

```clike
Input: word1 = "horse", word2 = "ros"
Output: 3
Explanation: 
horse -> rorse (replace 'h' with 'r')
rorse -> rose (remove 'r')
rose -> ros (remove 'e') 
```

<p><strong>Example 2:</strong></p>

```clike
Input: word1 = "intention", word2 = "execution"
Output: 5
Explanation: 
intention -> inention (remove 't')
inention -> enention (replace 'i' with 'e')
enention -> exention (replace 'n' with 'x')
exention -> exection (replace 'n' with 'c')
exection -> execution (insert 'u') 
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= word1.length, word2.length &lt;= 500</code></li>
	<li><code>word1</code> and <code>word2</code> consist of lowercase English letters.</li>
</ul>



题意：给你两个单词 `word1` 和 `word2`，请你计算出将 `word1` 转换成 `word2` 所使用的最少操作数 。

你可以对一个单词进行如下三种操作：

- 插入一个字符
 -  删除一个字符
 -  替换一个字符

---
### 解法 动态规划
这类两个字符串间的最优化问题，往往考虑 `word1[i]` 和 `word2[j]` 的关系：
- `word1[i] == word2[j]` ，不用插入/删除/替换字符，因此看 `word1[0:i)` 和 `word2[0:j)` 之间的关系；
- `word1[i] != word2[j]` ，看插入/删除/替换字符中哪一种操作的步数最少。

因此设 `dp[m][n]` 为将 `word1` 转换为 `word2` 的最小操作步数，于是有：
- `word1[i] == word2[j]` ，`dp[i][j] = dp[i - 1][j - 1]` ；
- `word1[i] != word2[j]` ，`dp[i][j]  = min(dp[i - 1][j - 1], min(dp[i][j - 1], dp[i - 1][j])) + 1` ，其中 `dp[i - 1][j - 1] + 1` 表示替换当前的字符，`dp[i][j - 1] + 1` 表示插入字符 `word2[j]` 到已经变换过的 `word1` 中，`dp[i - 1][j] + 1` 表示删除 `word1` 中当前的字符 `word1[i]` 。 

具体代码如下：
```cpp
class Solution {
public:

    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 1; i <= m; ++i) dp[i][0] = i;
        for (int i = 1; i <= n; ++i) dp[0][i] = i;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (word1[i] == word2[j]) dp[i + 1][j + 1] = dp[i][j];
                else dp[i + 1][j + 1] = 1 + min(dp[i][j], min(dp[i + 1][j], dp[i][j + 1]));
            }
        }
        return dp[m][n];
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了90.38% 的用户
内存消耗：8.7 MB, 在所有 C++ 提交中击败了65.96% 的用户
```
