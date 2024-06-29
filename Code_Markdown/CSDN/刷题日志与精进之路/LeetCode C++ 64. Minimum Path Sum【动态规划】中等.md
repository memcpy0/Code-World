Given a `m x n` grid filled with non-negative numbers, find a path **from top left to bottom right** which `minimizes` the sum of all numbers along its path.

Note: You can only move either down or right at any point in time.

Example:
```cpp
Input:
[
  [1,3,1],
  [1,5,1],
  [4,2,1]
]
Output: 7
Explanation: Because the path 1→3→1→1→1 minimizes the sum.
```

题意：从左上角到右下角，求路径和最小值。

思路：很简单的动态规划题目，直接用原矩阵就行。

代码：
```cpp
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        int m = grid.size(), n = grid[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (!i && !j) continue;
                if (!i) grid[i][j] += grid[i][j - 1];
                else if (!j) grid[i][j] += grid[i - 1][j];
                else grid[i][j] += min(grid[i - 1][j], grid[i][j - 1]);
            }
        }
        return grid[m - 1][n - 1];
    }
};
```
效率：
```
执行用时：12 ms, 在所有 C++ 提交中击败了48.42% 的用户
内存消耗：8.1 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
