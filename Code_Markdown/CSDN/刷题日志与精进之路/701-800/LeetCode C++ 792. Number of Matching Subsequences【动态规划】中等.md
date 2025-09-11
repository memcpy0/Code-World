Given string `S` and a dictionary of words `words` , find the number of `words[i]` that is a subsequence of `S` .

**Example**:
```cpp
Input: 
S = "abcde"
words = ["a", "bb", "acd", "ace"]
Output: 3
Explanation: There are three words in words that are a subsequence of S: "a", "acd", "ace".
```

**Note**:
-  All words in `words` and `S` will only consists of lowercase letters.
-  The length of `S` will be in the range of `[1, 50000]` .
-  The length of `words` will be in the range of `[1, 5000]` .
-  The length of `words[i]` will be in the range of `[1, 50]` .

题意：求一个字典中是 `S` 的子序列的所有字符串的数量。

---
思路：动态规划。

代码：
```cpp
class Solution {
public:
    int numMatchingSubseq(string S, vector<string>& words) {
        int m = S.size();
        //dp[i][j]表示在字符S[i:m)部分,字符j下一次出现的位置
        vector<vector<int>> dp(m + 1, vector<int>(26, 0));
        //dp[m][j]表示在字符S[m:m)部分,字符j下一次出现的位置,出现在m处表示不存在
        for (int j = 0; j < 26; ++j) dp[m][j] = m; 
        for (int i = m - 1; i >= 0; --i) {
            for (int j = 0; j < 26; ++j) {
                if (S[i] == 'a' + j) dp[i][j] = i;
                else dp[i][j] = dp[i + 1][j];
            }
        }
        int match = 0;
        for (const string &str : words) {
            int n = str.size(), add = 0;
            bool isSubSeq = true;
            for (int i = 0; i < n; ++i) {
                if (dp[add][str[i] - 'a'] == m) { isSubSeq = false; break; }
                add = dp[add][str[i] - 'a'] + 1;    //add跳到匹配str[i]的下标位置+1
            }
            if (isSubSeq) ++match;
        }
        return match;
    }
};
```
效率：
```
执行用时：1284 ms, 在所有 C++ 提交中击败了10.99% 的用户
内存消耗：347.1 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
