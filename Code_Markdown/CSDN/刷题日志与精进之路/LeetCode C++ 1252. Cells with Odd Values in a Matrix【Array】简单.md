

Given `n` and `m` which are the dimensions of a matrix initialized by zeros and given an array `indices` where `indices[i] = [ri, ci]`. For each pair of `[ri, ci]` you have to increment all cells in row `ri` and column `ci` by `1`.

Return *the number of cells with odd values* in the matrix after applying the increment to all `indices`.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201115002358366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```swift
Input: n = 2, m = 3, indices = [[0,1],[1,1]]
Output: 6
Explanation: Initial matrix = [[0,0,0],[0,0,0]].
After applying first increment it becomes [[1,2,1],[0,1,0]].
The final matrix will be [[1,3,1],[1,3,1]] which contains 6 odd numbers.
```

**Example 2:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201115002352814.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```swift
Input: n = 2, m = 2, indices = [[1,1],[0,0]]
Output: 0
Explanation: Final matrix = [[2,2],[2,2]]. There is no odd number in the final matrix.
```

 

**Constraints:**
- `1 <= n <= 50`
- `1 <= m <= 50`
 - `1 <= indices.length <= 100`
 - `0 <= indices[i][0] < n`
- `0 <= indices[i][1] < m`

题意：给定一个 `n` 行 `m` 列的矩阵，最开始的时候，每个单元格中的值都是 `0` 。在执行完所有 `indices` 指定的增量操作后，返回矩阵中「奇数值单元格」的数目。

---
### 解法1 暴力
对 `indices` 中的每个 `indices[i] = [ri, ci]` 指定的行和列，分别进行加一操作。最后统计奇数值单元格的数目：
```cpp
class Solution {
public:
    int oddCells(int n, int m, vector<vector<int>>& indices) {
        vector<vector<int>> t(n, vector<int>(m));
        for (const vector<int>& v : indices) {
            //r = v[0], c = v[1]
            for (int i = 0; i < n; ++i) ++t[i][v[1]]; //对每行的该列+1
            for (int j = 0; j < m; ++j) ++t[v[0]][j]; //对每列的该行+1
        }
        int oddNums = 0;
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < m; ++j)
                oddNums += (t[i][j] & 1);
        return oddNums;
    }
};
```
