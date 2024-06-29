> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

一场比赛中共有 `n` 支队伍，按从 `0` 到  `n - 1` 编号。

给你一个下标从 **0** 开始、大小为 `n * n` 的二维布尔矩阵 `grid` 。对于满足 `0 <= i, j <= n - 1` 且 `i != j` 的所有 `i, j` ：如果 `grid[i][j] == 1`，那么 `i` 队比 `j` 队 **强** ；否则，`j` 队比 `i` 队 **强** 。

在这场比赛中，如果不存在某支强于 `a` 队的队伍，则认为 `a` 队将会是 **冠军** 。

返回这场比赛中将会成为冠军的队伍。

**示例 1：**
```js
输入：grid = [[0,1],[0,0]]
输出：0
解释：比赛中有两支队伍。
grid[0][1] == 1 表示 0 队比 1 队强。所以 0 队是冠军。
```
**示例 2：**
```js
输入：grid = [[0,0,1],[1,0,1],[0,0,0]]
输出：1
解释：比赛中有三支队伍。
grid[1][0] == 1 表示 1 队比 0 队强。
grid[1][2] == 1 表示 1 队比 2 队强。
所以 1 队是冠军。
```
**提示：**
- `n == grid.length`
- `n == grid[i].length`
- `2 <= n <= 100`
- `grid[i][j]` 的值为 `0` 或 `1`
- 对于满足 `i != j` 的所有 `i, j` ，`grid[i][j] != grid[j][i]` 均成立
- 生成的输入满足：如果 `a` 队比 `b` 队强，`b` 队比 `c` 队强，那么 `a` 队比 `c` 队强

---
### 解法 寻找全为 $0$ 的列
这道题其实就是找有没有**入度为零的点**，可以统计所有点的入度，但这需要 $O(n)$ 的空间。

不妨考虑，【入度为零的点】对应到邻接矩阵中是什么情况——就是**某一列都为 $0$ 、或某一行都为 $1$**（不考虑自己到自己的入边和出边，即 $grid[i][i]$ 的值不被考虑）。
```cpp
class Solution {
public:
    int findChampion(vector<vector<int>>& grid) {
        for (int i = 0, n = grid.size(); i < n; ++i) {
            bool ok = true;
            for (int j = 0; j < n; ++j) {
                if (i != j && grid[j][i] == 1) {
                    ok = false;
                    break;
                }
            }
            if (ok) return i;
        }
        return -1;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n^2)$ ，其中 $n$ 为 $\textit{grid}$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。Python 可以把切片改成手动枚举。

https://leetcode.cn/problems/find-champion-i/solutions/2513136/jian-ji-xie-fa-by-endlesscheng-wry2/