 <p>On a 2-dimensional&nbsp;<code>grid</code>, there are 4 types of squares:</p>

<ul>
	<li><code>1</code> represents the starting square.&nbsp; There is exactly one starting square.</li>
	<li><code>2</code> represents the ending square.&nbsp; There is exactly one ending square.</li>
	<li><code>0</code> represents empty squares we can walk over.</li>
	<li><code>-1</code> represents obstacles that we cannot walk over.</li>
</ul>

<p>Return the number of 4-directional walks&nbsp;from the starting square to the ending square, that <strong>walk over every non-obstacle square&nbsp;exactly once</strong>.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: [[1,0,0,0],[0,0,0,0],[0,0,2,-1]]
Output: 2
Explanation: We have the following two paths: 
1. (0,0),(0,1),(0,2),(0,3),(1,3),(1,2),(1,1),(1,0),(2,0),(2,1),(2,2)
2. (0,0),(1,0),(2,0),(2,1),(1,1),(0,1),(0,2),(0,3),(1,3),(1,2),(2,2)
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: [[1,0,0,0],[0,0,0,0],[0,0,0,2]]
Output: 4
Explanation: We have the following four paths: 
1. (0,0),(0,1),(0,2),(0,3),(1,3),(1,2),(1,1),(1,0),(2,0),(2,1),(2,2),(2,3)
2. (0,0),(0,1),(1,1),(1,0),(2,0),(2,1),(2,2),(1,2),(0,2),(0,3),(1,3),(2,3)
3. (0,0),(1,0),(2,0),(2,1),(2,2),(1,2),(1,1),(0,1),(0,2),(0,3),(1,3),(2,3)
4. (0,0),(1,0),(2,0),(2,1),(1,1),(0,1),(0,2),(0,3),(1,3),(1,2),(2,2),(2,3)
```
 
<p><strong>Example 3:</strong></p>

```cpp
Input: [[0,1],[2,0]]
Output: 0
Explanation: 
There is no path that walks over every empty square exactly once.
Note that the starting and ending square can be anywhere in the grid.
```
<p><strong>Note:</strong></p>

<ol>
	<li><code>1 &lt;= grid.length * grid[0].length &lt;= 20</code></li>
</ol>

题意：在二维网格 `grid` 上，有 `4` 种类型的方格：`1` 表示起始方格，且只有一个起始方格；`2` 表示结束方格，且只有一个结束方格；`0` 表示我们可以走过的空方格；`-1` 表示我们无法跨越的障碍。返回在四个方向（上、下、左、右）上行走时，从起始方格到结束方格的不同路径的数目，一条路径中需要通过每一个无障碍方格一次且仅一次。

---
### 解法 DFS+回溯
乍一看，这道题有点麻烦，但是其实很简单。由于数据范围实在太小，深搜+回溯即可解决这一题：
```cpp
class Solution {
private:
    int  m, n; // 网格行数, 网格列数
    //返回从(i,j)继续出发、遍历g中所有空格一次且仅一次的路径数目
    int dfs(vector<vector<int>>& g, int i, int j, int emptys) {
        if (i < 0 || i >= m || j < 0 || j >= n || g[i][j] == -1) return 0; //超出范围或遇到障碍
        if (g[i][j] == 2) return emptys <= 0 ? 1 : 0;
        int temp = g[i][j]; //此时temp可能为1或者0
        if (temp == 0) emptys -= 1;
        g[i][j] = -1; //标记访问
        int paths = dfs(g, i + 1, j, emptys) + dfs(g, i - 1, j, emptys) + 
                    dfs(g, i, j + 1, emptys) + dfs(g, i, j - 1, emptys);
        g[i][j] = temp; //回溯
        return paths;
    }
public:
    int uniquePathsIII(vector<vector<int>>& grid) {
        int emptys = 0, sx, sy; //空格个数, 起点坐标(sx, sy)
        m = grid.size(), n = grid[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 0) ++emptys;
                else if (grid[i][j] == 1) sx = i, sy = j; //起点
            }
        }
        return dfs(grid, sx, sy, emptys);
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.8 MB, 在所有 C++ 提交中击败了91.39% 的用户
```
 
