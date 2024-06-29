> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Given an <code>m x n</code> binary <code>matrix</code> filled with <code>0</code>'s and <code>1</code>'s, <em>find the largest square containing only</em> <code>1</code>'s <em>and return its area</em>.</p>
 
<p><strong>Example 1:</strong></p>
<img style="width: 400px; height: 319px;" src="https://assets.leetcode.com/uploads/2020/11/26/max1grid.jpg" alt="">

```cpp
Input: matrix = [["1","0","1","0","0"],["1","0","1","1","1"],["1","1","1","1","1"],["1","0","0","1","0"]]
Output: 4 
```

<p><strong>Example 2:</strong></p>
<img style="width: 165px; height: 165px;" src="https://assets.leetcode.com/uploads/2020/11/26/max2grid.jpg" alt="">

```cpp
Input: matrix = [["0","1"],["1","0"]]
Output: 1 
```

<p><strong>Example 3:</strong></p>

```cpp
Input: matrix = [["0"]]
Output: 0 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>m == matrix.length</code></li>
	<li><code>n == matrix[i].length</code></li>
	<li><code>1 &lt;= m, n &lt;= 300</code></li>
	<li><code>matrix[i][j]</code> is <code>'0'</code> or <code>'1'</code>.</li>
</ul>


题意：在一个由 `'0'` 和 `'1'` 组成的二维矩阵内，找到只包含 `'1'` 的最大正方形，并返回其面积。

---
### 解法1 单调栈
这一道题只是[85. 最大矩形](https://leetcode-cn.com/problems/maximal-rectangle/)的变形，思路都差不多。算法的时间复杂度为 $O(mn)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int m = matrix.size(), n = matrix[0].size(), ans = 0;
        vector<int> heights(n, 0);
        for (int i = 0; i < m; ++i) {
            stack<pair<int, int>> st; //height,width
            int maxArea = 0;
            for (int j = 0; j < n; ++j) 
                (matrix[i][j] == '1') ? (++heights[j]) : (heights[j] = 0);
            for (int j = 0; j <= n; ++j) {
                int totWidth = 0, height = j < n ? heights[j] : 0; //高度为0的哨兵
                while (!st.empty() && height < st.top().first) {
                    int h = st.top().first, w = st.top().second;
                    st.pop();
                    totWidth += w; 
                    int edge = min(h, totWidth); //正方形的边长
                    maxArea = max(maxArea, edge * edge);
                }
                st.push(pair<int, int>{height, totWidth + 1});
            }
            ans = max(ans, maxArea);
        }
        return ans;
    }
};
//执行用时：28 ms, 在所有 C++ 提交中击败了41.71% 的用户
//内存消耗：11.9 MB, 在所有 C++ 提交中击败了5.00% 的用户
```

---
### 解法2 悬线法
关于悬线法的讲解，具体看[这篇文章](https://memcpy0.blog.csdn.net/article/details/119908788)。算法的时间复杂度为 $O(mn)$ ，空间复杂度为 $O(mn)$ ：
```cpp
//C++ version
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int m = matrix.size(), n = matrix[0].size(), ans = 0;
        using Matrix = vector<vector<int>>;
        using Row = vector<int>;
        Matrix u(m + 2, Row(n + 2)), l(m + 2, Row(n + 2)), r(m + 2, Row(n + 2));
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (matrix[i - 1][j - 1] == '1') {
                    u[i][j] = u[i - 1][j] + 1;
                    l[i][j] = l[i][j - 1] + 1;
                }
            }
        }
        for (int i = 1; i <= m; ++i)
            for (int j = n; j >= 1; --j)
                if (matrix[i - 1][j - 1] == '1')
                    r[i][j] = r[i][j + 1] + 1;
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (matrix[i - 1][j - 1] == '1') {
                    if (i > 1 && matrix[i - 2][j - 1] == '1') {
                        l[i][j] = min(l[i][j], l[i - 1][j]);
                        r[i][j] = min(r[i][j], r[i - 1][j]);
                    }
                    int edge = min(u[i][j], l[i][j] + r[i][j] - 1); //极大有效子正方形的边长
                    ans = max(ans, edge * edge);
                }
            }
        }
        return ans;
    }
};
//执行用时：32 ms, 在所有 C++ 提交中击败了27.39% 的用户
//内存消耗：12.8 MB, 在所有 C++ 提交中击败了5.00% 的用户
```
---
### 解法3 动态规划
使用纯粹的动态规划，可以得到更高效率的算法。这里设 `dp[i][j]` 为方格矩阵中以 `matrix[i - 1][j - 1]` 为右下角、能构成的最大正方形的边长，取所有这些边长的最大值，即可计算最大正方形的面积。递归公式如下：
```haskell
dp[i][j] = 
	| (matrix[i - 1][j - 1] == '1') = 1 + min(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1])
	| otherwise = 0
```
如果一时间想不明白，可以画个图：
<img src="https://img-blog.csdnimg.cn/0e07cb48f34c4ddc894aeba9e997d64f.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16" width="55%">

虽然时空复杂度仍为 $O(mn)$ ，但常数明显更小： 
```cpp
//C++ version
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1)); 
        //dp[i][j]为以第i-1行第j-1列为右下角,能构成的最大的正方形边长
        //dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]);
        int maxEdge = 0; //最长的正方形边长
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (matrix[i - 1][j - 1] == '1') {
                    dp[i][j] = 1 + min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]);
                    maxEdge = max(maxEdge, dp[i][j]);
                }
            }
        }
        return maxEdge * maxEdge;
    } 
};
//执行用时：24 ms, 在所有 C++ 提交中击败了74.30% 的用户
//内存消耗：11.5 MB, 在所有 C++ 提交中击败了74.41% 的用户
```
通过使用滚动数组，可以优化空间到 $O(2n)$ ：
```cpp
//C++ version
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> dp(2, vector<int>(n + 1)); 
        //dp[i][j]为以第i-1行第j-1列为右下角,能构成的最大的正方形边长
        //dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]);
        int maxEdge = 0; //最长的正方形边长
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (matrix[i - 1][j - 1] == '1') {
                    dp[i & 1][j] = 1 + min(min(dp[!(i & 1)][j], dp[i & 1][j - 1]), dp[!(i & 1)][j - 1]);
                    maxEdge = max(maxEdge, dp[i & 1][j]);
                } else dp[i & 1][j] = 0; //记得清零
            }
        }
        return maxEdge * maxEdge;
    } 
};
//执行用时：16 ms, 在所有 C++ 提交中击败了98.94% 的用户
//内存消耗：10.9 MB, 在所有 C++ 提交中击败了89.09% 的用户
```
如果再狠一点，可以用一维数组加上几个变量，进一步优化空间到 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<int> dp(n); 
        //dp[i][j]为以第i-1行第j-1列为右下角,能构成的最大的正方形边长
        //dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]);
        int pre = 0, temp = 0;
        int maxEdge = 0; //最长的正方形边长
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == 0 || j == 0 || matrix[i][j] == '0') { //注意边界情况
                    pre = dp[j]; dp[j] = matrix[i][j] - '0';
                } else { //此处原dp[j]指的是dp[i-1][j],dp[j-1]指的是dp[i][j-1],
                    temp = dp[j];
                    dp[j] = 1 + min(min(dp[j], dp[j - 1]), pre); //pre指的是dp[i-1][j-1]
                    pre = temp;
                }
                maxEdge = max(maxEdge, dp[j]);
            }
        }
        return maxEdge * maxEdge;
    } 
};
//执行用时：16 ms, 在所有 C++ 提交中击败了98.94% 的用户
//内存消耗：10.9 MB, 在所有 C++ 提交中击败了87.50% 的用户
```
