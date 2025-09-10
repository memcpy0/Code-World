> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

袋子中装有一些物品，每个物品上都标记着数字 `1` 、`0` 或 `-1` 。

给你四个非负整数 `numOnes` 、`numZeros` 、`numNegOnes` 和 `k` 。

袋子最初包含：
- `numOnes` 件标记为 `1` 的物品。
- `numZeroes` 件标记为 `0` 的物品。
- `numNegOnes` 件标记为 `-1` 的物品。

现计划从这些物品中恰好选出 `k` 件物品。返回所有可行方案中，物品上所标记数字之和的最大值。

**示例 1：**
```java
输入：numOnes = 3, numZeros = 2, numNegOnes = 0, k = 2
输出：2
解释：袋子中的物品分别标记为 {1, 1, 1, 0, 0} 。取 2 件标记为 1 的物品，得到的数字之和为 2 。
可以证明 2 是所有可行方案中的最大值。
```
**示例 2：**
```java
输入：numOnes = 3, numZeros = 2, numNegOnes = 0, k = 4
输出：3
解释：袋子中的物品分别标记为 {1, 1, 1, 0, 0} 。取 3 件标记为 1 的物品，1 件标记为 0 的物品，得到的数字之和为 3 。
可以证明 3 是所有可行方案中的最大值。
```
**提示：**
- `0 <= numOnes, numZeros, numNegOnes <= 50`
- `0 <= k <= numOnes + numZeros + numNegOnes`

---
### 解法 贪心
贪心，按照 $1,0,−1$ 的顺序选：
1. 先选 $1$ ，如果 $k\le \textit{numOnes}$ 那么答案就是 $k$ 。
2. 再选 $0$ ，如果 $k\le \textit{numOnes}+\textit{numZeros}$ ，那么答案为 $\textit{numOnes}$ 。
3. 最后选 $-1$（题目要求恰好选 $k$ 个），那么剩余必须选 $k-\textit{numOnes}-\textit{numZeros}$ 个 $-1$ ，答案为 $$\textit{numOnes} + (k-\textit{numOnes}-\textit{numZeros}) \cdot (-1)= \textit{numOnes} \cdot 2 + \textit{numZeros} -k$$
```java
class Solution {
    public int kItemsWithMaximumSum(int numOnes, int numZeros, int numNegOnes, int k) {
        if (k <= numOnes) return k;
        if (k <= numOnes + numZeros) return numOnes;
        return numOnes - (k - numOnes - numZeros);
    }
}
```
复杂度分析
- 时间复杂度：$O(1)$ 。
- 空间复杂度：$O(1)$ 。