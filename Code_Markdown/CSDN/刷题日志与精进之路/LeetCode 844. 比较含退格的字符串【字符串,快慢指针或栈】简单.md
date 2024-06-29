> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定 `s` 和 `t` 两个字符串，当它们分别被输入到空白的文本编辑器后，如果两者相等，返回 `true` 。`#` 代表退格字符。

**注意：** 如果对空文本输入退格字符，文本继续为空。

**示例 1：**
```js
输入：s = "ab#c", t = "ad#c"
输出：true
解释：s 和 t 都会变成 "ac"。
```
**示例 2：**
```js
输入：s = "ab##", t = "c#d#"
输出：true
解释：s 和 t 都会变成 ""。
```
**示例 3：**
```js
输入：s = "a#c", t = "b"
输出：false
解释：s 会变成 "c"，但 t 仍然是 "b"。
```
**提示：**
- `1 <= s.length, t.length <= 200`
- `s` 和 `t` 只含有小写字母以及字符 `'#'`

**进阶：**
- 你可以用 `O(n)` 的时间复杂度和 `O(1)` 的空间复杂度解决该问题吗？

---
### 解法1 暴力方法
如果没有其它思路的话，可能想到的就是暴力。对原始字符串，如果一个字符是 `#` ，就将它前一个字符（如果存在的话）和 `#` 一起删去，然后检查下一个字符。最终整理得到真正的文本串，进行比较。在原始字符串之上进行这些操作，时间复杂度是 $O(n^2)$ 级别，意义不大，因而此处不讨论具体代码的实现。

---
### 解法2 额外的字符串空间
如果使用额外的字符串空间，可以将暴力方法的复杂度降低到 $O(n)$ 时间、$O(n)$ 空间，具体代码很简单：
```cpp
class Solution { 
public:
    bool backspaceCompare(string s, string t) {
        string st, tt;
        for (const char &c : s) {
            if (c != '#') st.push_back(c);
            else if (!st.empty()) st.pop_back();
        }
        for (const char &c : t) {
            if (c != '#') tt.push_back(c);
            else if (!tt.empty()) tt.pop_back();
        }  
        return st == tt;
    }
};
```
运行结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了41.17% 的用户
```
不过这种**后进先出**的删除方法让人想到了栈，因此另一种写法如下：
```cpp
class Solution {
private: 
    string convertToNormalText(const string &s) {
        string textStack;
        for (const char &c : s) {
            if (c != '#') textStack.push_back(c);
            else if (!textStack.empty()) textStack.pop_back();
        }
        return textStack;
    }
public:
    bool backspaceCompare(string s, string t) {
        return convertToNormalText(s) == convertToNormalText(t);
    }
};
```
效率几乎一样：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了41.17% 的用户
```
---
### 解法3 双指针
使用双指针的方法，可以将空间复杂度降低到 $O(1)$ 。对于两个字符串，如果从前往后比较，需要考虑到 `#` 的影响，比较麻烦。因此使用从后往前比较的做法。即使这样也要注意，**这题有很多细节要仔细考虑，不然很容易出错**。具体步骤如下：
- 我们维护两个计数器 `backOfS, backOfT` 和两个指针 `sright, tright` ，**计数器记录退格的个数**，指针分别指向两个字符串的尾部。然后倒序遍历字符串，决定当前字符是否需要跳过，从而**将两个指针分别指向对应字符串的有效字符**； 
	- 遇到 `#` 时，将对应的计数器 `+ 1` ，遍历指针往前移一位从而跳过该字符；
	- 遇到非 `#` 字符时，会有如下的判断：
	     -   **如果退格计数器为 `0` ，那么该字符无法跳过，属于有效字符**；
	     -  如果退格计数器 `>= 0` ，那么该字符需要跳过，所以需要让计数器 `- 1` ，同时让遍历指针往前移一位；
- **经过这一过程，此时两个指针都指向有效字符，然后进行比较**。相等则重复遍历过程；如果发现两个位置上的有效字符并不相同，或者其中一个字符串已经遍历完，那么直接返回 `false` ，否则继续往前遍历剩下的字符；
- 最后，如果两个字符串都已经遍历完，可知两者经过退格操作后是相等的字符串，返回 `true` ；时间复杂度是 $O(N)$ ，空间复杂度是 $O(1)$ 。

```cpp
class Solution {
private: //跳跃过退格和无效字符,到下一个有效字符 
    void jumpBackspace(const string &s, int &i, int &b) {
        while (i >= 0) {
            if (s[i] == '#') ++b; //计算退格
            else if (b) --b;      //不是退格但存在退格,因此不是有效字符
            else break;           //不是退格,不存在退格,是有效字符,返回
            --i;                  //是退格并计算;不是退格但存在退格---->都不是有效字符
        }
    }
public:
    bool backspaceCompare(string s, string t) { //双指针,从后往前扫描
        int sright = s.size() - 1, tright = t.size() - 1;
        int backOfS = 0, backOfT = 0;
        while (sright >= 0 || tright >= 0) {
            jumpBackspace(s, sright, backOfS);
            jumpBackspace(t, tright, backOfT);
            if (sright < 0 || tright < 0) break;          //判断是否超出范围
            if (s[sright--] != t[tright--]) return false; //有效字符比较,不相等则返回false
        }
        return sright < 0 && tright < 0;
    }
};
```
效率如下，虽然好像更低了，但是对于字符串 `s, t` 十分庞大、内存无法装入的情况而言，更有用处：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了51.14% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了70.90% 的用户
```
当然还有种最简单的写法，**使用快慢指针**，一个读、一个原地覆盖写。最终原地会有一段长度有效的字符，比较有效长度字符串的内容是否相同：
```cpp
class Solution {
public:
    bool backspaceCompare(string s, string t) {
        int k = 0;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] != '#') s[k++] = s[i];
            else if (k > 0) k--;
        }
        int d = 0;
        for (int i = 0; i < t.size(); ++i) {
            if (t[i] != '#') t[d++] = t[i];
            else if (d > 0) d--;
        }
        return s.compare(0, k, t, 0, d) == 0;
    }
};
```