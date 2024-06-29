> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>There is a city composed of <code>n x n</code> blocks, where each block contains a single building shaped like a vertical square prism. You are given a <strong>0-indexed</strong> <code>n x n</code> integer matrix <code>grid</code> where <code>grid[r][c]</code> represents the <strong>height</strong> of the building located in the block at row <code>r</code> and column <code>c</code>.</p>

<p>A city's <strong>skyline</strong> is the the outer contour formed by all the building when viewing the side of the city from a distance. The <strong>skyline</strong> from each cardinal direction north, east, south, and west may be different.</p>

<p>We are allowed to increase the height of <strong>any number of buildings by any amount</strong> (the amount can be different per building). The height of a <code>0</code>-height building can also be increased. However, increasing the height of a building should <strong>not</strong> affect the city's <strong>skyline</strong> from any cardinal direction.</p>

<p>Return <em>the <strong>maximum total sum</strong> that the height of the buildings can be increased by <strong>without</strong> changing the city's <strong>skyline</strong> from any cardinal direction</em>.</p>


<p><strong>Example 1:</strong></p>
<img width="55%" src="https://assets.leetcode.com/uploads/2021/06/21/807-ex1.png" alt="">

```cpp
Input: grid = [[3,0,8,4],[2,4,5,7],[9,2,6,3],[0,3,1,0]]
Output: 35
Explanation: The building heights are shown in the center of the above image.
The skylines when viewed from each cardinal direction are drawn in red.
The grid after increasing the height of buildings without affecting skylines is:
gridNew = [ [8, 4, 8, 7],
            [7, 4, 7, 7],
            [9, 4, 8, 7],
            [3, 3, 3, 3] ]
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: grid = [[0,0,0],[0,0,0],[0,0,0]]
Output: 0
Explanation: Increasing the height of any building will result in the skyline changing.
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == grid.length</code></li>
	<li><code>n == grid[r].length</code></li>
	<li><code>2 &lt;= n &lt;= 50</code></li>
	<li><code>0 &lt;= grid[r][c] &lt;= 100</code></li>
</ul>

题意：给你一座由 <code>n x n</code> 个街区组成的城市，每个街区都包含一座立方体建筑。给你一个下标从 <strong>0</strong> 开始的 <code>n x n</code> 整数矩阵 <code>grid</code> ，其中 <code>grid[r][c]</code> 表示坐落于 <code>r</code> 行 <code>c</code> 列的建筑物的 <strong>高度</strong> 。</p>

<p>城市的 <strong>天际线</strong> 是从远处观察城市时，所有建筑物形成的外部轮廓。从东、南、西、北四个主要方向观测到的 <strong>天际线</strong> 可能不同。</p>

<p>我们被允许为 <strong>任意数量的建筑物 </strong>的高度增加<strong> 任意增量（不同建筑物的增量可能不同）</strong> 。 高度为 <code>0</code> 的建筑物的高度也可以增加。然而，增加的建筑物高度 <strong>不能影响</strong> 从任何主要方向观察城市得到的 <strong>天际线</strong> 。</p>

<p>在 <strong>不改变</strong> 从任何主要方向观测到的城市 <strong>天际线</strong> 的前提下，返回建筑物可以增加的 <strong>最大高度增量总和</strong> 。</p>
 
---
### 解法 贪心
根据题意，我们要确保在调整建筑物高度后，从「水平」和「垂直」方向上看到的「各行」和「各列」的最大高度不变。

为此，先要通过 $O(n^2)$ 的复杂度预处理出 `grid` 中每行的最大值（用 `lr[]` 存储），以及每列的最大值（用 `ud[]` 存储）。然后再用一个二重循环，判断当前位置的高度 `grid[i][j]` 与 `min(lr[i], ud[j])` 的关系，决定当前位置可以增高多少——即 `min(lr[i], ud[j]) - grid[i][j]` 。

可以证明，**当每个位置都取最大的增高高度（局部最优）时，总的增高高度最大（全局最优）**。因此，本题是一道简单的贪心题。
```cpp
//C++ version
class Solution {
public:
    int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
        vector<int> ud, lr;
        int n = grid.size(), ans = 0;
        for (int i = 0; i < n; ++i) { //得到原始数组的天际线
            int colMaxVal = 0;
            for (int j = 0; j < n; ++j) colMaxVal = max(colMaxVal, grid[j][i]); //得到这一列的最大值
            ud.push_back(colMaxVal);
            lr.push_back(*max_element(grid[i].begin(), grid[i].end())); //得到这一行的最大值
        }
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) 
        	ans += min(lr[i], ud[j]) - grid[i][j]; 
        return ans;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了96.97% 的用户
//内存消耗：9.4 MB, 在所有 C++ 提交中击败了97.59% 的用户
```
