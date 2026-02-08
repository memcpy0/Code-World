> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个大小为 `m x n` 的矩阵 `mat` 和一个整数阈值 `threshold`。

请你返回元素总和小于或等于阈值的正方形区域的最大边长；如果没有这样的正方形区域，则返回 **0** 。  
 
**示例 1：**
![](https://assets.leetcode.cn/aliyun-lc-upload/uploads/2019/12/15/e1.png)
```java
输入：mat = [[1,1,3,2,4,3,2],[1,1,3,2,4,3,2],[1,1,3,2,4,3,2]], threshold = 4
输出：2
解释：总和小于或等于 4 的正方形的最大边长为 2，如图所示。
```
**示例 2：**
```java
输入：mat = [[2,2,2,2,2],[2,2,2,2,2],[2,2,2,2,2],[2,2,2,2,2],[2,2,2,2,2]], threshold = 1
输出：0
```
**提示：**
- `m == mat.length`
- `n == mat[i].length`
- `1 <= m, n <= 300`
- `0 <= mat[i][j] <= 10^4`
- `0 <= threshold <= 10^5`
---
### 解法 无需二分，暴力从头枚举边长就是 $O(mn)$
预处理二维前缀和后，暴力的做法是写一个三重循环：
1. 外面两重循环，枚举正方形的左上角 $(i,j)$ 。
2. 最内层循环，枚举正方形的边长为 $1,2,3,\dots$ 直到出界或者正方形元素和超过 $\text{threshold}$ 为止。在此过程中更新答案 $ans$ 的最大值。
这样做的时间复杂度是 $O(mn \min(m, n))$ 。

修改枚举算法的最直观思路是，使用二分法计算正方形的最大边长，时间复杂度 $O(mn\log min(m,n))$ 。

但我们也可以考虑下直接优化枚举算法，三重循环中前两重枚举正方形的左上角位置，似乎没有什么优化的空间；而第三重枚举的是正方形的边长，对此容易想到两个优化思路：
1. 如果边长为 $c$ 的正方形的元素和已超过阈值，那就没必要枚举更大的边长了。因为数组 $mat$ 中的所有元素均为非负整数，如果固定了左上角的位置 $(i, j)$（即前两重循环），那么随着边长的增大，正方形的元素和也会增大。
2. 由于目标是找到边长最大的正方形，那如果前两重循环枚举到 $(i, j)$ 前，已经找到一个边长为 $ans$ 的正方形，那在枚举以 $(i, j)$ 为左上角的正方形时，可忽略所有边长小于等于 $ans$ 的正方形，直接从 $ans + 1$ 开始枚举。比如现在 $ans = 3$ ，那么枚举正方形的边长 $1,2,3$ 毫无意义，**不会让答案变得更大**，所以直接从 $ans+1=4$ 开始枚举更好。
```java
class Solution {
    public int maxSideLength(int[][] mat, int threshold) {
        int m = mat.length;
        int n = mat[0].length;
        int[][] sum = new int[m + 1][n + 1];
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                sum[i + 1][j + 1] = sum[i + 1][j] + sum[i][j + 1] - sum[i][j] + mat[i][j];
            }
        }
        int ans = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                // 边长为 ans+1 的正方形，左上角在 (i,j)，右下角在 (i+ans, j+ans)
                while (i + ans < m && j + ans < n && query(sum, i, j, i + ans, j + ans) <= threshold) {
                    ++ans;
                }
            }
        }
        return ans;
    }
    // 返回左上角在 (r1,c1) ，右下角在 (r2,c2) 的子矩阵元素和
    private int query(int[][] sum, int r1, int c1, int r2, int c2) {
        return sum[r2 + 1][c2 + 1] - sum[r2 + 1][c1] - sum[r1][c2 + 1] + sum[r1][c1];
    }
}
```
```cpp
class Solution {
public:
    int maxSideLength(vector<vector<int>>& mat, int threshold) {
        int m = mat.size(), n = mat[0].size();
        vector sum(m + 1, vector<int>(n + 1));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                sum[i + 1][j + 1] = sum[i + 1][j] + sum[i][j + 1] - sum[i][j] + mat[i][j];
            }
        }
        // 返回左上角在 (r1, c1)，右下角在 (r2, c2) 的子矩阵元素和
        auto query = [&](int r1, int c1, int r2, int c2) -> int {
            return sum[r2 + 1][c2 + 1] - sum[r2 + 1][c1] - sum[r1][c2 + 1] + sum[r1][c1];
        };

        int ans = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // 边长为 ans+1 的正方形，左上角在 (i, j)，右下角在 (i+ans, j+ans)
                while (i + ans < m && j + ans < n && query(i, j, i + ans, j + ans) <= threshold) {
                    ans++;
                }
            }
        }
        return ans;
    }
};
```
```python
class Solution:
    def maxSideLength(self, mat: List[List[int]], threshold: int) -> int:
        m, n = len(mat), len(mat[0])
        s = [[0] * (n + 1) for _ in range(m + 1)]
        for i, row in enumerate(mat):
            for j, x in enumerate(row):
                s[i + 1][j + 1] = s[i + 1][j] + s[i][j + 1] - s[i][j] + x

        # 返回左上角在 (r1, c1)，右下角在 (r2, c2) 的子矩阵元素和
        def query(r1: int, c1: int, r2: int, c2: int) -> int:
            return s[r2 + 1][c2 + 1] - s[r2 + 1][c1] - s[r1][c2 + 1] + s[r1][c1]

        ans = 0
        for i in range(m):
            for j in range(n):
                # 边长为 ans+1 的正方形，左上角在 (i, j)，右下角在 (i+ans, j+ans)
                while i + ans < m and j + ans < n and query(i, j, i + ans, j + ans) <= threshold:
                    ans += 1
        return ans
```
```go
func maxSideLength(mat [][]int, threshold int) (ans int) {
	m, n := len(mat), len(mat[0])
	sum := make([][]int, m+1)
	sum[0] = make([]int, n+1)
	for i, row := range mat {
		sum[i+1] = make([]int, n+1)
		for j, x := range row {
			sum[i+1][j+1] = sum[i+1][j] + sum[i][j+1] - sum[i][j] + x
		}
	}

	// 返回左上角在 (r1, c1)，右下角在 (r2, c2) 的子矩阵元素和
	query := func(r1, c1, r2, c2 int) int {
		return sum[r2+1][c2+1] - sum[r2+1][c1] - sum[r1][c2+1] + sum[r1][c1]
	}

	for i := range m {
		for j := range n {
			// 边长为 ans+1 的正方形，左上角在 (i, j)，右下角在 (i+ans, j+ans)
			for i+ans < m && j+ans < n && query(i, j, i+ans, j+ans) <= threshold {
				ans++
			}
		}
	}
	return
}
```
注：**外层循环可以改成 `i + ans < m` 以及 `j + ans < n`** 。但测试了一下，并没有明显提升。

**还可把二维前缀和与答案计算放一起循环**，此时内部计算 $ans$ 时，需考虑【右下角为 $(i,j)$ 的正方形】在 $(0,0)$ 到 $(i,j)$ 形成的矩形范围内，保持 $ans \le \min(i,j)+1$ 。
```java
class Solution {
    public int maxSideLength(int[][] mat, int threshold) {
        int m = mat.length;
        int n = mat[0].length;
        int[][] sum = new int[m + 1][n + 1];
        int ans = 0; // 无解情况
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                sum[i + 1][j + 1] = sum[i + 1][j] + sum[i][j + 1] - sum[i][j] + mat[i][j];
                // 左上角在 (0,0) ，右下角在 (i,j) 的矩形范围内，正方形右下角为 (i,j) ，枚举边长
                int k = ans + 1; // 从1开始枚举
                while (k <= Math.min(i, j) + 1) { 
                    if (query(sum, i - k + 1, j - k + 1, i, j) > threshold) {
                        break;
                    }
                    ans = k++;
                }
            }
        }
        return ans;
    }
    // 返回左上角在 (r1,c1) ，右下角在 (r2,c2) 的子矩阵元素和
    private int query(int[][] sum, int r1, int c1, int r2, int c2) {
        return sum[r2 + 1][c2 + 1] - sum[r2 + 1][c1] - sum[r1][c2 + 1] + sum[r1][c1];
    }
}
```
复杂度分析：
- 时间复杂度：$O(mn)$ ，其中 $m$ 和 $n$ 分别是 $grid$ 的行数和列数。虽然我们写了个三重循环，但由于答案最大是 $\min(m,n)$ ，所以最内层的 $\text{++ans}$ 最多执行 $\min(m,n)$ 次，三重循环的时间复杂度为 $O(mn+\min(m,n))=O(mn)$ 。
- 空间复杂度：$O(mn)$ 。
> **证明1**：反证法。如果这个算法的复杂度为 $O(mn \min(m, n))$ ，则最内层的 $ans++$ 会执行 $O(mn\min (m, n))$ 次，但答案最大是 $O(\min(m, n))$ ，矛盾。所以这个算法不可能跑到 $O(mn\min (m, n))$ 。
>
> **证明2**：算法时间复杂度，显然等于第三重循环中边长 $c$ 被枚举的次数。由于优化后第三重循环的上下界并不固定，因此使用一些技巧，将第三重循环中边长 $c$ 的枚举分为两类：
> - 成功枚举：如果当前枚举的边长为 $c$ 的正方形的元素和不超过阈值，称为一次「**成功枚举**」。成功枚举后，找到了比之前边长更大的正方形。
> - 失败枚举：如果当前枚举的边长为 $c$ 的正方形的元素和大于阈值，称为一次「**失败枚举**」。失败枚举后，就没有必要枚举更大的边长了，会直接跳出第三重循环。
> - 对于「成功枚举」而言，由于每进行一次「成功枚举」，都会得到一个边长更大的正方形，而边长的最大值不会超过 $\min(m, n)$ ，因此 **「成功枚举」的总次数也不会超过 $\min(m, n)$** ；
> - 对于「失败枚举」而言，由于每进行一次「失败枚举」，都会直接跳出第三重循环，因此每一个左上角的位置 $(i, j)$ 最多只会对应一次「失败枚举」，即 **「失败枚举」的总次数不会超过 $mn$** 。因此，优化后算法的时间复杂度为 $O(\min(m, n)+mn)=O(mn)$ ，它比二分查找更优。 

本题还有一种做法：**枚举对角线**，在对角线上做 **不定长滑动窗口**，把正方形的左上角和右下角看作滑动窗口的左右端点。这也可以做到 $O(mn)$ 时间。

用这个思路解决如下问题：计算元素总和小于或等于阈值的正方形的**个数**。
