> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091741390.png)

Given a matrix of `m x n` elements (`m` rows, `n` columns), return all elements of the matrix **in spiral order**.

Example 1:
```cpp
Input:
[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
Output: [1,2,3,6,9,8,7,4,5]
```

Example 2:
```cpp
Input:
[
  [1, 2, 3, 4],
  [5, 6, 7, 8],
  [9,10,11,12]
]
Output: [1,2,3,4,8,12,11,10,9,5,6,7]
```

题意：给出一个 `m x n` 的矩阵，按照螺旋式打印其数值。

思路：螺旋矩阵，先是使用第一行的数字，然后使用最后一列，然后是最后一行，然后是第一列，如此循环。因此，可以用 `left, right, up, down` 四个变量来控制要打印的矩阵的范围，然后按照右下左上的动作循环。

代码：
```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> ans;
        if (matrix.empty()) return ans;
        int down = matrix.size(), right = matrix[0].size(), left = 0, up = 0; //四周边界
        int tot = down * right - 1, r = 0, c = 0;
        ans.push_back(matrix[r][c]);
        while (tot > 0) {  //用数量控制循环            
            while (c + 1 < right && tot > 0)  ans.push_back(matrix[r][++c]), --tot;
            if (tot > 0) ++up;    //去除第一行
            while (r + 1 < down && tot > 0)  ans.push_back(matrix[++r][c]), --tot;
            if (tot > 0) --right; //去除最后一列
            while (c - 1 >= left && tot > 0) ans.push_back(matrix[r][--c]), --tot;
            if (tot > 0) --down;  //去除最后一行
            while (r - 1 >= up && tot > 0) ans.push_back(matrix[--r][c]), --tot;
            if (tot > 0) ++left;  //去除第一列
        }
        return ans;
    }
};
```
