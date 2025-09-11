<p>Given a non-empty 2D array <code>grid</code> of 0's and 1's, an <b>island</b> is a group of <code>1</code>'s (representing land) connected 4-directionally (horizontal or vertical.) You may assume all four edges of the grid are surrounded by water.</p>

<p>Find the maximum area of an island in the given 2D array. (If there is no island, the maximum area is 0.)</p>

<p><b>Example 1:</b></p>

```clike
[[0,0,1,0,0,0,0,1,0,0,0,0,0],
 [0,0,0,0,0,0,0,1,1,1,0,0,0],
 [0,1,1,0,1,0,0,0,0,0,0,0,0],
 [0,1,0,0,1,1,0,0,1,0,1,0,0],
 [0,1,0,0,1,1,0,0,1,1,1,0,0],
 [0,0,0,0,0,0,0,0,0,0,1,0,0],
 [0,0,0,0,0,0,0,1,1,1,0,0,0],
 [0,0,0,0,0,0,0,1,1,0,0,0,0]]
```
 
Given the above `grid` , return <code>6</code>. Note the answer is not 11, because the island must be connected 4-directionally.

<p><b>Example 2:</b></p>

```clike
[[0,0,0,0,0,0,0,0]]
```
Given the above `grid` , return `0` . 
 
<p><b>Note:</b> The length of each dimension in the given <code>grid</code> does not exceed 50.</p>

题意：求出给定的二维数组中最大的岛屿面积。

---
### 解法 DFS
用DFS可以很简单地解决这一题：
```cpp
class Solution {
private: 
    int m, n, maxArea;
    int dfs(vector<vector<int>>& grid, int i, int j) {
        if (i < 0 || i >= m || j < 0 || j >= n || !grid[i][j]) return 0;
        grid[i][j] = 0;
        return 1 + dfs(grid, i + 1, j) + dfs(grid, i - 1, j) + dfs(grid, i, j + 1) + dfs(grid, i, j - 1);
    }
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        if (grid.empty()) return 0;
        m = grid.size(), n = grid[0].size(), maxArea = 0;
        for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) if (grid[i][j] == 1) maxArea = max(maxArea, dfs(grid, i, j)); 
        return maxArea;
    }
};
```
运行效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了64.33% 的用户
内存消耗：21.8 MB, 在所有 C++ 提交中击败了81.36% 的用户
```
