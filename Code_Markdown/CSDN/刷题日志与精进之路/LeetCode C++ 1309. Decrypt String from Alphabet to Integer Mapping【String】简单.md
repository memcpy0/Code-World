

Given a string s formed by digits (`'0'` - `'9'`) and `'#'` . We want to map `s` to English lowercase characters as follows:
- Characters (`'a'` to `'i'`) are represented by (`'1'` to `'9'`) respectively.
 - Characters (`'j'` to `'z'`) are represented by (`'10#'` to `'26#'`) respectively. 

Return the string formed after mapping.

It's guaranteed that a unique mapping will always exist.

 

**Example 1:**

```swift
Input: s = "10#11#12"
Output: "jkab"
Explanation: "j" -> "10#" , "k" -> "11#" , "a" -> "1" , "b" -> "2".
```

**Example 2:**

```swift
Input: s = "1326#"
Output: "acz"
```

**Example 3:**

```swift
Input: s = "25#"
Output: "y"
```

**Example 4:**

```swift
Input: s = "12345678910#11#12#13#14#15#16#17#18#19#20#21#22#23#24#25#26#"
Output: "abcdefghijklmnopqrstuvwxyz"
```

**Constraints:**
- `1 <= s.length <= 1000`
- `s[i]` only contains digits letters (`'0'`-`'9'`) and `'#'` letter.
- `s` will be valid string such that mapping is always possible.




题意：给出一个字符串 `s`，它由数字（`'0'` - `'9'`）和 `'#'` 组成。按下述规则将 `s` 映射为一些小写英文字符：
- 字符（`'a'` - `'i'`）分别用（`'1'` - `'9'`）表示。
 - 字符（`'j'` - `'z'`）分别用（`'10#'` - `'26#'`）表示。 

返回映射之后形成的新字符串。

 ---
### 解法 正向遍历
```cpp
class Solution {
public:
    string freqAlphabets(string s) {
        string ans;
        int n = s.size();
        for (int i = 0; i < n; ++i) {
            if (i + 2 >= n || s[i + 2] != '#') ans.push_back('a' + s[i] - '1'); //'1'~'9' -> 'a'~'i'
            else { ans.push_back('a' - 1 + stoi(s.substr(i, 2))); i += 2; }
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了21.51% 的用户
```
