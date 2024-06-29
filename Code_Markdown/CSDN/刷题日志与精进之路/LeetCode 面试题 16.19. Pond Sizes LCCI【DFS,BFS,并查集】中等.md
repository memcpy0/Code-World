> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You have an integer matrix representing a plot of land, where the value at that loca­tion represents the height above sea level. A value of zero indicates water. A pond is a region of water connected vertically, horizontally, or diagonally. The size of the pond is the total number of connected water cells. Write a method to compute the sizes of all ponds in the matrix.

**Example:** 
```swift
Input: 
[
  [0,2,1,0],
  [0,1,0,1],
  [1,1,0,1],
  [0,1,0,1]
]
Output:  [1,2,4]
```
**Note:** 
- `0 < len(land) <= 1000`
-  `0 < len(land[i]) <= 1000`

题意：一个用于表示一片土地的整数矩阵 `land`，其中每个点的值都代表对应地点的海拔高度。若值为 `0` 则表示水域。由垂直、水平或对角连接的水域为池塘。池塘的大小是指相连接的水域的个数。计算矩阵中所有池塘的大小，返回值需要从小到大排序。 

---
### 解法1 DFS
带返回值的DFS模板。基本思路就是深度优先搜索，需要注意的是，这里不是传统的上下左右四个方向搜索，而是上、下、左、右、左上、右上、左下、右下共8个方向都搜索。

可以建立 `visited` 数组已标记访问过的位置，防止重复访问。越界也要提前返回。虽然也可以不用 `visited` 数组，直接修改原数组。

下面的实现中，我们遍历 `land` 矩阵，每找到一个 $0$ 值，就DFS求该 $0$ 值所属的池塘的大小，同时把该池塘的所有水域标记为 $1$ ，避免重复遍历。
```cpp
class Solution {
private:
    int m, n; 
    int dfs(vector<vector<int>>& land, int i, int j) {
        if (i < 0 || i >= m || j < 0 || j >= n || land[i][j] != 0) return 0;
        land[i][j] = 1;
        return 1 + dfs(land, i - 1, j) + dfs(land, i + 1, j) + dfs(land, i, j - 1) + dfs(land, i, j + 1)
        + dfs(land, i - 1, j - 1) + dfs(land, i - 1, j + 1) + dfs(land, i + 1, j - 1) + dfs(land, i + 1, j + 1);
    }
public:
    vector<int> pondSizes(vector<vector<int>>& land) { 
        m = land.size(), n = land[0].size(); 
        vector<int> ans;
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j) 
                if (land[i][j] == 0)
                    ans.push_back(dfs(land, i, j));
        sort(ans.begin(), ans.end());
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(mn\times \log mn)$ ，$m, n$ 是矩阵的行列数。深度优先搜索需要 $O(mn)$ 。最坏情况下，结果数组 $ans$ 的大小为 $mn$ 的量级，排序需要 $O(mn\times \log mn)$ 。
- 空间复杂度：$O(mn)$ 。返回值不计入空间复杂度，最坏情况下，深度优先搜索需要 $O(mn)$ 的栈空间。

---
### 解法2 BFS
同样，也可以使用广度优先搜索，统计所有池塘的大小。
```cpp
class Solution { 
public:
    vector<int> pondSizes(vector<vector<int>>& land) { 
        int m = land.size(), n = land[0].size(); 
        vector<int> ans;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (land[i][j] == 0) {
                    int area = 0;
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    land[i][j] = 1;
                    while (!q.empty()) {
                        auto [x, y] = q.front(); q.pop();
                        ++area;
                        for (int dx = -1; dx <= 1; ++dx) {
                            for (int dy = -1; dy <= 1; ++dy) {
                                if (dx == 0 && dy == 0) continue;
                                int tx = x + dx, ty = y + dy;
                                if (tx < 0 || tx >= m || ty < 0 || ty >= n || land[tx][ty] != 0) continue;
                                land[tx][ty] = 1;
                                q.push({tx, ty});
                            }

                        }
                    } 
                    ans.push_back(area);
                }
            }
        }
        sort(ans.begin(), ans.end());
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(mn\times \log mn)$ ，$m, n$ 是矩阵的行列数。广度优先搜索需要 $O(mn)$ 。最坏情况下，结果数组 $ans$ 的大小为 $mn$ 的量级，排序需要 $O(mn\times \log mn)$ 。
- 空间复杂度：$O(m+n)$ 。返回值不计入空间复杂度，最坏情况下，广度优先搜索的队列需要 $O(m+n)$ 的空间。

实际运行效果而言，DFS更胜BFS。