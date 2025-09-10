> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given `row x col` `grid` representing a map where `grid[i][j] = 1` represents land and `grid[i][j] = 0` represents water.

Grid cells are connected **horizontally/vertically** (not diagonally). The `grid` is completely surrounded by water, and there is exactly one island (i.e., one or more connected land cells).

The island doesn't have "lakes", meaning the water inside isn't connected to the water around the island. One cell is a square with side length 1. The grid is rectangular, width and height don't exceed 100. Determine the perimeter of the island.

**Example 1:**
```swift
Input: grid = [[0,1,0,0],[1,1,1,0],[0,1,0,0],[1,1,0,0]]
Output: 16
Explanation: The perimeter is the 16 yellow stripes in the image above.
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201102195821959.png#pic_center)
**Example 2:**
```swift
Input: grid = [[1]]
Output: 4
```
**Example 3:**
```swift
Input: grid = [[1,0]]
Output: 4
```
**Constraints:**
- `row == grid.length`
- `col == grid[i].length`
- `1 <= row, col <= 100`
- `grid[i][j]` is `0` or `1` .

题意：给定一个包含 `0` 和 `1` 的二维网格地图，其中 `1` 表示陆地、 `0` 表示水域。网格中的格子**水平和垂直方向相连**（对角线方向不相连）。整个网格被水完全包围，但其中恰好有一个岛屿（或者说，一个或多个表示陆地的格子相连组成的岛屿）。岛屿中没有“湖”（“湖” 指水域在岛屿内部且不和岛屿周围的水相连）。

格子是边长为 `1` 的正方形。网格为长方形，且宽度和高度均不超过 `100` 。计算这个岛屿的周长。

---
这道题的解法有点多，记录下来。而且这一题其实和[HDU 5538](http://acm.hdu.edu.cn/showproblem.php?pid=5538)类似，HDU 5538除了不算与地面接触的面积外，和[Leetcode 892. 三维形体的表面积](https://leetcode-cn.com/problems/surface-area-of-3d-shapes/)完全相同。
### 解法1 直接遍历+容斥原理
对于每个格子，如果它是岛屿，则边长加上 $4$ ，如果它左部为岛屿格子，则边长减去 $2$（重合的两边）；如果它上边为岛屿格子，则边长减去 $2$ （重合的两边）。**这样从上到下、从左到右遍历完矩阵，就得到了整个岛屿的周长**。

或者对于每个格子，如果它是岛屿，则边长加上 $4$ ，如果它右边为岛屿格子，则边长减去 $2$（重合的两边）；如果它下边为岛屿格子，则边长减去 $2$（重合的两边）。**这样从上到下、从左到右遍历完矩阵，就得到了整个岛屿的周长**。

这种做法的根据是**容斥定理**：如果被计数的事物有 $A,B,C$ 三类，那么，$A$ 类和 $B$ 类和 $C$ 类元素个数总和 = $A$ 类元素个数 + $B$ 类元素个数 + $C$ 类元素个数 - 既是 $A$ 类又是 $B$ 类的元素个数 - 既是 $A$ 类又是 $C$ 类的元素个数 - 既是 $B$ 类又是 $C$ 类的元素个数 + 既是 $A$ 类又是 $B$ 类而且是 $C$ 类的元素个数。$$(A\cup B\cup C = A + B + C - A\cap B - B\cap C - C\cap A + A\cap B\cap C)$$
本题中，如果考虑仅有两个格子相交，假设一个叫做陆地 $A$ ，另一个叫做陆地 $B$ ，根据容斥原理，它们各自的周长分别是 $4$ ，**既属于 $A$ 又属于 $B$ 的边数是 $2$ 条**，所以 $8−2=6$ 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202306161700767.png)

再来看三个格子相交的情况，假设叫做陆地 $A$ ，陆地 $B$ ，陆地 $C$ ，根据容斥原理，它们各自的周长分别是 $4$ ，既属于 $A$ 又属于 $B$ 的有 $2$ 条，既属于 $A$ 又属于 $C$ 的也有 $2$ 条，**$B$ 与 $C$ 不相交**，那么总周长为 $4\times 3−2\times 2=8$ 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202306161700447.png)
以此可以推广到更多的格子相交，因此，我们在遍历到每个点时，只需要对其下方或者右边判断是否有格子即可。具体代码实现如下：
```java
class Solution { 
    public int islandPerimeter(int[][] grid) {
        int ans = 0;
        for (int i = 0; i < grid.length; ++i) {
            for (int j = 0; j < grid[i].length; ++j) {
                if (grid[i][j] == 1) {
                    ans += 4;
                    if (i + 1 < grid.length && grid[i + 1][j] == 1) ans -= 2;
                    if (j + 1 < grid[i].length && grid[i][j + 1] == 1) ans -= 2;
                }
            }
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(mn)$
- 空间复杂度：$O(1)$

---
### 解法2 DFS
可将方法一改成深度优先搜索遍历的方式，此时遍历的方式**可扩展至统计多个岛屿各自的周长**。注意，为了防止陆地格子在深度优先搜索中被重复遍历导致死循环，要将遍历过的陆地格子标记为已遍历过，下面的代码中设定值为 $2$ 的格子为「已经遍历过的陆地格子」。

计算周长时，这里没有看每个格子的上边或左边是否为陆地，而是采用了另一种思想：==一个陆地格子的每条边，它被算作岛屿的周长当且仅当**这条边为网格的边界**或者**相邻的另一个格子为水域**==。我们对每个陆地格子的四条边，检查其是否能算作岛屿的周长。
> 当然，这种检查可以用在直接遍历中，但不如解法1的检查快。且解法2还使用DFS，这在某种程度上进一步降低了速度。

```java
class Solution { 
    int[][] d = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    int dfs(int[][] grid, int a, int b) {
        if (a < 0 || a >= grid.length || b < 0 || b >= grid[0].length || grid[a][b] == 0) return 1;
        if (grid[a][b] == 2) return 0;
        grid[a][b] = 2; // 标记已被访问
        int cnt = 0;
        for (int i = 0; i < 4; ++i) {
            int x = a + d[i][0], y = b + d[i][1];  
            cnt += dfs(grid, x, y); // 检查每条边,看能否算作岛屿周长
        } 
        return cnt;
    }
    public int islandPerimeter(int[][] grid) {
        for (int i = 0; i < grid.length; ++i)
            for (int j = 0; j < grid[i].length; ++j) 
                if (grid[i][j] == 1) // 不会再出现第二个为1的格子
                    return dfs(grid, i, j); 
        return 0;
    }
}
```
复杂度分析：
- 时间复杂度：$O(nm)$ ，其中 $n$ 为网格的高度，$m$ 为网格的宽度。每个格子至多会被遍历一次，因此总时间复杂度为 $O(nm)$ 。
- 空间复杂度：$O(nm)$ 。深度优先搜索复杂度取决于递归的栈空间，而栈空间最坏情况下会达到 $O(nm)$ 。

---
### 解法3 锐化算法
算周长说白了就是找边缘，找边缘说白了就是锐化，所以可以按照锐化的算法来算。
```python
from scipy.signal import convolve2d
class Solution:
    def islandPerimeter(self, grid: List[List[int]]) -> int:
        return int(abs(convolve2d(grid,[[-2,1],[1,0]])).sum())
```
这个卷积核就是拉普拉斯算子 $[[0,-1,0],[-1,4,-1],[0,-1,0]]$ 的一半，正负无所谓，只要异号就行，$[[2,-1],[-1,0]]$ 和 $[[-2,1],[1,0]]$ 的结果是一样的。直接用拉普拉斯算子算也可以，不过因为拉普拉斯算子是二阶微分算子，每条边找了两次，所以结果要除 $2$ 。
