

Given an `m x n` 2d `grid` map of `'1'`s (land) and `'0'`s (water), return *the number of islands*.

An **island** is surrounded by water and is formed by connecting adjacent lands horizontally or vertically. You may assume all four edges of the grid are all surrounded by water.

 

**Example 1:**

```swift
Input: grid = [
  ["1","1","1","1","0"],
  ["1","1","0","1","0"],
  ["1","1","0","0","0"],
  ["0","0","0","0","0"]
]
Output: 1
```

**Example 2:**

```swift
Input: grid = [
  ["1","1","0","0","0"],
  ["1","1","0","0","0"],
  ["0","0","1","0","0"],
  ["0","0","0","1","1"]
]
Output: 3
```

 

**Constraints:**
- `m == grid.length`
- `n == grid[i].length`
 - `1 <= m, n <= 300`
 - `grid[i][j]` is `'0'` or `'1'` .

题意：给出一个由 `'1'`（陆地）和 `'0'`（水）组成的二维网格，请你计算网格中岛屿的数量。岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。

---
### 解法1 DFS
对二维网格中的每个值为 `1` 且不为已知岛屿的单元格进行DFS：
```cpp
class Solution {
private:
    vector<vector<bool>> vis;
    int m, n;
    void dfs(const vector<vector<char>>& grid, int i, int j) {
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == '0' || vis[i][j]) return;
        vis[i][j] = true;
        dfs(grid, i + 1, j);
        dfs(grid, i - 1, j);
        dfs(grid, i, j - 1);
        dfs(grid, i, j + 1);
    }
public:
    int numIslands(vector<vector<char>>& grid) {
        m = grid.size(), n = grid[0].size();
        int islands = 0;
        vis.resize(m, vector<bool>(n, false));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (vis[i][j] == false && grid[i][j] == '1') {
                    dfs(grid, i, j);
                    ++islands;
                }
            }
        }
        return islands;
    }
};
```
执行效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了74.97% 的用户
内存消耗：10 MB, 在所有 C++ 提交中击败了29.30% 的用户
```
---
### 解法2 BFS
```cpp
class Solution { 
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size(), islands = 0;
        int Moves[][2] = {0, 1, 0, -1, 1, 0, -1, 0};
        vector<vector<bool>> vis(m, vector<bool>(n, false));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (vis[i][j] == false && grid[i][j] == '1') {
                    ++islands; //岛屿数量+1
                    queue<pair<int, int>> q; //BFS这个岛屿
                    q.push({i, j});
                    vis[i][j] = true;
                    while (!q.empty()) { 
                        pair<int, int> t = q.front(); q.pop();
                        for (int b = 0; b < 4; ++b) { //遍历四个方向
                            int tx = t.first + Moves[b][0], ty = t.second + Moves[b][1];
                            if (tx >= 0 && tx < m && ty >= 0 && ty < n && 
                                grid[tx][ty] == '1' && !vis[tx][ty]) {
                                q.push({tx, ty});
                                vis[tx][ty] = true;
                            }
                        } 
                    }
                }
            }
        }
        return islands;
    }
};
```
执行用时和内存消耗如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了74.28% 的用户
内存消耗：10.6 MB, 在所有 C++ 提交中击败了10.55% 的用户
```
