> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Given the coordinates of two <strong>rectilinear</strong> rectangles in a 2D plane, return <em>the total area covered by the two rectangles</em>.</p>

<p>The first rectangle is defined by its <strong>bottom-left</strong> corner <code>(ax1, ay1)</code> and its <strong>top-right</strong> corner <code>(ax2, ay2)</code>.</p>

<p>The second rectangle is defined by its <strong>bottom-left</strong> corner <code>(bx1, by1)</code> and its <strong>top-right</strong> corner <code>(bx2, by2)</code>.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 700px; height: 365px;" src="https://assets.leetcode.com/uploads/2021/05/08/rectangle-plane.png" alt="Rectangle Area">

```cpp
Input: ax1 = -3, ay1 = 0, ax2 = 3, ay2 = 4, bx1 = 0, by1 = -1, bx2 = 9, by2 = 2
Output: 45
```

<p><strong>Example 2:</strong></p>

```cpp
Input: ax1 = -2, ay1 = -2, ax2 = 2, ay2 = 2, bx1 = -2, by1 = -2, bx2 = 2, by2 = 2
Output: 16
```
<p><strong>Constraints:</strong></p>
<ul>
	<li><code>-10<sup>4</sup> &lt;= ax1, ay1, ax2, ay2, bx1, by1, bx2, by2 &lt;= 10<sup>4</sup></code></li>
</ul>


题意：给你 <strong>二维</strong> 平面上两个 <strong>由直线构成的</strong> 矩形，请你计算并返回两个矩形覆盖的总面积。每个矩形由其 <strong>左下</strong> 顶点和 <strong>右上</strong> 顶点坐标表示：</p>
<div class="MachineTrans-Lines">
<ul>
	<li class="MachineTrans-lang-zh-CN">第一个矩形由其左下顶点 <code>(ax1, ay1)</code> 和右上顶点 <code>(ax2, ay2)</code> 定义。</li>
	<li class="MachineTrans-lang-zh-CN">第二个矩形由其左下顶点 <code>(bx1, by1)</code> 和右上顶点 <code>(bx2, by2)</code> 定义。</li>
</ul>
</div>

 
---
### 解法 容斥原理
**判断两个水平矩形是否重叠，可以转为求两矩形在坐标轴上的重合长度**。若两矩形在 `x` 轴上的重合长度为 `lx` ，在 `y` 轴的重合长度为 `ly` ，且 `lx, ly` 都大于零，则两个矩形重叠，重叠面积为 `C` 。注意，若两个矩形在任一坐标轴上没有重合长度，则重叠面积为 `0` 。

然后直接算出两个给定矩形的面积 `A, B` ，利用「容斥原理」，`A + B` 减去交集面积 `C` 就是答案。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
        int lx = max(0, min(ax2, bx2) - max(ax1, bx1));
        int ly = max(0, min(ay2, by2) - max(ay1, by1));
        return (ax2 - ax1) * (ay2 - ay1) + (bx2 - bx1) * (by2 - by1) - lx * ly;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了90.76% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了23.68% 的用户
```
