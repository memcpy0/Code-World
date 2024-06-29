> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091403665.png)
Given an input string (`s`) and a pattern (`p`), implement wildcard pattern matching with support for `'?'` and `'*'`.
- `'?'` Matches any single character.
- `'*'` Matches any sequence of characters (including the empty sequence).

The matching should cover the **entire** input string (not partial).

Note:
-    `s` could be empty and contains only lowercase letters `a-z`.
-    `p` could be empty and contains only lowercase letters `a-z`, and characters like `?` or `*`.

Example 1:
```cpp
Input:
s = "aa"
p = "a"
Output: false
Explanation: "a" does not match the entire string "aa".
```

Example 2:
```cpp
Input:
s = "aa"
p = "*"
Output: true
Explanation: '*' matches any sequence.
```

Example 3:
```cpp
Input:
s = "cb"
p = "?a"
Output: false
Explanation: '?' matches 'c', but the second letter is 'a', which does not match 'b'.
```

Example 4:
```cpp
Input:
s = "adceb"
p = "*a*b"
Output: true
Explanation: The first '*' matches the empty sequence, while the second '*' matches the substring "dce".
```

Example 5:
```cpp
Input:
s = "acdcb"
p = "a*c?b"
Output: false
```

---
题意：这道题和 `LeetCode 10. Regular Expression Matching` 很像，但是又有不同。按照第10题的思路写，会超出时间限制。
```cpp
class Solution {
public:
    bool isMatch(string s, string p) {
        return match(s, 0, p, 0);
    }
    bool match(const string &s, int sd, const string &p, int pd) {
        if (pd >= p.size()) return sd >= s.size(); 
        if (sd >= s.size()) {
            for (int i = pd; i < p.size(); ++i)
                if (p[i] != '*')
                    return false;
            return true;
        }

        if (p[pd] == '*') {
            return match(s, sd + 1, p, pd) || match(s, sd, p, pd + 1);
        } else {
            bool firstMatch = (sd < s.size()) && (s[sd] == p[pd] || p[pd] == '?');
            //递归匹配后面的字符串
            return firstMatch && match(s, sd + 1, p, pd + 1);
        } 
    }
};
```
这里的题意是：给出一个匹配串和一个模式串，匹配串中全是小写字母，模式串中是小写字母、 `?` 和 `*` —— 其中 `?` 可以匹配任何一个单独的字符，`*` **可以匹配任何字符序列**（第10题中的 `*` 是重复匹配前导字符任意次）。最后判断是否匹配。
### 解法1 双指针+回溯法
思路1：回溯法。两个指针 `i=0,j=0` 分别指向 `s` 和 `p` ，**一个 `start = -1` 记录 `j` 回溯时的前一个位置，`match` 记录 `i` 回溯时的前一个位置**：
- 每次都要检查 `i,j` 是否超出范围；
- 如果 `s[i] == p[j] || p[j] == ?` 则匹配，两个指针都往后移动一步；
- **如果 `p[j] == *` 说明将来这里有回溯的余地**——如果后面匹配失败，可以用 `*` 匹配任意字符序列。为此：
	- 需要用一个 `start = j` 记录 `p[j]` 的位置，方便将来回溯 `p` ；
	- 需要用一个 `match = i` 记录此时 `s[i]` 的位置，方便将来回溯 `s` ；
	- 然后 `++j` ，用下一个 `p[j]` 来和此时的 `s[i]` 相匹配，**实质上是先用** `start` 指向的 `*` **匹配空序列**；
	- 按照题意，连续的 `*` 序列等价于一个 `*` (其他 `*` 匹配空序列)，因此，**上述写法只会用 `start` 记录连续 `*` 序列中最后一个 `*` 的位置**，`match = i` 在这一段中不变，`++j` 直到 `j` 指向非 `*` 字符；
	- 如果前面 `s,p` 匹配成功，遇到下一个 `*` 时就会更新新的 `*` 位置，以待下一次失配；
- 如果前两个分支都无法进入，说明 `s[i] != p[j] && p[j] != ? && p[j] != *` ，那么 `s[i],p[j]` 此时都是字母，匹配失败，此时：
	- 如果 `start != -1` 说明存在 `*` ，有回溯的可能，于是将 `j = start + 1` ，`++match` ，`i = match` ，**实质上**相当于用 `*` 匹配这个 `s[i]` 字符， `j` 必定指向的非*字符匹配下一个 `s[i]` ，而后继续比较——直到下一次失配，再用 `start` 指向的这个 `*` 匹配下一个 `s[i]` 字符...
	- 否则，直接返回 `false` ，因为匹配失败，而且没有 `*` ，不可能回溯。
- 这样一直比较，直到 `i >= s.size()` 时退出。如果 `j >= p.size() || p[j:] == "***..."` 时返回 `true` ，否则返回 `false` 。

回溯法代码如下：
```cpp
class Solution {
public:
    bool isMatch(string s, string p) { 
        int i = 0, j = 0, start = -1, match = 0, slen = s.size(), plen = p.size();
        while (i < slen) {
            if (j < plen && (s[i] == p[j] || p[j] == '?')) { //一个字符匹配成功
                ++i;
                ++j; 
            } else if (j < plen && p[j] == '*') { //记录*的位置和此时i的位置,以便回溯
                start = j;
                match = i;
                ++j;  //先用*匹配空串
            } else if (start != -1) { //前面失配
                j = start + 1;        //j回溯指向下一个非*字符
                ++match;              //用*匹配原来记录的s[match]字符
                i = match;            //i回溯指向下一个位置
            } else return false;
        }
        for (; j < plen; ++j) if (p[j] != '*') return false; //非*字符存在说明没有完全匹配
        return j >= plen; //j>=plen说明完全匹配
    } 
};
```
效率：
```
执行用时：4 ms, 在所有 C++ 提交中击败了99.22% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
思路2：动态规划。设 `dp[i][j]` 表示 `s[0:i-1]` 与 `p[0:j-1]` 相匹配，则：
- 初始化：
	- `dp[0][0] = true` ，都是空串；
	- `dp[i][0] = false` ，`s[0:i-1]` 与空串比较，当然全部是 `false` ；
	- `dp[0][j]` ，`s` 为空，如果 `p` 开始为 `*` 才为 `true` ；
- 动态方程：
	- 如果 `(s[i] == p[j] || p[j] == '?') && dp[i - 1][j - 1]` ，则有 `dp[i][j] = true` ；
	- 如果 `p[j] == '*' && (dp[i - 1][j] || dp[i][j - 1])` ，则 `dp[i][j] = true` ：
		- 这里 `dp[i - 1][j] == true` 到 `dp[i][j]` 的转移表示 `*` 现在匹配非空字符串，如 `ab, ab*` ；
		- `dp[i][j - 1] == true` 到 `dp[i][j]` 的转移表示 `*` 现在匹配空字符串，如 `abcd, ab*` 。

以 `abcd, ab*` 为例：
```
   j   a     b     *
i  1   0     0     0
a  0   1     0     0
b  0   0     1     1
c  0   0     0     1
d  0   0     0     1
```
动态规划代码如下：
```cpp
class Solution {
public:
    bool isMatch(string s, string p) { 
        int slen = s.size(), plen = p.size();
        vector<vector<bool>> dp(slen + 1, vector<bool>(plen + 1, false));
        dp[0][0] = true;
        for (int i = 1; i <= plen; ++i) //p开始为*则对应的dp[0][1]为true,如果是后面的*,则重复之前的值
            if (p[i - 1] == '*')
                dp[0][i] = dp[0][i - 1];
        for (int i = 1; i <= slen; ++i) {
            for (int j = 1; j <= plen; ++j) {
                if (dp[i - 1][j - 1] && (s[i - 1] == p[j - 1] || p[j - 1] == '?'))
                    dp[i][j] = true;
                else if (p[j - 1] == '*' && (dp[i - 1][j] || dp[i][j - 1])) 
                    dp[i][j] = true;
            }
        }
        return dp[slen][plen];
    }
};
```
效率比起回溯要差一些，回溯没有用到额外的空间：
```
执行用时：248 ms, 在所有 C++ 提交中击败了43.11% 的用户
内存消耗：11.5 MB, 在所有 C++ 提交中击败了66.67% 的用户
```
```java
class Solution {
    public boolean isMatch(String s, String p) {
        int sn = s.length(), pn = p.length();
        boolean[][] dp = new boolean[sn + 1][pn + 1];
        dp[0][0] = true;
        for (int i = 1; i <= pn; ++i)
            if (p.charAt(i - 1) == '*') dp[0][i] = dp[0][i - 1];
        for (int i = 1; i <= sn; ++i) {
            for (int j = 1; j <= pn; ++j) {
                if (s.charAt(i - 1) == p.charAt(j - 1) || p.charAt(j - 1) == '?')
                    dp[i][j] = dp[i - 1][j - 1];
                else if (p.charAt(j - 1) == '*') 
                    dp[i][j] = dp[i - 1][j] || dp[i][j - 1];   
            }
        }
        return dp[sn][pn];
    }
}
 
```

前言
本题与「10. 正则表达式匹配」非常类似，但相比较而言，本题稍微容易一些。因为在本题中，模式 ppp 中的任意一个字符都是独立的，即不会和前后的字符互相关联，形成一个新的匹配模式。因此，本题的状态转移方程需要考虑的情况会少一些。

方法一：动态规划
思路与算法

在给定的模式 ppp 中，只会有三种类型的字符出现：

小写字母 a−za-za−z，可以匹配对应的一个小写字母；

问号 ???，可以匹配任意一个小写字母；

星号 ∗*∗，可以匹配任意字符串，可以为空，也就是匹配零或任意多个小写字母。

其中「小写字母」和「问号」的匹配是确定的，而「星号」的匹配是不确定的，因此我们需要枚举所有的匹配情况。为了减少重复枚举，我们可以使用动态规划来解决本题。

我们用 dp[i][j]\textit{dp}[i][j]dp[i][j] 表示字符串 sss 的前 iii 个字符和模式 ppp 的前 jjj 个字符是否能匹配。在进行状态转移时，我们可以考虑模式 ppp 的第 jjj 个字符 pjp_jp 
j
​
 ，与之对应的是字符串 sss 中的第 iii 个字符 sis_is 
i
​
 ：

如果 pjp_jp 
j
​
  是小写字母，那么 sis_is 
i
​
  必须也为相同的小写字母，状态转移方程为：

dp[i][j]=(si 与 pj 相同)∧dp[i−1][j−1] \textit{dp}[i][j] = (s_i~与~p_j~相同) \wedge \textit{dp}[i-1][j-1]
dp[i][j]=(s 
i
​
  与 p 
j
​
  相同)∧dp[i−1][j−1]
其中 $\wedge$ 表示逻辑与运算。也就是说，$\textit{dp}[i][j]$ 为真，当且仅当 $\textit{dp}[i-1][j-1]$ 为真，并且 $s_i$ 与 $p_j$ 相同。
如果 pjp_jp 
j
​
  是问号，那么对 sis_is 
i
​
  没有任何要求，状态转移方程为：

dp[i][j]=dp[i−1][j−1] \textit{dp}[i][j] = \textit{dp}[i-1][j-1]
dp[i][j]=dp[i−1][j−1]
如果 pjp_jp 
j
​
  是星号，那么同样对 sis_is 
i
​
  没有任何要求，但是星号可以匹配零或任意多个小写字母，因此状态转移方程分为两种情况，即使用或不使用这个星号：

dp[i][j]=dp[i][j−1]∨dp[i−1][j] \textit{dp}[i][j] = \textit{dp}[i][j-1] \vee \textit{dp}[i-1][j]
dp[i][j]=dp[i][j−1]∨dp[i−1][j]
其中 $\vee$ 表示逻辑或运算。如果我们不使用这个星号，那么就会从 $\textit{dp}[i][j-1]$ 转移而来；如果我们使用这个星号，那么就会从 $\textit{dp}[i-1][j]$ 转移而来。
最终的状态转移方程如下：

dp[i][j]={(si 与 pj 相同)∧dp[i−1][j−1],pj 是小写字母dp[i−1][j−1],pj 是问号dp[i][j−1]∨dp[i−1][j],pj 是星号 \textit{dp}[i][j] = \begin{cases} (s_i~与~p_j~相同) \wedge \textit{dp}[i-1][j-1], & p_j~是小写字母 \\ \textit{dp}[i-1][j-1], & p_j~是问号 \\ \textit{dp}[i][j-1] \vee \textit{dp}[i-1][j], & p_j~是星号 \end{cases}
dp[i][j]= 
⎩
⎨
⎧
​
  
(s 
i
​
  与 p 
j
​
  相同)∧dp[i−1][j−1],
dp[i−1][j−1],
dp[i][j−1]∨dp[i−1][j],
​
  
p 
j
​
  是小写字母
p 
j
​
  是问号
p 
j
​
  是星号
​
 
我们也可以将前两种转移进行归纳：

dp[i][j]={dp[i−1][j−1],si 与 pj 相同或者 pj 是问号dp[i][j−1]∨dp[i−1][j],pj 是星号False,其它情况 \textit{dp}[i][j] = \begin{cases} \textit{dp}[i-1][j-1], & s_i~与~p_j~相同或者~p_j~是问号 \\ \textit{dp}[i][j-1] \vee \textit{dp}[i-1][j], & p_j~是星号 \\ \text{False}, & 其它情况 \end{cases}
dp[i][j]= 
⎩
⎨
⎧
​
  
dp[i−1][j−1],
dp[i][j−1]∨dp[i−1][j],
False,
​
  
s 
i
​
  与 p 
j
​
  相同或者 p 
j
​
  是问号
p 
j
​
  是星号
其它情况
​
 
细节

只有确定了边界条件，才能进行动态规划。在上述的状态转移方程中，由于 dp[i][j]\textit{dp}[i][j]dp[i][j] 对应着 sss 的前 iii 个字符和模式 ppp 的前 jjj 个字符，因此所有的 dp[0][j]\textit{dp}[0][j]dp[0][j] 和 dp[i][0]\textit{dp}[i][0]dp[i][0] 都是边界条件，因为它们涉及到空字符串或者空模式的情况，这是我们在状态转移方程中没有考虑到的：

dp[0][0]=True\textit{dp}[0][0] = \text{True}dp[0][0]=True，即当字符串 sss 和模式 ppp 均为空时，匹配成功；

dp[i][0]=False\textit{dp}[i][0] = \text{False}dp[i][0]=False，即空模式无法匹配非空字符串；

dp[0][j]\textit{dp}[0][j]dp[0][j] 需要分情况讨论：因为星号才能匹配空字符串，所以只有当模式 ppp 的前 jjj 个字符均为星号时，dp[0][j]\textit{dp}[0][j]dp[0][j] 才为真。

我们可以发现，dp[i][0]\textit{dp}[i][0]dp[i][0] 的值恒为假，dp[0][j]\textit{dp}[0][j]dp[0][j] 在 jjj 大于模式 ppp 的开头出现的星号字符个数之后，值也恒为假，而 dp[i][j]\textit{dp}[i][j]dp[i][j] 的默认值（其它情况）也为假，因此在对动态规划的数组初始化时，我们就可以将所有的状态初始化为 False\text{False}False，减少状态转移的代码编写难度。

最终的答案即为 dp[m][n]\textit{dp}[m][n]dp[m][n]，其中 mmm 和 nnn 分别是字符串 sss 和模式 ppp 的长度。需要注意的是，由于大部分语言中字符串的下标从 000 开始，因此 sis_is 
i
​
  和 pjp_jp 
j
​
  分别对应着 s[i−1]s[i-1]s[i−1] 和 p[j−1]p[j-1]p[j−1]。

C++
Java
Python3
Golang
C
class Solution {
public:
    bool isMatch(string s, string p) {
        int m = s.size();
        int n = p.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        dp[0][0] = true;
        for (int i = 1; i <= n; ++i) {
            if (p[i - 1] == '*') {
                dp[0][i] = true;
            }
            else {
                break;
            }
        }
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (p[j - 1] == '*') {
                    dp[i][j] = dp[i][j - 1] | dp[i - 1][j];
                }
                else if (p[j - 1] == '?' || s[i - 1] == p[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                }
            }
        }
        return dp[m][n];
    }
};
复杂度分析

时间复杂度：O(mn)O(mn)O(mn)，其中 mmm 和 nnn 分别是字符串 sss 和模式 ppp 的长度。

空间复杂度：O(mn)O(mn)O(mn)，即为存储所有 (m+1)(n+1)(m+1)(n+1)(m+1)(n+1) 个状态需要的空间。此外，在状态转移方程中，由于 dp[i][j]\textit{dp}[i][j]dp[i][j] 只会从 dp[i][..]\textit{dp}[i][..]dp[i][..] 以及 dp[i−1][..]\textit{dp}[i - 1][..]dp[i−1][..] 转移而来，因此我们可以使用滚动数组对空间进行优化，即用两个长度为 n+1n+1n+1 的一维数组代替整个二维数组进行状态转移，空间复杂度为 O(n)O(n)O(n)。

方法二：贪心算法
方法一的瓶颈在于对星号 ∗*∗ 的处理方式：使用动态规划枚举所有的情况。由于星号是「万能」的匹配字符，连续的多个星号和单个星号实际上是等价的，那么不连续的多个星号呢？

我们以 p=∗ abcd ∗p = *~\text{abcd}~*p=∗ abcd ∗ 为例，ppp 可以匹配所有包含子串 abcd\text{abcd}abcd 的字符串，也就是说，我们只需要暴力地枚举字符串 sss 中的每个位置作为起始位置，并判断对应的子串是否为 abcd\text{abcd}abcd 即可。这种暴力方法的时间复杂度为 O(mn)O(mn)O(mn)，与动态规划一致，但不需要额外的空间。

如果 p=∗ abcd∗efgh∗i ∗p = *~\text{abcd}*\text{efgh}*\text{i}~*p=∗ abcd∗efgh∗i ∗ 呢？显然，ppp 可以匹配所有依次出现子串 abcd\text{abcd}abcd、efgh\text{efgh}efgh、i\text{i}i 的字符串。此时，对于任意一个字符串 sss，我们首先暴力找到最早出现的 abcd\text{abcd}abcd，随后从下一个位置开始暴力找到最早出现的 efgh\text{efgh}efgh，最后找出 i\text{i}i，就可以判断 sss 是否可以与 ppp 匹配。这样「贪心地」找到最早出现的子串是比较直观的，因为如果 sss 中多次出现了某个子串，那么我们选择最早出现的位置，可以使得后续子串能被找到的机会更大。

因此，如果模式 ppp 的形式为 ∗ u1∗u2∗u3 ∗⋯∗ux ∗*~u_1*u_2*u_3~*\cdots*u_x~*∗ u 
1
​
 ∗u 
2
​
 ∗u 
3
​
  ∗⋯∗u 
x
​
  ∗，即字符串（可以为空）和星号交替出现，并且首尾字符均为星号，那么我们就可以设计出下面这个基于贪心的暴力匹配算法。算法的本质是：如果在字符串 sss 中首先找到 u1u_1u 
1
​
 ，再找到 u2,u3,⋯ ,uxu_2, u_3, \cdots, u_xu 
2
​
 ,u 
3
​
 ,⋯,u 
x
​
 ，那么 sss 就可以与模式 ppp 匹配，伪代码如下：

// 我们用 sIndex 和 pIndex 表示当前遍历到 s 和 p 的位置
// 此时我们正在 s 中寻找某个 u_i
// 其在 s 和 p 中的起始位置为 sRecord 和 pRecord

// sIndex 和 sRecord 的初始值为 0
// 即我们从字符串 s 的首位开始匹配
sIndex = sRecord = 0

// pIndex 和 pRecord 的初始值为 1
// 这是因为模式 p 的首位是星号，那么 u_1 的起始位置为 1
pIndex = pRecord = 1

while sIndex < s.length and pIndex < p.length do
    if p[pIndex] == '*' then
        // 如果遇到星号，说明找到了 u_i，开始寻找 u_i+1
        pIndex += 1
        // 记录下起始位置
        sRecord = sIndex
        pRecord = pIndex
    else if match(s[sIndex], p[pIndex]) then
        // 如果两个字符可以匹配，就继续寻找 u_i 的下一个字符
        sIndex += 1
        pIndex += 1
    else if sRecord + 1 < s.length then
        // 如果两个字符不匹配，那么需要重新寻找 u_i
        // 枚举下一个 s 中的起始位置
        sRecord += 1
        sIndex = sRecord
        pIndex = pRecord
    else
        // 如果不匹配并且下一个起始位置不存在，那么匹配失败
        return False
    end if
end while

// 由于 p 的最后一个字符是星号，那么 s 未匹配完，那么没有关系
// 但如果 p 没有匹配完，那么 p 剩余的字符必须都是星号
return all(p[pIndex] ~ p[p.length - 1] == '*')
然而模式 ppp 并不一定是 ∗ u1∗u2∗u3 ∗⋯∗ux ∗*~u_1*u_2*u_3~*\cdots*u_x~*∗ u 
1
​
 ∗u 
2
​
 ∗u 
3
​
  ∗⋯∗u 
x
​
  ∗ 的形式：

模式 ppp 的开头字符不是星号；

模式 ppp 的结尾字符不是星号。

第二种情况处理起来并不复杂。如果模式 ppp 的结尾字符不是星号，那么就必须与字符串 sss 的结尾字符匹配。那么我们不断地匹配 sss 和 ppp 的结尾字符，直到 ppp 为空或者 ppp 的结尾字符是星号为止。在这个过程中，如果匹配失败，或者最后 ppp 为空但 sss 不为空，那么需要返回 False\text{False}False。

第一种情况的处理也很类似，我们可以不断地匹配 sss 和 ppp 的开头字符。下面的代码中给出了另一种处理方法，即修改 sRecord\textit{sRecord}sRecord 和 tRecord\textit{tRecord}tRecord 的初始值为 −1-1−1，表示模式 ppp 的开头字符不是星号，并且在匹配失败时进行判断，如果它们的值仍然为 −1-1−1，说明没有「反悔」重新进行匹配的机会。

C++
Java
Python3
Golang
C
class Solution {
public:
    bool isMatch(string s, string p) {
        auto allStars = [](const string& str, int left, int right) {
            for (int i = left; i < right; ++i) {
                if (str[i] != '*') {
                    return false;
                }
            }
            return true;
        };
        auto charMatch = [](char u, char v) {
            return u == v || v == '?';
        };

        while (s.size() && p.size() && p.back() != '*') {
            if (charMatch(s.back(), p.back())) {
                s.pop_back();
                p.pop_back();
            }
            else {
                return false;
            }
        }
        if (p.empty()) {
            return s.empty();
        }

        int sIndex = 0, pIndex = 0;
        int sRecord = -1, pRecord = -1;
        while (sIndex < s.size() && pIndex < p.size()) {
            if (p[pIndex] == '*') {
                ++pIndex;
                sRecord = sIndex;
                pRecord = pIndex;
            }
            else if (charMatch(s[sIndex], p[pIndex])) {
                ++sIndex;
                ++pIndex;
            }
            else if (sRecord != -1 && sRecord + 1 < s.size()) {
                ++sRecord;
                sIndex = sRecord;
                pIndex = pRecord;
            }
            else {
                return false;
            }
        }
        return allStars(p, pIndex, p.size());
    }
};
复杂度分析

时间复杂度：

渐进复杂度：O(mn)O(mn)O(mn)，其中 mmm 和 nnn 分别是字符串 sss 和模式 ppp 的长度。从代码中可以看出，s[sIndex]s[\textit{sIndex}]s[sIndex] 和 p[pIndex]p[\textit{pIndex}]p[pIndex] 至多只会被匹配（判断）一次，因此渐进时间复杂度为 O(mn)O(mn)O(mn)。

最坏情况的例子：s=aaa⋯aaabs=\text{aaa}\cdots\text{aaab}s=aaa⋯aaab，p=∗ ab ∗p=*~\text{ab}~*p=∗ ab ∗。

平均复杂度：O(mlog⁡n)O(m \log n)O(mlogn)。直观上来看，如果 sss 和 ppp 是随机字符串，那么暴力算法的效率会非常高，因为大部分情况下只需要匹配常数次就可以成功匹配 uiu_iu 
i
​
  或者枚举下一个常数位置。具体的分析可以参考论文 On the Average-case Complexity of Pattern Matching with Wildcards https://leetcode.cn/link/?target=https%3A%2F%2Farxiv.org%2Fabs%2F1407.0950，注意论文中的分析是对于每一个 uiu_iu 
i
​
  而言的，即模式中只包含小写字母和问号，本题相当于多个连续模式的情况。由于超出了面试难度。这里不再赘述。

空间复杂度：O(1)O(1)O(1)。

结语
在方法二中，对于每一个被星号分隔的、只包含小写字符和问号的子模式 uiu_iu 
i
​
 ，我们在原串中使用的是暴力匹配的方法。然而这里是可以继续进行优化的，即使用 AC 自动机 代替暴力方法进行匹配。由于 AC 自动机本身已经是竞赛难度的知识点，而本题还需要在 AC 自动机中额外存储一些内容才能完成匹配，因此这种做法远远超过了面试难度。这里只给出参考讲义 Set Matching and Aho-Corasick Algorithm https://leetcode.cn/link/?target=http%3A%2F%2Fwww.cs.cmu.edu%2F~ab%2FCMU%2FWeek%2010-%20Strings%20Search%2Fprint04.pdf：

讲义的前 666 页介绍了字典树 Trie；

讲义的 7−197-197−19 页介绍了 AC 自动机，它是以字典树为基础的；

讲义的 20−2320-2320−23 页介绍了基于 AC 自动机的一种 wildcard matching 算法，其中的 wildcard ϕ\phiϕ 就是本题中的问号。

感兴趣的读者可以尝试进行学习。

作者：力扣官方题解
链接：https://leetcode.cn/problems/wildcard-matching/solutions/315802/tong-pei-fu-pi-pei-by-leetcode-solution/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

这不就是编译原理课上经典的正则表达式转epsilon-NFA，再化简epsilon-NFA转换成DFA，然后O(N)时间过一遍DFA的问题么？




正在学习贪心算法，这套题我个人理解的贪心思想如下，方便理解答案：

1. _尽量不做匹配，去尝试下一个pattern与当前字符串的匹配。企图让pattern抛除_的情况下全部匹配成功。（应用同一规则，将原问题变为一个相似的但规模更小的子问题）
    
2. 但上述规则不能做到局部最优，所以有了jstart等记录*位置，*仅用作尝试失败后的回滚重试，每次回归重试即贪心选择，所作的选择在当前步骤看为局部正确，得到一个求出可行解的一个解元素，进而反复尝试得出最优值。

代码很棒。*从匹配一个字符开始向多个进行扩充