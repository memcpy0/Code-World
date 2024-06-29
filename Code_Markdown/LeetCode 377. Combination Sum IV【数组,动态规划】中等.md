> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309102021133.png)

给你一个由 **不同** 整数组成的数组 `nums` ，和一个目标整数 `target` 。请你从 `nums` 中找出并返回总和为 `target` 的元素组合的个数。

题目数据保证答案符合 32 位整数范围。

**示例 1：**
```js
输入：nums = [1,2,3], target = 4
输出：7
解释：
所有可能的组合为：
(1, 1, 1, 1)
(1, 1, 2)
(1, 2, 1)
(1, 3)
(2, 1, 1)
(2, 2)
(3, 1)
请注意，顺序不同的序列被视作不同的组合。
```
**示例 2：**
```java
输入：nums = [9], target = 3
输出：0
```
**提示：**
- `1 <= nums.length <= 200`
- `1 <= nums[i] <= 1000`
- `nums` 中的所有元素 **互不相同**
- `1 <= target <= 1000`

**进阶：** 如果给定的数组中含有负数会发生什么？问题会产生何种变化？如果允许负数出现，需要向题目中添加哪些限制条件？

---
[77. 组合](https://leetcode.cn/problems/combinations/)
[39. 组合总和](https://leetcode.cn/problems/combination-sum/)：回溯+剪枝
[40. 组合总和 II](https://leetcode.cn/problems/combination-sum-ii/)
[216. 组合总和 III](https://leetcode.cn/problems/combination-sum-iii/)
[377. 组合总和 Ⅳ](https://leetcode.cn/problems/combination-sum-iv/)


[983. 最低票价](https://leetcode.cn/problems/minimum-cost-for-tickets/)
[322. 零钱兑换](https://leetcode.cn/problems/coin-change/)
[518. 零钱兑换 II](https://leetcode.cn/problems/coin-change-ii/)

---
常见的背包问题有：
1. $True/False$ 问题，即背包能否装满。
    1. 139. 单词拆分 
    2. **416. 分割等和子集**
    3. 1049. 最后一块石头的重量 II
2. 组合问题，即装满背包有几种方法。又分为无序组合和有序组合。
    1. 377. 组合总和 Ⅳ
    2. **494. 目标和**
    3. 518. 零钱兑换 II
    4. 983. 最低票价
    5. 70. 爬楼梯
3. 最大最小问题。不超过/恰好装满背包的最大价值、恰好装满背包的最小物品数。
    1. **474. 一和零**
    2. [343. Integer Break](https://leetcode.com/problems/integer-break/)
    3. 322. 零钱兑换
    4. [剑指 Offer II 103. 最少的硬币数目](https://leetcode.cn/problems/gaM7Ch/)

求组合数问题公式：$$dp[i] += dp[i-num]$$ $True/False$ 问题公式：$$dp[i] = dp[i] or dp[i-num]$$
最大最小问题公式：$$\begin{aligned}dp[i] = \min(dp[i],\ dp[i-num]+1)\\ 或者 dp[i] = \max(dp[i],\ dp[i-num]+1)\end{aligned}$$
以上三组公式是解决对应问题的**核心公式**。

拿到问题后，做以下几个步骤：
1. **分析是否为背包问题**。 背包问题的判定——背包问题具备的特征：给定一个 $target$ ，$target$ 可以是数字也可以是字符串，再给定一个数字或字符串数组，问：能否使用该数组中的元素**做各种排列组合**得到 $target$ 。
2. 是以上三种背包问题中的**哪一种**。如果是求组合数问题，**是否需要考虑元素之间的顺序**。需要考虑顺序有顺序的解法，不需要考虑顺序又有对应的解法。
3. **再分为**是0-1背包问题还是完全背包问题。也就是题目给的 $nums$ 数组中的元素**是否可以重复使用**。 

接下来讲一下背包问题技巧：
1. 如果是0-1背包，即数组中的元素不可重复使用，**$nums$ 放在外循环，$target$ 在内循环，且内循环倒序**；
```python
for num in nums:
    for i in range(target, nums - 1, -1):
```
2. 如果是完全背包，即数组中的元素可重复使用，**$nums$ 放在外循环，$target$ 在内循环。且内循环正序**。
```python
for num in nums:
    for i in range(nums, target + 1):
```
3. 如果求组合数问题需考虑元素之间的顺序，**需将 $target$ 放在外循环，将 $nums$ 放在内循环**。
```python
for i in range(1, target + 1):
    for num in nums:
```
背包在里面，如果物品拿了，则一定是加在前面可能的最后面； 背包在外面，如果物品拿了，则考虑了所有可能的排列。

---
### 解法 求组合数问题+完全背包（可重复取）+考虑元素顺序

