> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Given a <code>rows x cols</code>&nbsp;binary <code>matrix</code> filled with <code>0</code>'s and <code>1</code>'s, find the largest rectangle containing only <code>1</code>'s and return <em>its area</em>.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 402px; height: 322px;" src="https://assets.leetcode.com/uploads/2020/09/14/maximal.jpg" alt="">
 

```clike
Input: matrix = [["1","0","1","0","0"],["1","0","1","1","1"],["1","1","1","1","1"],["1","0","0","1","0"]]
Output: 6
Explanation: The maximal rectangle is shown in the above picture.
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: matrix = []
Output: 0
```

<p><strong>Example 3:</strong></p>

```clike
Input: matrix = [["0"]]
Output: 0
```

<p><strong>Example 4:</strong></p>

```clike
Input: matrix = [["1"]]
Output: 1
```
 
<p><strong>Example 5:</strong></p>

```clike
Input: matrix = [["0","0"]]
Output: 0
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>rows == matrix.length</code></li>
	<li><code>cols == matrix[i].length</code></li>
	<li><code>0 &lt;= row, cols &lt;= 200</code></li>
	<li><code>matrix[i][j]</code> is <code>'0'</code> or <code>'1'</code>.</li>
</ul>

题意：给定一个仅包含 `0` 和 `1` 、大小为 `rows x cols` 的二维二进制矩阵，找出只包含 `1` 的最大矩形，并返回其面积。

---
### 解法1 单调栈
如果暴力确定矩形左上点和右下点，计算是否全1（用上前缀和优化），这种做法的复杂度可以达到 $O(n^4)$ ，绝对会超时。

但是又不难发现，只要把包含 `1` 的格子看做柱子的组成部分，本题就十分类似于84. Largest Rectangle in Histogram。于是在每一层，我们将当前层看做柱状图的底部，本层某个格子为 `1` 时就累计上层的 `1` ，作为一根柱子，然后套用84题柱状图最大面积的解法，不断更新全局的全 `1` 矩形面积最大值。整个算法的时间复杂度为 $O(mn)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty()) return 0;
        int m = matrix.size(), n = matrix[0].size(), ans = 0;
        vector<int> heights(n, 0);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == '0') heights[j] = 0;
                else ++heights[j]; //连接到本层的柱子的高度
            }
            ans = max(ans, maxRectInHist(heights));
        }
        return ans;
    }
    int maxRectInHist(const vector<int>& hist) {
        int maxArea = 0, n = hist.size();
        vector<pair<int, int>> st;
        for (int i = 0; i <= n; ++i) {
            int height = i < n ? hist[i] : 0, width = 0; //最后多加一个0高度柱子作为哨兵
            while (!st.empty() && height < st.back().first) { //当前柱子高度更低,前面的柱子无法往右扩张 
                width += st.back().second;
                maxArea = max(maxArea, width * st.back().first); //前面的柱子形成的最大矩形面积可被计算
                st.pop_back();
            }
            st.push_back({height, width + 1});
        }
        return maxArea;
    }
};
//执行用时：28 ms, 在所有 C++ 提交中击败了95.92% 的用户
//内存消耗：11.3 MB, 在所有 C++ 提交中击败了78.53% 的用户
```
稍微简化了一下代码：
```cpp
//C++ version
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty()) return 0;
        int m = matrix.size(), n = matrix[0].size(), ans = 0;
        vector<int> heights(n, 0);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) 
                matrix[i][j] == '0' ? (heights[j] = 0) : (heights[j] += 1); //连接到本层的柱子
            int maxArea = 0;
            vector<pair<int, int>> st;
            for (int j = 0; j <= n; ++j) {
                int height = j < n ? heights[j] : 0, width = 0; //最后多加一个0高度柱子作为哨兵
                while (!st.empty() && height < st.back().first) { //当前柱子高度更低,前面的柱子无法往右扩张 
                    width += st.back().second;
                    maxArea = max(maxArea, width * st.back().first); //前面的柱子形成的最大矩形面积可被计算
                    st.pop_back();
                }
                st.push_back({height, width + 1});
            }
            ans = max(ans, maxArea);
        }
        return ans;
    } 
};
//执行用时：24 ms, 在所有 C++ 提交中击败了99.07% 的用户
//内存消耗：11.2 MB, 在所有 C++ 提交中击败了86.21% 的用户
```

---
### 解法2 悬线法
关于悬线法的具体讲解，见[这篇文章](https://memcpy0.blog.csdn.net/article/details/119908788)。下面只给出代码，时间复杂度为 $O(mn)$ ，空间复杂度为 $O(mn)$ ：
```cpp
//C++ version
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int m = matrix.size(), n = matrix[0].size(), ans = 0;
        using Matrix = vector<vector<int>>;
        using row = vector<int>;
        Matrix u(m + 2, row(n + 2)), l(m + 2, row(n + 2)), r(m + 2, row(n + 2)); //便于处理边界
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
                    if (i > 1 && matrix[i - 2][j - 1] == '1') { //注意是字符
                        l[i][j] = min(l[i - 1][j], l[i][j]);
                        r[i][j] = min(r[i - 1][j], r[i][j]);
                    }
                    ans = max(ans, u[i][j] * (l[i][j] + r[i][j] - 1));
                }
            }
        }
        return ans;
    }   
};
//执行用时：36 ms, 在所有 C++ 提交中击败了54.06% 的用户
//内存消耗：12.7 MB, 在所有 C++ 提交中击败了38.14% 的用户
```
