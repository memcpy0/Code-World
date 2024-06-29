> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a non-negative integer numRows, generate the first *numRows* of Pascal's triangle. ![在这里插入图片描述](https://img-blog.csdnimg.cn/img_convert/8c3da7a30a5796e97de5bd9cbc677776.png#pic_center)
In Pascal's triangle, each number is the sum of the two numbers directly above it.

**Example:**
```swift
Input: 5
Output:
[
     [1],
    [1,1],
   [1,2,1],
  [1,3,3,1],
 [1,4,6,4,1]
]
```
题意：给定一个非负整数 `numRows` ，生成杨辉三角的前 `numRows` 行。

---
### 解法 递推
杨辉三角，是二项式系数在三角形中的一种几何排列。它是中国古代数学的杰出研究成果之一，它把二项式系数图形化，**把组合数内在的一些代数性质直观地从图形中体现出来**，是一种离散型的数与形的结合。

杨辉三角具有以下性质：
1. **每行数字左右对称**，由 $1$ 开始逐渐变大再变小，并最终回到 $1$ 。
2. 第 $n$ 行（从 $0$ 开始编号）的数字有 $n+1$ 项，前 $n$ 行共有 $\frac{n(n+1)}{2}$ 个数。
3. **第 $n$ 行的第 $m$ 个数（从 $0$ 开始编号）可表示为组合数 $\mathcal{C}(n,m)$** ，记作 $\mathcal{C}_n^m$ 或 $\binom{n}{m}$ ，即为**从 $n$ 个不同元素中取 $m$ 个元素的组合数**。我们可以用公式来表示它：$$\mathcal{C}_n^m=\dfrac{n!}{m!\times (n-m)!}$$
4. **每个数字等于上一行的左右两个数字之和**，可用此性质写出整个杨辉三角。即第 $n$ 行的第 $i$ 个数等于第 $n-1$ 行的第 $i−1$ 个数和第 $i$ 个数之和。这也是组合数的性质之一，即 $$\mathcal{C}_n^i=\mathcal{C}_{n-1}^i+\mathcal{C}_{n-1}^{i-1}$$
5. $(a+b)^n$ 的展开式（**二项式展开**）中的各项系数依次对应杨辉三角的第 $n$ 行中的每一项。

依据性质 $4$ ，我们可以一行一行地计算杨辉三角。每当我们计算出第 $i$ 行的值，可以在线性时间复杂度内计算出第 $i+1$ 行的值。

```cpp
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> ret(numRows);
        for (int i = 0; i < numRows; ++i) {
            ret[i].resize(i + 1);
            ret[i][0] = ret[i][i] = 1;
            for (int j = 1; j < i; ++j) {
                ret[i][j] = ret[i - 1][j] + ret[i - 1][j - 1];
            }
        }
        return ret;
    }
};
```
复杂度分析：
- 时间复杂度：$O(\textit{numRows}^2)$ 。
- 空间复杂度：$O(1)$ 。不考虑返回值的空间占用。