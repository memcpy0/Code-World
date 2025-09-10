> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a **0-indexed** string `blocks` of length `n`, where `blocks[i]` is either `'W'` or `'B'`, representing the color of the `ith` block. The characters `'W'` and `'B'` denote the colors white and black, respectively.

You are also given an integer `k`, which is the desired number of **consecutive** black blocks.

In one operation, you can **recolor** a white block such that it becomes a black block.

Return _the **minimum** number of operations needed such that there is at least **one** occurrence of_ `k` _consecutive black blocks._

**Example 1:**
```java
Input: blocks = "WBBWWBBWBW", k = 7
Output: 3
Explanation:
One way to achieve 7 consecutive black blocks is to recolor the 0th, 3rd, and 4th blocks
so that blocks = "BBBBBBBWBW". 
It can be shown that there is no way to achieve 7 consecutive black blocks in less than 3 operations.
Therefore, we return 3.
```
**Example 2:**
```java
Input: blocks = "WBWBBBW", k = 2
Output: 0
Explanation:
No changes need to be made, since 2 consecutive black blocks already exist.
Therefore, we return 0.
```
**Constraints:**
-   `n == blocks.length`
-   `1 <= n <= 100`
-   `blocks[i]` is either `'W'` or `'B'`.
-   `1 <= k <= n`
 
题意：给出一个长度为 `n` 下标从0开始的字符串 `blocks` ，`blocks[i]` 要么是 `'W'` 要么是 `'B'` ，表示第 `i` 块的颜色。字符 `'W'` 和 `'B'` 分别表示白色和黑色。给出一个整数 `k` ，表示想要 **连续** 黑色块的数目。

每一次操作中，可以选择一个白色块将它 **涂成** 黑色块。返回至少出现 **一次** 连续 `k` 个黑色块的 **最少** 操作次数。

---
### 解法1 暴力
暴力循环遍历字符串 `blocks` ，循环中每次都从当前字符 `blocks[i]` 开始，往后查看 $k$ 个字母，看有多少白色块要涂成黑色。取最少的操作次数作为答案。
```cpp
class Solution {
public:
    int minimumRecolors(string blocks, int k) {
        int ans = blocks.size();
        for (int i = 0, n = blocks.size(); i + k <= n; ++i) {
            int op = 0;
            for (int j = 0; j < k; ++j) 
                if (blocks[i + j] == 'W') ++op;
            ans = min(ans, op);
        }
        return ans;
    }
};
```
- 时间复杂度：$O(n^2)$
- 空间复杂度：$O(1)$

---
### 解法2 前缀和
创建前缀和数组 `partial_sum` ，保存 `'W'` 出现的次数，$k$ 个字符之间 `'W'` 出现的次数为 `partial_sum[i] - partial_sum[i - k]`，白色块最小出现的次数为 `min(partial_sum[i] - partial_sum[i-k], min)` 。
```cpp
class Solution {
public:
    int minimumRecolors(string blocks, int k) {
        int n = blocks.size(), partial_sum[n + 1];
        memset(partial_sum, 0, sizeof(partial_sum)); 
        for (int i = 1; i <= k; ++i)
            partial_sum[i] = partial_sum[i - 1] + (blocks[i - 1] == 'W'); 
        int ans = partial_sum[k];
        for (int i = k + 1; i <= n; ++i) {
            partial_sum[i] = partial_sum[i - 1] + (blocks[i - 1] == 'W'); 
            ans = min(ans, partial_sum[i] - partial_sum[i - k]);
        }
        return ans;
    }
};
```
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$

---
### 解法3 滑动窗口
相似题目：
- [第3题-无重复字符的最长子串](https://leetcode.cn/problems/longest-substring-without-repeating-characters/)
- [第159题-至多包含两个不同字符的最长子串](https://leetcode.cn/problems/longest-substring-with-at-most-two-distinct-characters/)
- [第340题-至多包含 K 个不同字符的最长子串](https://leetcode.cn/problems/longest-substring-with-at-most-k-distinct-characters/)

我们还可用「滑动窗口」来解决该问题。用一个「**固定大小为 $k$ 的滑动窗口**」表示**出现连续 $k$ 个黑色块的区间**，要将该区间全部变为黑色块，此时需要的操作次数为该区间中白色块的数目，那么==只要在「滑动窗口」从左向右滑动的过程中、维护窗口中白色块的数目——遇到新白色块则加1、离开窗口范围的是白色块则减1，最后返回移动过程中白色块数目的最小值==，即为至少出现一次连续 $k$ 个黑色块的最少操作次数。
```cpp
class Solution {
public:
    int minimumRecolors(string blocks, int k) {
        int n = blocks.size(), wnum = 0;
        // 前k个字符中W的数量
        for (int i = 0; i < k; ++i)
            wnum += (blocks[i] == 'W'); // 长度为k的滑窗
        int ans = wnum;
        for (int i = k; i < n; ++i) {
            if (blocks[i] == 'W') ++wnum; // 向右扩张一步,是白方块则+1
            if (blocks[i - k] == 'W') --wnum;
            ans = min(ans, wnum);
        }
        return ans;
    }
};
```
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$

> 来自大佬的讲解：
> 【滑动窗口三部曲】不管窗口是否固定，我们都可以同样地思考滑窗三问三答（而且固定滑窗的代码肯定更简单）
> Q1：对于每一个右指针 `right` 所指的元素 `block[right]` ，做什么操作？
> Q2：什么时候要令左指针 `left` 右移？在什么条件下 `left` 停止右移？即while中的【循环不变量】是什么？
> Q3：什么时候进行 `ans` 的更新？
> 
> 对于本题而言，这三个问题的回答如下：
> A1：如果 `block[right] == 'W'` ，则滑窗中白块的数量应该+1，否则不变。
> A2：因为滑窗的长度固定，所以**对于每一次 `right` 的移动，`left` 都要跟着右移**。另外，`left` 右移之前，若 `block[left] == 'W'` ，则滑窗中白块的数量应该-1，否则不变。
> A3：**对于每一次窗口整体的右移，都应该更新 `ans`** 。
> 
> 想好这三个问题的答案，几乎可以秒杀所有滑动窗口类的题了。把这三个回答组织成代码，并拼接在 `for right in range(k, len(blocks))` 这个循环下面就可以完成题目了。
> 
> 另外，**对于固定滑窗类的题目，往往还需要进行一些初始化**。在这道题中可以用 `ans = num = blocks[:k].count("W")` 来初始化第一个固定窗口中 `'W'` 的个数。

