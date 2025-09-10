> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given two strings `s1` and `s2` of equal length consisting of letters `"x"` and `"y"` **only**. Your task is to make these two strings equal to each other. You can swap any two characters that belong to **different** strings, which means: swap `s1[i]` and `s2[j]`.

Return the minimum number of swaps required to make `s1` and `s2` equal, or return `-1` if it is impossible to do so.

**Example 1:**
```java
Input: s1 = "xx", s2 = "yy"
Output: 1
Explanation: Swap s1[0] and s2[1], s1 = "yx", s2 = "yx".
```

**Example 2:**
```java
Input: s1 = "xy", s2 = "yx"
Output: 2
Explanation: Swap s1[0] and s2[0], s1 = "yy", s2 = "xx".
Swap s1[0] and s2[1], s1 = "xy", s2 = "xy".
Note that you cannot swap s1[0] and s1[1] to make s1 equal to "yx", cause we can only swap chars in different strings.
```
**Example 3:**
```java
Input: s1 = "xx", s2 = "xy"
Output: -1
```
**Constraints:**
-   `1 <= s1.length, s2.length <= 1000`
-   `s1, s2` only contain `'x'` or `'y'`.

题意：有两个长度相同的字符串 `s1` 和 `s2`，且它们其中 **只含有** 字符 `"x"` 和 `"y"`，需要通过「交换字符」的方式使这两个字符串相同。每次「交换字符」的时候，你都可以在两个字符串中**各选一个字符**进行交换。交换只能发生在两个不同的字符串之间，绝对不能发生在同一个字符串内部。最后，返回使 `s1` 和 `s2` 相同的最小交换次数，如果没有方法能使这两个字符串相同，则返回 `-1` 。

---
### 解法 贪心
首先，**忽略所有已经匹配的位置，它们根本不会影响答案**。对于不匹配的位置，有三种基本情况（示例中已给出）：
```java
"xx", "yy" => 1次互换

"xy", "xy" => 2次互换 
"yx", "yx" => 2次互换 
"xy", "yx" => 2次互换 
"yx", "xy" => 2次互换 
```
因此，贪心策略是，尽可能应用情况1、它需要的交换次数最少；如果最后剩下两个字符与另外两个不匹配，则应用情况2；如果只剩下一个字符与另一个字符不匹配，就不可能实现题意了（字符串 `s1, s2` 之间如果存在奇数个位置不同，就无法使它们相同）。具体代码如下：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$
```cpp
class Solution {
public:
    int minimumSwap(string s1, string s2) {
        int s1XCnt = 0, s1YCnt = 0;
        for (int i = 0, n = s1.size(); i < n; ++i) {
            if (s1[i] != s2[i]) {
                if (s1[i] == 'x') ++s1XCnt;
                else ++s1YCnt;
            }
        }
        // s1XCnt = s2YCnt, s1YCnt = s2XCnt
        // 忽略匹配的字符;跳过不是同个字符的位置来观察(如将不同位置的x连着看,对面是连着的y)
        // 尽量用xx与对面同样位置yy(或者反过来这边用yy那边用xx)交换,这样交换次数最少,次数为s1XCnt/2+s1YCnt/2
        // 最后可能剩下一对xy与yx,要两次交换才行
        // 也可能都没剩下,之间返回
        // 如果最后只剩下一个x与一个y,则无解
        if (s1XCnt % 2 + s1YCnt % 2 == 1) return -1; // 这边只剩下一个x或y
        int ans = s1XCnt / 2 + s1YCnt / 2;
        return s1XCnt % 2 == 1 ? ans + 2 : ans;
    }
};
```