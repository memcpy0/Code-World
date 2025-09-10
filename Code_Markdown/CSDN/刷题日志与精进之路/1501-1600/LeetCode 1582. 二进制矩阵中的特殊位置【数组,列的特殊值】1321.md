> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个大小为 `rows x cols` 的矩阵 `mat` ，其中 `mat[i][j]` 是 `0` 或 `1` ，请返回 **矩阵 `mat` 中特殊位置的数目** 。

**特殊位置** 定义：如果 `mat[i][j] == 1` 并且第 `i` 行和第 `j` 列中的所有其他元素均为 `0`（行和列的下标均 **从 `0` 开始** ），则位置 `(i, j)` 被称为特殊位置。

**示例 1：**
```cpp
输入：mat = [[1,0,0],
            [0,0,1],
            [1,0,0]]
输出：1
解释：(1,2) 是一个特殊位置，因为 mat[1][2] == 1 且所处的行和列上所有其他元素都是 0
```
**示例 2：**
```cpp
输入：mat = [[1,0,0],
            [0,1,0],
            [0,0,1]]
输出：3
解释：(0,0), (1,1) 和 (2,2) 都是特殊位置
```
**示例 3：**
```cpp
输入：mat = [[0,0,0,1],
            [1,0,0,0],
            [0,1,1,0],
            [0,0,0,0]]
输出：2
```
**示例 4：**
```cpp
输入：mat = [[0,0,0,0,0],
            [1,0,0,0,0],
            [0,1,0,0,0],
            [0,0,1,0,0],
            [0,0,0,1,1]]
输出：3
```
**提示：**
-  `rows == mat.length`
- `cols == mat[i].length`
- `1 <= rows, cols <= 100`
-  `mat[i][j]` 是 `0` 或 `1`

题意：求出特殊位置的个数。

---
### 解法1 暴力+模拟
题目给定了一个大小为 $m \times n$ 的矩阵 $mat$ ，并满足矩阵中的任意元素为 $1$ 或者 $0$ 。现在给出特殊位置的定义：如果 $\textit{mat}[i][j] = 1, i \in [0,m),j \in [0, n)$ ，并且第 $i$ 行和第 $j$ 列的其他元素均为 $0$ ，则位置 $(i,j)$ 为特殊位置。

那么我们**枚举每一个位置**，然后按照特殊位置的定义来判断该位置是否满足要求，又因为矩阵中的每一个元素只能为 $1$ 或者 $0$ ，所以我们可以**预处理出每一行和列的和**来快速的得到每一行和列中的 $1$ 的个数。
```cpp
// cpp
class Solution {
public:
    int numSpecial(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<int> rowsSum(m), colsSum(n);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                rowsSum[i] += mat[i][j];
                colsSum[j] += mat[i][j];
            }
        }
        int ans = 0;
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j)
                if (mat[i][j] == 1 && rowsSum[i] == 1 && colsSum[j] == 1)
                    ++ans;
        return ans;
    }
};

// java
class Solution {
    public int numSpecial(int[][] mat) {
        int m = mat.length, n = mat[0].length;
        int[] rowsSum = new int[m];
        int[] colsSum = new int[n];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                rowsSum[i] += mat[i][j];
                colsSum[j] += mat[i][j];
            }
        }
        int ans = 0;
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j)
                if (mat[i][j] == 1 && rowsSum[i] == 1 && colsSum[j] == 1)
                    ++ans;
        return ans;
    }
}

// python
class Solution:
    def numSpecial(self, mat: List[List[int]]) -> int:
        rows_sum = [sum(row) for row in mat]
        cols_sum = [sum(col) for col in zip(*mat)]
        ans = 0
        for i, row in enumerate(mat):
            for j, x in enumerate(row):
                if x == 1 and rows_sum[i] == 1 and cols_sum[j] == 1:
                    ans += 1
        return ans
    
// go
func numSpecial(mat [][]int) (ans int) {
    rowsSum := make([]int, len(mat))
    colsSum := make([]int, len(mat[0]))
    for i, row := range mat {
        for j, x := range row {
            rowsSum[i] += x
            colsSum[j] += x
        }
    }
    for i, row := range mat {
        for j, x := range row {
            if x == 1 && rowsSum[i] == 1 && colsSum[j] == 1 {
                ans++
            }
        }
    }
    return
}
```
复杂度分析：
- 时间复杂度：$O(m \times n)$ ，其中 $m$ 为矩阵 $\textit{mat}$ 的行数，$n$ 为矩阵 $\textit{mat}$ 的列数。
- 空间复杂度：$O(m + n)$ ，主要为预处理每一行和列的空间开销。
---
### 解法2 列的标记值
在方法一的基础上，我们可以看到对于 $(i,j)$ ，它为特殊位置的条件为 $\textit{mat}[i][j]=1$ 且该行和该列中 $1$ 的数量都为 $1$ 。据此，定义**第 $j$ 列的标记值为：该列所有 $1$ 所在行中的 $1$ 的数量之和**。下面证明，$(i,j)$ 为特殊位置的充要条件是，第 $j$ 列的标记值恰好为 $1$ ：
- 如果 $(i,j)$ 为特殊位置，则说明第 $j$ 列只有一个 $1$ ，这一个 $1$ 所在的第 $i$ 行也只有它这一个 $1$ ，那么按照标记值的定义可得，第 $j$ 列的标记值为 $1$ 。
- 如果第 $j$ 列的标记值为 $1$ ，那么说明该列只能有一个 $1$ 。反证：如果有 $x$ 个 $1$（$x > 1$），则第 $j$ 列的标记值一定 $\ge x$ 。**既然只能有一个 $1$ ，设其在第 $i$ 行，那么标记值也是第 $i$ 行中的 $1$ 的数量**，即：第 $i$ 行也有且仅有这个 $1$ 。所以 $(i,j)$ 为特殊位置。
- 那么==整个矩阵的特殊位置的数量，就是最后标记值为 $1$ 的列的数量==。

进一步地，我们可以**用原始矩阵的第一行来作为我们标记列的额外空间**，从而使空间复杂度降至 $O(1)$ 。
```cpp
// cpp
class Solution {
public:
    int numSpecial(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        for (int i = 0; i < m; ++i) {
            int cnt1 = 0;
            for (int j = 0; j < n; ++j) 
                if (mat[i][j] == 1) ++cnt1;
            if (i == 0) --cnt1;
            if (cnt1 > 0) {
                for (int j = 0; j < n; ++j) 
                    if (mat[i][j] == 1) 
                        mat[0][j] += cnt1;
            }
        }
        int sum = 0;
        for (int i = 0; i < n; ++i)
            if (mat[0][i] == 1) ++sum;
        return sum;
    }
};

// java
class Solution {
    public int numSpecial(int[][] mat) {
        int m = mat.length, n = mat[0].length;
        for (int i = 0; i < m; ++i) {
            int cnt1 = 0;
            for (int j = 0; j < n; ++j) 
                if (mat[i][j] == 1) ++cnt1;
            if (i == 0) --cnt1;
            if (cnt1 > 0) {
                for (int j = 0; j < n; ++j) 
                    if (mat[i][j] == 1) 
                        mat[0][j] += cnt1;
            }
        }
        int sum = 0;
        for (int i = 0; i < n; ++i)
            if (mat[0][i] == 1) ++sum;
        return sum;
    }
}

// python
class Solution:
    def numSpecial(self, mat: List[List[int]]) -> int:
        for i, row in enumerate(mat):
            cnt1 = sum(row) - (i == 0)
            if cnt1:
                for j, x in enumerate(row):
                    if x == 1:
                        mat[0][j] += cnt1
        return sum(x == 1 for x in mat[0])

// go
func numSpecial(mat [][]int) (ans int) {
    for i, row := range mat {
        cnt1 := 0
        for _, x := range row {
            cnt1 += x
        }
        if i == 0 {
            cnt1--
        }
        if cnt1 > 0 {
            for j, x := range row {
                if x == 1 {
                    mat[0][j] += cnt1
                }
            }
        }
    }
    for _, x := range mat[0] {
        if x == 1 {
            ans++
        }
    }
    return
}
```
复杂度分析：
- 时间复杂度：$O(m×n)$ ，其中 $m$ 为矩阵 $\textit{mat}$ 的行数，$n$ 为矩阵 $\textit{mat}$ 的列数。
- 空间复杂度：$O(1)$ ，由于用了原始矩阵的空间来作为我们的辅助空间，所以我们仅使用常量空间。
