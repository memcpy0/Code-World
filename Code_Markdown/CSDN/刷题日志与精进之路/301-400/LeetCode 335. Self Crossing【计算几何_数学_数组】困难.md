> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given an array of integers <code>distance</code>.</p>

<p>You start at point <code>(0,0)</code> on an <strong>X-Y</strong> plane and you move <code>distance[0]</code> meters to the north, then <code>distance[1]</code> meters to the west, <code>distance[2]</code> meters to the south, <code>distance[3]</code> meters to the east, and so on. In other words, after each move, your direction changes counter-clockwise.</p>

<p>Return <code>true</code> if your path crosses itself, and <code>false</code> if it does not.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 400px; height: 435px;" src="https://assets.leetcode.com/uploads/2021/03/14/selfcross1-plane.jpg" alt="">

```bash
Input: distance = [2,1,1,2]
Output: true 
```

<p><strong>Example 2:</strong></p>
<img style="width: 400px; height: 435px;" src="https://assets.leetcode.com/uploads/2021/03/14/selfcross2-plane.jpg" alt="">

```bash
Input: distance = [1,2,3,4]
Output: false 
```

<p><strong>Example 3:</strong></p>
<img style="width: 400px; height: 435px;" src="https://assets.leetcode.com/uploads/2021/03/14/selfcross3-plane.jpg" alt="">

```bash
Input: distance = [1,1,1,1]
Output: true 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= distance.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= distance[i] &lt;= 10<sup>5</sup></code></li>
</ul>

题意：给你一个整数数组 `distance` 。从 **X-Y** 平面上的点 `(0,0)` 开始，先向北移动 `distance[0]` 米，然后向西移动 `distance[1]` 米，向南移动 `distance[2]` 米，向东移动 `distance[3]` 米，持续移动。也就是说，每次移动后你的方位会发生逆时针变化。**判断你所经过的路径是否相交**。如果相交，返回 `true` ；否则，返回 `false` 。 


---
### 解法 计算几何/数学
这道题确实有点难度，我们需要「画图找规律」。

首先显然，**至少需要 $4$ 条边才可能存在相交路径**，因此如果 `distance[]` 的长度小于 $4$ ，可直接返回 `false` 。此外，基于上-左-下-右且平行于X轴或Y轴的移动方式、以及 <code>1 &lt;= distance[i] &lt;= 10<sup>5</sup></code> 的条件，我们知道**如果某条边 $i$ 发生相交，不可能是与边 $i - 1$ 或 $i - 2$ 之间发生的**（边的编号从 $0$ 开始），边 $i - 1$ 与边 $i$ 垂直，边 $i - 2$ 与边 $i$ 平行。

更隐晦的条件是，从左到右遍历 `distance[]` 时，**在「之前的边都不发生相交」的前提下**，某条边 $i$ 如果和其他边相交，则最早会和边 $i - 3, i - 4, i - 5$ 相交——**即将边 $i$ 作为一次规律回旋移动的最后一条边，该边最早和「本次回旋移动的第一条边」以及「上次回旋移动的最后两条边」相交**。注意，边 $i$ 可以是任意方向的边，**此时只是矩形发生旋转，不会影响路径相交的事实**。

即使能和其他边相交，也**必然先穿过**「本次回旋移动的第一条边」或「上次回旋移动的最后两条边」，即这两种情况是某种**基准条件**。前者由上上段和**题目的Example 1**易知，后者有点困难——想象一个一直不相交的移动路径，要么不断往外回旋、要么不断往内回旋（或两者有机结合），此时边 $i$ 可能和哪些边相交呢？以下图为例，`distance[]` 长度为 $6$ ，（按照上文的说明，在边 $4$ 的长度不定的情况下）边 $5$ 可能与边 $2, 1, 0$ 相交。
<img src="https://img-blog.csdnimg.cn/8f2d1391899c422a9e3e3eee560f9bfe.png#pic_center" width="45%">

不过，显然有~~边 $5$ 和边 $2$ 相交一定会和边 $1$ 相交~~（错，边 $5$ 可以从下方和边 $2$ 相交）**边 $5$ 和边 $1$ 相交一定会和边 $0$ 相交**，所以**为什么我们不只讨论边 $i$ 和边 $i - 3, i - 5$ 呢**？

在上面那幅图或更长的 `distance[]` 中，这一结论倒是没错，只是没有考虑到**基础情况**（<code>4 &lt;= distance.length &lt;= 5</code>），比如下面这幅图中，边 $4$ 和边 $0$ 相交，却没有边 $4 - 5$ 和边 $4$ 相交：
<img src="https://img-blog.csdnimg.cn/62c821b8e0e14edab2368de653933c6c.png#pic_center" width="35%">

因此，考虑「边 $i$ 和边 $i - 3$ 是否相交」对应基础情况 `distance.length = 4` ，「边 $i$ 和边 $i - 3, i - 4$ 是否相交」对应基础情况 `distance.length = 5` ，「边 $i$ 和边 $i - 3, i - 4, i - 5$ 是否相交」对应更复杂的情形。

上面说明了**为什么要选取这三条边来讨论**（各自对应不同的情形），不过没有体现出 **「必然先穿过这三条边」** 这一事实。下图则展现得更清晰，在「**之前的边不发生相交**」的前提下，边 $9$ 显然不可能和边 $0, 1, 2$ 等内层边相交（如果它不穿过边 $4, 5, 6$ 尤其是边 $4$ ）：
<img src="https://img-blog.csdnimg.cn/6bc78c5af21549f6abb41bc0f0027136.png#pic_center" width="45%">
下面是更细致的讨论，我们该如何判断边 $i$ 是否与边 $i - 1, i - 2, i - 3$ 相交？为了方便叙述，这里分别使用 $\vec{a}, \vec{b}, \vec{c}, \vec{d}$ 来代指「向上」、「向左」、「向下」、「向右」四类方向边，以 $d[i]$ 来表示 $distance[i]$ ，然后对可能相交的情况分类讨论。
- 边 $i$ 与边 $i - 3$ 相交。此时满足 $d[i] \ge d[i - 2]$ 且 $d[i - 1] \le d[i - 3]$ ：
<img src="https://img-blog.csdnimg.cn/7dc777603409432f927f05d18c511631.png#pic_center" width="45%">

- 边 $i$ 与边 $i - 4$ 相交。此时满足  $d[i - 1] = d[i - 3]$ 且 $d[i] + d[i - 4] \ge d[i - 2]$ ：
 <img src="https://img-blog.csdnimg.cn/dd12af6e465b4b33a8d24692ed197718.png#pic_center" width="45%">

- 边 $i$ 与边 $i - 5$ 相交。此时满足 $d[i - 1] \le d[i - 3]$ 且 $d[i - 1] + d[i - 5] \ge d[i - 3]$ ，同时 $d[i - 2] > d[i - 4]$ 且 $d[i] +d[i - 4] \ge d[i - 2]$ ：
<img src="https://img-blog.csdnimg.cn/047e918d197a41c4b4bcd7e9f42aad90.png#pic_center" width="45%">


综上所述，**边 $i$ 不会与边 $i - 1, i - 2$ 相交**，而**边 $i$ 与边 $i - 3, i - 4, i - 5$ 的相交条件如上所述**。并且边 $i$ 不会在与边 $i - x\ (x > 5)$ 相交的同时不与 $i - y\ (3\le y \le 5)$ 相交，即**边 $i$ 与边 $i - x\ (x > 5)$ 相交前，必然与边 $i - y\ (3\le y \le 5)$ 相交**。

对应的代码如下所示。时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isSelfCrossing(vector<int>& d) {
        if (d.size() <= 3) return false; // 3条边,不可能相交
        // 不可能与d[i-1],d[i-2]对应的边相交
        for (int i = 3, n = d.size(); i < n; ++i) {
            // d[i]对应的边和d[i-3]对应的边相交
            if (d[i] >= d[i - 2] && d[i - 1] <= d[i - 3]) return true;
            // d[i]对应的边和上一个逆时针四边形移动是否相交
            // d[i]对应的边和d[i - 4]对应的边相交
            if (i >= 4 && d[i - 1] == d[i - 3] && d[i] + d[i - 4] >= d[i - 2]) return true;
            // d[i]对应的边和d[i - 5]对应的边相交
            if (i >= 5 && 
                d[i - 1] <= d[i - 3] && d[i - 1] + d[i - 5] >= d[i - 3] &&
                d[i - 2] > d[i - 4] && d[i] >= d[i - 2] - d[i - 4]) // 注意边界条件
                return true;
        }
        return false;
    }
};
//执行用时：16 ms, 在所有 C++ 提交中击败了89.21% 的用户
//内存消耗：18.2 MB, 在所有 C++ 提交中击败了70.79% 的用户
```
