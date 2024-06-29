> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091351437.png)

Implement `strStr()` .

Return the index of the **first** occurrence of needle in haystack, or  `-1` if needle is not part of haystack.

**Example 1:**
```clike
Input: haystack = "hello", needle = "ll"
Output: 2
```
**Example 2:**
```clike
Input: haystack = "aaaaa", needle = "bba"
Output: -1
```
**Clarification**:
- What should we return when `needle` is an **empty** string? This is a great question to ask during an interview.
- For the purpose of this problem, we will return `0` when needle is an empty string. This is consistent to C's `strstr()` and Java's `indexOf()`.

**Constraints**:
 -  `haystack` and `needle` consist only of lowercase English characters.

题意：实现C语言的 `strstr()` 函数。如果 `needle` 是 `haystack` 的子串，返回 `needle` 的第一次出现的位置。如果 `needle` 是空串，就返回 `0` ，这和 `strstr(), indexOf()` 的行为相一致。

---
### 解法1 API调用
用 `C++` 的 `find` 函数。代码：
```cpp
class Solution {
public:
    int strStr(string haystack, string needle) {
        int i = 0, j = 0, m = haystack.size(), n = needle.size();
        size_t pos = haystack.find(needle);
        return pos == string::npos ? -1 : pos; 
    }
};
```
效率：
```
执行用时：12 ms, 在所有 C++ 提交中击败了7.54% 的用户
内存消耗：7 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
## 题解2：Brute Force
手写 `BruteForce` 算法。

代码：
```cpp
class Solution {
public:
    int strStr(string haystack, string needle) { 
        int i = 0, j = 0, n = haystack.size(), m = needle.size();
	    while (i < n && j < m) {
            if (haystack[i] == needle[j]) {
                ++i;
                ++j;
            } else {
                i = i - j + 1;
                j = 0;
            }
        }
        return j >= m ? i - j : -1;
    }
};
```
效率：
```
执行用时：12 ms, 在所有 C++ 提交中击败了7.54% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
这两种做法都是 $\text{O(mn)}$ 的。

---
## 题解3：Rabin Karp算法(滚动哈希)
我们用一个小的 `trick` 就可以极大的提升算法的效率。计算 `needle` 的哈希值，然后滚动计算 `haystack` 中的子串的哈希值，如果相等，再进行字符串的比较，相同就返回下标位置。

代码：
```clike
class Solution {
public:
    int hash(const string &s, int b, int e) { 
        int code = 0;
        for (int i = b; i <= e; ++i) code += (s[i] - 'a');
        return code;
    }
    bool strCmp(const string &a, int i, const string &b) {
        int j = 0, len = b.size();
        while (j < len  && a[i++] == b[j++]);
        return j >= len;
    } 
    int strStr(string haystack, string needle) {
        int m = haystack.size(), n = needle.size();
        if (m < n) return -1;
        
        int scode = hash(haystack, 0, n - 1);
        int pcode = hash(needle, 0, n - 1);
        
        for (int i = 0; i <= m - n; ++i) { 
            if (scode == pcode && strCmp(haystack, i, needle)) return i;
            if (i < m - n) scode += (haystack[i + n] - haystack[i]);
        }
        return -1;
    }   
};
```
效率：
```
执行用时：4 ms, 在所有 C++ 提交中击败了79.26% 的用户
内存消耗：7 MB, 在所有 C++ 提交中击败了100.00% 的用户
```

---
## 题解4：Sunday算法
Sunday算法是一个很容易理解的字符串匹配算法：
- 首先原串 `s` 和子串 `t` 左端对齐，从头比较；
- 如果 `s[i] == t[j]` ，就比较下一个字符；
- 如果发现 `s[i] != t[j]` ，就检测原字符串中，将 `i` 回溯移动到下一位置后对应的子串的末尾位置的字符，设为 `c` ，看它是否出现在子串 `t` 中：
	- 出现了，我们就将 `i` 回溯移动到下一位置 `i = i - j + 1, j = 0` ；
	- 没有出现，就直接将 `i` 移动到字符 `c` 的下一个位置，跳过这段原串，同时 `j = 0` 。

代码：
```cpp
class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (n < m) return -1;
        bitset<30> bst;
        for (const char c : needle) bst.set(c - 'a');
        int i = 0, j = 0;
        while (i < n && j < m) {
            if (haystack[i] == needle[j]) {
                ++i; ++j;
                continue;
            }
            int nextPos = i - j + m;
            if (nextPos >= n) return -1;
            if (bst[haystack[nextPos] - 'a']) i = i - j + 1; 
            else i = nextPos + 1;
            j = 0;
        }
        return j >= m ? i - j : -1;
    }   
};
```
效率：
```
执行用时：4 ms, 在所有 C++ 提交中击败了79.26% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
总的来说，上面的两种算法虽然最坏复杂度仍然是 $\text{O(mn)}$ ，但是平均而言效率都有很大的提升，虽然不太稳定。



方法二：Knuth-Morris-Pratt\text{Knuth-Morris-Pratt}Knuth-Morris-Pratt 算法
思路及算法

Knuth-Morris-Pratt\text{Knuth-Morris-Pratt}Knuth-Morris-Pratt 算法，简称 KMP\text{KMP}KMP 算法，由 Donald Knuth\text{Donald Knuth}Donald Knuth、James H. Morris\text{James H. Morris}James H. Morris 和 Vaughan Pratt\text{Vaughan Pratt}Vaughan Pratt 三人于 197719771977 年联合发表。

Knuth-Morris-Pratt\text{Knuth-Morris-Pratt}Knuth-Morris-Pratt 算法的核心为前缀函数，记作 π(i)\pi(i)π(i)，其定义如下：

对于长度为 mmm 的字符串 sss，其前缀函数 π(i)(0≤i<m)\pi(i)(0 \leq i < m)π(i)(0≤i<m) 表示 sss 的子串 s[0:i]s[0:i]s[0:i] 的最长的相等的真前缀与真后缀的长度。特别地，如果不存在符合条件的前后缀，那么 π(i)=0\pi(i) = 0π(i)=0。其中真前缀与真后缀的定义为不等于自身的的前缀与后缀。

我们举个例子说明：字符串 aabaaabaabaaabaabaaab 的前缀函数值依次为 0,1,0,1,2,2,30,1,0,1,2,2,30,1,0,1,2,2,3。

π(0)=0\pi(0) = 0π(0)=0，因为 aaa 没有真前缀和真后缀，根据规定为 000（可以发现对于任意字符串 π(0)=0\pi(0)=0π(0)=0 必定成立）；

π(1)=1\pi(1) = 1π(1)=1，因为 aaaaaa 最长的一对相等的真前后缀为 aaa，长度为 111；

π(2)=0\pi(2) = 0π(2)=0，因为 aabaabaab 没有对应真前缀和真后缀，根据规定为 000；

π(3)=1\pi(3) = 1π(3)=1，因为 aabaaabaaaba 最长的一对相等的真前后缀为 aaa，长度为 111；

π(4)=2\pi(4) = 2π(4)=2，因为 aabaaaabaaaabaa 最长的一对相等的真前后缀为 aaaaaa，长度为 222；

π(5)=2\pi(5) = 2π(5)=2，因为 aabaaaaabaaaaabaaa 最长的一对相等的真前后缀为 aaaaaa，长度为 222；

π(6)=3\pi(6) = 3π(6)=3，因为 aabaaabaabaaabaabaaab 最长的一对相等的真前后缀为 aabaabaab，长度为 333。

有了前缀函数，我们就可以快速地计算出模式串在主串中的每一次出现。

如何求解前缀函数

长度为 mmm 的字符串 sss 的所有前缀函数的求解算法的总时间复杂度是严格 O(m)O(m)O(m) 的，且该求解算法是增量算法，即我们可以一边读入字符串，一边求解当前读入位的前缀函数。

为了叙述方便，我们接下来将说明几个前缀函数的性质：

π(i)≤π(i−1)+1\pi(i) \leq \pi(i-1) + 1π(i)≤π(i−1)+1。
依据 π(i)\pi(i)π(i) 定义得：s[0:π(i)−1]=s[i−π(i)+1:i]s[0:\pi(i)-1]=s[i-\pi(i)+1:i]s[0:π(i)−1]=s[i−π(i)+1:i]。
将两区间的右端点同时左移，可得：s[0:π(i)−2]=s[i−π(i)+1:i−1]s[0:\pi(i)-2] = s[i-\pi(i)+1:i-1]s[0:π(i)−2]=s[i−π(i)+1:i−1]。
依据 π(i−1)\pi(i-1)π(i−1) 定义得：π(i−1)≥π(i)−1\pi(i-1) \geq \pi(i) - 1π(i−1)≥π(i)−1，即 π(i)≤π(i−1)+1\pi(i) \leq \pi(i-1) + 1π(i)≤π(i−1)+1。
如果 s[i]=s[π(i−1)]s[i]=s[\pi(i-1)]s[i]=s[π(i−1)]，那么 π(i)=π(i−1)+1\pi(i)=\pi(i-1)+1π(i)=π(i−1)+1。
依据 π(i−1)\pi(i-1)π(i−1) 定义得：s[0:π(i−1)−1]=s[i−π(i−1):i−1]s[0:\pi(i-1)-1]=s[i-\pi(i-1):i-1]s[0:π(i−1)−1]=s[i−π(i−1):i−1]。
因为 s[π(i−1)]=s[i]s[\pi(i-1)]=s[i]s[π(i−1)]=s[i]，可得 s[0:π(i−1)]=s[i−π(i−1):i]s[0:\pi(i-1)]=s[i-\pi(i-1):i]s[0:π(i−1)]=s[i−π(i−1):i]。
依据 π(i)\pi(i)π(i) 定义得：π(i)≥π(i−1)+1\pi(i)\geq\pi(i-1)+1π(i)≥π(i−1)+1，结合第一个性质可得 π(i)=π(i−1)+1\pi(i)=\pi(i-1)+1π(i)=π(i−1)+1。
这样我们可以依据这两个性质提出求解 π(i)\pi(i)π(i) 的方案：找到最大的 jjj，满足 s[0:j−1]=s[i−j:i−1]s[0:j-1]=s[i-j:i-1]s[0:j−1]=s[i−j:i−1]，且 s[i]=s[j]s[i]=s[j]s[i]=s[j]（这样就有 s[0:j]=s[i−j:i]s[0:j]=s[i-j:i]s[0:j]=s[i−j:i]，即 π(i)=j+1\pi(i)=j+1π(i)=j+1）。

注意这里提出了两个要求：

jjj 要求尽可能大，且满足 s[0:j−1]=s[i−j:i−1]s[0:j-1]=s[i-j:i-1]s[0:j−1]=s[i−j:i−1]；
jjj 要求满足 s[i]=s[j]s[i]=s[j]s[i]=s[j]。
由 π(i−1)\pi(i-1)π(i−1) 定义可知：

s[0:π(i−1)−1]=s[i−π(i−1):i−1](1) s[0:\pi(i-1)-1]=s[i-\pi(i-1):i-1] \tag{1}
s[0:π(i−1)−1]=s[i−π(i−1):i−1](1)
那么 j=π(i−1)j = \pi(i-1)j=π(i−1) 符合第一个要求。如果 s[i]=s[π(i−1)]s[i]=s[\pi(i-1)]s[i]=s[π(i−1)]，我们就可以确定 π(i)\pi(i)π(i)。

否则如果 s[i]≠s[π(i−1)]s[i]\neq s[\pi(i-1)]s[i]

=s[π(i−1)]，那么 π(i)≤π(i−1)\pi(i) \leq \pi(i-1)π(i)≤π(i−1)，因为 j=π(i)−1j=\pi(i)-1j=π(i)−1，所以 j < π(i−1)j < \pi(i-1)j < π(i−1)，于是可以取 (1)(1)(1) 式两子串的长度为 jjj 的后缀，它们依然是相等的：s[π(i−1)−j:π(i−1)−1]=s[i−j:i−1]s[\pi(i-1)-j:\pi(i-1)-1]=s[i-j:i-1]s[π(i−1)−j:π(i−1)−1]=s[i−j:i−1]。

当 s[i]≠s[π(i−1)]s[i]\neq s[\pi(i-1)]s[i]

=s[π(i−1)] 时，我们可以修改我们的方案为：找到最大的 jjj，满足 s[0:j−1]=s[π(i−1)−j:π(i−1)−1]s[0:j-1]=s[\pi(i-1)-j:\pi(i-1)-1]s[0:j−1]=s[π(i−1)−j:π(i−1)−1]，且 s[i]=s[π(i−1)]s[i]=s[\pi(i-1)]s[i]=s[π(i−1)]（这样就有 s[0:j]=s[π(i−1)−j:π(i−1)]s[0:j]=s[\pi(i-1)-j:\pi(i-1)]s[0:j]=s[π(i−1)−j:π(i−1)]，即 π(i)=π(i−1)+1\pi(i)=\pi(i-1)+1π(i)=π(i−1)+1）。

注意这里提出了两个要求：

jjj 要求尽可能大，且满足 s[0:j−1]=s[π(i−1)−j:π(i−1)−1]s[0:j-1]=s[\pi(i-1)-j:\pi(i-1)-1]s[0:j−1]=s[π(i−1)−j:π(i−1)−1]；
jjj 要求满足 s[i]=s[j]s[i]=s[j]s[i]=s[j]。
由 π(π(i−1)−1)\pi(\pi(i-1)-1)π(π(i−1)−1) 定义可知 j=π(π(i−1)−1)j = \pi(\pi(i-1)-1)j=π(π(i−1)−1) 符合第一个要求。如果 s[i]=s[π(π(i−1)−1)]s[i]=s[\pi(\pi(i-1)-1)]s[i]=s[π(π(i−1)−1)]，我们就可以确定 π(i)\pi(i)π(i)。

此时，我们可以发现 jjj 的取值总是被描述为 π(π(π(…)−1)−1)\pi(\pi(\pi(\ldots)-1)-1)π(π(π(…)−1)−1) 的结构（初始为 π(i−1)\pi(i-1)π(i−1)）。于是我们可以描述我们的算法：设定 π(i)=j+1\pi(i)=j+1π(i)=j+1，jjj 的初始值为 π(i−1)\pi(i-1)π(i−1)。我们只需要不断迭代 jjj（令 jjj 变为 π(j−1)\pi(j-1)π(j−1)）直到 s[i]=s[j]s[i]=s[j]s[i]=s[j] 或 j=0j=0j=0 即可，如果最终匹配成功（找到了 jjj 使得 s[i]=s[j]s[i]=s[j]s[i]=s[j]），那么 π(i)=j+1\pi(i)=j+1π(i)=j+1，否则 π(i)=0\pi(i)=0π(i)=0。

复杂度证明

时间复杂度部分，注意到 π(i)≤π(i−1)+1\pi(i)\leq \pi(i-1)+1π(i)≤π(i−1)+1，即每次当前位的前缀函数至多比前一位增加一，每当我们迭代一次，当前位的前缀函数的最大值都会减少。可以发现前缀函数的总减少次数不会超过总增加次数，而总增加次数不会超过 mmm 次，因此总减少次数也不会超过 mmm 次，即总迭代次数不会超过 mmm 次。

空间复杂度部分，我们只用到了长度为 mmm 的数组保存前缀函数，以及使用了常数的空间保存了若干变量。

如何解决本题

记字符串 haystack\textit{haystack}haystack 的长度为 nnn，字符串 needle\textit{needle}needle 的长度为 mmm。

我们记字符串 str=needle+#+haystack\textit{str} = \textit{needle} + \# + \textit{haystack}str=needle+#+haystack，即将字符串 needle\textit{needle}needle 和 haystack\textit{haystack}haystack 进行拼接，并用不存在于两串中的特殊字符 #\## 将两串隔开，然后我们对字符串 str\textit{str}str 求前缀函数。

因为特殊字符 #\## 的存在，字符串 str\textit{str}str 中 haystack\textit{haystack}haystack 部分的前缀函数所对应的真前缀必定落在字符串 needle\textit{needle}needle 部分，真后缀必定落在字符串 haystack\textit{haystack}haystack 部分。当 haystack\textit{haystack}haystack 部分的前缀函数值为 mmm 时，我们就找到了一次字符串 needle\textit{needle}needle 在字符串 haystack\textit{haystack}haystack 中的出现（因为此时真前缀恰为字符串 needle\textit{needle}needle）。

实现时，我们可以进行一定的优化，包括：

我们无需显式地创建字符串 str\textit{str}str。
为了节约空间，我们只需要顺次遍历字符串 needle\textit{needle}needle、特殊字符 #\## 和字符串 haystack\textit{haystack}haystack 即可。
也无需显式地保存所有前缀函数的结果，而只需要保存字符串 needle\textit{needle}needle 部分的前缀函数即可。
特殊字符 #\## 的前缀函数必定为 000，且易知 π(i)≤m\pi(i) \leq mπ(i)≤m（真前缀不可能包含特殊字符 #\##）。
这样我们计算 π(i)\pi(i)π(i) 时，j=π(π(π(…)−1)−1)j=\pi(\pi(\pi(\ldots)-1)-1)j=π(π(π(…)−1)−1) 的所有的取值中仅有 π(i−1)\pi(i-1)π(i−1) 的下标可能大于等于 mmm。我们只需要保存前一个位置的前缀函数，其它的 jjj 的取值将全部为字符串 needle\textit{needle}needle 部分的前缀函数。
我们也无需特别处理特殊字符 #\##，只需要注意处理字符串 haystack\textit{haystack}haystack 的第一个位置对应的前缀函数时，直接设定 jjj 的初值为 000 即可。
这样我们可以将代码实现分为两部分：

第一部分是求 needle\textit{needle}needle 部分的前缀函数，我们需要保留这部分的前缀函数值。
第二部分是求 haystack\textit{haystack}haystack 部分的前缀函数，我们无需保留这部分的前缀函数值，只需要用一个变量记录上一个位置的前缀函数值即可。当某个位置的前缀函数值等于 mmm 时，说明我们就找到了一次字符串 needle\textit{needle}needle 在字符串 haystack\textit{haystack}haystack 中的出现（因为此时真前缀恰为字符串 needle\textit{needle}needle，真后缀为以当前位置为结束位置的字符串 haystack\textit{haystack}haystack 的子串），我们计算出起始位置，将其返回即可。
代码

C++
Java
JavaScript
Golang
C
class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (m == 0) {
            return 0;
        }
        vector<int> pi(m);
        for (int i = 1, j = 0; i < m; i++) {
            while (j > 0 && needle[i] != needle[j]) {
                j = pi[j - 1];
            }
            if (needle[i] == needle[j]) {
                j++;
            }
            pi[i] = j;
        }
        for (int i = 0, j = 0; i < n; i++) {
            while (j > 0 && haystack[i] != needle[j]) {
                j = pi[j - 1];
            }
            if (haystack[i] == needle[j]) {
                j++;
            }
            if (j == m) {
                return i - m + 1;
            }
        }
        return -1;
    }
};
复杂度分析

时间复杂度：O(n+m)O(n+m)O(n+m)，其中 nnn 是字符串 haystack\textit{haystack}haystack 的长度，mmm 是字符串 needle\textit{needle}needle 的长度。我们至多需要遍历两字符串一次。

空间复杂度：O(m)O(m)O(m)，其中 mmm 是字符串 needle\textit{needle}needle 的长度。我们只需要保存字符串 needle\textit{needle}needle 的前缀函数。

```cpp
class Solution {
private: 
    vector<int> next;
    vector<int> getNext(const string& p) { 
        next[0] = -1;
        for (int i = -1, j = 0; j < p.size(); ) {
            if (i == -1 || p[j] == p[i]) next[++j] = ++i;
            else i = next[i];
        }
        return next;
    }
public:
    int strStr(string haystack, string needle) {
        int m = haystack.size(), n = needle.size();
        next.resize(n + 1);
        getNext(needle);
        int i = 0, j = 0;
        while (i < m && j < n) {
            if (j == -1 || haystack[i] == needle[j]) ++i, ++j;
            else j = next[j];
        }
        return j >= n ? i - j : -1;
    }
};
```