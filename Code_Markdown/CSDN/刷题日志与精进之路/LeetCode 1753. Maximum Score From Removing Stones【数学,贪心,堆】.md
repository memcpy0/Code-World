> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。
<p>You are playing a solitaire game with <strong>three piles</strong> of stones of sizes <code>a</code>​​​​​​, <code>b</code>,​​​​​​ and <code>c</code>​​​​​​ respectively. Each turn you choose two <strong>different non-empty </strong>piles, take one stone from each, and add <code>1</code> point to your score. The game stops when there are <strong>fewer than two non-empty</strong> piles (meaning there are no more available moves).</p>
<p>Given three integers <code>a</code>​​​​​, <code>b</code>,​​​​​ and <code>c</code>​​​​​, return <em>the</em> <strong><em>maximum</em> </strong><em><strong>score</strong> you can get.</em></p>
<p><strong class="example">Example 1:</strong></p>
<pre><strong>Input:</strong> a = 2, b = 4, c = 6
<strong>Output:</strong> 6
<strong>Explanation:</strong> The starting state is (2, 4, 6). One optimal set of moves is:
- Take from 1st and 3rd piles, state is now (1, 4, 5)
- Take from 1st and 3rd piles, state is now (0, 4, 4)
- Take from 2nd and 3rd piles, state is now (0, 3, 3)
- Take from 2nd and 3rd piles, state is now (0, 2, 2)
- Take from 2nd and 3rd piles, state is now (0, 1, 1)
- Take from 2nd and 3rd piles, state is now (0, 0, 0)
There are fewer than two non-empty piles, so the game ends. Total: 6 points.
</pre>
<p><strong class="example">Example 2:</strong></p>
<pre><strong>Input:</strong> a = 4, b = 4, c = 6
<strong>Output:</strong> 7
<strong>Explanation:</strong> The starting state is (4, 4, 6). One optimal set of moves is:
- Take from 1st and 2nd piles, state is now (3, 3, 6)
- Take from 1st and 3rd piles, state is now (2, 3, 5)
- Take from 1st and 3rd piles, state is now (1, 3, 4)
- Take from 1st and 3rd piles, state is now (0, 3, 3)
- Take from 2nd and 3rd piles, state is now (0, 2, 2)
- Take from 2nd and 3rd piles, state is now (0, 1, 1)
- Take from 2nd and 3rd piles, state is now (0, 0, 0)
There are fewer than two non-empty piles, so the game ends. Total: 7 points.
</pre>
<p><strong class="example">Example 3:</strong></p>
<pre><strong>Input:</strong> a = 1, b = 8, c = 8
<strong>Output:</strong> 8
<strong>Explanation:</strong> One optimal set of moves is to take from the 2nd and 3rd piles for 8 turns until they are empty.
After that, there are fewer than two non-empty piles, so the game ends.
</pre>
<p><strong>Constraints:</strong></p><ul>
	<li><code>1 &lt;= a, b, c &lt;= 10<sup>5</sup></code></li>
</ul>

题意：玩一个单人游戏 *solitaire* ，有三堆石头，大小分别为 `a, b, c` 。每一轮你都从不同且非空的两堆中，各自取出一个石头，并在你的分数上加1。当非空石堆少于两堆时（即只有一堆非空石头或都不存在），游戏结束。返回你能得到的最大分数。

---
### 解法1 贪心+堆
为了使得分数最大化，每次将最大的两个数进行减少，直到找不到非零的两个数为止—— `rec[0]` 不为零时更大两个数一定不为零，`rec[0]` 为零但 `rec[1]` 不为零、则 `rec[2]` 一定不为零。
- 时间复杂度：$O(n)$ ，$n$ 最大为 $10^5$ 。
- 空间复杂度：$O(1)$ 。

```java
class Solution {
    public int maximumScore(int a, int b, int c) {
        int ans = 0;
        int[] rec = new int[]{a, b, c};
        Arrays.sort(rec);
        while (rec[0] != 0 || rec[1] != 0) {
            --rec[2];
            --rec[1];
            ++ans;
            Arrays.sort(rec); // 重新排序
        }
        return ans;
    }
}
```
---
### 解法2 数学
对解法1进行数学推导，不妨假设 $a\le b\le c$ 。
1. 若 $a + b \le c$ ，这代表任意两个数之和小于等于第三个数。==从解法1的做法，我们不难想到，不断将 $a, b$ 中大的那个数（一开始是 $b$ ）依次与 $c$ 配对==，直到拿完 $a + b$ 个石头为止，分数为 $a + b$ 。**过程中 $b$ 会先降低到与 $a$ 相等，然后 $a$ 或 $b$ 依次与 $c$ 配对**。由于 $a,b$ 之和小于等于 $c$ ，所以 **$a,b$ 最后一定为零，$c \ge 0$** 。
2. 若 $a + b \gt c$ ，这代表任意两个数之和大于第三个数。==从解法1的做法，我们可以想到，不断将 $a, b$ 中大的那个数（一开始是 $b$ ）依次与 $c$ 配对==。**过程中 $b$ 会先降低到与 $a$ 相等，然后 $a$ 或 $b$ 依次与 $c$ 配对**。由于 $c$ 虽最大，但 $a, b$ 之和大于 $c$ ，所以~~**$c$ 最后可能为零，而**~~过程中**必然会出现 $a = b = c$ 的时候**，
	此时根据贪心策略，可能会出现好几种最终状况，但**必然有一个数先归零**（这里假设为有数相等时先用 $c$ ，于是 $c$ 先归零），**剩下的另两个数必然相等或相差1**（假设是 $a, b$ ），$a,b$ 之间再互相匹配，**它们相加再除以2就是互相匹配的分数**（或者说两者中哪个更小）。
	假设 $a, c$ 匹配 $x$ 次，$b,c$ 匹配 $y$ 次，$x + y=c$ ，最后变为 $(a-x,b-y, 0)$ ，所以总的分数为 $x + y + \lfloor \dfrac{ a - x + b - y}{2} \rfloor$ ，因此总分数为 $\lfloor \dfrac{a+b+c}{2}\rfloor$ 。
- 时间复杂度：$O(1)$
- 空间复杂度：$O(1)$

```cpp
class Solution {
public:
    int maximumScore(int a, int b, int c) {
        if (a + b <= c) return a + b;
        if (a + c <= b) return a + c;
        if (b + c <= a) return b + c;
        return (a + b + c) / 2;
    }
};
```