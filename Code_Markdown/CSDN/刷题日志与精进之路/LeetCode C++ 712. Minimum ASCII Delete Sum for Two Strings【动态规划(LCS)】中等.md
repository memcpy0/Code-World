<p>Given two strings <code>s1, s2</code>, find the lowest ASCII sum of deleted characters to make two strings equal.</p>

<p><b>Example 1:</b><br>

```clike
Input: s1 = "sea", s2 = "eat"
Output: 231
Explanation: Deleting "s" from "sea" adds the ASCII value of "s" (115) to the sum.
Deleting "t" from "eat" adds 116 to the sum.
At the end, both strings are equal, and 115 + 116 = 231 is the minimum sum possible to achieve this.
```

 

<p><b>Example 2:</b><br>

```clike
Input: s1 = "delete", s2 = "leet"
Output: 403
Explanation: Deleting "dee" from "delete" to turn the string into "let",
adds 100[d]+101[e]+101[e] to the sum.  Deleting "e" from "leet" adds 101[e] to the sum.
At the end, both strings are equal to "let", and the answer is 100+101+101+101 = 403.
If instead we turned both strings into "lee" or "eet", we would get answers of 433 or 417, which are higher.
```

 
<b>Note:</b>
<ul><li><code>0 &lt; s1.length, s2.length &lt;= 1000</code>.</li>
<li>All elements of each string will have an ASCII value in <code>[97, 122]</code>.</li> 
 </ul>



题意：给定两个字符串 `s1, s2` ，找到使两个字符串相等所需删除字符的ASCII值的最小和。

---
### 解法 动态规划
LCS问题的变体，相当于求具有最大ASCII值之和的LCS。
```cpp
class Solution {
public:
    int minimumDeleteSum(string s1, string s2) {
        int m = s1.size(), n = s2.size(), tot = 0;
        for (const char &c : s1) tot += c;
        for (const char &c : s2) tot += c;
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        //dp[i][j]表示s1[0:i),s2[0:j)中具有的最大ASCII值之和
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (s1[i] == s2[j]) dp[i + 1][j + 1] = dp[i][j] + s1[i];
                else dp[i + 1][j + 1] = max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
        return tot - 2 * dp[m][n];
    }
};
```
运行效率如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了84.34% 的用户
内存消耗：14.8 MB, 在所有 C++ 提交中击败了74.80% 的用户
```
