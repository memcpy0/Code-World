Write an efficient algorithm that searches for a value in an **m x n matrix**. This matrix has the following properties:

-    Integers in each row are **sorted from left to right**.
-    The first integer of each row is **greater than** the last integer of the previous row.

Example 1:
```
Input:
matrix = [
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]
target = 3
Output: true
```
Example 2:
```
Input:
matrix = [
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]
target = 13
Output: false
```
题意：题目给出一个m X n维的数组，要求我们查找一个元素，如果元素存在的话，就返回true，否则就是false。

思路：二维矩阵有以下性质：每一行的整数都从左到右从小到大排序；每一行的第一个整数都比上一行的最后一个整数大。

可以知道以下性质：1. 二维矩阵每一行、每一列都是有序的；2. 二维矩阵每一行连接起来后是一个从小到大的一维序列。

方法一：暴力法，依次扫描，如果**找到或者扫描的元素>target**就退出(利用有序性)。O(1)空间, O(MN)的时间复杂度。==如果二维矩阵是无序的，就只能够使用这种方法==。
- 执行用时 :20 ms, 在所有 C++ 提交中击败了14.75% 的用户
内存消耗 :13.2 MB, 在所有 C++ 提交中击败了5.33%的用户
```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {  
        for (int i = 0; i < matrix.size(); ++i) {
             for (int j = 0; j < matrix[i].size(); ++j) {
                 if (matrix[i][j] == target) return true;
                 else if (matrix[i][j] > target) return false; //利用矩阵性质, 提前退出
             }
        }
        return false; 
    }
};
```
方法二：将二维矩阵中的值复制到数组中，O(MN)空间，复制一遍+搜索：O(MN+log(MN))的时间。
 -  执行用时 :24 ms, 在所有 C++ 提交中击败了5.00% 的用户
        内存消耗 :13.7 MB, 在所有 C++ 提交中击败了5.33%的用户
```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {  
        vector<int> vi;
        for (int i = 0; i < matrix.size(); ++i) 
            for (int j = 0; j < matrix[i].size(); ++j) 
                vi.push_back(matrix[i][j]);
        int lo = 0, hi = vi.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (vi[mid] == target) return true;
            else if (vi[mid] < target) lo = mid + 1;
            else if (vi[mid] > target) hi = mid - 1;
        } 
        return false;
    }
};
```
方法三：利用矩阵性质1，需要**选择合适的切入点**与目标值相比较以缩小数据返围，很明显如果选择左上角或右下角当做切入点并不能起到作用，只能分别排除一个选项。而如果**选择右上角或者左下角进行比较**即可起到更明显的效果。如果target>右上角的值，说明要往下面几行查找；如果target<右上角的值，说明要往左边几列查找。**最大为O(m+n)时间复杂度**。==如果题目没有性质2，这样做就是最优的解法==。
  - 执行用时 :4 ms, 在所有 C++ 提交中击败了99.44% 的用户
     内存消耗 :13.4 MB, 在所有 C++ 提交中击败了5.33%的用户
```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) { 
        if (matrix.empty()) return false; 
        int m = matrix.size(), n = matrix[0].size(), i = 0, j = n - 1;
        while (i < m && j >= 0) {
            if (target == matrix[i][j]) return true;
            else if (target > matrix[i][j]) ++i;
            else if (target < matrix[i][j]) --j;
        }
        return false; 
    }
};
```
方法四：方法三的时间有了一定的优化，但是还不够。如果能够改进方法二，省去那麽多空间，O(log(MN))的时间更优。从性质2，我们注意到输入的 m x n 矩阵可以**直接视为**长度为 m x n的有序数组，由于该`虚`数组的序号可以由下式方便地**转化为原矩阵中的行号和列号** (而不是真的创建一个新数组) ，该有序数组非常适合二分查找。

这样就是O(1)空间，O(log(MN))的时间。
- 执行用时 :4 ms, 在所有 C++ 提交中击败了99.45% 的用户
   内存消耗 :13.3 MB, 在所有 C++ 提交中击败了5.33%的用户
![二分查找](https://img-blog.csdnimg.cn/20200222160945892.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) { 
        if (matrix.empty()) return false;
        int m = matrix.size(), n = matrix[0].size(), lo = 0, hi = m * n - 1;
        while (lo <= hi) {
            int mid = (lo + (hi - lo) / 2), midI = mid / n, midJ = mid % n;
            if (matrix[midI][midJ] == target) return true;
            else if (matrix[midI][midJ] > target) hi = mid - 1;
            else if (matrix[midI][midJ] < target) lo = mid + 1;
        }
        return false;
    }
};
```
