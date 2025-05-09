
> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://img-blog.csdnimg.cn/img_convert/c1d8e422859baafcde4b0bf115a62737.png)

给你一个整数数组 `coins` ，表示不同面额的硬币；以及一个整数 `amount` ，表示总金额。

计算并返回可以凑成总金额所需的 **最少的硬币个数** 。如果没有任何一种硬币组合能组成总金额，返回 `-1` 。

你可以认为每种硬币的数量是无限的。

**示例 1：**
```js
输入：coins = [1, 2, 5], amount = 11
输出：3
解释：11 = 5 + 5 + 1
```
**示例 2：**
```js
输入：coins = [2], amount = 3
输出：-1
```
**示例 3：**
```js
输入：coins = [1], amount = 0
输出：0
```
**提示：**
- `1 <= coins.length <= 12`
- `1 <= coins[i] <= 2^31 - 1`
- `0 <= amount <= 10^4`

---
可能想到使用贪心，一种常见的贪心策略是**每次先使用能够用的面额最大的硬币**。不过这是错误的，找出一个反例如下：
```cpp
Input: coins = [1, 3, 8, 9], amount = 14
Output: 
1. Greedy: 14 = 9 + 3 + 1 + 1
2. Solution: 14 = 8 + 3 + 3
```
> 什么样的面值组合情况下，这种贪心策略是有效的呢？这又是一个问题。 

### 解法 完全背包+恰好装满背包的最小问题
拿到动态规划问题后，做以下几个步骤：
1. **分析是否为背包问题**。 背包问题的判定——背包问题具备的特征：给定一个 $target$ ，$target$ 可以是数字也可以是字符串，再给定一个数组 $nums$ ，$nums$ 中装的可能是数字，也可能是字符串，问：能否使用 $nums$ 中的元素**做各种排列组合**得到 $target$ 。
2. 如果是背包问题，那么是求组合数、True/False、最大最小三种背包问题中的**哪一种**。如果是求组合数问题，**是否需要考虑元素之间的顺序**。需要考虑顺序有顺序的解法，不需要考虑顺序又有对应的解法。
3. **再分为**是0-1背包问题还是完全背包问题。也就是题目给的 $nums$ 数组中的元素**是否可以重复使用**。 

粗略一看，本题是求装满背包的最小问题+完全背包。套用模板：
```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1); // dp[i]表示值为i的时候, 最少的硬币数量
        dp[0] = 0; // 完全背包
        for (int i = 0; i < coins.size(); ++i)
            for (int j = coins[i]; j <= amount; ++j)
                dp[j] = min(dp[j], dp[j - coins[i]] + 1);
        if (dp[amount] <= amount) return dp[amount]; // 可以凑够总金额
        return -1; // 如果dp[amount]>amount,则说明它是从某个无法凑够j-coins[i]的位置转移过来的
    }
};
```
