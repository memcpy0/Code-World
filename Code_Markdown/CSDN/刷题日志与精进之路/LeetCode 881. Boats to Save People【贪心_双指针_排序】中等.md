> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>You are given an array <code>people</code> where <code>people[i]</code> is the weight of the <code>i<sup>th</sup></code> person, and an <strong>infinite number of boats</strong> where each boat can carry a maximum weight of <code>limit</code>. Each boat carries at most two people at the same time, provided the sum of the weight of those people is at most <code>limit</code>.</p>

<p>Return <em>the minimum number of boats to carry every given person</em>.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: people = [1,2], limit = 3
Output: 1
Explanation: 1 boat (1, 2)
```
<p><strong>Example 2:</strong></p>

```cpp
Input: people = [3,2,2,1], limit = 3
Output: 3
Explanation: 3 boats (1, 2), (2) and (3)
```

<p><strong>Example 3:</strong></p>

```cpp
Input: people = [3,5,3,4], limit = 5
Output: 4
Explanation: 4 boats (3), (3), (4), (5)
```

 
<p><strong>Constraints:</strong></p> 
<ul>
	<li><code>1 &lt;= people.length &lt;= 5 * 10<sup>4</sup></code></li>
	<li><code>1 &lt;= people[i] &lt;= limit &lt;= 3 * 10<sup>4</sup></code></li>
</ul>


题意：第 `i` 个人的体重为 `people[i]` ，每艘船可以承载的最大重量为 `limit` 。每艘船最多可同时载两人，但条件是这些人的重量之和最多为 `limit` 。返回载到每一个人所需的**最小船数**。(保证每个人都能被船载)。
 
---
### 解法 贪心+排序+双指针
一开始我想错了，认为排序后、从前往后载人能得到最优解。简单举出一个反例即可：
```cpp
people = [1, 2, 4, 5], limit = 6
```
按照这种贪心策略，首先载 `1, 2` ，然后载 `4` ，再载 `5` ，需要三次。正确解法只需要两次，先载 `1, 5` ，再载 `2, 4` 。

后来想了想，认为：一条船要么载一人要么载两人，人数及其重量给定时，**要让船数最少就应该让船尽可能多载**，更准确的说是「**让更多的船载两个人**」，即「**尽可能多的构造出体重之和不超过 `limit` 的二元组**」。

于是先对 `people[]` 排序，然后两个指针 `i, j` 分别从首尾开始匹配：
- 如果 `people[i] + people[j] <= limit` ，说明他们两人可以同船，船数加一，两个指针均往中间移动一步，`++i, --j` ；
- 如果 `people[i] + people[j] > limit` ，说明他们两人不可以同船，且人的体重不超过 `limit` ，此时应该让 `people[j]` 独自一个人坐船，船数加一，`--j` 指针左移。**为什么不让 `people[i]` 独立乘船**？因为 `people[i]` 的体重更小，让他一个人坐船浪费船数，且我们「**期望有更小的 `people[j]` ，满足 `people[i] + people[j] <= limit`，使得两人可以同船**」。

这种「**最大体重匹配最小体重、次重匹配次轻**」的做法，能够使双人船最多，从而船数最少吗？贪心题目中，「**如果要证明一个策略是错误的，只需要举出一个反例；如果要得到一个正确的策略，就需要实际证明**」。因此不只在这题，以后的贪心题目中都会尽可能给出贪心证明——要么用反证法，要么用归纳法。

假设正确策略中，二元组的数量为 $k$ ，我们的贪心策略中的二元组数量为 $z$ ：



排序需要 $O(n\log n)$ 时间，双指针需要 $O(n)$ 时间，整体为 $O(n\log n)$ ，空间复杂度为 $O(1)$ ：





