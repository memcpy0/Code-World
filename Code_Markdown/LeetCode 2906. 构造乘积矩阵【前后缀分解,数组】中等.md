> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始、大小为 `n * m` 的二维整数矩阵 `grid` ，定义一个下标从 **0** 开始、大小为 `n * m` 的的二维矩阵 `p`。如果满足以下条件，则称 `p` 为 `grid` 的 **乘积矩阵** ：
- 对于每个元素 `p[i][j]` ，它的值等于除了 `grid[i][j]` 外所有元素的乘积。乘积对 `12345` 取余数。

返回 `grid` 的乘积矩阵。

**示例 1：**
```js
输入：grid = [[1,2],[3,4]]
输出：[[24,12],[8,6]]
解释：p[0][0] = grid[0][1] * grid[1][0] * grid[1][1] = 2 * 3 * 4 = 24
p[0][1] = grid[0][0] * grid[1][0] * grid[1][1] = 1 * 3 * 4 = 12
p[1][0] = grid[0][0] * grid[0][1] * grid[1][1] = 1 * 2 * 4 = 8
p[1][1] = grid[0][0] * grid[0][1] * grid[1][0] = 1 * 2 * 3 = 6
所以答案是 [[24,12],[8,6]] 。
```
**示例 2：**
```js
输入：grid = [[12345],[2],[1]]
输出：[[2],[0],[0]]
解释：p[0][0] = grid[0][1] * grid[0][2] = 2 * 1 = 2
p[0][1] = grid[0][0] * grid[0][2] = 12345 * 1 = 12345. 12345 % 12345 = 0 ，所以 p[0][1] = 0
p[0][2] = grid[0][0] * grid[0][1] = 12345 * 2 = 24690. 24690 % 12345 = 0 ，所以 p[0][2] = 0
所以答案是 [[2],[0],[0]] 。
```
**提示：**
- `1 <= n == grid.length <= 10^5`
- `1 <= m == grid[i].length <= 10^5`
- `2 <= n * m <= 10^5`
- `1 <= grid[i][j] <= 10^9`

---
前后缀分解（右边的数字为难度分）
- [238. 除自身以外数组的乘积](https://leetcode.cn/problems/product-of-array-except-self/) **和本题几乎一样**
- [剑指Offer66. 构建乘积数组](https://leetcode.cn/problems/gou-jian-cheng-ji-shu-zu-lcof/) 和本题几乎一样
- [2256. 最小平均差](https://leetcode.cn/problems/minimum-average-difference/) 1395
- [2483. 商店的最少代价](https://leetcode.cn/problems/minimum-penalty-for-a-shop/) 1495
- [2420. 找到所有好下标](https://leetcode.cn/problems/find-all-good-indices/) 1695
- [2167. 移除所有载有违禁货物车厢所需的最少时间](https://leetcode.cn/problems/minimum-time-to-remove-all-cars-containing-illegal-goods/) 2219
- [2484. 统计回文子序列数目](https://leetcode.cn/problems/count-palindromic-subsequences/) 2223
- [2565. 最少得分子序列](https://leetcode.cn/problems/subsequence-with-the-minimum-score/) 2432
- [2552. 统计上升四元组](https://leetcode.cn/problems/count-increasing-quadruplets/) 2433
- [42. 接雨水](https://leetcode.cn/problems/trapping-rain-water/)
### 解法 前后缀分解
> 这题直接展开就是238原题。但这题数据没有 $0$ ，用除法的诱惑实在太大……只是 $12345=3 \times 5 \times 823$ 不是素数，逆元不一定存在，得用扩展欧几里得……

核心思想：把矩阵拉成一维的，我们需要算出每个数左边所有数的乘积，以及右边所有数的乘积，这都可以用递推得到。

先算出从 $grid[i][j]$ 的下一个元素开始，到最后一个元素 $grid[n−1][m−1]$ 的乘积，记作 $suf[i][j]$ 。这可以从最后一行最后一列开始，倒着遍历得到。

然后算出从第一个元素 $grid[0][0]$ 开始，到 $grid[i][j]$ 的上一个元素的乘积，记作 $pre[i][j]$ 。这可以从第一行第一列开始，正着遍历得到。

那么： $$p[i][j]=pre[i][j]⋅suf[i][j]$$
代码实现时，可以先初始化 $p[i][j]=suf[i][j]$ ，然后把 $pre[i][j]$ 乘到 $p[i][j]$ 中，就得到了答案。这样 $pre$ 和 $suf$ 就可以压缩成一个变量。
```cpp
class Solution {
public:
    vector<vector<int>> constructProductMatrix(vector<vector<int>>& grid) {
        const int MOD = 12345;
        int n = grid.size(), m = grid[0].size();
        vector<vector<int>> p(n, vector<int>(m));
        long long suf = 1; // 后缀乘积
        for (int i = n - 1; i >= 0; --i) {
            for (int j = m - 1; j >= 0; --j) {
                p[i][j] = suf; // p[i][j]先初始化为后缀乘积
                suf = suf * grid[i][j] % MOD;
            }
        }
        long long pre = 1; // 前缀乘积
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                p[i][j] = p[i][j] * pre % MOD; // 然后再乘上前缀乘积
                pre = pre * grid[i][j] % MOD;
            }
        }
        return p;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(nm)$ ，其中 $n$ 和 $m$ 分别为 $\textit{grid}$ 的行数和列数。
- 空间复杂度：$\mathcal{O}(1)$ 。返回值不计入。
