Given an M x N matrix in which each row and each column is sorted in ascending order, write a method to find an element.

**Example:**

```clike
Given matrix:

[
  [1,   4,  7, 11, 15],
  [2,   5,  8, 12, 19],
  [3,   6,  9, 16, 22],
  [10, 13, 14, 17, 24],
  [18, 21, 23, 26, 30]
]
```
Given target = `5` , return `true` . Given target = `20` , return `false` . 

题意：给定M×N矩阵，每一行、每一列都按升序排列，请编写代码找出某元素。

---
### 解法 二分
二分的关键在于找到**单调性**，很明显从矩阵左上角到右下角是单调递增的。我们从中间大小的元素开始搜索——即右上角或左下角，每次可以排除一行或者一列。直到找到元素或者超出范围。
```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty()) return false;
        int m = matrix.size(), n = matrix[0].size(), i = 0, j = n - 1;
        while (i < m && j >= 0) {
            if (matrix[i][j] == target) return true;
            else if (matrix[i][j] < target) ++i;
            else --j;
        }
        return false;
    }
};
```
运行效率如下：
```cpp
执行用时：112 ms, 在所有 C++ 提交中击败了79.44% 的用户
内存消耗：10.5 MB, 在所有 C++ 提交中击败了52.14% 的用户
```
