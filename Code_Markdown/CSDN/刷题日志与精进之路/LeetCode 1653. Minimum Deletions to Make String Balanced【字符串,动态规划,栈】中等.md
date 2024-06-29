> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a string `s` consisting only of characters `'a'` and `'b'`​​​​.

You can delete any number of characters in `s` to make `s` **balanced**. `s` is **balanced** if there is no pair of indices `(i,j)` such that `i < j` and `s[i] = 'b'` and `s[j]= 'a'`.

Return _the **minimum** number of deletions needed to make_ `s` _**balanced**_.

**Example 1:**
```java
Input: s = "aababbab"
Output: 2
Explanation: You can either:
Delete the characters at 0-indexed positions 2 and 6 ("aababbab" -> "aaabbb"), or
Delete the characters at 0-indexed positions 3 and 6 ("aababbab" -> "aabbbb").
```
**Example 2:**
```java
Input: s = "bbaaaaabb"
Output: 2
Explanation: The only solution is to delete the first two characters.
```
**Constraints:**
-   `1 <= s.length <= 105`
-   `s[i]` is `'a'` or `'b'`​​.

题意：给出一个仅由 `'a', 'b'` 组成的字符串 `s` ，如果任意下标对 `(i, j), i < j` ，都不存在 `s[i] == 'b' and s[j] == 'a'` ，就称 `s` 是**平衡的**。你可以删除 `s` 中任意数量的字符，使 `s` 平衡。返回最少的使 `s` 平衡的删除数。

---
### 解法1 枚举分割线（两次遍历）
完全不需要过多的思考。由于平衡字符串的 `'a'` 都在 `'b'` 的左边，因此一定存在一条分割线，将 `s` 分成前后两部分，`'a'` 都在前面一部分，`'b'` 都在后面一部分。反过来说，要删除的 `'b'` 都在前一部分，要删除的 `'a'` 都在后一部分。我们要做的就是，**枚举所有 $n +1$ 条分割线**，计算删除次数的**最小值**，就是答案。

最简单的实现如下所示。**第一次遍历计算 `'a'` 的总个数 $a$** ，并将其作为「以下标 $0$ 之前作为分割线、要删除全部 `'a'` 」的删除次数 $minDel$ ；第二次遍历时，当前字符如果是 `'a'` 就令 `ta` 加1，并以下标 $i$ 之后作为分割线，然后计算出前一部分中 `'b'` 的个数 $i + 1 - ta$ 、和后一部分中 `'a'` 的个数 $a - ta$ ，这二者之和与 $minDel$ 比较求最小值：
```java
class Solution {
    public int minimumDeletions(String s) {
        int a = 0, ta = 0, n = s.length();
        for (int i = 0; i < n; ++i)
            if (s.charAt(i) == 'a') ++a;
        int minDel = a; // 以下标0之前作为a,b分割线,要删除全部a
        for (int i = 0; i < n; ++i) { // 以下标i之后作为分割线
            if (s.charAt(i) == 'a') ++ta;
            int bComeBeforeA = i + 1 - ta;
            // ta bComeBeforeA | a-ta n-a-bComeBeforeA
            // 删除前面的bComeBeforeA个b和后面的a-ta个a
            minDel = Math.min(minDel, bComeBeforeA + a - ta);
        }
        return minDel;
    }
}
```
上面的写法简单易懂，但我们还可稍微优化一下。
- 去掉if语句，如第一次遍历中改为 `a += 'b' - s.charAt(i)` ；第二次遍历中改为 `(s.charAt(i) - 'a') * 2 - 1` 。从而避免因条件跳转指令而出现的CPU分支预测。
- 第一次遍历仍然是统计 $s$ 中 `'a'` 的个数，并将其作为 $del$ 。只是第二次遍历时，下标 $i$ 变大的过程就是分割线不断右移的过程，**我们每遇到一个 `'a'` ，就相当于将一个 `'a'` 纳入前一部分，从而删除 `'a'` 的次数减1**；每遇到一个 `'b'` ，就相当于要删除的 `'b'` 的次数加1。这样不断减少或增加删除次数，并用 $ans$ 统计所有 $del$ 中的最小值。示例图（借用[灵茶山艾府](https://leetcode.cn/u/endlesscheng/)大佬画的图）如下：
	![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081447322.png)

```java
class Solution {
    public int minimumDeletions(String s) {
        int del = 0, n = s.length();
        for (int i = 0; i < n; ++i)
            del += ('b' - s.charAt(i));
        int ans = del;
        for (int i = 0; i < n; ++i) { // 以下标i之后作为分割线
            del += (s.charAt(i) - 'a') * 2 - 1; // 'a':-1,'b':1
            ans = Math.min(del, ans);
        }
        return ans;
    }
}
```
---
### 解法2 动态规划（1次遍历）
令 $f[i]$ 为使 `s[0...i]` 平衡的最少删除次数，考虑 `s[0...i]` 的最后一个字母：
- 如果它是 `'b'` ，则无需删除，问题规模缩小，变为「使 `s[0...i-1]` 平衡的最少删除次数」即 $f[i] = f[i-1]$ ；
- 如果它是 `'a'` ：
	- 删除它，则答案为「使 `s[0...i-1]` 平衡的最少删除次数」加上1，即 $f[i - 1] + 1$ 。
	- 保留它，则前面所有的 `'b'` 都要删除。
	- 令 `bComeBeforeA` 为 `s[0...i]` 中 `'b'` 的个数，从而 `f[i] = min(f[i - 1] + 1, bComeBeforeA)` 。

代码实现时，可以只用一个变量表示 $f$ 。
```java
class Solution {
    public int minimumDeletions(String s) {
        int bComeBeforeA = 0, minDel = 0;
        for (int i = 0, n = s.length(); i < n; ++i) {
            char c = s.charAt(i);
            // c前面的字符串已经平衡,如果前面存在'b',则最后的'a'会失衡
            if (c == 'a') {
                // 要么删除前面的'b',要么删除这里的'a'
                // 看哪种做法删除数最少
                minDel = Math.min(bComeBeforeA, minDel + 1);
            } else ++bComeBeforeA; // 最后一个字符是'b',不会导致失衡
        }
        return minDel;
    }
}
```
