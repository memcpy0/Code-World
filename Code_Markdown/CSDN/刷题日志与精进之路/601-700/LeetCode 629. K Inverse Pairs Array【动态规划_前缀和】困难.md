> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309101212844.png)

<p>For an integer array <code>nums</code>, an <strong>inverse pair</strong> is a pair of integers <code>[i, j]</code> where <code>0 &lt;= i &lt; j &lt; nums.length</code> and <code>nums[i] &gt; nums[j]</code>.</p>

<p>Given two integers n and k, return the number of different arrays consist of numbers from <code>1</code> to <code>n</code> such that there are exactly <code>k</code> <strong>inverse pairs</strong>. Since the answer can be huge, return it <strong>modulo</strong> <code>10<sup>9</sup> + 7</code>.</p>

 
<p><strong>Example 1:</strong></p>

```java
Input: n = 3, k = 0
Output: 1
Explanation: Only the array [1,2,3] which consists of numbers from 1 to 3 has exactly 0 inverse pairs.
```

<p><strong>Example 2:</strong></p>

```java
Input: n = 3, k = 1
Output: 2
Explanation: The array [1,3,2] and [2,1,3] have exactly 1 inverse pair.
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 1000</code></li>
	<li><code>0 &lt;= k &lt;= 1000</code></li>
</ul>

题意：给出两个整数 <code>n</code> 和 <code>k</code>，找出所有包含从 <code>1</code> 到 <code>n</code> 的数字，且恰好拥有 <code>k</code> 个逆序对的不同的数组的个数。由于答案可能很大，只需要返回 答案 mod 10<sup>9</sup> + 7 的值。 
 
---
### 解法 区间DP+前缀和
从 $n$ 和 $k$ 数据范围均为 $10^3$ ，可以看出这是一道二维的动态规划题。我们从题意入手，定义 $dp[i][j]$ 为「**使用数值 $1$ 到 $i$ 、凑成逆序对数量恰好为 $j$ 的数组的个数**」。

不失一般性地考虑 $dp[i][j]$ 如何转移——我们对第 $i$ 个数（且数值为 $i$ 的数）所在位置进行讨论，共有 $i$ 种选择。**假设第 $i$ 个数所在位置为 $k$** ，由于数 $i$ 为整个数组的最大值，因此 $i$ 与前面所有数均不形成逆序对，与后面的所有数均形成逆序对。因此，**与 $i$ 直接相关的逆序对数量为 $i - k - 1$** ，**与 $i$ 不相关的逆序对数量为 $j - (i - k - 1)$** ，这一数量可由 $dp[i - 1][j - (i - k - 1)]$ 得到。

举个实例来理解。假设 $[1, 4]$ 这四个数字的排列方式是 `xxxx` ，再往其中添加一个数字 $i = 5$ 有如下几种方式：
- `xxxx5` ：数字 $5$ 放在下标为 $k = 4$ 的位置上，那么由 $5$ 产生的逆序对数量为 $(i - k) - 1 = 0$ 。对于 $dp[5][j]$ ，总逆序对数量为 $j$ ，因此由其他四个数字 $[1, 4]$（与数字 $5$ 不相关）构成的逆序对数量为 $j$ ，对应的数组个数为 $dp[4][j]$ ；
- `xxx5x` ：数字 $5$ 放在下标为 $k = 3$ 的位置上，那么由 $5$ 产生的逆序对数量为 $(i - k) - 1 = 1$ 。对于 $dp[5][j]$ ，总逆序对数量为 $j$ ，因此由其他四个数字 $[1, 4]$（与数字 $5$ 不相关）构成的逆序对数量为 $j - 1$ ，对应的数组个数为 $dp[4][j - 1]$ ；
- `xx5xx` ：数字 $5$ 放在下标为 $k = 2$ 的位置上，那么由 $5$ 产生的逆序对数量为 $(i - k) - 1 = 2$ 。对于 $dp[5][j]$ ，总逆序对数量为 $j$ ，因此由其他四个数字 $[1, 4]$（与数字 $5$ 不相关）构成的逆序对数量为 $j - 2$ ，对应的数组个数为 $dp[4][j - 2]$ ；
- `x5xxx` ：数字 $5$ 放在下标为 $k = 1$ 的位置上，那么由 $5$ 产生的逆序对数量为 $(i - k) - 1 = 3$ 。对于 $dp[5][j]$ ，总逆序对数量为 $j$ ，因此由其他四个数字 $[1, 4]$（与数字 $5$ 不相关）构成的逆序对数量为 $j - 3$ ，对应的数组个数为 $dp[4][j - 3]$ ；
- `5xxxx` ：数字 $5$ 放在下标为 $k = 0$ 的位置上，那么由 $5$ 产生的逆序对数量为 $(i - k) - 1 = 4$ 。对于 $dp[5][j]$ ，总逆序对数量为 $j$ ，因此由其他四个数字 $[1, 4]$（与数字 $5$ 不相关）构成的逆序对数量为 $j - 4$ ，对应的数组个数为 $dp[4][j - 4]$ ；

因此，$[1, 5]$ 五个数字构成的逆序对为 $j$ 时，对应的数组个数为：
$$\begin{aligned}
dp[5][j] &= dp[4][j] + dp[4][j - 1] + dp[4][j - 2] + dp[4][j - 3] + dp[4][j - 4] \\
\end{aligned}$$

综上，可知 $dp[i][j]$ 的转移方程为（ $i$ 为当前数据范围 $[1, i]$ ，$j$ 为逆序对数量，$k$ 为数字 $i$ 能放置的下标位置）：
$$\begin{aligned}
dp[i][j] &= \sum_{k=0}^{i-1} dp[i - 1][j - (i - k - 1)]
\end{aligned}$$

因此对于题意——给出两个整数 $n$ 和 $k$ ，找出所有包含从 $1$ 到 $n$ 的数字、且恰好拥有 $k$ 个逆序对的不同的数组的个数，**共有 $nk$ 个状态，每个 $dp[i][j]$ 的计算需要枚举数字 $i$ 能放置的所有位置**，总体复杂度为 $O(n^2k)$ ，计算量达到 $10^9$ ，会TLE。

显然，**状态数量不可减少**，我们考虑如何优化单个状态的转移过程。不难发现 $\displaystyle \sum^{i-1}_{k=0} dp[i - 1][j - (i - 1 - k)]$ 为上个转移结果 $dp[i - 1][x]$ 的某个前缀，可以使用「**前缀和**」进行优化，**将计算单个状态的复杂度从 $O(n)$ 降低到 $O(1)$** 。即：
$$\begin{aligned}
&dp[i][j] = \sum^{i-1}_{k=0} dp[i - 1][j - (i - 1 - k)] \\
&dp[i][j - 1] = \sum^{i - 1}_{k = 0} dp[i - 1][j - 1 - (i - 1 - k)] \\
\therefore \ \ &dp[i][j] = dp[i][j- 1] + dp[i - 1][j] - dp[i - 1][j - i]
\end{aligned}$$

其他不可忽略的细节有：
- 为了避免溢出，需要在加法和减法时取模；
- $dp[i][j]$ 可能为负数，因为 $j \ge i$ 时要减去 $dp[i - 1][j - i]$ ，而 $dp[i][j] = (dp[i][j - 1] + dp[i - 1][j]) \ \%  \bmod$ 后可能小于 $dp[i - 1][j- i]$ ，因此**对减法取模前先要加一次 $\bmod$** ； 
- 注意对 $j$ 和 $i$ 的大小分类讨论，避免数字 $i$ 放置的位置过于靠前，导致与 $i$ 相关的逆序对数量太多，使得 $j - i \lt 0$ 

综上所述，最终的转移方程为：
$$dp[i][j] =  \begin{cases}
(dp[i][j - 1] + dp[i - 1][j]) \ \% \bmod{} \ \ &\textrm{if}\ j < i \\
\Big((dp[i][j - 1] + dp[i - 1][j]) \ \% \bmod -\ dp[i - 1][j - i] + \bmod\Big) \ \% \bmod{} \ \ &\textrm{if} \ j \ge i
\end{cases}
$$

最终代码如下，时间复杂度为 $O(nk)$ ，空间复杂度为 $O(nk)$ ：
```cpp
//C++ version
class Solution {
public:
    int kInversePairs(int n, int k) {
        if (k > n * (n - 1) / 2 || k < 0) return 0; // 边界情况
        if (k == 0 || k == n * (n - 1) / 2) return 1;

        const int mod = 1e9 + 7;
        vector<vector<int>> dp(n + 1, vector<int>(k + 1));
        for (int i = 1; i <= n; ++i) {
            int bound = min(k, i * (i - 1) / 2); // i个数,逆序对数量最大为i*(i-1)/2
            dp[i][0] = 1; // 边界情况
            for (int j = 1; j <= bound; ++j) {
                dp[i][j] = (dp[i][j - 1] + dp[i - 1][j]) % mod;
                if (j >= i)
                    dp[i][j] = (dp[i][j] - dp[i - 1][j - i] + mod) % mod;
            }
        }
        return dp[n][k];
    }
};
//执行用时：32 ms, 在所有 C++ 提交中击败了66.56% 的用户
//内存消耗：15.7 MB, 在所有 C++ 提交中击败了34.11% 的用户
```
由于 $dp[i][j]$ 仅与当前行和前一行相关，我们可以用「**滚动数组**」进行优化。时间复杂度仍为 $O(nk)$ ，空间复杂度降低到 $O(2k)$ ： 
```cpp
//C++ version
class Solution {
public:
    int kInversePairs(int n, int k) {
        if (k > n * (n - 1) / 2 || k < 0) return 0; // 边界情况
        if (k == 0 || k == n * (n - 1) / 2) return 1;

        const int mod = 1e9 + 7;
        vector<vector<int>> dp(2, vector<int>(k + 1));
        for (int i = 1; i <= n; ++i) {
            int bound = min(k, i * (i - 1) / 2); // i个数,逆序对数量最大为i*(i-1)/2
            dp[i & 1][0] = 1; // 边界情况
            for (int j = 1; j <= bound; ++j) {
                dp[i & 1][j] = (dp[i & 1][j - 1] + dp[!(i & 1)][j]) % mod;
                if (j >= i)
                    dp[i & 1][j] = (dp[i & 1][j] - dp[!(i & 1)][j - i] + mod) % mod;
            }
        }
        return dp[n & 1][k];
    }
};
//执行用时：32 ms, 在所有 C++ 提交中击败了66.56% 的用户
//内存消耗：6.1 MB, 在所有 C++ 提交中击败了90.40% 的用户
```
