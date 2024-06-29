> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

骑士在一张 `n x n` 的棋盘上巡视。在有效的巡视方案中，骑士会从棋盘的 **左上角** 出发，并且访问棋盘上的每个格子 **恰好一次** 。

给你一个 `n x n` 的整数矩阵 `grid` ，由范围 `[0, n * n - 1]` 内的不同整数组成，其中 `grid[row][col]` 表示单元格 `(row, col)` 是骑士访问的第 `grid[row][col]` 个单元格。骑士的行动是从下标 **0** 开始的。

如果 `grid` 表示了骑士的有效巡视方案，返回 `true`；否则返回 `false`。

**注意**，骑士行动时可以垂直移动两个格子且水平移动一个格子，或水平移动两个格子且垂直移动一个格子。下图展示了骑士从某个格子出发可能的八种行动路线。  
![](https://assets.leetcode.com/uploads/2018/10/12/knight.png)

**示例 1：**
![](https://assets.leetcode.com/uploads/2022/12/28/yetgriddrawio-5.png)
```js
输入：grid = [[0,11,16,5,20],[17,4,19,10,15],[12,1,8,21,6],[3,18,23,14,9],[24,13,2,7,22]]
输出：true
解释：grid 如上图所示，可以证明这是一个有效的巡视方案。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2022/12/28/yetgriddrawio-6.png)
```js
输入：grid = [[0,3,6],[5,8,1],[2,7,4]]
输出：false
解释：grid 如上图所示，考虑到骑士第 7 次行动后的位置，第 8 次行动是无效的。
```
**提示：**
- `n == grid.length == grid[i].length`
- `3 <= n <= 7`
- `0 <= grid[row][col] < n * n`
- `grid` 中的所有整数 **互不相同**

---
### 解法 直接模拟
题目要求骑士的移动的每一步均按照「日」字形跳跃，假设从位置 $(x_1, y_1)$ 跳跃到 $(x_2, y_2)$ ，则此时一定满足下面两种情形之一：

$$|x_1 - x_2| = 1, |y_1 - y_2| = 2$$
​
设矩阵的长度为 $n$ ，其中 $grid[row][col]$ 表示单元格$(row,col)$是骑士访问的第 $grid[row][col]$ 个单元格，因此可以知道每个单元格的访问顺序，我们用 $indices$ 存储单元格的访问顺序，其中 $indices[i]$ 表示骑士在经过第 $i-1$ 次跳跃后的位置。

由于骑士的行动是从下标 $0$ 开始的，因此一定需要满足 $grid[0][0]=0$ ，接下来依次遍历 $indices$ 中的每个元素。由于 $indices[i]$ 是一次跳跃的起点，$indices[i+1]$ 是该次跳跃的终点，则依次检测每一次跳跃的行动路径是否为「日」字形，即满足如下条件：
- $|\textit{indices}[i][0] - \textit{indices}[i+1][0]| = 1, |\textit{indices}[i][1] - \textit{indices}[i+1][1]| = 2∣$ ；
- $|\textit{indices}[i][0] - \textit{indices}[i+1][0]| = 2, |\textit{indices}[i][1] - \textit{indices}[i+1][1]| = 1∣$ 。

为了方便计算，我们只需检测 $|x_1 - x_2| \times |y_1 - y_2|$ ​是否等于 $2$ 即可。如果所有跳跃路径均合法则返回 $\text{true}$ ，否则返回 $\text{false}$ 。
```cpp
class Solution {
public:
    bool checkValidGrid(vector<vector<int>>& grid) {
        if (grid[0][0] != 0) return false;
        int n = grid.size();
        vector<array<int, 2>> indices(n * n);
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < n; ++j)
                indices[grid[i][j]] = {i, j};
        for (int i = 1; i < indices.size(); ++i) {
            int dx = abs(indices[i][0] - indices[i - 1][0]);
            int dy = abs(indices[i][1] - indices[i - 1][1]);
            if (dx * dy != 2) return false;
        }
        return true;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$ ，其中 $n$ 表示二维棋盘边的长度。需要检测棋盘中的每个位置，一共需要检测 $n^2$ 个位置，因此时间复杂度为 $O(n^2)$ 。
- 空间复杂度：$O(n^2)$ ，其中 $n$ 表示二维棋盘边的长度。用来需要存放每个位置的访问顺序，一共有 $n^2$ 个位置，需要的空间为 $O(n^2)$ 。