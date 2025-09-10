Given an arbitrary ransom note string and another string containing letters from all the magazines, write a function that will return true if the ransom note can be constructed from the magazines ; otherwise, it will return false.

Each letter in the magazine string can only be used once in your ransom note.

 

**Example 1:**

```swift
Input: ransomNote = "a", magazine = "b"
Output: false
```

**Example 2:**

```swift
Input: ransomNote = "aa", magazine = "ab"
Output: false
```

**Example 3:**

```swift
Input: ransomNote = "aa", magazine = "aab"
Output: true
```

 

**Constraints:** You may assume that both strings contain only lowercase letters.

 


题意：给定一个赎金信 `ransom` 字符串和一个杂志 `magazine` 字符串，判断字符串 `ransom` 能不能由字符串 `magazines` 里面的字符构成。

---
### 解法 哈希计数
计算 `magazine` 中各字符的出现次数，然后比较即可：
```cpp
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        int mset[26] = {0};
        for (const char &c : magazine) ++mset[c - 'a'];
        for (const char &c : ransomNote) {
            --mset[c - 'a'];
            if (mset[c - 'a'] < 0) return false;
        }
        return true;
    }
};
```
效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了98.93% 的用户
内存消耗：8.8 MB, 在所有 C++ 提交中击败了23.97% 的用户
```
