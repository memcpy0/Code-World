> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在一个 `m * n` 的棋盘的每一格都放有一个礼物，每个礼物都有一定的价值（价值大于 0）。你可以从棋盘的左上角开始拿格子里的礼物，并每次向右或者向下移动一格、直到到达棋盘的右下角。给定一个棋盘及其上面的礼物的价值，请计算你最多能拿到多少价值的礼物？

**示例 1:**
```java
输入: 
[
  [1,3,1],
  [1,5,1],
  [4,2,1]
]
输出: 12
解释: 路径 1→3→5→2→1 可以拿到最多价值的礼物
```
**提示：**
- `0 < grid.length <= 200`
- `0 < grid[0].length <= 200`

---
### 解法 动态规划
```cpp
class Solution {
public:
    int maxValue(vector<vector<int>>& grid) {
        if (grid.empty()) return 0;
        int m = grid.size(), n = grid[0].size();
        vector<vector<int>> dp(m, vector<int>(n));
        //dp[i][j]表示从grid[0][0]到grid[i][j]得到的最大价值
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == 0 && j == 0) dp[i][j] = grid[i][j];
                else if (i == 0) dp[i][j] = dp[i][j - 1] + grid[i][j];
                else if (j == 0) dp[i][j] = dp[i - 1][j] + grid[i][j];
                else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
            }
        }
        return dp[m - 1][n - 1];
    }
};
```
一次性通过，运行效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了85.59% 的用户
内存消耗：9.6 MB, 在所有 C++ 提交中击败了28.22% 的用户
```
类似于01背包，空间优化如下：
```cpp
class Solution {
public:
    int maxValue(vector<vector<int>>& grid) {
        if (grid.empty()) return 0;
        int m = grid.size(), n = grid[0].size();
        vector<vector<int>> dp(2, vector<int>(n));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == 0 && j == 0) dp[i & 1][j] = grid[i][j];
                else if (i == 0) dp[i & 1][j] = dp[i & 1][j - 1] + grid[i][j];
                else if (j == 0) dp[i & 1][j] = dp[!(i & 1)][j] + grid[i][j];
                else dp[i & 1][j] = max(dp[!(i & 1)][j], dp[i & 1][j - 1]) + grid[i][j];
            }
        }
        return max(dp[0][n - 1], dp[1][n - 1]);
    }
};
```
更简洁的写法如下：
```cpp
class Solution {
public:
    int maxValue(vector<vector<int>>& grid) {
        if (grid.empty()) return 0;
        int m = grid.size(), n = grid[0].size();
        vector<vector<int>> dp(2, vector<int>(n + 1)); //空间+1
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                dp[i & 1][j + 1] = max(dp[!(i & 1)][j + 1], dp[i & 1][j]) + grid[i][j];
        return max(dp[0][n], dp[1][n]);
    }
};
```
运行效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了98.51% 的用户
内存消耗：9.2 MB, 在所有 C++ 提交中击败了77.27% 的用户
```
继续优化空间：
```cpp
class Solution {
public:
    int maxValue(vector<vector<int>>& grid) {
        if (grid.empty()) return 0;
        int m = grid.size(), n = grid[0].size();
        vector<int> dp(n + 1);
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                dp[j + 1] = max(dp[j], dp[j + 1]) + grid[i][j];
        return dp[n];
    }
};
```
运行效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了98.51% 的用户
内存消耗：9.2 MB, 在所有 C++ 提交中击败了77.27% 的用户
```
