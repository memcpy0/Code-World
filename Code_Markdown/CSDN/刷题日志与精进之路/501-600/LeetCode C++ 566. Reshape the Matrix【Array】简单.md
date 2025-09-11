In MATLAB, there is a very useful function called 'reshape', which can reshape a matrix into a new one with different size but keep its original data.

You're given a matrix represented by a two-dimensional array, and two **positive** integers `r` and `c` representing the **row** number and **column** number of the wanted reshaped matrix, respectively.

The reshaped matrix need to be filled with all the elements of the original matrix in the same **row-traversing** order as they were.

If the 'reshape' operation with given parameters is possible and legal, output the new reshaped matrix; Otherwise, output the original matrix.

**Example 1:**

```swift
Input: 
nums = 
[[1,2],
 [3,4]]
r = 1, c = 4
Output: 
[[1,2,3,4]]
Explanation:
The row-traversing of nums is [1,2,3,4]. The new reshaped matrix is a 1 * 4 matrix, fill it row by row by using the previous list.
```

**Example 2:**

```swift
Input: 
nums = 
[[1,2],
 [3,4]]
r = 2, c = 4
Output: 
[[1,2],
 [3,4]]
Explanation:
There is no way to reshape a 2 * 2 matrix to a 2 * 4 matrix. So output the original matrix.
```

**Note:**
- The height and width of the given matrix is in range `[1, 100]` .
-  The given `r` and `c` are all positive.

题意：给出一个由二维数组表示的矩阵，以及两个正整数 `r` 和 `c` ，分别表示想要的重构的矩阵的行数和列数。我们要做的是，将原始矩阵的所有元素，以相同的**行遍历顺序**填充到重构后的矩阵。

如果具有给定参数的 `reshape` 操作是可行且合理的，则输出新的重塑矩阵；否则，输出原始矩阵。

---
### 解法 遍历取值
```cpp
class Solution {
public:
    vector<vector<int>> matrixReshape(vector<vector<int>>& nums, int r, int c) {
        int n = nums.size(), m = nums[0].size();
        if (n * m != r * c) return nums;
        vector<vector<int>> ans;
        int tr = 0, tc = 0;
        for (int i = 0; i < r; ++i) {
            ans.push_back(vector<int>());
            for (int j = 0; j < c; ++j) {
                ans[i].push_back(nums[tr][tc]);
                if (++tc == m) { tc = 0; ++tr; }
            }
        }
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了74.36% 的用户
内存消耗：12 MB, 在所有 C++ 提交中击败了9.05% 的用户
```
对应的Java代码如下：
```java
class Solution {
    public int[][] matrixReshape(int[][] nums, int r, int c) {
        int mr = nums.length, mc = nums[0].length;
        if (mr * mc != r * c) return nums;
        int x = 0, y = 0;
        int[][] ans = new int[r][c];
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < c; ++j) {
                ans[i][j] = nums[x][y++];
                if (y >= mc) { ++x; y = 0; }
            }
        }
        return ans;
    }
}
```
运行效率就高多了：
```java
执行用时：1 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：39.6 MB, 在所有 Java 提交中击败了55.39% 的用户
```
