> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 `m x n` 大小的矩阵 `grid` ，由若干正整数组成。

执行下述操作，直到 `grid` 变为空矩阵：
- 从每一行删除值最大的元素。如果存在多个这样的值，删除其中任何一个。
- 将删除元素中的最大值与答案相加。

**注意** 每执行一次操作，矩阵中列的数据就会减 1 。

返回执行上述操作后的答案。

**示例 1：**
![](https://assets.leetcode.com/uploads/2022/10/19/q1ex1.jpg)

```js
输入：grid = [[1,2,4],[3,3,1]]
输出：8
解释：上图展示在每一步中需要移除的值。
- 在第一步操作中，从第一行删除 4 ，从第二行删除 3（注意，有两个单元格中的值为 3 ，我们可以删除任一）。在答案上加 4 。
- 在第二步操作中，从第一行删除 2 ，从第二行删除 3 。在答案上加 3 。
- 在第三步操作中，从第一行删除 1 ，从第二行删除 1 。在答案上加 1 。
最终，答案 = 4 + 3 + 1 = 8 。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2022/10/19/q1ex2.jpg)
```js
输入：grid = [[10]]
输出：10
解释：上图展示在每一步中需要移除的值。
- 在第一步操作中，从第一行删除 10 。在答案上加 10 。
最终，答案 = 10 。
```
**提示：**
- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 50`
- `1 <= grid[i][j] <= 100`

---
### 解法 排序
将题目给出大小为 $m \times n$ 的矩阵 $\textit{grid}$ 每一行从小到大排序，那么**题目等价于每次删除矩阵的末尾列，得分为该列的最大值**。那么最后的答案就是每一列的最大值之和。
```cpp
class Solution {
public:
    int deleteGreatestValue(vector<vector<int>>& grid) { 
        int m = grid.size(), n = grid[0].size();
        for (int i = 0; i < m; ++i) sort(grid[i].begin(), grid[i].end());
        int ans = 0;
        for (int i = n - 1; i >= 0; --i) { // 每列
            int mx = grid[0][i];
            for (int j = 1; j < m; ++j) mx = max(mx, grid[j][i]);
            ans += mx;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(m \times n \times \log n)$ ，其中 $m, n$ 分别为矩阵 $\textit{grid}$ 的行列数，对矩阵 $\textit{grid}$ 的每一行排序的时间复杂度为 $n \times \log n$ ，共有 $m$ 行，所以总的时间复杂度为 $O(m \times n \times \log n)$ 。
- 空间复杂度：$O(\log n)$ ，排序需要的栈开销。