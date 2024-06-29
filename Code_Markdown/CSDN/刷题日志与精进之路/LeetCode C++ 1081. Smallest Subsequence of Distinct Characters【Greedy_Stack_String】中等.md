

Return the lexicographically smallest subsequence of `s` that contains all the distinct characters of `s` **exactly once**.
 

**Example 1:** 
```cpp
Input: s = "bcabc"
Output: "abc"
```

**Example 2:**

```cpp
Input: s = "cbacdcbc"
Output: "acdb"
```
 

**Constraints:**
- `1 <= s.length <= 1000`
- `s` consists of lowercase English letters.

题意：返回 `s` 字典序最小的子序列，该子序列包含 `s` 的所有不同字符，且只包含一次。

---
### 解法 贪心+栈
```cpp
class Solution {
public:
    string smallestSubsequence(string s) {
        if (s.size() <= 1) return s;
        vector<char> st;
        int n = s.size();
        bool vis[26] = {false};
        for (int i = 0; i < n; ++i) {
            char c = s[i];
            if (vis[c - 'a']) continue; //栈中已有这一字符,不必再添加
            //栈非空且栈顶字符>新字符且新字符之后有同样的栈顶字符,就弹出栈顶字符
            while (!st.empty() && c < st.back() && s.find(st.back(), i) != string::npos) {
                vis[st.back() - 'a'] = false; 
                st.pop_back();
            }
            vis[c - 'a'] = true;
            st.push_back(c);
        }
        string ans;
        for (const char &c : st) ans += c;
        return ans;
    }
};
```
提交后的运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了63.23% 的用户
```

