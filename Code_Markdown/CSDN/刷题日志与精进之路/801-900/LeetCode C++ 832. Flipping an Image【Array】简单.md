Given a binary matrix `A` , we want to flip the image horizontally, then invert it, and return the resulting image.

To flip an image horizontally means that each row of the image is reversed.  For example, flipping `[1, 1, 0]` horizontally results in `[0, 1, 1]` .

To invert an image means that each `0` is replaced by `1` , and each `1` is replaced by `0` . For example, inverting `[0, 1, 1]` results in `[1, 0, 0]` .

**Example 1:**
```cpp
Input: [[1,1,0],[1,0,1],[0,0,0]]
Output: [[1,0,0],[0,1,0],[1,1,1]]
Explanation: First reverse each row: [[0,1,1],[1,0,1],[0,0,0]].
Then, invert the image: [[1,0,0],[0,1,0],[1,1,1]]
```

**Example 2:**
```cpp
Input: [[1,1,0,0],[1,0,0,1],[0,1,1,1],[1,0,1,0]]
Output: [[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]
Explanation: First reverse each row: [[0,0,1,1],[1,0,0,1],[1,1,1,0],[0,1,0,1]].
Then invert the image: [[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]
```

**Notes:**
- `1 <= A.length = A[0].length <= 20`
-  `0 <= A[i][j] <= 1`


题意：翻转像素，将一个二进制矩阵的每一行翻转，然后对每个像素取反。

---
思路1：直观的解法，两个循环。代码如下：
```cpp
class Solution {
public:
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& A) {
        int m = A.size(), n = A[0].size();
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n / 2; ++j) 
                swap(A[i][j], A[i][n - j - 1]);
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j)
                A[i][j] ^= 1;
        return A;
    }
};
```
---
思路2：用一个循环，翻转每一行，然后对每一行的每一个元素1/0取非。代码如下：
```cpp
class Solution {
public:
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& A) {
        for (int i = 0; i < A.size(); i++) {
            reverse(A[i].begin(), A[i].end());
            for (int j = 0; j < A[i].size(); j++)  
                A[i][j] = !A[i][j]; 
        }
        return A;
    }
};
```
---
思路3：将两个循环合二为一，同时我们发现：
- `1 ... 0` 对称，取反是 `0 ... 1` ，翻转又是 `1 ... 0` ；
- `0 ... 1` 对称，取反是 `1 ... 0` ，翻转又是 `0 ... 1` ；
- 因此，当两个对称的像素点不同的时候，直接跳过；只有对称的两个像素点相同时，**才取反一个，赋值给两个像素点**。

代码如下：
```cpp
class Solution {
public:
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& A) {
        int m = A.size(), n = A[0].size();
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < (n + 1) / 2; ++j) 
                if (A[i][j] == A[i][n - j - 1]) 
                    A[i][j] = A[i][n - j - 1] = A[i][j] ^ 1;   
        return A;
    }
};
```
