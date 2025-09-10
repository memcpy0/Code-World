> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>There are <code>n</code> bulbs that are initially off. You first turn on all the bulbs, then&nbsp;you turn off every second bulb.</p>

<p>On the third round, you toggle every third bulb (turning on if it's off or turning off if it's on). For the <code>i<sup>th</sup></code> round, you toggle every <code>i</code> bulb. For the <code>n<sup>th</sup></code> round, you only toggle the last bulb.</p>

<p>Return <em>the number of bulbs that are on after <code>n</code> rounds</em>.</p>

<p><strong>Example 1:</strong></p>
<img style="width: 421px; height: 321px;" src="https://assets.leetcode.com/uploads/2020/11/05/bulb.jpg" alt="">
 
```cpp
Input: n = 3
Output: 1
Explanation: At first, the three bulbs are [off, off, off].
After the first round, the three bulbs are [on, on, on].
After the second round, the three bulbs are [on, off, on].
After the third round, the three bulbs are [on, off, off]. 
So you should return 1 because there is only one bulb is on.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: n = 0
Output: 0
```
<p><strong>Example 3:</strong></p>

```cpp
Input: n = 1
Output: 1
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= n &lt;= 10<sup>9</sup></code></li>
</ul>


题意：初始时有 `n` 个灯泡处于关闭状态。第一轮，你将会打开所有灯泡。接下来的第二轮，你将会每两个灯泡关闭第二个。

第三轮，你每三个灯泡就切换第三个灯泡的开关（即，打开变关闭，关闭变打开）。第 `i` 轮，你每 `i` 个灯泡就切换第 `i` 个灯泡的开关。直到第 `n` 轮，你只需要切换最后一个灯泡的开关。

找出并返回 `n` 轮后有多少个亮着的灯泡。

---
### 解法 数论推论
这是一道典型的数论题目。我们要经历 $n$ 轮，第 $i$ 轮改变所有编号为 $i$ 的倍数的灯泡的状态，灯泡编号从 $1$ 到 $n$ 。

显然，**一个编号为 $k$ 的灯泡经过 $n$ 轮后仍然亮着，当且仅当该灯泡被切换状态的次数为奇数**（初始时都是关闭状态）。又因为一个灯泡切换状态的次数为其因子（包括 $1$ 和数字自身）的个数，若其有 $t$ 个因子，则最终被切换 $t$ 次，所以问题可转换为：**在 $[1, n]$ 内有多少个数，具有奇数个因子**。这些具有奇数个因子的灯泡，就是 $n$ 轮后仍然亮着的灯泡。

根据[因子的定义](https://memcpy0.blog.csdn.net/article/details/121160470)（若 $a, b\in \Z,\ b \ne 0,\ a = bq$ ，则 $b\mid a$ ，$b$ 为 $a$ 的因子）知，若 $q \ne 0$ ，则 $q\mid a$ ，并可写成 $q = \dfrac{a}{b}\ (b, q \ne 0)$ 。因此，如果某个数 $b$ 为 $a$ 的因子，则 $\dfrac{a}{b}$ 也是 $a$ 的因子，即「**因数总是成对出现**」。

而某个数的因子个数为奇数，说明某个因子在分解时出现了两次，且必然重复出现在同一次拆解中，即 $b = \dfrac{a}{b}$ ，于是 $a$ 为完全平方数。从而问题最终转换为：**计算在 $[1, n]$ 中完全平方数的个数**。

根据数论中的推论，$[1, n]$ 中完全平方数的个数为 $\lfloor \sqrt{n} \rfloor$ ，即最后亮着的灯泡数量为 $\lfloor \sqrt{n} \rfloor$ 。

时间复杂度：$O(1)$ ，空间复杂度：$O(1)$ 。代码如下：
```java
//Java version
class Solution {
    public int bulbSwitch(int n) {
        return (int)Math.sqrt(n);
    }
}
//执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
//内存消耗：35.1 MB, 在所有 Java 提交中击败了35.14% 的用户
```
