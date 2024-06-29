> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>An axis-aligned rectangle is represented as a list <code>[x1, y1, x2, y2]</code>, where <code>(x1, y1)</code> is the coordinate of its bottom-left corner, and <code>(x2, y2)</code> is the coordinate of its top-right corner. Its top and bottom edges are parallel to the X-axis, and its left and right edges are parallel to the Y-axis.</p>

<p>Two rectangles overlap if the area of their intersection is <strong>positive</strong>. To be clear, two rectangles that only touch at the corner or edges do not overlap.</p>

<p>Given two axis-aligned rectangles <code>rec1</code> and <code>rec2</code>, return <code>true</code><em> if they overlap, otherwise return </em><code>false</code>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: rec1 = [0,0,2,2], rec2 = [1,1,3,3]
Output: true
```
<p><strong>Example 2:</strong></p>

```cpp
Input: rec1 = [0,0,1,1], rec2 = [1,0,2,1]
Output: false 
```
<p><strong>Example 3:</strong></p>

```cpp
Input: rec1 = [0,0,1,1], rec2 = [2,2,3,3]
Output: false 
```
<p><strong>Constraints:</strong></p>
<ul>
	<li><code>rect1.length == 4</code></li>
	<li><code>rect2.length == 4</code></li>
	<li><code>-10<sup>9</sup> &lt;= rec1[i], rec2[i] &lt;= 10<sup>9</sup></code></li>
	<li><code>rec1</code> and <code>rec2</code> represent a valid rectangle with a non-zero area.</li>
</ul>

题意：矩形以列表 <code>[x1, y1, x2, y2]</code> 的形式表示，其中 <code>(x1, y1)</code> 为左下角的坐标，<code>(x2, y2)</code> 是右上角的坐标。矩形的上下边平行于 x 轴，左右边平行于 y 轴。

<p>如果相交的面积为 <strong>正</strong> ，则称两矩形重叠。需要明确的是，只在角或边接触的两个矩形不构成重叠。给出两个矩形 <code>rec1</code> 和 <code>rec2</code> 。如果它们重叠，返回 <code>true</code>；否则，返回 <code>false</code> 。</p>

---
### 解法 容斥原理
**判断两个水平矩形是否重叠，可以转为求两矩形在坐标轴上的重合长度**。若两矩形在 `x` 轴上的重合长度为 `lx` ，在 `y` 轴的重合长度为 `ly` ，且 `lx, ly` 都大于零，则两个矩形重叠。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        int lx = min(rec1[2], rec2[2]) - max(rec1[0], rec2[0]);
        int ly = min(rec1[3], rec2[3]) - max(rec1[1], rec2[1]);
        return lx > 0 && ly > 0;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：7.7 MB, 在所有 C++ 提交中击败了5.19% 的用户
```
不求出重合长度的做法是，**正难则反**，先判断是否不相交（边重合也是不相交）——第一个矩形的 `x2` 小于等于第二个矩形的 `x1` 、第二个矩形的 `x2` 小于等于第一个矩形的 `x1` 、第一个矩形的 `y2` 小于等于第二个矩形的 `y1` 、第二个矩形的 `y2` 小于等于第一个矩形的 `y1` ，接着取反即可。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        return !(rec1[2] <= rec2[0] || rec2[2] <= rec1[0] || rec1[3] <= rec2[1] || rec2[3] <= rec1[1]);
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：7.6 MB, 在所有 C++ 提交中击败了35.81% 的用户
```
本题的进阶题目是：[223. Rectangle Area](https://leetcode-cn.com/problems/rectangle-area/)。
 

 
