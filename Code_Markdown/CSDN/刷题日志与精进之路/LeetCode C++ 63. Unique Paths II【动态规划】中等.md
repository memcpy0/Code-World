A robot is located at the top-left corner of a `m x n` grid (marked 'Start' in the diagram below).

The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).

Now consider if some obstacles are added to the grids. How many unique paths would there be?

An obstacle and empty space is marked as `1` and `0` respectively in the grid.

Note: m and n will be at most `100`.

Example 1:
```cpp
Input:
[
  [0,0,0],
  [0,1,0],
  [0,0,0]
]
Output: 2
Explanation:
There is one obstacle in the middle of the 3x3 grid above.
There are two ways to reach the bottom-right corner:
1. Right -> Right -> Down -> Down
2. Down -> Down -> Right -> Right
```

题意：从左上角走到右下角，如果有障碍，总走法是多少。

思路：还是动态规划，如果是障碍，`dp` 矩阵中该位置的走法设为 `0` 即可。和洛谷的【过河卒】很像。

代码：
```cpp
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        if (obstacleGrid.empty() || obstacleGrid[0].empty()) return 0;
        const auto &v = obstacleGrid;
        int m = v.size(), n = v[0].size();
        vector<vector<long long>> dp(m, vector<long long>(n, 0));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {  
                if (v[i][j] == 1) dp[i][j] = 0; //设置障碍
                else {
                    if (!i && !j) dp[0][0] = 1; //从(0,0)开始
                    else if (!i) dp[i][j] = dp[i][j - 1]; //第0行
                    else if (!j) dp[i][j] = dp[i - 1][j]; //第0列
                    else dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
                }
            }
        }
        return dp[m - 1][n - 1];
    }
};
```
效率：
```
执行用时：8 ms, 在所有 C++ 提交中击败了30.49% 的用户
内存消耗：8 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
