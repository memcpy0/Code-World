Given a string `s` which consists of lowercase or uppercase letters, return *the length of the **longest palindrome*** that can be built with those letters.

Letters are **case sensitive**, for example, `"Aa"` is not considered a palindrome here.

 

**Example 1:**

```swift
Input: s = "abccccdd"
Output: 7
Explanation:
One longest palindrome that can be built is "dccaccd", whose length is 7.
```

**Example 2:**

```swift
Input: s = "a"
Output: 1
```

**Example 3:**

```swift
Input: s = "bb"
Output: 2 
```

**Constraints:**
- `1 <= s.length <= 2000`
- s consits of lower-case **and/or** upper-case English letters only.

题意：给定一个包含大写字母和小写字母的字符串 `s` ，找到通过这些字母构造成的最长的回文串（大小写敏感）。

---
### 思路
利用回文串的特征——串中每个字符出现的次数为偶数次，或者只有**奇数长度回文串中心位置的字符**出现次数为奇数次。因此，我们记录字符串 `s` 中每个字母的出现次数，然后找到其中最大的奇数出现次数：
- 如果没有奇数次数，全是偶数次数，则能够构成的回文串长度就是 `s` 的长度；
- 不然就找到了最大的奇数出现次数，然后加上其他偶数出现次数或者奇数出现次数减一，就是最长的回文串长度。

代码如下：
```cpp
class Solution {
public:
    int longestPalindrome(string s) {
        int charset[52] = {0};
        for (const char &c : s) {
            if (islower(c)) ++charset[c - 'a']; //小写字母
            else ++charset[c - 'A' + 26]; //大写字母
        }
        int even = 0, odd = 0; //最大的奇数次出现次数
        bool isMaxOdd = true;
        for (int i = 0; i < 52; ++i) if (charset[i] & 1) odd = max(odd, charset[i]);
        for (int i = 0; i < 52; ++i) { 
            if ((charset[i] & 1)) { //出现次数是奇数
                if (charset[i] == odd && isMaxOdd) isMaxOdd = false;  //避免重复计算最大的奇数次出现次数             
                else even += charset[i] - 1;  
            }
            else even += charset[i];   
        } 
        return odd + even;
    }
};
```
提交后效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了46.88% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了89.87% 的用户
```
