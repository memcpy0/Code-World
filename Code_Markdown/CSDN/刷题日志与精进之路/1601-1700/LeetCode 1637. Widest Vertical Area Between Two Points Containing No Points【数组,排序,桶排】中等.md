 >本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given `n` `points` on a 2D plane where `points[i] = [xi, yi]`, Return _the **widest vertical area** between two points such that no points are inside the area._

A **vertical area** is an area of fixed-width extending infinitely along the y-axis (i.e., infinite height). The **widest vertical area** is the one with the maximum width.

Note that points **on the edge** of a vertical area **are not** considered included in the area.

**Example 1:**

![](https://assets.leetcode.com/uploads/2020/09/19/points3.png)​
```java
Input: points = [[8,7],[9,9],[7,4],[9,7]]
Output: 1
Explanation: Both the red and the blue area are optimal.
```
**Example 2:**
```java
Input: points = [[3,1],[9,0],[1,0],[1,4],[5,3],[8,8]]
Output: 3
```
**Constraints:**
-   `n == points.length`
-   `2 <= n <= 105`
-   `points[i].length == 2`
-   `0 <= xi, yi <= 10^9`

题意：给你 `n` 个二维平面上的点 `points` ，其中 `points[i] = [xi, yi]` ，返回两点之间内部不包含任何点的 **最宽垂直区域** 的宽度。**垂直区域** 的定义是固定宽度，而 `y` 轴上无限延伸的一块区域（也就是高度为无穷大）。 **最宽垂直区域** 为宽度最大的一个垂直区域。垂直区域 **边上** 的点 **不在** 区域内。

---
### 解法1 排序
可对数组 $points$ 按照 $x$ 升序排列，获取相邻点之间 $x$ 的差值的最大值。
```cpp
class Solution {
public:
    int maxWidthOfVerticalArea(vector<vector<int>>& points) {
        sort(points.begin(), points.end());
        int ans = 0;
        for (int i = 0; i < points.size() - 1; ++i) {
            ans = max(ans, points[i + 1][0] - points[i][0]);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log⁡n)$ 
- 空间复杂度：$O(\log n)$ 。其中 $n$ 为数组 $points$ 的长度。

---
### 解法2 桶排序+估算最大间距（最优） 
我们可以利用桶排序，将时间复杂度降低到 $O(n)$ 。

**我们将数组 $points$ 的横坐标放入数组 $nums$ 中**，纵坐标不用考虑。假设数组 $nums$ 有 $n$ 个元素，所有元素从小到大依次是 $nums[0], nums[1],\ \dots,\ nums[n-1]$ ，最大间距是 $maxGap$ 。考虑**数组中的最大元素和最小元素之差**：
$$nums[n -1] - nums[0] = \sum_{i=1}^{n-1} (nums[i] - nums[i - 1]) \le maxGap \times (n - 1)$$ 因此：$$maxGap \ge \dfrac{nums[n - 1] - nums[0]}{n - 1}$$，即最大间距至少为 $\dfrac{nums[n - 1] - nums[0] }{n - 1}$ 。

利用桶排序的思想，设定桶的大小（每个桶中最多包含的不同元素个数）为  $\dfrac{nums[n - 1] - nums[0] }{n - 1}$ ，然后： 
- 将元素按照**元素值**均匀分布到各个桶内，==令同一个桶内的任意两个元素之差小于 $maxGap$  ，即不会影响到最大间距==；==差大于等于 $maxGap$ 的两个元素一定在两个不同的桶内，会影响到最大间距==。
- **对于每个桶，维护桶内的最小值和最大值**，初始时每个桶内的最小值和最大值分别是正无穷和负无穷，表示桶内没有元素。
- 遍历数组 $nums$ 中的所有元素。对于每个元素，根据该元素与最小元素之差以及桶的大小、计算该元素应分到的桶的编号，可以**确保「编号小的桶内的元素」都「小于编号大的桶内的元素」**，使用元素值更新元素所在的桶内的最小值和最大值。
- 遍历结束之后，每个非空的桶内的最小值和最大值都可以确定。==按照桶的编号从小到大的顺序依次遍历每个桶，**当前的桶的最小值**和**上一个非空的桶的最大值**是排序后的相邻元素，计算两个相邻元素之差==，并更新最大间距。遍历桶结束之后，即可得到最大间距。

```cpp
class Solution {
public:
    int maxWidthOfVerticalArea(vector<vector<int>>& points) {
        int n = points.size();
        vector<int> nums;
        for (auto& p : points) {
            nums.push_back(p[0]);
        }
        const int inf = 1 << 30;
        int mi = inf, mx = -inf;
        for (int v : nums) {
            mi = min(mi, v);
            mx = max(mx, v);
        }
        int bucketSize = max(1, (mx - mi) / (n - 1)); // 桶大小,桶中任意元素的差<bucketSize
        int bucketCount = (mx - mi) / bucketSize + 1; // 桶数量,相当于[mi,mx]这段区间分成bucketCount个大小bucketSize的区间
        vector<pair<int, int>> buckets(bucketCount, {inf, -inf});
        for (int v : nums) {
            int i = (v - mi) / bucketSize; // 计算该值放入哪个桶
            buckets[i].first = min(buckets[i].first, v);
            buckets[i].second = max(buckets[i].second, v);
        }
        int ans = 0, prev = inf;
        for (auto [curmin, curmax] : buckets) {
            if (curmin > curmax) continue; // 这个桶中没有值
            ans = max(ans, curmin - prev);
            prev = curmax;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度 $O(n)$ 
- 空间复杂度 $O(n)$ 。其中 $n$ 为数组 $points$ 的长度。
