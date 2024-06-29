Write an algorithm such that if an element in an MxN matrix is 0, its entire row and column are set to `0` .

**Example 1:**
```clike
Input: 
[
  [1,1,1],
  [1,0,1],
  [1,1,1]
]
Output: 
[
  [1,0,1],
  [0,0,0],
  [1,0,1]
]
```
**Example 2:**
```clike
Input: 
[
  [0,1,2,0],
  [3,4,5,2],
  [1,3,1,5]
]
Output: 
[
  [0,0,0,0],
  [0,4,5,0],
  [0,3,1,0]
]
```

题意：给出一个矩阵，如果一个 `M x N` 的矩阵中某个元素是 `0` ，则需要将它所在的那一整行和一整列设置为 `0` 。

---
### 解法1 另一个标记矩阵或坐标数组
这个问题似乎很简单：只需迭代矩阵，每次我们看到值为零的单元格时，将其行和列设置为零。问题是：访问到另一个单元格（在那行或那列中）时又将看到零，然后改变它们对应的行和列为零。很快，整个矩阵就会被设为零。总结为一句话：**找到零时如果直接清除行和列，则可能会清理整个矩阵。**

因此，**在对矩阵进行任何更改之前，首先尝试找到所有的零**。解决这个问题的一个方法是**使用第二个矩阵来标记零位置**。然后对矩阵进行第二次遍历来设置零行和零列。这将始终需要 $O(mn)$ 空间。代码如下：
```cpp
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<bool>> tempMatrix(m, vector<bool>(n));
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j) 
                tempMatrix[i][j] = !matrix[i][j]; 
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (tempMatrix[i][j]) {
                    for (int k = 0; k < n; ++k) matrix[i][k] = 0;
                    for (int k = 0; k < m; ++k) matrix[k][j] = 0;
                }
            }
        }
    }
};
```
提交后结果如下，时空效率不算高，因为我们做了许多次重复的工作，对已经为零的行和列进行置零操作：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了64.86% 的用户
内存消耗：12.1 MB, 在所有 C++ 提交中击败了35.80% 的用户
```
**使用零元素坐标数组**，在零元素较少时能节省空间，也可能增加空间的浪费：
```cpp
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        vector<pair<int, int>> coordinates;
        int m = matrix.size(), n = matrix[0].size();
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j) 
                if (!matrix[i][j]) 
                    coordinates.emplace_back(i, j);
        for (int i = 0; i < coordinates.size(); ++i) {
            int row = coordinates[i].first, col = coordinates[i].second;
            for (int j = 0; j < n; ++j) matrix[row][j] = 0;
            for (int j = 0; j < m; ++j) matrix[j][col] = 0;
        }
    }
};
```
效率如下，差不多：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了64.86% 的用户
内存消耗：11.9 MB, 在所有 C++ 提交中击败了62.15% 的用户
```

---
### 解法2 行列布尔数组
实际上，我们不需要 $O(mn)$ 空间。因为会将整个行和列设置为零，所以不需要知道它正好是 `cell[2][4]` （第2行，第4列）。只需要知道第 `2` 行的某个地方有个 `0` ，第 `4` 列的某个地方有个 `0` 。**无论如何，我们都会将整个行和列设置为零**，所以不需要跟踪零的确切行列坐标。

下面的Java代码实现了这个算法。它使用两个布尔数组来跟踪**所有带零的行**和**所有带零的列**，然后根据这些数组中的值，将对应的行和列置零。这样做提高了算法的时间效率，**避免重复置零**，也降低了空间消耗，空间效率为 $O(n)$ ：
```cpp
class Solution {
    public void setZeroes(int[][] matrix) {
        boolean[] row = new boolean[matrix.length];
        boolean[] col = new boolean[matrix[0].length];
        //Store the row and colum index with value 0
        for (int i = 0; i < matrix.length; ++i) {
            for (int j = 0; j < matrix[0].length; ++j) {
                if (matrix[i][j] == 0) {
                    row[i] = true;
                    col[j] = true;
                }
            }
        }
        //Nullify rows
        for (int i = 0; i < row.length; ++i)
            if (row[i]) nullifyRow(matrix, i);
        //Nullify columns
        for (int j = 0; j < col.length; ++j) 
            if (col[j]) nullifyCol(matrix, j);
    }
    private void nullifyRow(int[][] matrix, int row) {
        for (int j = 0; j < matrix[0].length; ++j)
            matrix[row][j] = 0;
    }
    private void nullifyCol(int[][] matrix, int col) {
        for (int i = 0; i < matrix.length; ++i)
            matrix[i][col] = 0;
    }
}
```
啧啧啧，吊打C艹啊：
```cpp
执行用时：1 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：40.1 MB, 在所有 Java 提交中击败了82.95% 的用户
```

---
### 解法三 
**可以用位向量来代替解法二的行列布尔数组**。这仍然是 $O(n)$ 空间。但是通过使用矩阵本身来存储数据，则可以把额外的空间占用减小到 $O(1)$ 。

做法是**将第一行作为列数组的替换，将第一列用作行数组的替换**，从而将空间减少到 $O(1)$ 。其工作原理如下：
1. 检查第一行和第一列是否有零，并设置变量 `rowHasZero, colHasZero` ，如有必要，则最后使第一行和第一列为零。
2. 遍历矩阵的其余部分，只要 `matrix[i][j]` 为零，就将 `matrix[i][0]` 和 `matrix[0][j]`设置为零。
3. 遍历矩阵的第一列，若 `matrix[i][0] == 0` ，则将第 `i` 行置空。
4. 遍历矩阵的第一行，若 `matrix[0][j] == 0` ，则将第 `j` 列置空。
5. 如有必要（基于步骤1中的值），使第一行和第一列为零。 

具体Java代码如下：
```cpp
class Solution {
    public void setZeroes(int[][] matrix) {
        boolean rowHasZero = false, colHasZero = false;
        //Check if first row has a zero
        for (int j = 0; j < matrix[0].length; ++j) {
            if (matrix[0][j] == 0) {
                rowHasZero = true;
                break;
            }
        }
        //Check if first col has a zero
        for (int i = 0; i < matrix.length; ++i) {
            if (matrix[i][0] == 0) {
                colHasZero = true;
                break;
            }
        }
        //Check for zeros in the rest of the array
        for (int i = 1; i < matrix.length; ++i) {
            for (int j = 1; j < matrix[0].length; ++j) {
                if (matrix[i][j] == 0) {
                    matrix[i][0] = 0;
                    matrix[0][j] = 0;
                }
            }
        }
        //Nullify rows bases on values in first column
        for (int i = 1; i < matrix.length; ++i) 
            if (matrix[i][0] == 0)
                nullifyRow(matrix, i);
        //Nullify cols bases on values in first rows
        for (int j = 1; j < matrix[0].length; ++j)
            if (matrix[0][j] == 0)
                nullifyCol(matrix, j);
        //Nullify first row
        if (rowHasZero) nullifyRow(matrix, 0);
        //Nullify first col
        if (colHasZero) nullifyCol(matrix, 0);
    }
    private void nullifyRow(int[][] matrix, int row) {
        for (int j = 0; j < matrix[0].length; ++j)
            matrix[row][j] = 0;
    }
    private void nullifyCol(int[][] matrix, int col) {
        for (int i = 0; i < matrix.length; ++i)
            matrix[i][col] = 0;
    }
}
```
Java的空间占用不行，试一下C++：
```cpp
class Solution {
private: 
    void nullifyRow(vector<vector<int>>& matrix, int row) {
        for (int j = 0; j < matrix[0].size(); ++j) matrix[row][j] = 0;
    }
    void nullifyCol(vector<vector<int>>& matrix, int col) {
        for (int i = 0; i < matrix.size(); ++i) matrix[i][col] = 0;
    }
public:
    void setZeroes(vector<vector<int>>& matrix) {
        bool firstRowHasZero = false, firstColHasZero = false;
        int m = matrix.size(), n = matrix[0].size(); 
        for (int j = 0; j < n; ++j) {
            if (matrix[0][j] == 0) {
                firstRowHasZero = true;
                break;
            }
        } 
        for (int i = 0; i < m; ++i) {
            if (matrix[i][0] == 0) {
                firstColHasZero = true;
                break;
            }
        } 
        for (int i = 1; i < m; ++i) 
            for (int j = 1; j < n; ++j) 
                if (matrix[i][j] == 0) 
                    matrix[i][0] = matrix[0][j] = 0; 
        for (int i = 1; i < m; ++i) if (matrix[i][0] == 0) nullifyRow(matrix, i);
        for (int j = 1; j < n; ++j) if (matrix[0][j] == 0) nullifyCol(matrix, j);
        if (firstRowHasZero) nullifyRow(matrix, 0); 
        if (firstColHasZero) nullifyCol(matrix, 0);
    }
};     
```
空间效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了98.28% 的用户
内存消耗：11.7 MB, 在所有 C++ 提交中击败了95.09% 的用户
```
