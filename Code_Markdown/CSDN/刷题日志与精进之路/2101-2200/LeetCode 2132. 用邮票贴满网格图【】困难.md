> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 `m x n` 的二进制矩阵 `grid` ，每个格子要么为 `0` （空）要么为 `1` （被占据）。

给你邮票的尺寸为 `stampHeight x stampWidth` 。我们想将邮票贴进二进制矩阵中，且满足以下 **限制** 和 **要求** ：

1. 覆盖所有 **空** 格子。
2. 不覆盖任何 **被占据** 的格子。
3. 我们可以放入任意数目的邮票。
4. 邮票可以相互有 **重叠** 部分。
5. 邮票不允许 **旋转** 。
6. 邮票必须完全在矩阵 **内** 。

如果在满足上述要求的前提下，可以放入邮票，请返回 `true` ，否则返回 `false` 。

**示例 1：**

![](https://assets.leetcode.com/uploads/2021/11/03/ex1.png)

**输入：**grid = [[1,0,0,0],[1,0,0,0],[1,0,0,0],[1,0,0,0],[1,0,0,0]], stampHeight = 4, stampWidth = 3
**输出：**true
**解释：**我们放入两个有重叠部分的邮票（图中标号为 1 和 2），它们能覆盖所有与空格子。

**示例 2：**

![](https://assets.leetcode.com/uploads/2021/11/03/ex2.png)

**输入：**grid = [[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]], stampHeight = 2, stampWidth = 2 
**输出：**false 
**解释：**没办法放入邮票覆盖所有的空格子，且邮票不超出网格图以外。

**提示：**

- `m == grid.length`
- `n == grid[r].length`
- `1 <= m, n <= 105`
- `1 <= m * n <= 2 * 105`
- `grid[r][c]` 要么是 `0` ，要么是 `1` 。
- `1 <= stampHeight, stampWidth <= 105`

---
### 【图解】从一维差分到二维差分（Python/Java/C++/Go/JS/Rust）

一维差分的思想可以推广至二维，请点击图片放大查看：

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202402172219113.png)


前置知识：二维前缀和
请看【图解】二维前缀和

思路
由于邮票可以互相重叠，贪心地想，能放邮票就放邮票。
遍历所有能放邮票的位置去放邮票。注意邮票不能覆盖被占据的格子，也不能出界。
放邮票的同时，记录每个空格子被多少张邮票覆盖。如果存在一个空格子没被邮票覆盖，则返回 false\texttt{false}false，否则返回 true\texttt{true}true。
细节
怎么快速判断一个矩形区域可以放邮票？求出 grid\textit{grid}grid 的二维前缀和，从而 O(1)\mathcal{O}(1)O(1) 地求出任意矩形区域的元素和。如果一个矩形区域的元素和等于 000，就表示该矩形区域的所有格子都是 000。
假设用一个二维计数矩阵 cnt\textit{cnt}cnt 记录每个空格子被多少张邮票覆盖，那么放邮票时，就需要把 cnt\textit{cnt}cnt 的一个矩形区域都加一。怎么快速实现？可以用二维差分矩阵 ddd 来代替 cnt\textit{cnt}cnt。矩形区域都加一的操作，转变成 O(1)\mathcal{O}(1)O(1) 地对 ddd 中四个位置的更新操作。
最后从二维差分矩阵 ddd 还原出二维计数矩阵 cnt\textit{cnt}cnt。类似对一维差分数组求前缀和得到原数组，我们需要对二维差分矩阵求二维前缀和。遍历 cnt\textit{cnt}cnt，如果存在一个空格子的计数值为 000，就表明该空格子没有被邮票覆盖，返回 false\texttt{false}false，否则返回 true\texttt{true}true。代码实现时，可以直接在 ddd 数组上原地计算出 cnt\textit{cnt}cnt。

作者：灵茶山艾府
链接：https://leetcode.cn/problems/stamping-the-grid/solutions/1199642/wu-nao-zuo-fa-er-wei-qian-zhui-he-er-wei-zwiu/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```cpp
class Solution:
    def possibleToStamp(self, grid: List[List[int]], stampHeight: int, stampWidth: int) -> bool:
        m, n = len(grid), len(grid[0])

        # 1. 计算 grid 的二维前缀和
        s = [[0] * (n + 1) for _ in range(m + 1)]
        for i, row in enumerate(grid):
            for j, v in enumerate(row):
                s[i + 1][j + 1] = s[i + 1][j] + s[i][j + 1] - s[i][j] + v

        # 2. 计算二维差分
        # 为方便第 3 步的计算，在 d 数组的最上面和最左边各加了一行（列），所以下标要 +1
        d = [[0] * (n + 2) for _ in range(m + 2)]
        for i2 in range(stampHeight, m + 1):
            for j2 in range(stampWidth, n + 1):
                i1 = i2 - stampHeight + 1
                j1 = j2 - stampWidth + 1
                if s[i2][j2] - s[i2][j1 - 1] - s[i1 - 1][j2] + s[i1 - 1][j1 - 1] == 0:
                    d[i1][j1] += 1
                    d[i1][j2 + 1] -= 1
                    d[i2 + 1][j1] -= 1
                    d[i2 + 1][j2 + 1] += 1

        # 3. 还原二维差分矩阵对应的计数矩阵（原地计算）
        for i, row in enumerate(grid):
            for j, v in enumerate(row):
                d[i + 1][j + 1] += d[i + 1][j] + d[i][j + 1] - d[i][j]
                if v == 0 and d[i + 1][j + 1] == 0:
                    return False
        return True

作者：灵茶山艾府
链接：https://leetcode.cn/problems/stamping-the-grid/solutions/1199642/wu-nao-zuo-fa-er-wei-qian-zhui-he-er-wei-zwiu/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
    public boolean possibleToStamp(int[][] grid, int stampHeight, int stampWidth) {
        int m = grid.length;
        int n = grid[0].length;

        // 1. 计算 grid 的二维前缀和
        int[][] s = new int[m + 1][n + 1];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                s[i + 1][j + 1] = s[i + 1][j] + s[i][j + 1] - s[i][j] + grid[i][j];
            }
        }

        // 2. 计算二维差分
        // 为方便第 3 步的计算，在 d 数组的最上面和最左边各加了一行（列），所以下标要 +1
        int[][] d = new int[m + 2][n + 2];
        for (int i2 = stampHeight; i2 <= m; i2++) {
            for (int j2 = stampWidth; j2 <= n; j2++) {
                int i1 = i2 - stampHeight + 1;
                int j1 = j2 - stampWidth + 1;
                if (s[i2][j2] - s[i2][j1 - 1] - s[i1 - 1][j2] + s[i1 - 1][j1 - 1] == 0) {
                    d[i1][j1]++;
                    d[i1][j2 + 1]--;
                    d[i2 + 1][j1]--;
                    d[i2 + 1][j2 + 1]++;
                }
            }
        }

        // 3. 还原二维差分矩阵对应的计数矩阵（原地计算）
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                d[i + 1][j + 1] += d[i + 1][j] + d[i][j + 1] - d[i][j];
                if (grid[i][j] == 0 && d[i + 1][j + 1] == 0) {
                    return false;
                }
            }
        }
        return true;
    }
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/stamping-the-grid/solutions/1199642/wu-nao-zuo-fa-er-wei-qian-zhui-he-er-wei-zwiu/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    bool possibleToStamp(vector<vector<int>> &grid, int stampHeight, int stampWidth) {
        int m = grid.size(), n = grid[0].size();

        // 1. 计算 grid 的二维前缀和
        vector<vector<int>> s(m + 1, vector<int>(n + 1));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                s[i + 1][j + 1] = s[i + 1][j] + s[i][j + 1] - s[i][j] + grid[i][j];
            }
        }

        // 2. 计算二维差分
        // 为方便第 3 步的计算，在 d 数组的最上面和最左边各加了一行（列），所以下标要 +1
        vector<vector<int>> d(m + 2, vector<int>(n + 2));
        for (int i2 = stampHeight; i2 <= m; i2++) {
            for (int j2 = stampWidth; j2 <= n; j2++) {
                int i1 = i2 - stampHeight + 1;
                int j1 = j2 - stampWidth + 1;
                if (s[i2][j2] - s[i2][j1 - 1] - s[i1 - 1][j2] + s[i1 - 1][j1 - 1] == 0) {
                    d[i1][j1]++;
                    d[i1][j2 + 1]--;
                    d[i2 + 1][j1]--;
                    d[i2 + 1][j2 + 1]++;
                }
            }
        }

        // 3. 还原二维差分矩阵对应的计数矩阵（原地计算）
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                d[i + 1][j + 1] += d[i + 1][j] + d[i][j + 1] - d[i][j];
                if (grid[i][j] == 0 && d[i + 1][j + 1] == 0) {
                    return false;
                }
            }
        }
        return true;
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/stamping-the-grid/solutions/1199642/wu-nao-zuo-fa-er-wei-qian-zhui-he-er-wei-zwiu/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func possibleToStamp(grid [][]int, stampHeight, stampWidth int) bool {
    m, n := len(grid), len(grid[0])

    // 1. 计算 grid 的二维前缀和
    s := make([][]int, m+1)
    s[0] = make([]int, n+1)
    for i, row := range grid {
        s[i+1] = make([]int, n+1)
        for j, v := range row {
            s[i+1][j+1] = s[i+1][j] + s[i][j+1] - s[i][j] + v
        }
    }

    // 2. 计算二维差分
    // 为方便第 3 步的计算，在 d 数组的最上面和最左边各加了一行（列），所以下标要 +1
    d := make([][]int, m+2)
    for i := range d {
        d[i] = make([]int, n+2)
    }
    for i2 := stampHeight; i2 <= m; i2++ {
        for j2 := stampWidth; j2 <= n; j2++ {
            i1 := i2 - stampHeight + 1
            j1 := j2 - stampWidth + 1
            if s[i2][j2]-s[i2][j1-1]-s[i1-1][j2]+s[i1-1][j1-1] == 0 {
                d[i1][j1]++
                d[i1][j2+1]--
                d[i2+1][j1]--
                d[i2+1][j2+1]++
            }
        }
    }

    // 3. 还原二维差分矩阵对应的计数矩阵（原地计算）
    for i, row := range grid {
        for j, v := range row {
            d[i+1][j+1] += d[i+1][j] + d[i][j+1] - d[i][j]
            if v == 0 && d[i+1][j+1] == 0 {
                return false
            }
        }
    }
    return true
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/stamping-the-grid/solutions/1199642/wu-nao-zuo-fa-er-wei-qian-zhui-he-er-wei-zwiu/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

// js
var possibleToStamp = function (grid, stampHeight, stampWidth) {
    const m = grid.length, n = grid[0].length;

    // 1. 计算 grid 的二维前缀和
    const s = Array(m + 1).fill(null).map(() => Array(n + 1).fill(0));
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            s[i + 1][j + 1] = s[i + 1][j] + s[i][j + 1] - s[i][j] + grid[i][j];
        }
    }

    // 2. 计算二维差分
    // 为方便第 3 步的计算，在 d 数组的最上面和最左边各加了一行（列），所以下标要 +1
    const d = Array(m + 2).fill(null).map(() => Array(n + 2).fill(0));
    for (let i2 = stampHeight; i2 <= m; i2++) {
        for (let j2 = stampWidth; j2 <= n; j2++) {
            const i1 = i2 - stampHeight + 1;
            const j1 = j2 - stampWidth + 1;
            if (s[i2][j2] - s[i2][j1 - 1] - s[i1 - 1][j2] + s[i1 - 1][j1 - 1] === 0) {
                d[i1][j1]++;
                d[i1][j2 + 1]--;
                d[i2 + 1][j1]--;
                d[i2 + 1][j2 + 1]++;
            }
        }
    }

    // 3. 还原二维差分矩阵对应的计数矩阵（原地计算）
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            d[i + 1][j + 1] += d[i + 1][j] + d[i][j + 1] - d[i][j];
            if (grid[i][j] === 0 && d[i + 1][j + 1] === 0) {
                return false;
            }
        }
    }
    return true;
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/stamping-the-grid/solutions/1199642/wu-nao-zuo-fa-er-wei-qian-zhui-he-er-wei-zwiu/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```
复杂度分析
时间复杂度：O(mn)\mathcal{O}(mn)O(mn)，其中 mmm 和 nnn 分别为 grid\textit{grid}grid 的行数和列数。
空间复杂度：O(mn)\mathcal{O}(mn)O(mn)。