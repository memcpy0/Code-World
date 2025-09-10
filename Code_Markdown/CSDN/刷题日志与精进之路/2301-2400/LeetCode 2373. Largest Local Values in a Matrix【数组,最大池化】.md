> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given an `n x n` integer matrix `grid`.

Generate an integer matrix `maxLocal` of size `(n - 2) x (n - 2)` such that:

-   `maxLocal[i][j]` is equal to the **largest** value of the `3 x 3` matrix in `grid` centered around row `i + 1` and column `j + 1`.

In other words, we want to find the largest value in every contiguous `3 x 3` matrix in `grid`.

Return _the generated matrix_.

**Example 1:**
![](https://assets.leetcode.com/uploads/2022/06/21/ex1.png)
```java
Input: grid = [[9,9,8,1],[5,6,2,6],[8,2,6,4],[6,2,2,2]]
Output: [[9,9],[8,6]]
Explanation: The diagram above shows the original matrix and the generated matrix.
Notice that each value in the generated matrix corresponds to the largest value of a contiguous 3 x 3 matrix in grid.
```

**Example 2:**
![](https://assets.leetcode.com/uploads/2022/07/02/ex2new2.png)
```java
Input: grid = [[1,1,1,1,1],[1,1,1,1,1],[1,1,2,1,1],[1,1,1,1,1],[1,1,1,1,1]]
Output: [[2,2,2],[2,2,2],[2,2,2]]
Explanation: Notice that the 2 is contained within every contiguous 3 x 3 matrix in grid.
```
**Constraints:**
-   `n == grid.length == grid[i].length`
-   `3 <= n <= 100`
-   `1 <= grid[i][j] <= 100`

题意：给定一个 `n x n` 的矩阵 `grid` ，返回一个 `(n - 2) x (n - 2)` 的矩阵 `maxLocal` ，其中 `maxLocal[i][j]` 是以 `grid[i][j]` 为中心、`3 x 3` 矩阵中的最大元素。

---
### 解法1 数组+模拟
暴力模拟，做法如下：
```cpp
class Solution {
private:
    int maxIn3x3(vector<vector<int>>& g, int i, int j) {
        int mx = 0;
        for (int x = i - 1; x < i + 2; ++x)
            for (int y = j - 1; y < j + 2; ++y)
                if (g[x][y] > mx) mx = g[x][y];
        return mx;
    }
public:
    vector<vector<int>> largestLocal(vector<vector<int>>& grid) {
        vector<vector<int>> ans(grid.size() - 2, vector<int>(grid.size() - 2));
        for (int i = 1, n = grid.size() - 1; i < n; ++i) 
            for (int j = 1; j < n; ++j) 
                ans[i - 1][j - 1] = maxIn3x3(grid, i , j);
        return ans;
    }
};
```
如果要优化一下的话，我们发现，在求 `grid[i][j] (i >= 1,j >= 2)` 周围 `3 x 3` 矩阵的最大值时，往往会重复比较「前一 `3 x 3` 矩阵比较过的两列」。如果我们直接重用前面 `3 x 3` 矩阵比较得到的结果，就会把没有比较的区域的值纳入进来。为此可以如下编写代码：
```cpp
class Solution { 
public:
    vector<vector<int>> largestLocal(vector<vector<int>>& grid) {
        int n = grid.size(), colMax[3];
        vector<vector<int>> ans(n - 2, vector<int>(n - 2));
        for (int i = 0; i < n - 2; ++i) {
            for (int k = 0; k <= 2; ++k)
                colMax[k] = max({ grid[i][k], grid[i + 1][k], grid[i + 2][k] });
            ans[i][0] = max({ colMax[0], colMax[1], colMax[2] });
            for (int j = 1; j < n - 2; ++j) {
                colMax[0] = colMax[1], colMax[1] = colMax[2];
                colMax[2] = max({ grid[i][j + 2], grid[i + 1][j + 2], grid[i + 2][j + 2] });
                ans[i][j] = max({ colMax[0], colMax[1], colMax[2] });
            }
        }
        return ans;
    }
};
```
