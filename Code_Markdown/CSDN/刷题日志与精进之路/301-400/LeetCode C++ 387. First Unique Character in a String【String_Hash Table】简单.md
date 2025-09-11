

Given a string, find the first non-repeating character in it and return its index. If it doesn't exist, return -1.

**Examples:** 
```cpp
s = "leetcode"
return 0.

s = "loveleetcode"
return 2.
```
**Note:** You may assume the string contains only lowercase English letters.

题意：找出字符串中第一个不重复的字符。

---
### 解法 哈希
```cpp
class Solution {
public:
    int firstUniqChar(string s) {
        int cnt[26] = {0}, n = s.size();
        for (const char &c : s) ++cnt[c - 'a'];
        for (int i = 0; i < n; ++i) if (cnt[s[i] - 'a'] == 1) return i;
        return -1;
    }
};
```
提交后的运行结果：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了91.78% 的用户
内存消耗：10.8 MB, 在所有 C++ 提交中击败了37.17% 的用户
```

