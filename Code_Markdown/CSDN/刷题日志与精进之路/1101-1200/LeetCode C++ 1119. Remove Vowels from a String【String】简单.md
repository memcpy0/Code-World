
Given a string `s` , remove the vowels `'a'` , `'e'` , `'i'` , `'o'` , and `'u'` from it, and return the new string.
 
**Example 1:**

```haskell
Input: s = "leetcodeisacommunityforcoders"
Output: "ltcdscmmntyfrcdrs"
```

**Example 2:**

```haskell
Input: s = "aeiou"
Output: ""
```
 
**Constraints:**
- `1 <= s.length <= 1000`
-  `s` consists of only lowercase English letters.

题意：删去字符串中的元音，并返回新的字符串。


---
### 解法 顺序遍历
```cpp
class Solution {
public:
    string removeVowels(string s) {
        string ans;
        for (const char &c : s) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') continue;
            ans.push_back(c);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了50.41% 的用户
```
