> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given an integer array `ranks` and a character array `suits`. You have `5` cards where the `ith` card has a rank of `ranks[i]` and a suit of `suits[i]`.

The following are the types of **poker hands** you can make from best to worst:
1.  `"Flush"`: Five cards of the same suit.
2.  `"Three of a Kind"`: Three cards of the same rank.
3.  `"Pair"`: Two cards of the same rank.
4.  `"High Card"`: Any single card.

Return _a string representing the **best** type of **poker hand** you can make with the given cards._

**Note** that the return values are **case-sensitive**.

<p>You are given an integer array <code>ranks</code> and a character array <code>suits</code>. You have <code>5</code> cards where the <code>i<sup>th</sup></code> card has a rank of <code>ranks[i]</code> and a suit of <code>suits[i]</code>.</p>
<p><strong class="example">Example 1:</strong></p>
<pre><strong>Input:</strong> ranks = [13,2,3,1,9], suits = ["a","a","a","a","a"]
<strong>Output:</strong> "Flush"
<strong>Explanation:</strong> The hand with all the cards consists of 5 cards with the same suit, so we have a "Flush".
</pre>
<p><strong class="example">Example 2:</strong></p>
<pre><strong>Input:</strong> ranks = [4,4,2,4,4], suits = ["d","a","a","b","c"]
<strong>Output:</strong> "Three of a Kind"
<strong>Explanation:</strong> The hand with the first, second, and fourth card consists of 3 cards with the same rank, so we have a "Three of a Kind".
Note that we could also make a "Pair" hand but "Three of a Kind" is a better hand.
Also note that other cards could be used to make the "Three of a Kind" hand.</pre>
<p><strong class="example">Example 3:</strong></p>
<pre><strong>Input:</strong> ranks = [10,10,2,12,9], suits = ["a","b","c","a","d"]
<strong>Output:</strong> "Pair"
<strong>Explanation:</strong> The hand with the first and second card consists of 2 cards with the same rank, so we have a "Pair".
Note that we cannot make a "Flush" or a "Three of a Kind".
</pre>

**Constraints:**
-   `ranks.length == suits.length == 5`
-   `1 <= ranks[i] <= 13`
-   `'a' <= suits[i] <= 'd'`
-   No two cards have the same rank and suit.

题意：给出一个整数数组 `ransk` 和一个字符数组 `suits` ，代表第i张牌值为 `ranks[i]` 和花色 `suits[i]` 。返回你能打出的最好的手牌 *Poker hand* 。

---
### 解法 哈希计数
很简单的题目，没有什么多说的：
```cpp
class Solution {
public:
    string bestHand(vector<int>& ranks, vector<char>& suits) {
        int rcnt[14] = {0}, scnt[4] = {0};
        for (int i = 0; i < 5; ++i) {
            ++scnt[suits[i] - 'a'];
            ++rcnt[ranks[i]];
        }
        for (int i = 0; i < 4; ++i) if (scnt[i] == 5) return "Flush";
        bool three = false, pair = false;
        for (int i = 1; i <= 13; ++i) {
            if (rcnt[i] >= 3) three = true;
            else if (rcnt[i] == 2) pair = true;
        }
        if (three) return "Three of a Kind";
        else if (pair) return "Pair";
        return "High Card";
    }
};
```