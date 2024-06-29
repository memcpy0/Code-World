 >本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You have a convex `n`-sided polygon where each vertex has an integer value. You are given an integer array `values` where `values[i]` is the value of the `ith` vertex (i.e., **clockwise order**).

You will **triangulate** the polygon into `n - 2` triangles. For each triangle, the value of that triangle is the product of the values of its vertices, and the total score of the triangulation is the sum of these values over all `n - 2` triangles in the triangulation.

Return _the smallest possible total score that you can achieve with some triangulation of the polygon_.

**Example 1:**
![](https://assets.leetcode.com/uploads/2021/02/25/shape1.jpg)
```java
Input: values = [1,2,3]
Output: 6
Explanation: The polygon is already triangulated, and the score of the only triangle is 6.
```
**Example 2:**

![](https://assets.leetcode.com/uploads/2021/02/25/shape2.jpg)

```java
Input: values = [3,7,4,5]
Output: 144
Explanation: There are two triangulations, with possible scores: 3*7*5 + 4*5*7 = 245, or 3*4*5 + 3*4*7 = 144.
The minimum score is 144.
```
**Example 3:**
![](https://assets.leetcode.com/uploads/2021/02/25/shape3.jpg)
```java
Input: values = [1,3,1,4,1,5]
Output: 13
Explanation: The minimum score triangulation has score 1*1*3 + 1*1*4 + 1*1*5 + 1*1*1 = 13.
```
**Constraints:**
-   `n == values.length`
-   `3 <= n <= 50`
-   `1 <= values[i] <= 100`

题意：有一个凸的 `n` 边形，每个顶点都有一个整数值。给定一个整数数组 `values` ，其中 `values[i]` 是第 `i` 个顶点的值（即 **顺时针顺序** ）。

假设将多边形 **剖分** 为 `n - 2` 个三角形。对于每个三角形，该三角形的值是顶点标记的**乘积**，三角剖分的分数是进行三角剖分后所有 `n - 2` 个三角形的值之和。返回 _多边形进行三角剖分后可以得到的最低分_ 。

---
### 解法1 记忆化搜索
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304022123366.png)

区间 DP 有一个「复制一倍，断环成链」的技巧，本题为什么不用这样计算？

答：无论如何旋转多边形，无论从哪条边开始计算，得到的结果都是一样的，那么不妨就从 000 - (n−1)(n-1)(n−1) 这条边开始计算。

Python3
Java
C++
Go
class Solution {
public:
    int minScoreTriangulation(vector<int> &v) {
        int n = v.size(), memo[n][n];
        memset(memo, -1, sizeof(memo)); // -1 表示还没有计算过
        function<int(int, int)> dfs = [&](int i, int j) -> int {
            if (i + 1 == j) return 0; // 只有两个点，无法组成三角形
            int &res = memo[i][j]; // 注意这里是引用，下面会直接修改 memo[i][j]
            if (res != -1) return res;
            res = INT_MAX;
            for (int k = i + 1; k < j; ++k) // 枚举顶点 k
                res = min(res, dfs(i, k) + dfs(k, j) + v[i] * v[j] * v[k]);
            return res;
        };
        return dfs(0, n - 1);
    }
};
复杂度分析
时间复杂度：O(n3)O(n^3)O(n 
3
 )，其中 nnn 为 values\textit{values}values 的长度。动态规划的时间复杂度 === 状态个数 ×\times× 单个状态的计算时间。本题中状态个数等于 O(n2)O(n^2)O(n 
2
 )，单个状态的计算时间为 O(n)O(n)O(n)，因此时间复杂度为 O(n3)O(n^3)O(n 
3
 )。
空间复杂度：O(n2)O(n^2)O(n 
2
 )。有 O(n2)O(n^2)O(n 
2
 ) 个状态。
二、1:1 翻译成递推
根据视频中讲的，把 dfs\textit{dfs}dfs 改成 fff 数组，把递归改成循环就好了。相当于原来是用递归计算每个状态 (i,j)(i,j)(i,j)，现在改用循环去计算每个状态 (i,j)(i,j)(i,j)。

状态转移方程和递归完全一致：

f[i][j]=min⁡k=i+1j−1{f[i][k]+f[k][j]+v[i]⋅v[j]⋅v[k]} f[i][j]=\min_{k=i+1}^{j-1}\{f[i][k]+f[k][j]+v[i]\cdot v[j]\cdot v[k]\}
f[i][j]= 
k=i+1
min
j−1
​
 {f[i][k]+f[k][j]+v[i]⋅v[j]⋅v[k]}
需要注意循环的顺序：

由于 i<ki<ki<k，f[i]f[i]f[i] 要能从 f[k]f[k]f[k] 转移过来，必须先计算出 f[k]f[k]f[k]，所以 iii 要倒序枚举；
由于 j>kj>kj>k，f[i][j]f[i][j]f[i][j] 要能从 f[i][k]f[i][k]f[i][k] 转移过来，必须先计算出 f[i][k]f[i][k]f[i][k]，所以 jjj 要正序枚举。
Python3
Java
C++
Go
class Solution {
public:
    int minScoreTriangulation(vector<int> &v) {
        int n = v.size(), f[n][n];
        memset(f, 0, sizeof(f));
        for (int i = n - 3; i >= 0; --i)
            for (int j = i + 2; j < n; ++j) {
                f[i][j] = INT_MAX;
                for (int k = i + 1; k < j; ++k)
                    f[i][j] = min(f[i][j], f[i][k] + f[k][j] + v[i] * v[j] * v[k]);
            }
        return f[0][n - 1];
    }
};
复杂度分析
时间复杂度：O(n3)O(n^3)O(n 
3
 )，其中 nnn 为 values\textit{values}values 的长度。动态规划的时间复杂度 === 状态个数 ×\times× 单个状态的计算时间。本题中状态个数等于 O(n2)O(n^2)O(n 
2
 )，单个状态的计算时间为 O(n)O(n)O(n)，因此时间复杂度为 O(n3)O(n^3)O(n 
3
 )。
空间复杂度：O(n2)O(n^2)O(n 
2
 )。有 O(n2)O(n^2)O(n 
2
 ) 个状态。
课后作业
312. 戳气球
375. 猜数字大小 II
1312. 让字符串成为回文串的最少插入次数
1547. 切棍子的最小成本
1000. 合并石头的最低成本

 