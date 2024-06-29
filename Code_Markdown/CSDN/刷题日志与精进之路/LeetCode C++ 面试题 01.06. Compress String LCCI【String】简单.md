Implement a method to perform basic string compression using the counts of repeated characters. For example, the string `aabcccccaaa` would become `a2blc5a3` . If the "compressed" string would not become smaller than the original string, your method should return the original string. You can assume the string has only uppercase and lowercase letters `(a - z)` .

**Example 1:**
```c
Input: "aabcccccaaa"
Output: "a2b1c5a3"
```
**Example 2:**
```c
Input: "abbccd"
Output: "abbccd"
Explanation: 
The compressed string is "a1b2c2d1", which is longer than the original string.
```
**Note:** `0 <= S.length <= 50000`

题意：利用字符重复出现的次数，进行字符串压缩。

---
### 背景知识
存在很多种压缩算法，行程长度算法也是其一，它根据字符串中**连续重复字符的长度**进行压缩，如： `aaaaaaabbccccdefffffffgg` ，压缩结果为：`a7b2c4d1e1f7g2` 。
 
很显然，这种压缩算法用于处理**存在连续重复字符的字符串**，效果较佳。最差的情况就是没有连续的字符，此时不仅没有压缩字符串，反而增加了字符串的长度，例如：`abcdefgh`，压缩后：`a1b1c1d1e1f1g1h1` 。因此，应用此算法要注意场景。
 
 ---
### 思路
代码实现很简单，只要做好前后字符对比，并对连续相同字符计数即可，时间复杂度为 $O(n)$ ：
```cpp
class Solution {
public:
    string compressString(string s) {
        if (s.size() <= 1) return s;
        string ans;
        int cnt = 1, len = s.size();
        char c = s[0];
        for (int i = 1; i < len; ++i) {
            if (c == s[i]) { //看下一个字符
                ++cnt; 
                continue; 
            }  
            ans.append(c + to_string(cnt));
            c = s[i];
            cnt = 1;
        }
        ans.append(c + to_string(cnt));
        return ans.size() < s.size() ? ans : s;
    }
};
```
算法效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了75.07% 的用户
内存消耗：7.3 MB, 在所有 C++ 提交中击败了32.12% 的用户
```
如果从 `0` 开始检查的话，代码会更加简单：
```cpp
class Solution {
public:
    string compressString(string s) {
        if (s.size() <= 1) return s;
        string compressedString;
        int countConsecutive = 0, len = s.size();
        for (int i = 0; i < len; ++i) {
            ++countConsecutive; //计算当前字符的连续重复次数
            //如果下一个字符和当前字符不同,附加这个字符到结果
            if (i + 1 >= len || s[i] != s[i + 1]) { 
                compressedString.append(s[i] + to_string(countConsecutive));
                countConsecutive = 0;
            }
        }  
        return compressedString.size() < s.size() ? compressedString : s;
    }
};
```
只是效率低一点（两次比较）：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了35.22% 的用户
内存消耗：7.2 MB, 在所有 C++ 提交中击败了67.53% 的用户
```
