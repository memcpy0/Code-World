Given a string, your task is to count how many **palindromic substrings** in this string.

The substrings with **different start indexes or end indexes** are counted as different substrings even they consist of same characters.

Example 1:
```cpp
Input: "abc"
Output: 3
Explanation: Three palindromic strings: "a", "b", "c".
```
Example 2:
```cpp
Input: "aaa"
Output: 6
Explanation: Six palindromic strings: "a", "a", "a", "aa", "aa", "aaa".
```

**Note:** The input string length won't exceed `1000` .



题意：计算字符串中有多少个回文子串。如果某些回文子串由相同的字符组成，只要它们的开始位置或结束位置不同，就会被视作不同的子串。

---
思路1：暴力解法，检查每个 `(i,j)` 组成的子串是否是回文串，有 $\text{O(N}^2)$ 个 `(i,j)` 对，检查回文串需要 $\text{O(N)}$ 的时间，于是总的时间复杂度是 $\text{O(N}^3)$ 。

代码：
```cpp
class Solution {
public:
    bool isPalindrome(const string &s, int i, int j) {
        for (; i <= j; ++i, --j) 
            if (s[i] != s[j])
                return false;
        return true;
    }

    int countSubstrings(string s) {
        if (s.empty()) return 0;
        int num = 0;
        for (int i = 0; i < s.size(); ++i) 
            for (int j = i; j < s.size(); ++j) 
                if (i == j || isPalindrome(s, i, j))
                    ++num;
        return num;
    }
};
```
啊这，竟然能过，效率：
```cpp
执行用时：804 ms, 在所有 C++ 提交中击败了5.03% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了93.35% 的用户
```

---
思路2：**中心扩展法**，从Manacher算法中学到的。之所以暴力算法效率低下，是因为它采用每对 `(i,j)` 独立判断回文性质的做法，无法使用之前计算过的结果计算更大的子串。中心扩展法能够做到这一点：`aba` 是一个回文串，我们在它的基础上往左右扩展，于是判断 `xabax` 的回文性质就很简单了。

代码：
```cpp
class Solution {
public:
    int countSubstrings(string s) {
        if (s.empty()) return 0;
        int len = s.size() * 2 + 1, idx = 0, num = 0;
        char *str = new char[len];
        for (int i = 0; i < len; ++i) 
            str[i] = (i & 1) ? s[idx++] : '#'; //偶数位序为'#'
        for (int i = 0; i < len; ++i) {
            int toLeft = i, toRight = i; 
            while (toLeft >= 0 && toRight < len && str[toLeft] == str[toRight]) {
                if (str[toLeft] != '#') ++num; //不为'#',表示发现新的回文子串
                --toLeft, ++toRight;
            }  
        }
        return num;
    }
};
```
优化一下，不额外申请空间和插入 `#` 字符，实现中心扩展法的代码如下：
```cpp
class Solution {
public:
    int num = 0, n;
    int countSubstrings(string s) {
        const char *str = s.c_str();
        n = s.size();
        for (int i = 0; str[i]; ++i) {
            count(str, i, i);   //回文子串长度为奇数,从长度1开始向两边扩展
            count(str, i, i + 1); //回文子串长度为偶数,从长度2开始向两边扩展
        }
        return num;
    }
    void count(const char *s, int toLeft, int toRight) {
        while (toLeft >= 0 && toRight < n && s[toLeft] == s[toRight]) {
            ++num;
            --toLeft;
            ++toRight;
        }
    }
};
```
效率：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了98.75% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了93.35% 的用户
```
---
思路3：我们可以将**最长回文串长度的动态规划解法**迁移到这个题目上。Java代码如下，因为C++动态申请二维数组的写法太麻烦了：
```java
class Solution {
    public int countSubstrings(String s) {
        if (s == null || s.length() <= 0) return 0;
        int n = s.length(), num = 0;
        //dp[i][j]表示s[i,j]区域的字符是否为回文子串
        boolean[][] dp = new boolean[n][n];
        //dp[i+1][j-1]=true表示s[i+1, j-1]是回文子串
        //如果s[i]==s[j]那么dp[i][j]=true
        for (int i = n - 1; i >= 0; --i) {
            //外层循环倒着写,内层循环正着写
            //因为要求dp[i][j]需知道dp[i+1][j-1]
            for (int j = i; j < n; ++j) {
                //s[i]==s[j]时,当元素个数为1,2,3时,一定为回文子串
                if (s.charAt(i) == s.charAt(j) && (j - i <= 2 || dp[i + 1][j - 1])) {
                    dp[i][j] = true;
                    ++num;
                }
            }
        }
        return num;
    }
}
```
效率：
```cpp
执行用时：13 ms, 在所有 Java 提交中击败了42.06% 的用户
内存消耗：39.8 MB, 在所有 Java 提交中击败了40.88% 的用户
```
