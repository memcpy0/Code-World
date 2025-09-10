> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Given <code>numBottles</code>&nbsp;full water bottles, you can exchange <code>numExchange</code> empty water bottles for one full water bottle.</p>

<p>The operation of drinking a full water bottle turns it into an empty bottle.</p>

<p>Return the <strong>maximum</strong> number of water bottles you can&nbsp;drink.</p>

<p><strong>Example 1:</strong></p>

<p><strong><img style="width: 480px; height: 240px;" src="https://assets.leetcode.com/uploads/2020/07/01/sample_1_1875.png" alt=""></strong></p>

```cpp
Input: numBottles = 9, numExchange = 3
Output: 13
Explanation: You can exchange 3 empty bottles to get 1 full water bottle.
Number of water bottles you can drink: 9 + 3 + 1 = 13.
```
<p><strong>Example 2:</strong></p>

<p><img style="width: 790px; height: 290px;" src="https://assets.leetcode.com/uploads/2020/07/01/sample_2_1875.png" alt=""></p>

```cpp
Input: numBottles = 15, numExchange = 4
Output: 19
Explanation: You can exchange 4 empty bottles to get 1 full water bottle. 
Number of water bottles you can drink: 15 + 3 + 1 = 19.
```

<p><strong>Example 3:</strong></p>

```cpp
Input: numBottles = 5, numExchange = 5
Output: 6
```
<p><strong>Example 4:</strong></p>

```cpp
Input: numBottles = 2, numExchange = 3
Output: 2
```
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;=&nbsp;numBottles &lt;= 100</code></li>
	<li><code>2 &lt;=&nbsp;numExchange &lt;= 100</code></li>
</ul>

题意：小区便利店正在促销，用 `numExchange` 个空酒瓶可以兑换一瓶新酒。你购入了 `numBottles` 瓶酒。如果喝掉了酒瓶中的酒，那么酒瓶就会变成空的。请你计算 **最多** 能喝到多少瓶酒。

---
### 解法1 模拟
根据题意进行模拟，使用 `ans` 统计答案，`numEmpty` 为空瓶子个数。

初始有 `numBottles` 瓶酒，喝完后 `ans = numBottles` ，空瓶子个数 `numEmpty = numBottles`——当且仅当空瓶子个数 `numEmpty` 大于等于兑换个数 `numExchange` 时，可以继续喝到酒，兑换后得到 `q = numEmpty / numExchange` 瓶酒，剩余空瓶子个数等于「本次兑换后剩余的酒瓶个数 `r = numEmpty % numExchange` 」加上「新兑换的 `q` 瓶酒」。

由于循环次数最多不超过能兑换新酒的数量 $\lfloor \dfrac{n}{m - 1}\rfloor$（假设一次用 $m$ 瓶酒兑换一瓶新酒），时间复杂度为 $O(\lfloor \dfrac{n}{m - 1}\rfloor)$ 。进一步分析，当 $m = 2$ 时能兑换酒的数量最多，此时更加准确的复杂度为 $O(\log_2 n)$ 。空间复杂度为 $O(1)$ ：
```java
//Java version
class Solution {
    public int numWaterBottles(int numBottles, int numExchange) {
        int ans = numBottles, numEmpty = numBottles; // 喝完已有的酒
        while (numEmpty >= numExchange) {
            int q = numEmpty / numExchange, r = numEmpty % numExchange;
            ans += q; // 新换的q瓶酒被喝完
            numEmpty = q + r; // 喝完兑换的酒后,空瓶的数量
        }
        return ans;
    }
}
//执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
//内存消耗：34.9 MB, 在所有 Java 提交中击败了95.91% 的用户
```
---
### 解法2 数学
最开始有 $n$ 瓶酒，使用 $m$ 个空酒瓶可换来一瓶新酒，从而饮用数量加一、新瓶子数量加一，于是**对于每次交换而言，会损失掉 $m - 1$ 个酒瓶，$m - 1$ 是一个定值**。因此，我们可以直接算出最大交换次数，即换到的新瓶子数量和额外饮用瓶数。

一个很简单的示例：$n = 15, m = 4$ ，第一次用 $4$ 个空瓶换了一个新瓶，于是 $n = 12$ ；第二次用 $4$ 个空瓶换了一个新瓶，于是 $n = 9$ ；第三次用 $4$ 个空瓶换了一个新瓶，于是 $n = 6$ ；第四次用 $4$ 个空瓶换了一个新瓶，于是 $n = 3$ ，接着就不满足兑换条件。总共能喝到 $15 + 4 = 19$ 瓶酒。 ==特别地，要注意边界条件：当 $n$ 为 $m - 1$ 的倍数时，最后一次不满足兑换条件==。

时间复杂度和空间复杂度都为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int numWaterBottles(int n, int m) {
        return n % (m - 1) == 0 ? (n + n / (m - 1) - 1) : (n + n / (m - 1));
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了23.02% 的用户
```
