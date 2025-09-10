> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091813389.png)
给你一个正整数 `n` ，生成一个包含 `1` 到 `n2` 所有元素，且元素按顺时针顺序螺旋排列的 `n x n` 正方形矩阵 `matrix` 。

**示例 1：**
![](https://assets.leetcode.com/uploads/2020/11/13/spiraln.jpg)
```js
输入：n = 3
输出：[[1,2,3],[8,9,4],[7,6,5]]
```
**示例 2：**
```js
输入：n = 1
输出：[[1]]
```
**提示：**
- `1 <= n <= 20`

---
### 解法 数组+模拟
一种写法如下，用 $0$ 和 $n$ 作为边界，并用「判断 $ans$ 要填的格子」是否为 $0$ 来判断是否越界：
```cpp
class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> ans(n, vector<int>(n));
        int c = 0, ns = n * n;
        int x = 0, y = -1;
        while (c < ns) {
            while (c < ns && y + 1 < n && !ans[x][y + 1]) 
                ans[x][++y] = ++c;
            while (c < ns && x + 1 < n && !ans[x + 1][y])
                ans[++x][y] = ++c;            
            while (c < ns && y - 1 >= 0 && !ans[x][y - 1])
                ans[x][--y] = ++c;
            while (c < ns && x - 1 >= 0 && !ans[x - 1][y])
                ans[--x][y] = ++c;
        }
        return ans;
    }
};
```
还可以这样写：
```cpp
class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> ans(n, vector<int>(n));
        int c = 0, ns = n * n;
        int x = 0;
        while (c < ns) {
            // 每次螺旋开始向右,y都等于x
            for (int y = x; y < n - x; ++y)
                ans[x][y] = ++c;
            // 向下
            for (int y = x + 1; y < n - x; ++y) 
                ans[y][n - x - 1] = ++c;
            // 向左
            for (int y = n - x - 2; y >= x; --y)
                ans[n - x - 1][y] = ++c;
            for (int y = n - x - 2; y > x; --y)
                ans[y][x] = ++c;
            ++x; // 加1层螺旋
        }
        return ans;
    }
};
```