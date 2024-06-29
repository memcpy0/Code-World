A string is called a happy prefix if is a **non-empty** prefix which is also a suffix (excluding itself).

Given a string `s` . Return **the longest happy prefix** of `s` .

Return an **empty** string if no such prefix exists.

**Example 1:**
```cpp
Input: s = "level"
Output: "l"
Explanation: s contains 4 prefix excluding itself ("l", "le", "lev", "leve"), and suffix ("l", "el", "vel", "evel"). The largest prefix which is also suffix is given by "l".
```
**Example 2:**
```cpp
Input: s = "ababab"
Output: "abab"
Explanation: "abab" is the largest prefix which is also suffix. They can overlap in the original string.
```
**Example 3:**
```cpp
Input: s = "leetcodeleet"
Output: "leet"
```
**Example 4:**
```cpp
Input: s = "a"
Output: ""
```
**Constraints:**
- `1 <= s.length <= 10^5`
- `s` contains only lowercase English letters.

题意：找出一个字符串 `s` 的最长快乐前缀——快乐前缀是指原串中既是**非空前缀**也是**非空后缀**(不包括原串本身)的子字符串。

---
思路：就是KMP求 `nextArr` 数组的过程。代码如下：
```cpp
class Solution {
public:
    string longestPrefix(string s) { 
        if (s.size() <= 1) return "";

        int slen = s.size(), pos = 2, cn = 0;
        int *nextArr = new int[slen + 1];
        nextArr[0] = -1, nextArr[1] = 0;
        while (pos <= slen) {
            if (s[pos - 1] == s[cn]) nextArr[pos++] = ++cn;
            else if (cn > 0) cn = nextArr[cn];
            else nextArr[pos++] = cn;
        }
        return s.substr(0, nextArr[slen]);
    }
};
```
效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了73.53% 的用户
内存消耗：19.2 MB, 在所有 C++ 提交中击败了28.38% 的用户
```
这样写：
```cpp
class Solution {
public:
    string longestPrefix(string s) { 
        if (s.size() <= 1) return "";

        int slen = s.size();
        int *nextArr = new int[slen + 1];
        nextArr[0] = nextArr[1] = 0;
        for (int i = 1; i < slen; ++i) {
            int j = nextArr[i];
            while (j && s[i] != s[j]) j = nextArr[j];
            nextArr[i + 1] = (s[i] == s[j] ? j + 1 : 0);
        }
        return s.substr(0, nextArr[slen]);
    }
};
```
效率是一样的：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了73.53% 的用户
内存消耗：19.2 MB, 在所有 C++ 提交中击败了28.38% 的用户
```
