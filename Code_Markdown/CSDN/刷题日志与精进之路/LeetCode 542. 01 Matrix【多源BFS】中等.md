> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个由 `0` 和 `1` 组成的矩阵 `mat` ，请输出一个大小相同的矩阵，其中每一个格子是 `mat` 中对应位置元素到最近的 `0` 的距离。

两个相邻元素间的距离为 `1` 。

**示例 1：**
![](https://pic.leetcode-cn.com/1626667201-NCWmuP-image.png)
```java
输入：mat = [[0,0,0],[0,1,0],[0,0,0]]
输出：[[0,0,0],[0,1,0],[0,0,0]]
```
**示例 2：**
![](https://pic.leetcode-cn.com/1626667205-xFxIeK-image.png)
```java
输入：mat = [[0,0,0],[0,1,0],[1,1,1]]
输出：[[0,0,0],[0,1,0],[1,2,1]]
```
**提示：**
- `m == mat.length`
- `n == mat[i].length`
- `1 <= m, n <= 10^4`
- `1 <= m * n <= 10^4`
- `mat[i][j] is either 0 or 1.`
- `mat` 中至少有一个 `0`

---
本题和「[1162.地图分析](https://leetcode-cn.com/problems/as-far-from-land-as-possible/)」 一样！那道题理解为需要找到每个 $0$ 最近的 $1$ ，而今天这道题是找每个 $1$ 最近的 $0$ 。
### 解法 多源BFS
首先把每个源点 $0$ 入队，然后从各个 $0$ 同时开始一圈一圈的向 $1$ 扩散（**每个 $1$ 都是被离它最近的 $0$ 扩散到的**），扩散时可以设置 `int[][] dist` 来记录距离（即扩散的层次）并同时标志是否访问过。对于本题，可以直接修改原数组 `int[][] matrix` 来**记录距离**和**标志是否访问**，这里要注意先把 $mat$ 数组中 $1$ 的位置设置成 $-1$ （设成 `Integer.MAX_VALUE` ，$m \times n,\ m + n$ 都行，只要是个无效的距离值来标志这个位置的 $1$ 没有被访问过就行）
```cpp
class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        int MAX_VALUE = m + n;
        queue<pair<int, int>> q;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (mat[i][j] == 0) q.push({i, j});
                else mat[i][j] = MAX_VALUE;
            }
        }
        int d[4][2] = {-1, 0, 1, 0, 0, -1, 0, 1};
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            for (int i = 0; i < 4; ++i) {
                int u = x + d[i][0], v = y + d[i][1];
                if (u >= 0 && u < m && v >= 0 && v < n && mat[x][y] + 1 < mat[u][v]) {
                    mat[u][v] = mat[x][y] + 1;
                    q.push({u, v});
                }
            }
        }
        return mat;
    }
};
```
复杂度分析：
- 时间复杂度：$O(mn)$
- 空间复杂度：虽然我们是直接修改原输入数组来存储结果，但最差的情况下即全都是 $0$ 时，需要把 $m * n$ 个 $0$ 都入队，因此空间复杂度是 $O(mn)$