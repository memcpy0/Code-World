Given a **2D matrix** , find the **sum** of the elements inside the **rectangle** defined by its upper left corner $(row1, col1)$ and lower right corner $(row2, col2)$.

**Example:**
```
Given matrix = [
  [3, 0, 1, 4, 2],
  [5, 6, 3, 2, 1],
  [1, 2, 0, 1, 5],
  [4, 1, 0, 1, 7],
  [1, 0, 3, 0, 5]
]

sumRegion(2, 1, 4, 3) -> 8
sumRegion(1, 1, 2, 2) -> 11
sumRegion(1, 2, 2, 4) -> 12
```

Range Sum Query 2D：The above rectangle (with the red border) is defined by $(row1, col1) = (2, 1)$ and $(row2, col2) = (4, 3)$, which contains sum = 8.

**Note:**

-    You may assume that the matrix does not change.
-    There are many calls to sumRegion function.
-    You may assume that row1 ≤ row2 and col1 ≤ col2.


题意：多次询问一个二维矩形区域的数值之和。

思路：很简单的一道题目，不过如果是暴力循环求解的话，空间为 $O(1)$，时间为 $O(NM)$。因此，我们使用二维前缀和+容斥原理来求，可以用 $O(NM)$ 的空间，得到 $O(1)$ 的时间。具体的原理就不多说了。[【前缀和】详细介绍前缀和的思想与实现](https://blog.csdn.net/myRealization/article/details/104470754)。
 

代码：
```cpp
class NumMatrix {
public:
    int **p;
    NumMatrix(vector<vector<int>>& matrix) {
        if (matrix.size() == 0 || matrix[0].size() == 0) return; //为空矩阵
        int m = matrix.size(), n = matrix[0].size();
        p = new int*[m + 1];
        for (int i = 0; i <= m; ++i)  
            p[i] = new int[n + 1]();

        for (int i = 1; i <= m; ++i) 
            for (int j = 1; j <= n; ++j) 
                p[i][j] = p[i - 1][j] + p[i][j - 1] - p[i - 1][j - 1] + matrix[i - 1][j - 1];
    }
    
    int sumRegion(int row1, int col1, int row2, int col2) { 
        return p[row2 + 1][col2 + 1] - p[row2 + 1][col1] - p[row1][col2 + 1] + p[row1][col1];
    }
};
```
