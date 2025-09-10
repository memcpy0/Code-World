> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的 `m x n` 整数矩阵 `grid` 。矩阵中某一列的宽度是这一列数字的最大 **字符串长度** 。

- 比方说，如果 `grid = [[-10], [3], [12]]` ，那么唯一一列的宽度是 `3` ，因为 `-10` 的字符串长度为 `3` 。

请你返回一个大小为 `n` 的整数数组 `ans` ，其中 `ans[i]` 是第 `i` 列的宽度。

一个有 `len` 个数位的整数 `x` ，如果是非负数，那么 **字符串长度** 为 `len` ，否则为 `len + 1` 。

**示例 1：**
```cpp
输入：grid = [[1],[22],[333]]
输出：[3]
解释：第 0 列中，333 字符串长度为 3 。
```
**示例 2：**
```cpp
输入：grid = [[-15,1,3],[15,7,12],[5,6,-2]]
输出：[3,1,2]
解释：
第 0 列中，只有 -15 字符串长度为 3 。
第 1 列中，所有整数的字符串长度都是 1 。
第 2 列中，12 和 -2 的字符串长度都为 2 。
```
**提示：**
- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 100`
- `-10^9 <= grid[r][c] <= 10^9`

---
### 解法1 数字转为字符串计算长度
```python
class Solution:
    def findColumnWidth(self, grid: List[List[int]]) -> List[int]:
        return [max(len(str(x)) for x in col) 
            for col in zip(*grid)]
```
```java
class Solution {
    public int[] findColumnWidth(int[][] grid) {
        int n = grid[0].length;
        int[] ans = new int[n];
        for (int j = 0; j < n; ++j) 
            for (int[] row : grid) 
                ans[j] = Math.max(ans[j], Integer.toString(row[j]).length());
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<int> findColumnWidth(vector<vector<int>>& grid) {
        int n = grid[0].size();
        vector<int> ans(n);
        for (int j = 0; j < n; ++j)
            for (auto& row : grid)
                ans[j] = max(ans[j], (int) to_string(row[j]).length());
        return ans;
    }
};
```
也可以手动计算长度。
```python
class Solution:
    def findColumnWidth(self, grid: List[List[int]]) -> List[int]:
        ans = [0] * len(grid[0])
        for j, col in enumerate(zip(*grid)):
            for x in col:
                x_len = int(x <= 0)
                x = abs(x)
                while x:
                    x_len += 1
                    x //= 10
                ans[j] = max(ans[j], x_len)
        return ans
```
```java
class Solution {
    public int[] findColumnWidth(int[][] grid) {
        int n = grid[0].length;
        int[] ans = new int[n];
        for (int j = 0; j < n; ++j) {
            for (int[] row : grid) {
                int len = row[j] <= 0 ? 1 : 0;
                for (int x = row[j]; x != 0; x /= 10) ++len;
                ans[j] = Math.max(ans[j], len);
            }
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<int> findColumnWidth(vector<vector<int>>& grid) {
        int n = grid[0].size();
        vector<int> ans(n);
        for (int j = 0; j < n; ++j) {
            for (auto& row : grid) {
                int len = row[j] <= 0;
                for (int x = row[j]; x; x /= 10) ++len;
                ans[j] = max(ans[j], len);
            }
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(mn\log U)$ ，其中 $m$ 和 $n$ 分别为 $\textit{grid}$ 的行数和列数，$U$ 为 $\textit{grid}[i][j]$ 的绝对值的最大值。
- 空间复杂度：$\mathcal{O}(1)$ 。返回值不计入。Python忽略 `zip(*grid)` 的空间。

---
### 解法2 优化
方法一需要对每个数字都计算长度。但实际上，**由于数字的绝对值越大，数字的长度就越长**，所以只需要**对每一列的最小值或最大值求长度**。

设列最小值和列最大值分别为 $\textit{mn}$ 和 $\textit{mx}$ 。由于负数中的负号也算一个长度，我们可以取
$$\max(\textit{mx}, -10\cdot \textit{mn})$$
的长度作为答案。

或者，**为避免乘法溢出**，取
$$\max\left(\left\lfloor\dfrac{\textit{mx}}{10}\right\rfloor, -\textit{mn}\right)$$​
的**长度加一**作为答案。此时把 $0$ 的长度视作 $0$ 。

注意上式在一整列全为负数或者全为正数时也是正确的。
```python
class Solution:
    def findColumnWidth(self, grid: List[List[int]]) -> List[int]:
        return [len(str(max(max(col), -10 * min(col))))
            for col in zip(*grid)]
```
```java
class Solution {
    public int[] findColumnWidth(int[][] grid) {
        int n = grid[0].length;
        int[] ans = new int[n];
        for (int j = 0; j < n; j++) {
            int mn = 0;
            int mx = 0;
            for (int[] row : grid) {
                mn = Math.min(mn, row[j]);
                mx = Math.max(mx, row[j]);
            }
            int len = 1;
            for (int x = Math.max(mx / 10, -mn); x > 0; x /= 10) {
                len++;
            }
            ans[j] = len;
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<int> findColumnWidth(vector<vector<int>>& grid) {
        int n = grid[0].size();
        vector<int> ans(n);
        for (int j = 0; j < n; j++) {
            int mn = 0, mx = 0;
            for (auto& row : grid) {
                mn = min(mn, row[j]);
                mx = max(mx, row[j]);
            }
            int len = 1;
            for (int x = max(mx / 10, -mn); x; x /= 10) {
                len++;
            }
            ans[j] = len;
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n(m+\log U))$ ，其中 $m$ 和 $n$ 分别为 $\textit{grid}$ 的行数和列数，$U$ 为 $\textit{grid}[i][j]$ 的绝对值的最大值。
- 空间复杂度：$\mathcal{O}(1)$ 。返回值不计入。Python 忽略 `zip(*grid)` 的空间。
