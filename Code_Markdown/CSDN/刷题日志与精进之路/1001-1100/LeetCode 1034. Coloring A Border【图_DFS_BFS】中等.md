> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>You are given an <code>m x n</code> integer matrix <code>grid</code>, and three integers <code>row</code>, <code>col</code>, and <code>color</code>. Each value in the grid represents the color of the grid square at that location.</p>

<p>Two squares belong to the same <strong>connected component</strong> if they have the same color and are next to each other in any of the 4 directions.</p>

<p>The <strong>border of a connected component</strong> is all the squares in the connected component that are either <strong>4-directionally</strong> adjacent to a square not in the component, or on the boundary of the grid (the first or last row or column).</p>

<p>You should color the <strong>border</strong> of the <strong>connected component</strong> that contains the square <code>grid[row][col]</code> with <code>color</code>.</p>

<p>Return <em>the final grid</em>.</p>


<p><strong>Example 1:</strong></p>

```cpp
Input: grid = [[1,1],[1,2]], row = 0, col = 0, color = 3
Output: [[3,3],[3,2]]
```
<p><strong>Example 2:</strong></p>

```cpp
Input: grid = [[1,2,2],[2,3,2]], row = 0, col = 1, color = 3
Output: [[1,3,3],[2,3,3]]
```
<p><strong>Example 3:</strong></p>

```cpp
Input: grid = [[1,1,1],[1,1,1],[1,1,1]], row = 1, col = 1, color = 2
Output: [[2,2,2],[2,1,2],[2,2,2]]
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>m == grid.length</code></li>
	<li><code>n == grid[i].length</code></li>
	<li><code>1 &lt;= m, n &lt;= 50</code></li>
	<li><code>1 &lt;= grid[i][j], color &lt;= 1000</code></li>
	<li><code>0 &lt;= row &lt; m</code></li>
	<li><code>0 &lt;= col &lt; n</code></li>
</ul>


题意：给出一个二维整数网格 `grid`，网格中的每个值表示该位置处的网格块的颜色。只有当两个网格块的颜色相同，而且在四个方向中任意一个方向上相邻时，它们才属于同一**连通分量**。

**连通分量的边界**是指该连通分量中所有与不在它之中的格子相邻（四个方向上）的所有网格，或者在网格的边界上（第一行/列或最后一行/列）的所有格子。

给出位于 `(r0, c0)` 的网格和颜色 `color`，使用指定颜色 `color` 为给定网格**所属的连通分量的边界**进行着色，并返回最终的网格 `grid` 。

 
---
### 解法 DFS
一个小的优化是，如果所给网格的颜色等于 `color` ，则直接返回原 `grid` 。然后递归，发现网格所属连通分量的边界后，进行着色即可。具体代码如下：
```cpp
class Solution {
private:
    int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0}, n, m; //左右上下
    vector<vector<bool>> vis;
    void dfs(vector<vector<int>>& g, int r, int c, int oldColor, int newColor) {
        vis[r][c] = true;
        for (int i = 0; i < 4; ++i) {
            int tr = r + step[i][0], tc = c + step[i][1];
            if (tr >= 0 && tr < n && tc >= 0 && tc < m) {
                if (vis[tr][tc]) continue; //已经访问过
                if (g[tr][tc] != oldColor) { g[r][c] = newColor; continue; } //边界1
                else dfs(g, tr, tc, oldColor, newColor);
            } else g[r][c] = newColor; //边界2(为矩阵边缘)
        }
    }
public:
    vector<vector<int>> colorBorder(vector<vector<int>>& grid, int r0, int c0, int color) {
        if (grid[r0][c0] == color) return grid; //颜色相同,直接返回
        n = grid.size(), m = grid[0].size();
        vis.resize(n, vector<bool>(m, false)); //避免重复访问
        dfs(grid, r0, c0, grid[r0][c0], color);
        return grid;
    }
};
```
效率比较低：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了35.06% 的用户
内存消耗：13 MB, 在所有 C++ 提交中击败了29.63% 的用户
```
