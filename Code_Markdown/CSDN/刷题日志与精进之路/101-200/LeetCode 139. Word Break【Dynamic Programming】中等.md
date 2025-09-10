
Given a **non-empty** string s and a dictionary wordDict containing a list of **non-empty** words, determine if s can be segmented into a space-separated sequence of one or more dictionary words.

Note:
-    The same word in the dictionary may be reused **multiple** times in the segmentation.
-    You may assume the dictionary does not contain duplicate words.

Example 1:
```cpp
Input: s = "leetcode", wordDict = ["leet", "code"]
Output: true
Explanation: Return true because "leetcode" can be segmented as "leet code".
```

Example 2:
```cpp
Input: s = "applepenapple", wordDict = ["apple", "pen"]
Output: true
Explanation: Return true because "applepenapple" can be segmented as "apple pen apple".
             Note that you are allowed to reuse a dictionary word.
```

Example 3:

```cpp
Input: s = "catsandog", wordDict = ["cats", "dog", "sand", "and", "cat"]
Output: false
```

题意：判断给出的字符串是否可以被 `wordDict` 中的字符串拼凑出来。

思路：动态规划的典型题目。

代码：
```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wd(wordDict.begin(), wordDict.end());
        vector<bool> dp(s.size() + 1, false);
        //dp[i]表示s的前i个字符可以被wordDict拼凑出来
        //dp[0]即s的前0个字符"", 为true
        dp[0] = true;
        //每次循环, 检查长度为i的字符串区间是否可以被拼凑出来,直到检查整个s
        for (int i = 1; i <= s.size(); ++i) {
            //每次循环, 若前面长j的字符串可以被拼出来, 则检查从j开始的长i区间
            //的后一部分长为i-j的字符串是否可以在wordDict中找到
            //接着依次检查长为j+1,j+2,...,i-1的字符串是否可以被拼出来,...
            for (int j = 0; j < i; ++j) {
                if (dp[j] && wd.find(s.substr(j, i - j)) != wd.end()) {
                    //前面的长j字符串和后面长i-j的字符串均可拼出
                    //则s的前i个字符都可拼凑, dp[i]为true
                    dp[i] = true; 
                    break;
                }
            }
        }
        return dp[s.size()];
    }
};
```
