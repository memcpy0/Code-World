> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `heights` ，表示建筑物的高度。另有一些砖块 `bricks` 和梯子 `ladders` 。

你从建筑物 `0` 开始旅程，不断向后面的建筑物移动，期间可能会用到砖块或梯子。

当从建筑物 `i` 移动到建筑物 `i+1`（下标 **从 0 开始** ）时：

- 如果当前建筑物的高度 **大于或等于** 下一建筑物的高度，则不需要梯子或砖块
- 如果当前建筑的高度 **小于** 下一个建筑的高度，您可以使用 **一架梯子** 或 **`(h[i+1] - h[i])` 个砖块**

如果以最佳方式使用给定的梯子和砖块，返回你可以到达的最远建筑物的下标（下标 **从 0 开始** ）。

**示例 1：**

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/10/31/q4.gif)

**输入：**heights = [4,2,7,6,9,14,12], bricks = 5, ladders = 1
**输出：**4
**解释：**从建筑物 0 出发，你可以按此方案完成旅程：
- 不使用砖块或梯子到达建筑物 1 ，因为 4 >= 2
- 使用 5 个砖块到达建筑物 2 。你必须使用砖块或梯子，因为 2 < 7
- 不使用砖块或梯子到达建筑物 3 ，因为 7 >= 6
- 使用唯一的梯子到达建筑物 4 。你必须使用砖块或梯子，因为 6 < 9
无法越过建筑物 4 ，因为没有更多砖块或梯子。

**示例 2：**

**输入：**heights = [4,12,2,7,3,18,20,3,19], bricks = 10, ladders = 2
**输出：**7

**示例 3：**

**输入：**heights = [14,3,19,3], bricks = 17, ladders = 0
**输出：**3

**提示：**

- `1 <= heights.length <= 105`
- `1 <= heights[i] <= 106`
- `0 <= bricks <= 109`
- `0 <= ladders <= heights.length`

---
前言
这道题好像数据又太弱了？所有不基于优先队列（直接排序也行）的贪心算法都是错的，可以试试下面这组测试数据：

[1,5,1,2,3,4]
4
1
方法一：优先队列 + 贪心
思路与算法

在移动的过程中，我们会需要若干次需要使用砖块或者梯子的情况。假设当前我们需要移动到下一建筑物，但必须使用 111 架梯子或者 Δh\Delta hΔh 个砖块，那么我们如何抉择是使用梯子还是砖块呢？

我们可以用贪心的思路来想这个问题。「梯子」相当于一次性的无限量砖块，那么我们一定是把梯子用在刀刃上。也就是说，如果我们有 lll 架梯子，那么我们会在 Δh\Delta hΔh 最大的那 lll 次使用梯子，而在剩余的情况下使用砖块。

这样一来，我们就可以得到正确的算法了：我们使用优先队列实时维护不超过 lll 个最大的 Δh\Delta hΔh，这些就是使用梯子的地方。对于剩余的 Δh\Delta hΔh，我们需要使用砖块，因此需要对它们进行累加，如果某一时刻这个累加值超过了砖块的数目 bbb，那么我们就再也无法移动了。

代码

C++
Python3
class Solution {
public:
    int furthestBuilding(vector<int>& heights, int bricks, int ladders) {
        int n = heights.size();
        // 由于我们需要维护最大的 l 个值，因此使用小根堆
        priority_queue<int, vector<int>, greater<int>> q;
        // 需要使用砖块的 delta h 的和
        int sumH = 0;
        for (int i = 1; i < n; ++i) {
            int deltaH = heights[i] - heights[i - 1];
            if (deltaH > 0) {
                q.push(deltaH);
                // 如果优先队列已满，需要拿出一个其中的最小值，改为使用砖块
                if (q.size() > ladders) {
                    sumH += q.top();
                    q.pop();
                }
                if (sumH > bricks) {
                    return i - 1;
                }
            }
        }
        return n - 1;
    }
};
class Solution:
    def furthestBuilding(self, heights: List[int], bricks: int, ladders: int) -> int:
        n = len(heights)
        # 由于我们需要维护最大的 l 个值，因此使用小根堆
        q = list()
        # 需要使用砖块的 delta h 的和
        sumH = 0
        for i in range(1, n):
            deltaH = heights[i] - heights[i - 1]
            if deltaH > 0:
                heapq.heappush(q, deltaH)
                # 如果优先队列已满，需要拿出一个其中的最小值，改为使用砖块
                if len(q) > ladders:
                    sumH += heapq.heappop(q)
                if sumH > bricks:
                    return i - 1
        return n - 1

作者：zerotrac 🌸
链接：https://leetcode.cn/problems/furthest-building-you-can-reach/solutions/468787/ke-yi-dao-da-de-zui-yuan-jian-zhu-by-zerotrac2/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

复杂度分析
时间复杂度：O(nlog⁡l)O(n \log l)O(nlogl)，其中 nnn 是建筑物的数量，lll 是梯子的数量。

空间复杂度：O(l)O(l)O(l)。

作者：zerotrac 🌸
链接：https://leetcode.cn/problems/furthest-building-you-can-reach/solutions/468787/ke-yi-dao-da-de-zui-yuan-jian-zhu-by-zerotrac2/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。