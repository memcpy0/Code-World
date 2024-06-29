> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个大小为 `m x n` 的二进制矩阵 `grid` ，其中 `0` 表示一个海洋单元格、`1` 表示一个陆地单元格。

一次 **移动** 是指从一个陆地单元格走到另一个相邻（**上、下、左、右**）的陆地单元格或跨过 `grid` 的边界。

返回网格中 **无法** 在任意次数的移动中离开网格边界的陆地单元格的数量。

**示例 1：**

![](https://assets.leetcode.com/uploads/2021/02/18/enclaves1.jpg)

**输入：**grid = [[0,0,0,0],[1,0,1,0],[0,1,1,0],[0,0,0,0]]
**输出：**3
**解释：**有三个 1 被 0 包围。一个 1 没有被包围，因为它在边界上。

**示例 2：**

![](https://assets.leetcode.com/uploads/2021/02/18/enclaves2.jpg)

**输入：**grid = [[0,1,1,0],[0,0,1,0],[0,0,1,0],[0,0,0,0]]
**输出：**0
**解释：**所有 1 都在边界上或可以到达边界。

**提示：**

- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 500`
- `grid[i][j]` 的值为 `0` 或 `1`

```cpp
class Solution {
public:
    void dfs(vector<vector<int>>& A,int i,int j)
    {
        if(i<0||i>=A.size()||j<0||j>=A[i].size()||A[i][j]==0)
        {
            return;
        }
        A[i][j]=0;
        dfs(A,i-1,j);
        dfs(A,i+1,j);
        dfs(A,i,j-1);
        dfs(A,i,j+1);
    }
    
    int numEnclaves(vector<vector<int>>& A) {
        int ans = 0;
        for(int i=0;i<A.size();i++)
        {
            for(int j=0;j<A[i].size();j++)
            {
                if((i==0||j==0||i==A.size()-1||j==A[0].size()-1)&&A[i][j]==1)
                {
                    dfs(A,i,j);
                }
            }
        }
        for(int i=0;i<A.size();i++)
        {
            for(int j=0;j<A[0].size();j++)
            {
                if(A[i][j]==1)
                {
                    ans++;
                }
            }
        }
        return ans;
    }
};
```


```cpp
class Solution {
private:
    int n, m;
    vector<vector<bool>> vis;
    bool dfs(vector<vector<int>>& g, int i, int j, int &cnt) {
        if (i < 0 || i >= n || j < 0 || j >= m) return true; // 是能到边界
        if (vis[i][j]) return false;
        if (g[i][j] == 0) return false;
        else if (i == 0 || i == n - 1 || j == 0 || j == m - 1) return true; // 是边界陆地
        vis[i][j] = true;
        ++cnt;
        bool a = dfs(g, i + 1, j, cnt);
        bool b = dfs(g, i - 1, j, cnt);
        bool c = dfs(g, i, j - 1, cnt);
        bool d = dfs(g, i, j + 1, cnt);
        return a || b || c || d;
    }
public:
    int numEnclaves(vector<vector<int>>& grid) {
        n = grid.size(), m = grid[0].size();
        vis.resize(n, vector<bool>(m));
        int ans = 0;
        for (int i = 1; i < n - 1; ++i) {
            for (int j = 1; j < m - 1; ++j) {
                int a = 0;
                if (grid[i][j] && vis[i][j] == false && dfs(grid, i, j, a) == false) {
                    // cout << i << " " <<j << " " << a << endl;
                    ans += a;
                }
            }
        }
        return ans;
    }
};
```