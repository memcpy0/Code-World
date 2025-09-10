> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 `2` 行 `n` 列的二进制数组：
- 矩阵是一个二进制矩阵，这意味着矩阵中的每个元素不是 `0` 就是 `1`。
- 第 `0` 行的元素之和为 `upper`。
- 第 `1` 行的元素之和为 `lower`。
- 第 `i` 列（从 `0` 开始编号）的元素之和为 `colsum[i]`，`colsum` 是一个长度为 `n` 的整数数组。

你需要利用 `upper`，`lower` 和 `colsum` 来重构这个矩阵，并以二维整数数组的形式返回它。

如果有多个不同的答案，那么任意一个都可以通过本题。

如果不存在符合要求的答案，就请返回一个空的二维数组。

**示例 1：**
```java
输入：upper = 2, lower = 1, colsum = [1,1,1]
输出：[[1,1,0],[0,0,1]]
解释：[[1,0,1],[0,1,0]] 和 [[0,1,1],[1,0,0]] 也是正确答案。
```
**示例 2：**
```java
输入：upper = 2, lower = 3, colsum = [2,2,1,1]
输出：[]
```
**示例 3：**
```java
输入：upper = 5, lower = 5, colsum = [2,1,2,0,1,0,1,2,0,1]
输出：[[1,1,1,0,1,0,0,1,0,0],[1,0,1,0,0,0,1,1,0,1]]
```
**提示：**
- `1 <= colsum.length <= 10^5`
- `0 <= upper, lower <= colsum.length`
- `0 <= colsum[i] <= 2`

---
### 解法 贪心
我们要求得任意一个 $2$ 行 $n$ 列的二进制数组（其中行和列的序号从 $0$ 开始），满足数组中的每一个元素不是 $0$ 就是 $1$ ，并且第 $0$ 行的元素和为 $\textit{upper}$ ，第 $1$ 行的元素和为 $\textit{lower}$ ，第 $i$ 列的元素之和为 $colsum[i]$ ，若不存在直接返回一个空的二维数组即可。

记 $sum$ 为数组 $colsum$ 的元素和，**$two$ 为数组 $colsum$ 中 $2$ 的个数**。明显**当 $sum≠upper+lower$ 时，一定不存在满足题意的矩阵**。然后当第 $i$ 列 $colsum[i]=2$ 时，第 $i$ 列的两个元素只能都为 $1$ 。那么**如果 $\textit{two} > \min\{\textit{upper}, \textit{lower}\}$ 时，此时同样不存在满足题意的矩阵**。

否则，一定可以通过下述的方案，构造一个符合题目要求的矩阵。设结果矩阵为 $\textit{ans}[2][n]$ 。当第 $i$ 列 $colsum[i]$ 等于 $0$ 或者 $2$ 时只有一种情况：
- $colsum[i]=0$ 时：$ans[0][i]=ans[1][i]=0$ 。
- $colsum[i]=2$ 时：$\textit{ans}[0][i] = \textit{ans}[1][i] = 1$ 。

所以，现在只关注 $colsum[i]=1$ 的情况。首先**将初始的 $\textit{upper}$ 和 $\textit{lower}$ 减去数组 $colsum$ 中 $2$ 的个数 $two$**（==**相当于先贪心地处理 $colsum[i]=2$ 的情况**==），那么现在 $upper+lower$ 为数组 $colsum$ 中 $1$ 的个数。那么从左到右遍历 $colsum$ 中的每一列，若第 $i$ 列 $colsum[i]$ 等于 $1$ ：
- 若 $upper>0$ ，则我们在该列的第一行放置 $1$ ，第二行放置 $0$ ：$ans[0][i]=1$，$ans[1][i]=0$ ，并且 $\textit{upper}$ 减一。
- 否则在该列的第一行放置 $0$ ，第二行放置 $1$ ：$ans[0][i]=0$ ，$\textit{ans}[1][i] = 1$ 。

当遍历完成后，就得到了符合题目要求的矩阵 $ans[2][n]$ 。

现在给出该方案的正确性证明：从上述的构造过程可得，整个数组中除了 $1$ 就 $0$ ，每一列中 $1$ 的个数完全符合数组 $colsum$ 描述，且在第一行中我们共放置了 $upper$ 个 $1$ ，第二行共放置了 $\textit{lower}$ 个 $1$ 。因此这样构造的矩阵 $ans[2][n]$ 为满足题意的二进制矩阵，正确性得证。

下面是我写的**先贪心处理 $2$ 的情况**的代码：
```cpp
class Solution {
public:
    vector<vector<int>> reconstructMatrix(int upper, int lower, vector<int>& colsum) {
        int n = colsum.size();
        vector<vector<int>> ans(2, vector<int>(n)); 
        for (int j = 0; j < n; ++j) {
            if (colsum[j] == 2) {
                if (!upper || !lower) return {}; // 不够组成列
                ans[0][j] = ans[1][j] = 1;
                --upper, --lower;
            }
        }
        for (int j = 0; j < n; ++j) { 
            if (colsum[j] == 1) {
                if (!upper && !lower) return {}; // 不够组成列
                if (upper) ans[0][j] = 1, --upper;
                else ans[1][j] = 1, --lower;
            }
        } 
        if (!lower && !upper) return ans;
        return {}; // 不够组成列   
    }
};
```
下面是**先检查 $colsum$ 是否有解**的代码：
```cpp
class Solution {
public:
    vector<vector<int>> reconstructMatrix(int upper, int lower, vector<int>& colsum) {
        int n = colsum.size();
        vector<vector<int>> ans(2, vector<int>(n)); 
        int sum = 0, two = 0; // two为colsum中2的个数
        for (int j = 0; j < n; ++j) {
            if (colsum[j] == 2) ++two; 
            sum += colsum[j];
        }
        if (sum != upper + lower || min(upper, lower) < two) return {};
        upper -= two, lower -= two; // 相当于先处理2的情况
        for (int j = 0; j < n; ++j) { 
            if (colsum[j] == 2) ans[0][j] = ans[1][j] = 1;
            else if (colsum[j] == 1) { 
                if (upper) ans[0][j] = 1, --upper;
                else ans[1][j] = 1;
            }
        } 
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为数组 $colsum$ 的长度。
- 空间复杂度：$O(1)$ ，仅使用常量空间。返回的结果数组不计入空间开销。
