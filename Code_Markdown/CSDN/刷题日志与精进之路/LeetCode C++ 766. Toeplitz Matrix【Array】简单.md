A matrix is ***Toeplitz*** if every diagonal from top-left to bottom-right has the same element.

Now given an `M x N` matrix, return `True` if and only if the matrix is ***Toeplitz*** .
 

**Example 1:**
```cpp
Input:
matrix = [
  [1,2,3,4],
  [5,1,2,3],
  [9,5,1,2]
]
Output: True
Explanation:
In the above grid, the diagonals are:
"[9]", "[5, 5]", "[1, 1, 1]", "[2, 2, 2]", "[3, 3]", "[4]".
In each diagonal all elements are the same, so the answer is True.
```

**Example 2:**
```cpp
Input:
matrix = [
  [1,2],
  [2,2]
]
Output: False
Explanation:
The diagonal "[1, 2]" has different elements.
```
**Note:**
- `matrix` will be a 2D array of integers.
- `matrix` will have a number of rows and columns in range `[1, 20]` .
 - `matrix[i][j]` will be integers in range `[0, 99]` .


**Follow up:**
- What if the matrix is stored on disk, and the memory is limited such that you can only **load at most one row of the matrix** into the memory **at once**?
- What if the matrix is so large that you can only **load up a partial row** into the memory at once?

题意：如果矩阵上每一条由左上到右下的对角线上的元素都相同，那么这个矩阵是 **托普利茨矩阵** 。给定一个 `M x N` 的矩阵，当且仅当它是托普利茨矩阵时返回 `True` 。
 
---
### 思路1
如果我们从第一行开始，对右上的每条对角线进行检查，然后从第一列开始，对左下的每条对角线进行检查，就可以完成这一任务。代码如下：
```cpp
class Solution {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int row = matrix.size(), col = matrix[0].size();
        for (int i = 0; i < col; ++i) {
            int diagonal = matrix[0][i], k = i;
            for (int j = 1; j < row; ++j) {
                if (k + 1 >= col) break;
                if (diagonal != matrix[j][++k]) return false;
            }
        }
        for (int i = 1; i < row; ++i) {
            int diagonal = matrix[i][0], k = 0;
            for (int j = i + 1; j < row; ++j) {
                if (k + 1 >= col) break;
                if (diagonal != matrix[j][++k]) return false;
            } 
        }
        return true;
    }
};
```
提交后结果如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了83.68% 的用户
内存消耗：15.9 MB, 在所有 C++ 提交中击败了92.76% 的用户
```


---
### 思路2
更简单的做法是，我们仔细观察矩阵，**如果它是托普利茨矩阵，那么每个元素都和其紧邻的左上位置的元素相等**。代码如下：
```cpp
class Solution {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int row = matrix.size(), col = matrix[0].size();
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (i == 0 || j == 0) continue;
                if (matrix[i][j] != matrix[i - 1][j - 1]) return false;
            }
        }
        return true;
    }
};
```
效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了83.68% 的用户
内存消耗：16 MB, 在所有 C++ 提交中击败了52.04% 的用户
```

---

进阶问题：
- 如果矩阵存储在磁盘上，并且磁盘内存是有限的，因此一次最多只能将一行矩阵加载到内存中，该怎么办？
	回答：内存地址实际上是一维的，我们比较的是 `arr[i][j]` 和 `arr[i - 1][j - 1]` ，只要知道矩阵的列数，就可以计算出 `arr[i - 1][j - 1]` 的位置。
- 如果矩阵太大以至于只能一次将部分行加载到内存中，该怎么办？
