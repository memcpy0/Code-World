> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091745078.png)

<p>Given a string <code>s</code> consisting&nbsp;of some words separated by some number of spaces, return <em>the length of the <strong>last</strong> word in the string.</em></p>

<p>A <strong>word</strong> is a maximal substring consisting of non-space characters only.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: s = "Hello World"
Output: 5
Explanation: The last word is "World" with length 5.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: s = "   fly me   to   the moon  "
Output: 4
Explanation: The last word is "moon" with length 4.
```

<p><strong>Example 3:</strong></p>

```cpp
Input: s = "luffy is still joyboy"
Output: 6
Explanation: The last word is "joyboy" with length 6.
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 10<sup>4</sup></code></li>
	<li><code>s</code> consists of only English letters and spaces <code>' '</code>.</li>
	<li>There will be at least one word in <code>s</code>.</li>
</ul>

题意：给你一个字符串 `s` ，由若干单词组成，单词前后用一些空格字符隔开。返回字符串中最后一个单词的长度。

---
### 解法 模拟
很简单的题目。 我们根据题意，「从后往前」对字符串进行遍历。具体来说：
- 使用指针 `j` 指向字符串末尾，`j` 不断前移跳过空格，直到指向目标单词的最后一个字符；
- 之后 `j` 不断前移，直至遇到空格或者到达字符串首位，同时使用 `len` 计数；
- 返回 `len` 作为函数结果。

实际代码如下所示。算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int lengthOfLastWord(string s) {
        int j = s.size() - 1, len = 0;
        while (j >= 0 && s[j] == ' ') --j; //跳过空格
        while (j >= 0 && s[j] != ' ') ++len, --j; //计算最后一个单词长度
        return len;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.3 MB, 在所有 C++ 提交中击败了97.48% 的用户
```
