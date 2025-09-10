> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在两条独立的水平线上按给定的顺序写下 `nums1` 和 `nums2` 中的整数。

现在，可以绘制一些连接两个数字 `nums1[i]` 和 `nums2[j]` 的直线，这些直线需要同时满足：
-  `nums1[i] == nums2[j]`
- 且绘制的直线不与任何其他连线（非水平线）相交。

请注意，连线即使在端点也不能相交：每个数字只能属于一条连线。

以这种方法绘制线条，并返回可以绘制的最大连线数。

**示例 1：**
![](https://assets.leetcode.com/uploads/2019/04/26/142.png)
```c
输入：nums1 = [1,4,2], nums2 = [1,2,4]
输出：2
解释：可以画出两条不交叉的线，如上图所示。 
但无法画出第三条不相交的直线，因为从 nums1[1]=4 到 nums2[2]=4 的直线将与从 nums1[2]=2 到 nums2[1]=2 的直线相交。
```
**示例 2：**
```cpp
输入：nums1 = [2,5,1,2,5], nums2 = [10,5,2,1,5,2]
输出：3
```
**示例 3：**
```cpp
输入：nums1 = [1,3,7,1,7,5], nums2 = [1,9,2,5,1]
输出：2
```
**提示：**
- `1 <= nums1.length, nums2.length <= 500`
- `1 <= nums1[i], nums2[j] <= 2000`

---
### 解法 动态规划
给定两个数组 $nums_1$ 和 $nums_2$ ，当 $nums_1[i]=nums_2[j]$ 时，可以用一条直线连接 $nums_1[i]$ 和 $nums_2[j]$ 。假设一共绘制了 $k$ 条互不相交的直线，其中第 $x$ 条直线连接 $nums_1[i_x]$ 和 $nums_2[j_x]$
 ，则对于任意 $1≤x≤k$ 都有 $nums_1[i_x]=nums_2[j_x]$ ，其中 $i_1 <i_2 <…<i_k$ ，$j_1 <j_2 < \dots ​ <j_k$ 。

上述 $k$ 条互不相交的直线分别连接了数组 $nums_1$ 和 $nums_2$ 的 $k$ 对**相等**的元素，而且这 $k$ 对相等的元素在两个数组中的**相对顺序是一致的**，因此，这 $k$ 对相等的元素组成的序列即为数组 $nums_1$
 和 $nums_2$ 的公共子序列。要计算可以绘制的最大连线数，即为计算数组 $nums_1$ 和 $nums_2$ 的**最长公共子序列**的长度。最长公共子序列问题是典型的二维动态规划问题。

假设数组 $nums_1$ 和 $nums_2$ 的长度分别为 $m$ 和 $n$ ，创建 $m+1$ 行 $n+1$ 列的二维数组 $dp$ ，其中 $dp[i][j]$ 表示 $nums_1[0:i]$ 和 $nums_2[0:j]$ 的最长公共子序列的长度。

上述表示中，$nums_1[0:i]$ 表示数组 $nums_1$ 的长度为 $i$ 的前缀，$nums_2[0:j]$ 表示数组 $nums_2$  的长度为 $j$ 的前缀。

考虑动态规划的边界情况：
- 当 $i=0$ 时，$nums_1[0:i]$ 为空，空数组和任何数组的最长公共子序列的长度都是 $0$ ，因此对任意 $0≤j≤n$ ，有 $dp[0][j]=0$ ；
- 当 $j=0$ 时，$nums_2[0:j]$ 为空，同理可得，对任意 $0≤i≤m$ ，有 $dp[i][0]=0$ 。

因此动态规划的边界情况是：当 $i=0$ 或 $j=0$ 时，$dp[i][j]=0$ 。

当 $i>0$ 且 $j>0$ 时，考虑 $dp[i][j]$ 的计算：
- 当 $nums_1[i−1]=nums_2[j−1]$ 时，将这两个相同的元素称为公共元素，考虑 $nums_1[0:i−1]$ 和 $nums_2[0:j−1]$ 的**最长公共子序列**，再增加一个元素（即公共元素）即可得到 $nums_1[0:i]$ 和 $nums_2[0:j]$ 的最长公共子序列，因此 $dp[i][j]=dp[i−1][j−1]+1$ 。
- 当 $nums_1[i−1] \ne nums_2[j−1]$ 时，考虑以下两项：
    - $nums_1[0:i−1]$ 和 $nums_2[0:j]$ 的最长公共子序列；
    - $nums_1[0:i]$ 和 $nums_2[0:j−1]$ 的最长公共子序列。
    - 要得到 $nums_1[0:i]$ 和 $nums_2[0:j]$ 的最长公共子序列，应取两项中的长度较大的一项，因此 $dp[i][j]=max(dp[i−1][j],dp[i][j−1])$ 。

由此可以得到如下状态转移方程：
$$dp[i][j]=\begin{cases} 
dp[i−1][j−1]+1, \quad &nums_1[i-1] = nums_2[j-1]   \\
\max(dp[i−1][j],dp[i][j−1]), \quad &nums_1[i-1] \ne nums_2[j-1]
\end{cases}
$$
​
最终计算得到 $dp[m][n]$ 即为数组 $nums_1$ 和 $nums_2$ 的最长公共子序列的长度，即可以绘制的最大连线数。
```java
class Solution {
    public int maxUncrossedLines(int[] nums1, int[] nums2) {
        int m = nums1.length, n = nums2.length;
        int[][] dp = new int[m + 1][n + 1];
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (nums1[i - 1] == nums2[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
                else dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        return dp[m][n];
    }
}
```
```cpp
class Solution {
public:
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {  
        int m = nums1.size(), n = nums2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (nums1[i] == nums2[j]) dp[i + 1][j + 1] = dp[i][j] + 1;
                else dp[i + 1][j + 1] = max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
        return  dp[m][n];
    }
};
```
```js
var maxUncrossedLines = function(nums1, nums2) {
    const m = nums1.length, n = nums2.length;
    const dp = new Array(m + 1).fill(0).map(() => new Array(n + 1).fill(0));
    for (let i = 1; i <= m; i++) {
        const num1 = nums1[i - 1];
        for (let j = 1; j <= n; j++) {
            const num2 = nums2[j - 1];
            if (num1 === num2) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
};
```
```go
func maxUncrossedLines(nums1, nums2 []int) int {
    m, n := len(nums1), len(nums2)
    dp := make([][]int, m+1)
    for i := range dp {
        dp[i] = make([]int, n+1)
    }
    for i, v := range nums1 {
        for j, w := range nums2 {
            if v == w {
                dp[i+1][j+1] = dp[i][j] + 1
            } else {
                dp[i+1][j+1] = max(dp[i][j+1], dp[i+1][j])
            }
        }
    }
    return dp[m][n]
}

func max(a, b int) int {
    if a > b {
        return a
    }
    return b
}
```
```python
class Solution:
    def maxUncrossedLines(self, nums1: List[int], nums2: List[int]) -> int:
        m, n = len(nums1), len(nums2)
        dp = [[0] * (n + 1) for _ in range(m + 1)]

        for i, num1 in enumerate(nums1):
            for j, num2 in enumerate(nums2):
                if num1 == num2:
                    dp[i + 1][j + 1] = dp[i][j] + 1
                else:
                    dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j])
        
        return dp[m][n]
```
```c
int maxUncrossedLines(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int m = nums1Size, n = nums2Size;
    int dp[m + 1][n + 1];
    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= m; i++) {
        int num1 = nums1[i - 1];
        for (int j = 1; j <= n; j++) {
            int num2 = nums2[j - 1];
            if (num1 == num2) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = fmax(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}
```
```csharp
public class Solution {
    public int MaxUncrossedLines(int[] nums1, int[] nums2) {
        int m = nums1.Length, n = nums2.Length;
        int[,] dp = new int[m + 1, n + 1];
        for (int i = 1; i <= m; i++) {
            int num1 = nums1[i - 1];
            for (int j = 1; j <= n; j++) {
                int num2 = nums2[j - 1];
                if (num1 == num2) {
                    dp[i, j] = dp[i - 1, j - 1] + 1;
                } else {
                    dp[i, j] = Math.Max(dp[i - 1, j], dp[i, j - 1]);
                }
            }
        }
        return dp[m, n];
    }
}
```
**复杂度分析**：
- 时间复杂度：$O(mn)$ ，其中 m 和 n 分别是数组 $nums_1$ 和 $nums_2$ 的长度。二维数组 $dp$ 有 $m+1$ 行和 $n+1$ 列，需要对 $dp$ 中的每个元素进行计算。
- 空间复杂度：$O(mn)$ 。
