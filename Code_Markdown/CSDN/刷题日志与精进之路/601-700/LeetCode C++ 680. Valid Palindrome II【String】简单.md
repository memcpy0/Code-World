Given a non-empty string s, you may delete **at most** one character. Judge whether you can make it a palindrome.

**Example 1:**
```c
Input: "aba"
Output: True
```

**Example 2:**
```c
Input: "abca"
Output: True
Explanation: You could delete the character 'c'.
```

**Note:** The string will only contain lowercase characters `a-z` . The maximum length of the string is `50000` .

题意：给定一个非空字符串 `s` ，最多删除一个字符。判断是否能成为回文字符串。
 
---
### 思路1：暴力算法
枚举不删除和删除一个字符的所有情况，代码如下：
```cpp
class Solution {
public:
    bool isPalindrome(const string &s) {
        for (int i = 0, j = s.size() - 1; i <= j; ++i, --j)
            if (s[i] != s[j]) return false;
        return true;
    }
    bool validPalindrome(string s) {
        if (s.size() <= 2 || isPalindrome(s)) return true;
        int n = s.size();
        for (int i = 0; i < n; ++i) 
            if (isPalindrome(s.substr(0, i) + s.substr(i + 1)))
                return true;
        return false;
    }
};
```
这种暴力算法，时间复杂度是 $O(n^2)$ 。果然，一提交就超出时间限制。

---
### 思路2：利用回文串特征
**利用回文串的特征**：在回文串左边和右边，分别添加字符串 `str` 和逆转的字符串 `rev_str` ，结果还是回文串。因此，我们从左右两端向中间开始验证是否是回文串，验证的过程中：
- 如果不存在两个不等的字符，则整个字符串就是回文串，直接返回 `false` ；
- 如果存在两个不等的字符，则判断删除一个字符后的字符串是不是回文串：
	- 以 `"abdda"` 这个串为例，如果 `left = 'b', right = 'd'` ，发现不一致，但是我们有一次删除的机会；
	- 那么很容易知道，此时 `(left + 1, right)` 或者 `(left, right - 1)` 这两个子串中只要有任意一个是回文串，则结果就是回文串。
	- 如果这两个子串都不是回文串，那么无论删不删除、删除 `left = 'b'` 还是删除 `right = 'd'` ，结果都不会是回文串。

C++代码如下：
```cpp
class Solution {
public:
    bool isPalindrome(const string &s, int b, int e) {
        for (int i = b, j = e; i <= j; ++i, --j)
            if (s[i] != s[j]) return false;
        return true;
    }
    bool validPalindrome(string s) {
        if (s.size() <= 2) return true;
        for (int left = 0, right = s.size() - 1; left < right; ++left, --right)
            if (s[left] != s[right]) 
                return isPalindrome(s, left + 1, right) || isPalindrome(s, left, right - 1);
        return true;
    }
};
```
算法的复杂度是 $O(n)$ ，效率如下：
```cpp
执行用时：80 ms, 在所有 C++ 提交中击败了92.48% 的用户
内存消耗：19.2 MB, 在所有 C++ 提交中击败了70.96% 的用户
```
