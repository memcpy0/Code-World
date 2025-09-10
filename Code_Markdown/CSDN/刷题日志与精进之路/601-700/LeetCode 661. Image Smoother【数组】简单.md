> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>An <strong>image smoother</strong> is a filter of the size <code>3 x 3</code> that can be applied to each cell of an image by rounding down the average of the cell and the eight surrounding cells (i.e., the average of the nine cells in the blue smoother). If one or more of the surrounding cells of a cell is not present, we do not consider it in the average (i.e., the average of the four cells in the red smoother).</p>
<img style="width: 493px; height: 493px;" src="https://assets.leetcode.com/uploads/2021/05/03/smoother-grid.jpg" alt="">
<p>Given an <code>m x n</code> integer matrix <code>img</code> representing the grayscale of an image, return <em>the image after applying the smoother on each cell of it</em>.</p>
 
<p><strong>Example 1:</strong></p>
<img style="width: 613px; height: 253px;" src="https://assets.leetcode.com/uploads/2021/05/03/smooth-grid.jpg" alt="">

```cpp
Input: img = [[1,1,1],[1,0,1],[1,1,1]]
Output: [[0,0,0],[0,0,0],[0,0,0]]
Explanation:
For the points (0,0), (0,2), (2,0), (2,2): floor(3/4) = floor(0.75) = 0
For the points (0,1), (1,0), (1,2), (2,1): floor(5/6) = floor(0.83333333) = 0
For the point (1,1): floor(8/9) = floor(0.88888889) = 0 
```
<p><strong>Example 2:</strong></p>
<img style="width: 613px; height: 253px;" src="https://assets.leetcode.com/uploads/2021/05/03/smooth2-grid.jpg" alt="">

```bash
Input: img = [[100,200,100],[200,50,200],[100,200,100]]
Output: [[137,141,137],[141,138,141],[137,141,137]]
Explanation:
For the points (0,0), (0,2), (2,0), (2,2): floor((100+200+200+50)/4) = floor(137.5) = 137
For the points (0,1), (1,0), (1,2), (2,1): floor((200+200+50+200+100+100)/6) = floor(141.666667) = 141
For the point (1,1): floor((50+200+200+200+200+100+100+100+100)/9) = floor(138.888889) = 138
```
 
<p><strong>Constraints:</strong></p>
<ul>
	<li><code>m == img.length</code></li>
	<li><code>n == img[i].length</code></li>
	<li><code>1 &lt;= m, n &lt;= 200</code></li>
	<li><code>0 &lt;= img[i][j] &lt;= 255</code></li>
</ul>
 

题意：**图像平滑器** 是大小为 `3 x 3` 的过滤器，用于对图像的每个单元格平滑处理，平滑处理后单元格的值为该单元格的平均灰度。每个单元格的  **平均灰度** 定义为：该单元格自身及其周围的 $8$ 个单元格的平均值，结果需向下取整。（即，需要计算蓝色平滑器中 $9$ 个单元格的平均值）。

如果一个单元格周围存在单元格缺失的情况，则计算平均灰度时不考虑缺失的单元格（即，需要计算红色平滑器中 $4$ 个单元格的平均值）。给你一个表示图像灰度的 `m x n` 整数矩阵 `img` ，返回对图像的每个单元格平滑处理后的图像 。

---
> 在图像处理中，最经常使用的算法之一就是图像平滑，其原理无非是——**任何一个网格点的值可由其邻近点的值计算出来**（和生命游戏 `Life Game` 的规则一模一样）。这也是并行计算领域中的一个经典问题——有限差分问题。 

### 解法1 朴素解法：方向数组
如果直接在原先的数组上修改，会影响到后续平均灰度的计算。因此另开一个数组：
```cpp
class Solution {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& M) {
        int n = M.size(), m = M[0].size();
        vector<vector<int>> ans(n, vector<int>(m));
        int Moves[][2] = {-1, -1, -1, 0, -1, 1, 0, -1, 0, 1, 1, -1, 1, 0, 1, 1};
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                int sum = M[i][j], num = 1;
                for (int k = 0; k < 8; ++k) { //计算八个方向的值
                    int ti = i + Moves[k][0], tj = j + Moves[k][1];
                    if (ti >= 0 && ti < n && tj >= 0 && tj < m) {
                        sum += M[ti][tj];
                        ++num;
                    }
                }
                ans[i][j] = (sum / num);
            }
        }
        return ans;
    }
};  
```
---
### 解法2 二维前缀和
