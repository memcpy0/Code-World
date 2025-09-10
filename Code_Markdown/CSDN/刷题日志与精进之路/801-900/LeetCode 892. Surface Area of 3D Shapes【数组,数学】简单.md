> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 `n * n` 的网格 `grid` ，上面放置着一些 `1 x 1 x 1` 的正方体。每个值 `v = grid[i][j]` 表示 `v` 个正方体叠放在对应单元格 `(i, j)` 上。

放置好正方体后，任何直接相邻的正方体都会互相粘在一起，形成一些不规则的三维形体。

请你返回最终这些形体的总表面积。

**注意：** 每个形体的底面也需要计入表面积中。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/01/08/tmp-grid2.jpg)
```java
输入：grid = [[1,2],[3,4]]
输出：34
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2021/01/08/tmp-grid4.jpg)
```java
输入：grid = [[1,1,1],[1,0,1],[1,1,1]]
输出：32
```
**示例 3：**
![](https://assets.leetcode.com/uploads/2021/01/08/tmp-grid5.jpg)
```java
输入：grid = [[2,2,2],[2,1,2],[2,2,2]]
输出：46
```
**提示：**
- `n == grid.length`
- `n == grid[i].length`
- `1 <= n <= 50`
- `0 <= grid[i][j] <= 50`

---
这一题和[HDU 5538](http://acm.hdu.edu.cn/showproblem.php?pid=5538)类似，也和[LeetCode 463. Island Perimeter](https://leetcode.cn/problems/island-perimeter)完全相同。
### 解法 直接遍历+容斥原理
本题求的是叠方块的表面积，每个位置上的立方体（叠放的多个正方体）没有和周边立方体重叠时，本身表面积为 $6 \times h - (h - 1) \times 2$（**每个正方体贡献了 $6$ 个表面积、再减去多个正方体重叠的面**）。简化一下就是 $4\times h+2$（**考虑上底和下底面，以及每个正方体贡献的 $4$ 个侧表面**）。

再通过从左上到右下遍历，判断下方和右边是否有重叠的立方体，如果有重叠，计算当前立方体的表面积时，要减去 $2\times \min {(grid[i+1][j],\ grid[i][j])}$ （下方有重叠），$2\times \min{}{(grid[i][j+1],\ grid[i][j])}$ （右侧有重叠），即可得到最终结果。
```java
class Solution {
    public int surfaceArea(int[][] grid) {
        int ans = 0;
        int n = grid.length, m = grid[0].length;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == 0) continue;
                // ans += grid[i][j] * 6 - (grid[i][j] - 1) * 2;
                ans += (grid[i][j] << 2) + 2;
                if (i + 1 < n && grid[i + 1][j] != 0) // 右边
                    ans -= Math.min(grid[i][j], grid[i + 1][j]) << 1;
                if (j + 1 < m && grid[i][j + 1] != 0) // 下边
                    ans -= Math.min(grid[i][j], grid[i][j + 1]) << 1;
            }
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(mn)$
- 空间复杂度：$O(1)$

似乎还可稍作优化。上面的代码在循环中做了太多次乘法/移位操作，可将这些操作放到最后进行。
作为改进，整体思路是看看**有多少个立方体**，总表面积是立方体的数量 $× 6$ ，但上下叠放的正方体、和相邻的立方体会互相遮盖住，统计一下被盖住的面，最后减去被盖住的面就行：
```java
class Solution {
    public int surfaceArea(int[][] grid) {
        int blocks = 0, cover = 0; // 正方体的个数,盖住的面的个数
        int n = grid.length, m = grid[0].length;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == 0) continue; 
                blocks += grid[i][j];
                cover += grid[i][j] - 1;
                if (i + 1 < n && grid[i + 1][j] != 0) // 右边
                    cover += Math.min(grid[i][j], grid[i + 1][j]);
                if (j + 1 < m && grid[i][j + 1] != 0) // 下边
                    cover += Math.min(grid[i][j], grid[i][j + 1]);
            }
        }
        return blocks * 6 - cover * 2;
    }
}
```
复杂度分析：
- 时间复杂度：$O(mn)$
- 空间复杂度：$O(1)$