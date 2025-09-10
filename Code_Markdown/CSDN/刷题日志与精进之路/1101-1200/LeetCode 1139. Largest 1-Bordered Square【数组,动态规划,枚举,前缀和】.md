> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a 2D `grid` of `0`s and `1`s, return the number of elements in the largest **square** subgrid that has all `1`s on its **border**, or `0` if such a subgrid doesn't exist in the `grid`.

**Example 1:**
```java
Input: grid = [[1,1,1],[1,0,1],[1,1,1]]
Output: 9
```
**Example 2:**
```java
Input: grid = [[1,1,0,0]]
Output: 1
```
**Constraints:**
-   `1 <= grid.length <= 100`
-   `1 <= grid[0].length <= 100`
-   `grid[i][j]` is `0` or `1`

---
### 解法1 动态规划+枚举
这题LeetCode上定义为动态规划，令人想起530. 动态规划解最大正方形，需要正方形所有网格中的数字都是1，用动态规划或二维前缀和之一，代码就非常简洁。而这题只要正方形4条边的网格都是1即可，中间什么数字都行。

这题其实也与前缀和有关系，我们设两个数组如下：
```java
left[i][j]: 从grid[i-1][j-1]开始,左边连续的1的个数
up[i][j]: 从grid[i-1][j-1]开始,上边连续的1的个数
```
注意，当 `grid[i - 1][j - 1]` 为0时，`left[i][j] = up[i][j] = 0` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303011736326.png)
我们**枚举「以 `grid` 中所有非零点为右下角的正方形」**，要求得最大正方形的面积（即格子数），首先考虑「以这个点为右下角可能构成的最大正方形」的边长可能是多少。

很明显，**这一边长不应超过「该点左边和上边连续1个数的最小值」**，如上图的 `grid[6] [5]` ，最大的可能边长就应该是6，然后**枚举所有的小于等于6且大于等于1的边长 `side`** ，验证能否构成正方形。

这也很容易，如上图，只需要考虑 `grid[6][5]` 上边距离为 `side` 的点的左边连续1的个数是否大于等于 `side`（ `left[i - side + 1][j] >= side` ），以及左边距离为 `side` 的点的上边连续的1的个数是否大于等于 `side`（ `up[i][j - side + 1] >= side` ），如果都大于等于 `side` 那么该边长就是合法的，统计最大边长形成的面积即可。
- 时间复杂度：$O(n\times m \times \max(side) )$ 
- 空间复杂度：$O(n\times m)$
```cpp
class Solution {
public:
    int largest1BorderedSquare(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size();
        vector<vector<int>> left(n + 1, vector<int>(m + 1)), up(n + 1, vector<int>(m + 1));
        int ans = 0;
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (grid[i - 1][j - 1] == 1) {
                    left[i][j] = left[i][j - 1] + 1;
                    up[i][j] = up[i - 1][j] + 1;
                    int side = min(left[i][j], up[i][j]);
                    for (int k = side; k >= 1; --k) {
                        if (up[i][j - k + 1] >= k && left[i - k + 1][j] >= k) {
                            ans = max(ans, k * k);
                            break;
                        }
                    }
                }
            }
        }
        return ans;
    }
};
```
---
此外还有 $O(n^2)$ 和 $O(n^2\log n)$ 的算法。
### 解法2 双指针枚举对角线
上述做法是立方复杂度，==通过从枚举四条边，变为先把「相互垂直的两条边」合起来组成直角，把两条边长的较小值作为直角的长度，然后枚举对角线上的两个直角==，可以得到平方复杂度。如果一个向右向下的直角和一个向左向上的直角之间的距离小于它们长度的较小值，就可以组成正方形。最后是一些排序+双指针的技巧简化复杂度。