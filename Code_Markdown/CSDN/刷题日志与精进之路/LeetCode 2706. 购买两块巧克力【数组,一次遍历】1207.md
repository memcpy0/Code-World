> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `prices` ，它表示一个商店里若干巧克力的价格。同时给你一个整数 `money` ，表示你一开始拥有的钱数。

你必须购买 **恰好** 两块巧克力，而且剩余的钱数必须是 **非负数** 。同时你想最小化购买两块巧克力的总花费。

请你返回在购买两块巧克力后，最多能剩下多少钱。如果购买任意两块巧克力都超过了你拥有的钱，请你返回 `money` 。注意剩余钱数必须是非负数。

**示例 1：**
```java
输入：prices = [1,2,2], money = 3
输出：0
解释：分别购买价格为 1 和 2 的巧克力。你剩下 3 - 3 = 0 块钱。所以我们返回 0 。
```
**示例 2：**
```java
输入：prices = [3,2,3], money = 3
输出：3
解释：购买任意 2 块巧克力都会超过你拥有的钱数，所以我们返回 3 。
```
**提示：**
- `2 <= prices.length <= 50`
- `1 <= prices[i] <= 100`
- `1 <= money <= 100`

---
### 解法 一次遍历
遍历 $\textit{prices}$ ，维护最小值 $\textit{mn}_1$ 和次小值 $\textit{mn}_2$ 。

如果 $\textit{mn}_1+\textit{mn}_2>\textit{money}$ ，那么剩余的钱会变成负数，返回 $\textit{money}$ ，否则返回 $\textit{money}-\textit{mn}_1-\textit{mn}_2$ 。
```cpp
// python
class Solution:
    def buyChoco(self, prices: List[int], money: int) -> int:
        mn1 = mn2 = inf
        for p in prices:
            if p < mn1:
                mn2 = mn1
                mn1 = p
            elif p < mn2:
                mn2 = p
        return money if mn1 + mn2 > money else money - mn1 - mn2

// go
func buyChoco(prices []int, money int) int {
    mn1, mn2 := math.MaxInt, math.MinInt
    for _, p := range prices {
        if p < mn1 {
            mn2 = mn1
            mn1 = p
        } else if p < mn2 {
            mn2 = p
        }
    }
    if mn1 + mn2 <= money {
        return money - mn1 - mn2
    }
    return money
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $prices$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。仅用到若干额外变量。
