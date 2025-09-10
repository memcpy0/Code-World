> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Alice and Bob continue their games with piles of stones.  There are a number of piles **arranged in a row**, and each pile has a positive integer number of stones `piles[i]`.  The objective of the game is to end with the most stones. 

Alice and Bob take turns, with Alice starting first.  Initially, `M = 1`.

On each player's turn, that player can take **all the stones** in the **first** `X` remaining piles, where `1 <= X <= 2M`.  Then, we set `M = max(M, X)`.

The game continues until all the stones have been taken.

Assuming Alice and Bob play optimally, return the maximum number of stones Alice can get.
<p><strong class="example">Example 1:</strong></p>
<pre><strong>Input:</strong> piles = [2,7,9,4,4]
<strong>Output:</strong> 10
<strong>Explanation:</strong>  If Alice takes one pile at the beginning, Bob takes two piles, then Alice takes 2 piles again. Alice can get 2 + 4 + 4 = 10 piles in total. If Alice takes two piles at the beginning, then Bob can take all three piles left. In this case, Alice get 2 + 7 = 9 piles in total. So we return 10 since it's larger. 
</pre>
<p><strong class="example">Example 2:</strong></p>
<pre><strong>Input:</strong> piles = [1,2,3,4,5,100]
<strong>Output:</strong> 104
</pre>

**Constraints:**
-   `1 <= piles.length <= 100`
-   `1 <= piles[i] <= 104`

题意：爱丽丝和鲍勃继续他们的石子游戏。许多堆石子 **排成一行**，每堆都有正整数颗石子 `piles[i]` 。游戏以谁手中的石子最多来决出胜负。爱丽丝和鲍勃轮流进行，爱丽丝先开始。最初，`M = 1` 。在每个玩家的回合中，该玩家可以拿走剩下的 **前** `X` 堆的所有石子，其中 `1 <= X <= 2M` 。然后，令 `M = max(M, X)`。

游戏一直持续到所有石子都被拿走。假设爱丽丝和鲍勃都发挥出最佳水平，返回爱丽丝可以得到的最大数量的石头。

---
### 解法1 博弈论+DFS+记忆化搜索
这种博弈游戏，假设双方都发挥最佳水平、都极其聪明，因此先手在做出最佳决策时（拿走一部分石头时），必须考虑后手的反映。后手也要考虑到先手紧随其后的反映。而且，==先后手在做出决策时遇到的局面是无差别的、同质的==，因此可以使用递归。

考虑最简单的情形，当最后剩下的石碓小于等于 $2M$ 时，玩家当前必然是把剩下的石子全部拿走最划算。接着考虑一般的情形，当前玩家想要拿到最多石头，则要综合所有可能：从第 $i$ 堆开始，他分别取走 $x =1,2,\dots, 2m$ 堆石头——==当前玩家要做出选择，拿多少堆石头，这取决于「对方玩家分别从 $i + 1, i + 2,\dots, i+2m$ 堆开始、最多拿走 $\max(x, m)$ 堆石头，且同样做出最佳决策、能拿到的所有最多石头数中的最小者 $temp$ 」==。就是说，他拿多少堆石头，要使得对方玩家拿到的石头数最少为 $temp$ ，而**他后面能拿到的石头数，等于后面的所有石头数减去对方玩家拿到的石头数 $temp$** 。

具体实现中，我们要**先求出后缀和数组**，然后使用 `dfs(0,1)` 求出爱丽丝能拿到的最多石头。
```cpp
class Solution {
private:
    vector<int> suffix_sum;
    int n;
    int dfs(int i, int m) {
        if (i + 2 * m >= n)
            return suffix_sum[i];
        int temp = INT_MAX;
        for (int x = 1; x <= 2 * m; ++x) {
            temp = min(dfs(i + x, max(x, m)), temp);
        }
        return suffix_sum[i] - temp;
    }
public:
    int stoneGameII(vector<int>& piles) {
        n = piles.size();
        suffix_sum.resize(n);
        suffix_sum[n - 1] = piles[n - 1];
        for (int i = n - 2; i >= 0; --i)
            suffix_sum[i] = piles[i] + suffix_sum[i + 1];
        return dfs(0, 1);
        // dfs(i,m)返回接着要取石头的玩家,从i开始取石头,
        // 最多能取2m堆,后面能得到的最大石头数
    }
};
```
提交后，发现这种做法会超时，因为**可能计算了大量重复子问题**，为此采用记忆化搜索，存储子问题的答案：
```cpp
class Solution {
private:
    vector<int> suffix_sum;
    int n;
    int dp[200][200];
    int dfs(int i, int m) {
        if (i + 2 * m >= n)
            return dp[i][m] = suffix_sum[i];
        int temp = INT_MAX;
        for (int x = 1; x <= 2 * m; ++x) {
            int b = max(x, m);
            if (dp[i + x][b]) temp = min(dp[i + x][b], temp);
            else temp = min(dfs(i + x, b), temp);
        }
        return dp[i][m] = suffix_sum[i] - temp;
    }
public:
    int stoneGameII(vector<int>& piles) {
        n = piles.size();
        suffix_sum.resize(n); 
        suffix_sum[n - 1] = piles[n - 1];
        for (int i = n - 2; i >= 0; --i)
            suffix_sum[i] = piles[i] + suffix_sum[i + 1];
        return dp[0][1] = dfs(0, 1); 
        // dfs(i,m)返回爱丽丝从0开始取石头,开始最多只能取1堆得到的最大石头数
    }
};
```

---
### 解法2 动态规划
如果直接使用动态规划的话，要想清楚要存储哪些子状态。首先一定要存储的是取到**某一个位置**时，已经得到的最大值或者**后面能得到的最大值**，但光有位置是不够的，**相同的位置有不同数量的堆可以取，所以我们还需存储当前的 $M$ 值**。

本题中的状态是从后往前递推的——假如最后只剩一堆，一定能算出来最佳方案，但剩很多堆时不好算（依赖后面的状态）。所以我们选择从后往前递推。

有了思路之后，我们就能很方便地定义 $dp$ 数组了：$dp[i][j]$ 表示剩余 $[i, n - 1]$ 堆时，$M = j$ 的情况下，当前取石头的玩家在后面能获得的最多石子数。
1. $i + 2M >= n$ ，$dp[i][M] = sum[i, n - 1]$ ，==剩下的堆数能够直接全部取走，那么最优的情况就是剩下的石子总和==
2. $i + 2M < n$ ，$dp[i][M] = \max(dp[i][M], sum[i, n- 1] - dp[i + x][\max(M, x)])$ ，其中 $1 \le x \le 2M$ ，剩下的堆数不能全部取走，那么**最优情况就是让下一个人取的更少**。对于所有的 $x$ 取值，下一个人从 $i+x$ 开始取起，$M$ 变为 $\max(M, x)$ ，所以下一个人能取 $dp[i + x][\max(M, x)]$ ，我最多能取 $sum[i, n-1] - dp[i + x][\max(M, x)]$ 。

对于 `piles = [2,7,9,4,4]` ，我们可以得到下面的 $dp$ 数组，结果为 $dp[0][1]$ 。
![img|500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302262357786.png)

```java
class Solution {
    public int stoneGameII(int[] piles) {
        int n = piles.length, sum = 0;
        int[][] dp = new int[n][n + 1];
        for (int i = n - 1; i >= 0; --i) { // 直接用一个sum替代了前缀和数组
            sum += piles[i];
            for (int M = 1; M <= n; ++M) {
                if (i + 2 * M >= n) // 考虑边界情况
                    dp[i][M] = sum;
                else 
                    for (int x = 1; x <= 2 * M; ++x)
                        dp[i][M] = Math.max(dp[i][M], sum - dp[i + x][Math.max(M, x)]);
            }
        }
        return dp[0][1];
    }
}
```