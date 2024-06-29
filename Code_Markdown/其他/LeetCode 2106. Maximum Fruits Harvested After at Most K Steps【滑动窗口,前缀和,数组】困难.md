> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在一个无限的 x 坐标轴上，有许多水果分布在其中某些位置。给你一个二维整数数组 `fruits` ，其中 `fruits[i] = [positioni, amounti]` 表示共有 `amounti` 个水果放置在 `positioni` 上。`fruits` 已经按 `positioni` **升序排列** ，每个 `positioni` **互不相同** 。

另给你两个整数 `startPos` 和 `k` 。最初，你位于 `startPos` 。从任何位置，你可以选择 **向左或者向右** 走。在 x 轴上每移动 **一个单位** ，就记作 **一步** 。你总共可以走 **最多** `k` 步。你每达到一个位置，都会摘掉全部的水果，水果也将从该位置消失（不会再生）。

返回你可以摘到水果的 **最大总数** 。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/11/21/1.png)
```java
输入：fruits = [[2,8],[6,3],[8,6]], startPos = 5, k = 4
输出：9
解释：
最佳路线为：
- 向右移动到位置 6 ，摘到 3 个水果
- 向右移动到位置 8 ，摘到 6 个水果
移动 3 步，共摘到 3 + 6 = 9 个水果
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2021/11/21/2.png)
```java
输入：fruits = [[0,9],[4,1],[5,7],[6,2],[7,4],[10,9]], startPos = 5, k = 4
输出：14
解释：
可以移动最多 k = 4 步，所以无法到达位置 0 和位置 10 。
最佳路线为：
- 在初始位置 5 ，摘到 7 个水果
- 向左移动到位置 4 ，摘到 1 个水果
- 向右移动到位置 6 ，摘到 2 个水果
- 向右移动到位置 7 ，摘到 4 个水果
移动 1 + 3 = 4 步，共摘到 7 + 1 + 2 + 4 = 14 个水果
```
**示例 3：**
![](https://assets.leetcode.com/uploads/2021/11/21/3.png)
```java
输入：fruits = [[0,3],[6,4],[8,5]], startPos = 3, k = 2
输出：0
解释：
最多可以移动 k = 2 步，无法到达任一有水果的地方
```
**提示：**
-   `1 <= fruits.length <= 10^5`
-   `fruits[i].length == 2`
-   `0 <= startPos, positioni <= 2 * 10^5`
-   对于任意 `i > 0` ，`positioni-1 < positioni` 均成立（下标从 **0** 开始计数）
-   `1 <= amounti <= 10^4`
-   `0 <= k <= 2 * 10^5`

---
### 解法 滑动窗口+前缀和
从题意中可知几个关键点：
1. 由于只能一步步地走，**移动的范围必然是一段连续的区间**。
2. **如果反复左右移动，会白白浪费移动次数**。最优方案很少，**从 `startPos` 出发**，要么只向右，要么只向左，要么先向右再向左，要么先向左再向右。

设向左走最远可以到达 fruits[left][0]\textit{fruits}[\textit{left}][0]fruits[left][0]，这可以用枚举或者二分查找得出，其中 left\textit{left}left 是最小的满足

fruits[left][0]≥startPos−k \textit{fruits}[\textit{left}][0]\ge \textit{startPos}-k
fruits[left][0]≥startPos−k
的下标。

假设位置不超过 startPos\textit{startPos}startPos 的最近水果在 fruits[right][0]\textit{fruits}[\textit{right}][0]fruits[right][0]，那么当 right\textit{right}right 增加时，left\textit{left}left 不可能减少，有单调性，因此可以用同向双指针（滑动窗口）解决。不了解的同学可以先看上面的视频讲解。

如何判断 left\textit{left}left 是否需要增加呢？

如果先向右再向左，那么移动距离为

(fruits[right][0]−startPos)+(fruits[right][0]−fruits[left][0]) (\textit{fruits}[\textit{right}][0] - \textit{startPos}) + (\textit{fruits}[\textit{right}][0] - \textit{fruits}[\textit{left}][0])
(fruits[right][0]−startPos)+(fruits[right][0]−fruits[left][0])
如果先向左再向右，那么移动距离为

(startPos−fruits[left][0])+(fruits[right][0]−fruits[left][0]) (\textit{startPos} - \textit{fruits}[\textit{left}][0]) + (\textit{fruits}[\textit{right}][0] - \textit{fruits}[\textit{left}][0])
(startPos−fruits[left][0])+(fruits[right][0]−fruits[left][0])
如果上面两个式子均大于 kkk，就说明 fruits[left][0]\textit{fruits}[\textit{left}][0]fruits[left][0] 太远了，需要增加 left\textit{left}left。

对于 right\textit{right}right，它必须小于 nnn，且满足

fruits[right][0]≤startPos+k \textit{fruits}[\textit{right}][0]\le \textit{startPos}+k
fruits[right][0]≤startPos+k
移动 left\textit{left}left 和 right\textit{right}right 的同时，维护窗口内的水果数量 sss，同时用 sss 更新答案的最大值。

class Solution {
public:
    int maxTotalFruits(vector<vector<int>> &fruits, int startPos, int k) {
        int left = lower_bound(fruits.begin(), fruits.end(), startPos - k, [](const auto &a, int b) {
            return a[0] < b;
        }) - fruits.begin(); // 向左最远能到 fruits[left][0]
        int right = left, s = 0, n = fruits.size();
        for (; right < n && fruits[right][0] <= startPos; ++right)
            s += fruits[right][1]; // 从 fruits[left][0] 到 startPos 的水果数
        int ans = s;
        for (; right < n && fruits[right][0] <= startPos + k; ++right) {
            s += fruits[right][1]; // 枚举最右位置为 fruits[right][0]
            while (fruits[right][0] * 2 - fruits[left][0] - startPos > k &&
                   fruits[right][0] - fruits[left][0] * 2 + startPos > k)
                s -= fruits[left++][1]; // fruits[left][0] 无法到达
            ans = max(ans, s); // 更新答案最大值
        }
        return ans;
    }
};
上面的代码可以再简化一点，把第一个 for 循环合并到第二个中。

注：这会让第一个 for 循环增加一些无效计算，运行速度可能不如上面的写法。


class Solution {
public:
    int maxTotalFruits(vector<vector<int>> &fruits, int startPos, int k) {
        int left = lower_bound(fruits.begin(), fruits.end(), startPos - k, [](const auto &a, int b) {
            return a[0] < b;
        }) - fruits.begin(); // 向左最远能到 fruits[left][0]
        int ans = 0, s = 0, n = fruits.size();
        for (int right = left; right < n && fruits[right][0] <= startPos + k; ++right) {
            s += fruits[right][1]; // 枚举最右位置为 fruits[right][0]
            while (fruits[right][0] * 2 - fruits[left][0] - startPos > k &&
                   fruits[right][0] - fruits[left][0] * 2 + startPos > k)
                s -= fruits[left++][1]; // fruits[left][0] 无法到达
            ans = max(ans, s); // 更新答案最大值
        }
        return ans;
    }
};
复杂度分析
时间复杂度：O(n)\mathcal{O}(n)O(n)，其中 nnn 为 fruits\textit{fruits}fruits 的长度。虽然写了个二重循环，但是内层循环中对 left\textit{left}left 加一的总执行次数不会超过 nnn 次，所以总的时间复杂度为 O(n)\mathcal{O}(n)O(n)。
空间复杂度：O(1)\mathcal{O}(1)O(1)。仅用到若干额外变量。

简单分析一下，发现无非就是线段 [l, r] 覆盖了多少水果。

线段有多少种可能呢？假设人向左走 yyy 步，然后回到原点，再向右走 xxx 步，那么区间长度就是：x+2yx + 2yx+2y ，其中 x+2y≤kx + 2y \le kx+2y≤k ，区间表示为 [startPos - y, startPos + x] 。

同理，如果向右走 yyy 步，然后回到原点，再向左走 xxx 步，那么区间表示为 [startPos - x, startPos + y] 。

所以我们枚举 y 长度就可以把所有最长线段都枚举出来：

for (int x = k; x >= 0; -- x)
{
    int y = (k - x) / 2;
    int l, r;
    // x + 2y = k
    l = startPos - x, r = startPos + y;
    ans = max(线段 [l, r] 覆盖了多少水果, ans)
    // 2y + x = k
    l = startPos - y, r = startPos + x;
    ans = max(线段 [l, r] 覆盖了多少水果, ans)
}
那么，如何求 线段 [l, r] 覆盖了多少水果 ？

很容易想到 前缀和预处理，累加一下各个水果节点数量，然后二分找 l 和 r 各自对应的水果节点。

完整代码如下。

class Solution {
public:
    int maxTotalFruits(vector<vector<int>>& fruits, int startPos, int k) {
        int n = fruits.size();
        
        // 前缀和，用于求区间 [l, r] 共有多少水果
        vector<int> sum;
        sum.push_back(0);
        for (int i = 1; i <= n; ++ i)
            sum.push_back(fruits[i - 1][1] + sum[i - 1]);

        vector<int> pos;
        for (int i = 0; i < n; ++ i)
            pos.push_back(fruits[i][0]);

        // 枚举可能抵达的区间
        int ans = 0;
        for (int x = k; x >= 0; -- x)
        {
            int y = (k - x) / 2;
            int l, r;
            // x + 2y = k
            l = startPos - x, r = startPos + y;
            auto pl = lower_bound(pos.begin(), pos.end(), l) - pos.begin(), pr = upper_bound(pos.begin(), pos.end(), r) - pos.begin();
            ans = max(ans, sum[pr] - sum[pl]);
            // 2y + x = k
            l = startPos - y, r = startPos + x;
            pl = lower_bound(pos.begin(), pos.end(), l) - pos.begin(), pr = upper_bound(pos.begin(), pos.end(), r) - pos.begin();
            ans = max(ans, sum[pr] - sum[pl]);
        }
        
        return ans;
    }
};
 