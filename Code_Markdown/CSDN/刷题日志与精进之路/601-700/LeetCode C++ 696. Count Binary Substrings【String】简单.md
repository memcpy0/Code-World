Give a string `s`, count the number of **non-empty (contiguous) substrings** that have the same number of 0's and 1's, and all the 0's and all the 1's in these substrings are **grouped consecutively.**

Substrings that occur multiple times are counted the number of times they occur.

**Example 1:**

```groovy
Input: "00110011"
Output: 6
Explanation: There are 6 substrings that have equal number of consecutive 1's and 0's: "0011", "01", "1100", "10", "0011", and "01".

Notice that some of these substrings repeat and are counted the number of times they occur.

Also, "00110011" is not a valid substring because all the 0's (and 1's) are not grouped together.
```

**Example 2:**

```groovy
Input: "10101"
Output: 4
Explanation: There are 4 substrings: "10", "01", "10", "01" that have equal number of consecutive 1's and 0's.
```

**Note:**
- `s.length` will be between `1` and `50,000`.
- `s` will only consist of `"0"` or `"1"` characters.

题意：给定一个字符串 `s` ，计算具有相同数量 `0` 和 `1` 的非空连续子串的数量，并且这些字符串中所有的 `0` 和所有的 `1` 分别聚合在一起。另外，重复出现的子串需要计算它们出现的次数。

---
### 解法 顺序遍历
 `"000111"` 中有多少有效的子串—— `"000111", "0011", "01"` 3个；那么 `"11100"` 呢—— `"1100", "10"` 2个。于是 `"00011100"` 的有效子串为 `"000111", "0011", "01", "1100", "10"` 5个。所以规律很简单了：如果前一类数 `0 or 1` 出现的次数为 `x` ，后一类数 `1 or 0` 出现的次数为 `y` ，有效子串的数量为 `min(x, y)` 。具体的代码如下：
```cpp
class Solution {
public:
    int countBinarySubstrings(string s) {
        int pre = 0, cur = 1, res = 0; 
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] == s[i - 1]) { ++cur; continue; }  
            res += min(pre, cur); //min(前一类字符的出现次数,新字符的出现次数)=有效子串的数目
            pre = cur;            
            cur = 1; 			  //用cur计算新类字符的出现次数
        }
        res += min(pre, cur);
        return res;
    }
};
```
效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了96.41% 的用户
内存消耗：10.8 MB, 在所有 C++ 提交中击败了37.58% 的用户
```
或者写成如下形式，当 `pre >= cur` 时，`++res` ：
```cpp
class Solution {
public:
    int countBinarySubstrings(string s) {
        int pre = 0, cur = 1, res = 0; 
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] == s[i - 1]) ++cur;
            else { pre = cur; cur = 1; } //用cur计算新类字符的出现次数
            if (pre >= cur) ++res; //min(前一类字符的出现次数,新字符的出现次数)=有效子串的数目
        } 
        return res;
    }
};
```
效率如下，还行：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了96.41% 的用户
内存消耗：10.7 MB, 在所有 C++ 提交中击败了37.83% 的用户
```
下面是Python3的代码：
```cpp
class Solution:
    def countBinarySubstrings(self, s: str) -> int:
        pre, cur = 0, 1
        ans = 0
        for i in range(1, len(s)):
            if s[i] == s[i - 1]: cur = cur + 1
            else: pre, cur = cur, 1
            if pre >= cur: ans = ans + 1
        return ans
```
效率之低，难以想象：
```py
执行用时：152 ms, 在所有 Python3 提交中击败了90.26% 的用户
内存消耗：13.8 MB, 在所有 Python3 提交中击败了54.04% 的用户
```
