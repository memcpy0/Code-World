Given a string `s` and a string `t` , check if `s` is subsequence of `t` .

A subsequence of a string is **a new string** which is formed from the original string by deleting some (can be none) of the characters without disturbing the relative positions of the remaining characters. (ie, `"ace"` is a subsequence of `"abcde"` while `"aec"` is not).

**Follow up:**
If there are lots of incoming `S`, say `S1, S2, ... , Sk` where `k >= 1B` , and you want to check one by one to see if T has its subsequence. In this scenario, how would you change your code?
 
Example 1:
```cpp
Input: s = "abc", t = "ahbgdc"
Output: true
```

Example 2:
```cpp
Input: s = "axc", t = "ahbgdc"
Output: false
```
**Constraints**:
- `0 <= s.length <= 100`
- `0 <= t.length <= 10^4`
- Both strings consists only of **lowercase** characters.


题意：判断字符串 `s` 是不是 `t` 的子序列。

---

思路1：双指针。`lo` 指向 `s` 的开头，如果扫描 `t` 字符串时发现一个字符 `== s[lo]` 就将 `lo++` ，然后继续扫描。

代码：
```cpp
class Solution {
public:
    bool isSubsequence(string s, string t) {
        if (s.empty()) return true;
        if (s.size() > t.size()) return false;
        int lo = 0, n = t.size();
        for (int i = 0; i < n && lo < s.size(); ++i) 
            if (t[i] == s[lo]) ++lo;
        return lo == s.size();
    }  
};
```
效率：$\text{O(m + n)}$ 时间，$\text{O(1)}$ 空间。不过如果 `s` 有很多个的话，就会是N方级别的算法。


---
思路2：分析并记录原串 `t` 的特征，反向DP，得到 `f[i][j]` ，然后判断 `s` 是否是 `t` 的子序列时，如果 `s[i]` 不存在，就不是；否则跳转到 `t` 中第一个 `s[i]` 字符的下一个位置，节省比较的时间。 

代码：
```cpp
class Solution {
public:
    bool isSubsequence(string s, string t) {
        int n = s.size(), m = t.size();
        //f[i][j]表示在字符串t[i:m)部分,字符j下一次出现的位置
        vector<vector<int> > f(m + 1, vector<int>(26, 0));
        //26个字符,在t[m:m)部分,初始化为m,表示不存在下一次出现的位置
        for (int i = 0; i < 26; i++) f[m][i] = m;
        //如果t[i]就是j的话,f[i][j]=i,表示字符j出现在t[i:m)的i处
        //否则f[i][j]=f[i + 1][j],表示字符j出现在t[i+1:m)中的某个位置
        for (int i = m - 1; i >= 0; i--) {
            for (int j = 0; j < 26; j++) {
                if (t[i] == j + 'a') f[i][j] = i;
                else f[i][j] = f[i + 1][j];
            }
        }
        int add = 0;
        for (int i = 0; i < n; i++) { //s[i]这个字符不存在于t[i:m)的部分,不是子序列
            if (f[add][s[i] - 'a'] == m) return false;
            add = f[add][s[i] - 'a'] + 1;
        }
        return true;
    }
};
```
效率：需要额外的 $\text{O(26 * (m + 1))}$ 空间。在单个字符串 `s` 判断是否是 `t` 的子序列时不占多少优势，不过存在多个 `s` 时，就会大大节省时间。
