Given an integer matrix, find the length of the longest increasing path.

From each cell, you can either move to four directions: left, right, up or down. You may NOT move diagonally or move outside of the boundary (i.e. wrap-around is not allowed).

**Example 1:**

```swift
Input: nums = 
[
  [9,9,4],
  [6,6,8],
  [2,1,1]
] 
Output: 4 
Explanation: The longest increasing path is [1, 2, 6, 9].
```

**Example 2:**

```swift
Input: nums = 
[
  [3,4,5],
  [3,2,6],
  [2,2,1]
] 
Output: 4 
Explanation: The longest increasing path is [3, 4, 5, 6]. Moving diagonally is not allowed.
```

题意：给定一个整数矩阵，找出最长递增路径的长度。对于每个单元格，可以往上、下、左、右四个方向移动，但是不能在对角线方向上移动或移动到边界外（即不允许环绕）。

---
### 解法 记忆化搜索
看到这道题目的第一反应是动态规划，但是用动态规划似乎有些不合适——当前点的最长递增路径长度，可以由上下左右分别递推而来。然而**动态规划需要按照一定的顺序进行更新**，那么在更新一个点的时候，它依靠的状态集一定有一些状态没有被更新。之后写好代码，果不其然没有成功。

正确做法是：将 `dp[][]` 换为递归函数，使用新的结果矩阵 `result[][]` 进行标记——如果已经处理过的话，那就相当于 `dp[][]` ，直接得到结果；如果这个点没有被处理过，就将递归函数当做 `dp[][]` 不断递归获取值。

由于最长递增路径可以从每个单元格出发，因此对每个单元格进行记忆化搜索——**如果已知当前单元格的最长递增路径长度，则返回；否则向上、下、左、右递归搜索，得到当前单元格的最长递增路径长度**。最后得到最长的递增路径长度。具体代码如下：
```cpp
class Solution {
private:
    vector<vector<int>> result;
    vector<vector<int>> temp;
    int n, m;
    int Move[4][2] = {1, 0, 0, 1, -1, 0, 0, -1};
    int dp(int x, int y) {
        if (result[x][y] != -1) return result[x][y]; //记忆化搜索的核心
        result[x][y] = 1; //最长递增路径只有自己,开始为1
        for (int i = 0; i < 4; ++i) { 
            int tx = x + Move[i][0], ty = y + Move[i][1];
            if (tx >= 0 && tx < n && ty >= 0 && ty < m && temp[tx][ty] < temp[x][y]) {
                result[x][y] = max(result[x][y], dp(tx, ty) + 1);
            }
        }
        return result[x][y];
    }
public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        if (matrix.empty()) return 0;
        n = matrix.size();
        m = matrix[0].size();
        temp = matrix;
        result.resize(n, vector<int>(m, -1));
        int maxLenOfIncreasingPath = 1;
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < m; ++j) 
                maxLenOfIncreasingPath = max(maxLenOfIncreasingPath, dp(i, j));
        return maxLenOfIncreasingPath;
    }
};
```
效率如下，比较低：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了72.40% 的用户
内存消耗：15 MB, 在所有 C++ 提交中击败了24.02% 的用户
```
