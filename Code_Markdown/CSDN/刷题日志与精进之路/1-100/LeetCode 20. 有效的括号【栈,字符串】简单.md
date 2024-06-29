> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091346731.png)

Given a string containing just the characters `'('`, `')'`, `'{'`, `'}'`, `'['` and `']'`, determine if the input string is valid.

An input string is valid if:
- Open brackets must be closed by the same type of brackets.
- Open brackets must be closed in **the correct order**.

Note that **an empty string** is also considered valid.

**Example 1:**
```cpp
Input: "()"
Output: true
```
**Example 2:**
```cpp
Input: "()[]{}"
Output: true
```
**Example 3:**
```cpp
Input: "(]"
Output: false
```
**Example 4:**
```cpp
Input: "([)]"
Output: false
```
**Example 5:**
```cpp
Input: "{[]}"
Output: true
```
题意：判断给出的字符串是否是有效的括号匹配。其中，**有效**的定义是：开括号必须由同样类型的闭括号闭合；开括号必须以正确的顺序闭合。

---
### 解法 栈
判断括号的有效性可以使用「栈」这一数据结构来解决。

我们遍历给定的字符串 $s$ 。当我们遇到一个左括号时，我们会期望在后续的遍历中，有一个相同类型的右括号将其闭合。**由于后遇到的左括号要先闭合，因此我们可以将这个左括号放入栈顶**。

==当我们遇到一个右括号时，我们需要将一个相同类型的左括号闭合==。此时，我们可以取出栈顶的左括号并判断它们是否是相同类型的括号。如果不是相同的类型，或者栈中并没有左括号，那么字符串 $s$ 无效，返回 $\text{False}$ 。为了快速判断括号的类型，我们可以使用哈希表存储每一种括号，==哈希表的键为右括号，值为相同类型的左括号==。

在遍历结束后，如果栈中没有左括号（即栈为空），说明我们将字符串 $s$ 中的所有左括号闭合，返回 $\text{True}$ ，否则返回 $\text{False}$ 。
```cpp
class Solution {
public:
    bool isValid(string s) {
        unordered_map<char, char> m;
        m[')'] = '(', m[']'] = '[', m['}'] = '{';
        stack<char> st;
        int size = s.size();
        for (int i = 0; i < size; ++i) {
            if (s[i] == '(' || s[i] == '[' || s[i] == '{') st.push(s[i]);
            else {
                if (st.empty() || st.top() != m[s[i]]) return false;
                else st.pop();
            }
        }
        return st.empty();
    }
};
```
---
我们注意到，**有效字符串的长度一定为偶数**，如果字符串的长度为奇数，可以直接返回 `false` ，省去后续的遍历。另外，查询一下 `ASCII` 码的差值，发现不需要用哈希表，也不用敲各种括号。
![](https://img-blog.csdnimg.cn/20200815002019382.png#pic_center)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815002024424.png#pic_center)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200815002028370.png#pic_center)

优化的代码如下：
```cpp
class Solution {
public:
    bool isValid(string s) {
        if (s.empty()) return true;
        if (s.size() & 1) return false; // 奇数长度优化
        stack<char> st;
        for (char c : s) {
            if (!st.empty() && (c - st.top() == 1 || c - st.top() == 2)) st.pop(); // (+1=),{+2=},[+2=]
            else st.push(c);
        }
        return st.empty();
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是字符串 $s$ 的长度。
- 空间复杂度：$O(n + |\Sigma|)$ ，其中 $\Sigma$ 表示字符集，本题中字符串只包含 $6$ 种括号，$\Sigma| = 6$ 。栈中的字符数量为 $O(n)$ ，而哈希表使用的空间为 $O(|\Sigma|)$ ，相加即可得到总空间复杂度。
 