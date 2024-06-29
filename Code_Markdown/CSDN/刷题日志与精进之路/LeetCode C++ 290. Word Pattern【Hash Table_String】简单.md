Given a `pattern` and a string `s` , find if `s` follows the same pattern.

Here **follow** means a full match, such that there is a bijection between a letter in `pattern` and a non-empty word in `s` .

 

**Example 1:**
```cpp
Input: pattern = "abba", s = "dog cat cat dog"
Output: true
```

**Example 2:**
```cpp
Input: pattern = "abba", s = "dog cat cat fish"
Output: false
```

**Example 3:**
```cpp
Input: pattern = "aaaa", s = "dog cat cat dog"
Output: false
```

**Example 4:**
```cpp
Input: pattern = "abba", s = "dog dog dog dog"
Output: false
```
**Constraints:**
- `1 <= pattern.length <= 300`
- `pattern` contains only lower-case English letters.
- `1 <= s.length <= 3000`
- `s` contains only lower-case English letters and spaces `' '` .
 - `s` **does not contain** any leading or trailing spaces.
-  All the words in `s` are separated by a **single space**.

 

题意：给出一种规律 `pattern` 和一个字符串 `str` ，判断 `str` 是否遵循相同的规律。

---
思路：这一题和LeetCode 205 同构字符串几乎一样。无非是同构的含义发生变化：`str` 中的以空格分割的单词一一对应到 `pattern` 中的字符。注意：**需要判断单词个数和字符个数是否相等**。

代码如下：
```cpp
class Solution { 
public:
    bool wordPattern(string pattern, string s) {
        if (pattern.empty()) return s.empty();
        //如果字符的个数不等于单词的个数,返回false
        int words = 1;
        for (int i = 0; i < s.size(); ++i) 
            if (s[i] == ' ') ++words;
        if (words != pattern.size()) return false;
        
        unordered_map<char, string> dict;
        int beginPos = 0, endPos;
        for (const char &ch : pattern) {
            endPos = s.find(' ', beginPos);
            const string &t = s.substr(beginPos, endPos - beginPos);

            if (dict.find(ch) == dict.end()) {
                for (const auto &it : dict) 
                    if (it.second == t) return false;
                dict[ch] = t;
            } else {  
                if (t != dict[ch]) return false;
            }
            beginPos = endPos + 1;
        }
        return true;
    }
};
```

