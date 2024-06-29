

字符串的左旋转操作是把字符串前面的若干个字符转移到字符串的尾部。请定义一个函数实现字符串左旋转操作的功能。比如，输入字符串 `"abcdefg"` 和数字 `2` ，该函数将返回左旋转两位得到的结果 `"cdefgab"` 。

 

**示例 1：**

```swift
输入: s = "abcdefg", k = 2
输出: "cdefgab"
```

**示例 2：**

```swift
输入: s = "lrloseumgh", k = 6
输出: "umghlrlose"
```

 

**限制：**  `1 <= k < s.length <= 10000`

---
### 解法1 辅助空间
```cpp
class Solution {
public:
    string reverseLeftWords(string s, int n) {
        return s.substr(n) + s.substr(0, n);
    }
};
```
时间效率还行：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了91.32% 的用户
内存消耗：8.2 MB, 在所有 C++ 提交中击败了5.00% 的用户
```
---
### 解法2 原地翻转
```cpp
class Solution {
public:
    string reverseLeftWords(string s, int n) {
        reverse(s.begin(), s.begin() + n);
        reverse(s.begin() + n, s.end());
        reverse(s.begin(), s.end());
        return s;
    }
};
```
执行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了91.32% 的用户
内存消耗：7.5 MB, 在所有 C++ 提交中击败了64.40% 的用户
```
