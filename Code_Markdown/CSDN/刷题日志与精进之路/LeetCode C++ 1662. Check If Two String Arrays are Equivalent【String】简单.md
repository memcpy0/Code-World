Given two string arrays `word1` and `word2`, return `true` *if the two arrays **represent** the same string, and* `false` *otherwise.*

A string is **represented** by an array if the array elements concatenated **in order** forms the string.

 

**Example 1:**

```swift
Input: word1 = ["ab", "c"], word2 = ["a", "bc"]
Output: true
Explanation:
word1 represents string "ab" + "c" -> "abc"
word2 represents string "a" + "bc" -> "abc"
The strings are the same, so return true.
```

**Example 2:**

```swift
Input: word1 = ["a", "cb"], word2 = ["ab", "c"]
Output: false
```

**Example 3:**

```swift
Input: word1  = ["abc", "d", "defg"], word2 = ["abcddefg"]
Output: true
```

 

**Constraints:**

- `1 <= word1.length, word2.length <= 10^3`
-  `1 <= word1[i].length, word2[i].length <= 10^3`
 - `1 <= sum(word1[i].length), sum(word2[i].length) <= 10^3`
 - `word1[i]` and `word2[i]` consist of lowercase letters.


题意：给出两个字符串数组 `word1` 和 `word2` 。如果两个数组表示的字符串相同，返回 `true` ；否则，返回 `false` 。数组表示的**字符串**是由数组中的所有元素**按顺序**连接形成的字符串。

---
### 解法 暴力拼接
暴力拼接两个字符串数组：
```cpp
class Solution {
public:
    bool arrayStringsAreEqual(vector<string>& word1, vector<string>& word2) {
        string s, t;
        int n = word1.size(), m = word2.size();
        for (int i = 0; i < n; ++i) s += word1[i];
        for (int i = 0; i < m; ++i) t += word2[i];
        return s == t;
    }
};
```
执行结果如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：11.6 MB, 在所有 C++ 提交中击败了100.00% 的用户
```

 

