<p>请实现一个函数用来匹配包含<code>'. '</code>和<code>'*'</code>的正则表达式。模式中的字符<code>'.'</code>表示任意一个字符，而<code>'*'</code>表示它前面的字符可以出现任意次（含0次）。在本题中，匹配是指字符串的所有字符匹配整个模式。例如，字符串<code>"aaa"</code>与模式<code>"a.a"</code>和<code>"ab*ac*a"</code>匹配，但与<code>"aa.a"</code>和<code>"ab*a"</code>均不匹配。</p>

<p><strong>示例 1:</strong></p>

```csharp
输入:
s = "aa"
p = "a"
输出: false
解释: "a" 无法匹配 "aa" 整个字符串。
```

<p><strong>示例 2:</strong></p>

```csharp
输入:
s = "aa"
p = "a*"
输出: true
解释: 因为 '*' 代表可以匹配零个或多个前面的那一个元素, 在这里前面的元素就是 'a'。因此，字符串 "aa" 可被视为 'a' 重复了一次。 
```

<p><strong>示例&nbsp;3:</strong></p>

```csharp
输入:
s = "ab"
p = ".*"
输出: true
解释: ".*" 表示可匹配零个或多个（'*'）任意字符（'.'）。
```

<p><strong>示例 4:</strong></p>

```csharp
输入:
s = "aab"
p = "c*a*b"
输出: true
解释: 因为 '*' 表示零个或多个，这里 'c' 为 0 个, 'a' 被重复一次。因此可以匹配字符串 "aab"。 
```

<p><strong>示例 5:</strong></p> 

```csharp
输入:
s = "aab"
p = "c*a*b"
输出: true
解释: 因为 '*' 表示零个或多个，这里 'c' 为 0 个, 'a' 被重复一次。因此可以匹配字符串 "aab"。
```

 
<ul>
	<li><code>s</code>&nbsp;可能为空，且只包含从&nbsp;<code>a-z</code>&nbsp;的小写字母。</li>
	<li><code>p</code>&nbsp;可能为空，且只包含从&nbsp;<code>a-z</code>&nbsp;的小写字母以及字符&nbsp;<code>.</code>&nbsp;和&nbsp;<code>*</code>，无连续的 <code>'*'</code>。</li>
</ul>
 
---
### 解法1 动态规划
```cpp
class Solution {
public:
    bool isMatch(string s, string p) {
        int n = s.size(), m = p.size(); //dp[i][j]表示s[0...i)与p[0...j)是否匹配
        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));
        dp[0][0] = true; //表示s[0,0)与p[0,0)匹配
        for (int i = 1; i <= n; ++i) dp[i][0] = false; //表示s[0,i)与p[0,0)不匹配
        for (int i = 1; i <= m; ++i) {
            if (p[i - 1] != '*') dp[0][i] = false; //表示s[0,0)与p[0,i)不匹配
            else dp[0][i] = dp[0][i - 2]; //表示s[0,0)与p[0,i)匹配结果,由s[0,0)与p[0,i-2)匹配的结果决定
        }
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (s[i - 1] == p[j - 1] || p[j - 1] == '.') dp[i][j] = dp[i - 1][j - 1]; //此处匹配
                else if (p[j - 1] == '*') //字符匹配且s[0,i-1)与p[0,j)匹配(匹配1次到多次) 或 s[0,i)与p[0,j-2)匹配(匹配0次)
                    dp[i][j] = (((s[i - 1] == p[j - 2] || p[j - 2] == '.' ) && dp[i - 1][j]) || dp[i][j - 2]); 
                else dp[i][j] = false;
            }
        }
        return dp[n][m];
    }  
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了94.17% 的用户
内存消耗：6.7 MB, 在所有 C++ 提交中击败了62.68% 的用户
```
---
### 解法2 递归匹配
```cpp
class Solution {
private:
    bool isMatchOf(const string &s, int sl, int sr, const string &p, int pl, int pr) {
        if (pl >= pr) return sl >= sr; 
        bool firstMatch = sl < sr && (s[sl] == p[pl] || p[pl] == '.');
        if (pl + 1 < pr && p[pl + 1] == '*')
            return (firstMatch && isMatchOf(s, sl + 1, sr, p, pl, pr)) || isMatchOf(s, sl, sr, p, pl + 2, pr);
        return firstMatch && isMatchOf(s, sl + 1, sr, p, pl + 1, pr);
    }
public:
    bool isMatch(string s, string p) { return isMatchOf(s, 0, s.size(), p, 0, p.size()); }  
};
```
运行效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了21.69% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了98.84% 的用户
```
