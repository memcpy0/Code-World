> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

## 2865. 美丽塔 I
给你一个长度为 `n` 下标从 **0** 开始的整数数组 `maxHeights` 。

你的任务是在坐标轴上建 `n` 座塔。第 `i` 座塔的下标为 `i` ，高度为 `heights[i]` 。

如果以下条件满足，我们称这些塔是 **美丽** 的：
1. `1 <= heights[i] <= maxHeights[i]`
2. `heights` 是一个 **山脉** 数组。

如果存在下标 `i` 满足以下条件，那么我们称数组 `heights` 是一个 **山脉** 数组：
- 对于所有 `0 < j <= i` ，都有 `heights[j - 1] <= heights[j]`
- 对于所有 `i <= k < n - 1` ，都有 `heights[k + 1] <= heights[k]`

请你返回满足 **美丽塔** 要求的方案中，**高度和的最大值** 。

**示例 1：**
```cpp
输入：maxHeights = [5,3,4,1,1]
输出：13
解释：和最大的美丽塔方案为 heights = [5,3,3,1,1] ，这是一个美丽塔方案，因为：
- 1 <= heights[i] <= maxHeights[i]  
- heights 是个山脉数组，峰值在 i = 0 处。
13 是所有美丽塔方案中的最大高度和。
```
**示例 2：**
```cpp
输入：maxHeights = [6,5,3,9,2,7]
输出：22
解释：和最大的美丽塔方案为 heights = [3,3,3,9,2,2] ，这是一个美丽塔方案，因为：
- 1 <= heights[i] <= maxHeights[i]
- heights 是个山脉数组，峰值在 i = 3 处。
22 是所有美丽塔方案中的最大高度和。
```
**示例 3：**
```cpp
输入：maxHeights = [3,2,5,5,2,3]
输出：18
解释：和最大的美丽塔方案为 heights = [2,2,5,5,2,2] ，这是一个美丽塔方案，因为：
- 1 <= heights[i] <= maxHeights[i]
- heights 是个山脉数组，最大值在 i = 2 处。
注意，在这个方案中，i = 3 也是一个峰值。
18 是所有美丽塔方案中的最大高度和。
```
**提示：**
- `1 <= n == maxHeights <= 10^3`
- `1 <= maxHeights[i] <= 10^9`

## 2866. 美丽塔 II
**提示：**
- `1 <= n == maxHeights <= 10^5`

### 解法 O(n) 前后缀分解+单调栈
下面把 $\textit{maxHeights}$ 简记为 $a$ 。
- 计算从 $a[0]$ 到 $a[i]$ 形成山状数组的左侧递增段，元素和最大是多少，记到数组 $\textit{pre}[i]$ 中。
- 计算从 $a[i]$ 到 $a[n−1]$ 形成山状数组的右侧递减段，元素和最大是多少，记到数组 $suf[i]$ 中。

那么答案就是 $\textit{pre}[i]+\textit{suf}[i+1]$ 的最大值。

如何计算 $\textit{pre}$ 和 $suf$ 呢？用单调栈，元素值从栈底到栈顶**严格递增**。

以 $\textit{suf}$ 为例，我们从右往左遍历 $a$ ，设**当前得到的元素和**为 $\textit{sum}$ ，即为 $suf[i]$ 的值。
- 如果 $a[i]$ 大于栈顶的元素值，那么直接把 $a[i]$ 加到 $\textit{sum}$ 中，同时把 $i$ 入栈（栈中只需要保存下标）。
- 否则，只要 $a[i]$ 小于等于栈顶元素值，就不断循环，把之前加到 $\textit{sum}$ 的撤销掉。循环结束后，从 $a[i]$ 到 $a[j-1]$（假设现在栈顶下标是 $j$ ）都必须是 $a[i]$ ，把 $a[i]\cdot (j-i)$ 加到 $\textit{sum}$ 中。

class Solution:
    def maximumSumOfHeights(self, a: List[int]) -> int:
        n = len(a)
        suf = [0] * (n + 1)
        st = [n]  # 哨兵
        s = 0
        for i in range(n - 1, -1, -1):
            x = a[i]
            while len(st) > 1 and x <= a[st[-1]]:
                j = st.pop()
                s -= a[j] * (st[-1] - j)  # 撤销之前加到 s 中的
            s += x * (st[-1] - i)  # 从 i 到 st[-1]-1 都是 x
            suf[i] = s
            st.append(i)

        ans = s
        st = [-1]  # 哨兵
        pre = 0
        for i, x in enumerate(a):
            while len(st) > 1 and x <= a[st[-1]]:
                j = st.pop()
                pre -= a[j] * (j - st[-1])  # 撤销之前加到 pre 中的
            pre += x * (i - st[-1])  # 从 st[-1]+1 到 i 都是 x
            ans = max(ans, pre + suf[i + 1])
            st.append(i)
        return ans
复杂度分析
时间复杂度：O(n)\mathcal{O}(n)O(n)，其中 nnn 为 maxHeights\textit{maxHeights}maxHeights 的长度。
空间复杂度：O(n)\mathcal{O}(n)O(n)。
思考题
改成先严格递增再严格递减，要怎么做？
改成去掉一些数变成先递增再递减，至少要去掉多少个数？
这题是 

