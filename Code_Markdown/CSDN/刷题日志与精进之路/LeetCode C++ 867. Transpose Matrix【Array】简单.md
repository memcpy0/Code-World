

Given a matrix `A` , return the transpose of `A` .

The transpose of a matrix is the matrix flipped over it's main diagonal, switching the row and column indices of the matrix.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201114133852127.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

 

**Example 1:**

```swift
Input: [[1,2,3],[4,5,6],[7,8,9]]
Output: [[1,4,7],[2,5,8],[3,6,9]]
```

**Example 2:**

```swift
Input: [[1,2,3],[4,5,6]]
Output: [[1,4],[2,5],[3,6]]
```

 

**Note:**
- `1 <= A.length <= 1000`
-  `1 <= A[0].length <= 1000`


题意：给定一个矩阵 `A`， 返回 `A` 的转置矩阵。矩阵的转置是指**将矩阵的主对角线翻转**，交换矩阵的行索引与列索引。

---
### 解法 辅助空间
方阵可以沿着主对角线直接交换数值，非方阵必须另外申请二维数组：
```cpp
class Solution {
public:
    vector<vector<int>> transpose(vector<vector<int>>& A) {
        int n = A.size(), m = A[0].size();
        vector<vector<int>> ans(m, vector<int>(n));
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j)
                ans[i][j] = A[j][i];
        return ans;
    }
};
```
时空效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了99.31% 的用户
内存消耗：9.8 MB, 在所有 C++ 提交中击败了70.18% 的用户
```
