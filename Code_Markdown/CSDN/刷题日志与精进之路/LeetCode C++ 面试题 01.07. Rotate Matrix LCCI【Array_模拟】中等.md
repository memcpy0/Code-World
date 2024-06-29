Given an image represented by an `N x N` matrix, where each pixel in the image is `4` bytes, write a method to rotate the image by `90` degrees. Can you do this in place?

**Example 1:**
```swift
Given matrix = 
[
  [1,2,3],
  [4,5,6],
  [7,8,9]
],

Rotate the matrix in place. It becomes:
[
  [7,4,1],
  [8,5,2],
  [9,6,3]
]
```
**Example 2:**
```swift
Given matrix =
[
  [ 5, 1, 9,11],
  [ 2, 4, 8,10],
  [13, 3, 6, 7],
  [15,14,12,16]
], 

Rotate the matrix in place. It becomes:
[
  [15,13, 2, 5],
  [14, 3, 4, 1],
  [12, 6, 8, 9],
  [16, 7,10,11]
]
```
题意：给出一幅由 `N × N` 矩阵表示的图像，其中每个像素的大小为 4 字节。请你设计一种算法，将图像旋转 `90` 度。要求不占用额外内存空间。

---
### 解法1 占用额外空间
占用额外空间的解法是显然的：
```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return;
        int n = matrix.size();
        vector<vector<int>> temp(n, vector<int>(n));
        for (int i = 0; i < n; ++i) //列
            for (int j = 0; j < n; ++j) //行
                temp[i][n - j - 1] = matrix[j][i]; //沿主对角线翻转后沿每一行中心轴翻转
        matrix = temp;
    }
};
```
效率如下，时间和空间复杂度都是 $O(n^2)$ ：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了52.35% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了35.06% 的用户
```
---
### 解法2 两次双重循环交换
我们在上一解法中注意到，**矩阵向右旋转90度，其实等价于对矩阵按照主对角线翻转交换，然后对每一行按照中心轴左右交换**，只是需要使用两次双重循环。具体代码如下：
```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return;
        int n = matrix.size(); 
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < i; ++j) 
                swap(matrix[i][j], matrix[j][i]);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n / 2; ++j)
                swap(matrix[i][j], matrix[i][n - j - 1]);    
    }
};
```
效率如下，似乎没怎么提高，但是空间复杂度为 $O(1)$ ：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了52.35% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了17.30% 的用户
```
---
### 解法3 一次双重循环交换
仔细注意矩阵旋转的规律，可知将矩阵旋转90度，**最简单的方法就是在层中实现旋转**。在每一层上执行循环旋转，将上边缘移动到右边缘，将右边缘移动到底部边缘，将底部边缘移动到左侧边缘，并将左侧边缘移动到顶部边缘：
<img src="https://img-blog.csdnimg.cn/20201009170228540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="27%">

如何执行这种四向边缘交换？一种方法是将顶部边缘复制到阵列，然后将左侧移动到顶部，底部移动到左侧，依此类推。这需要 $O(n)$ 内存，这实际上是不必要的。更好的方法是**按索引实现交换**。在这种情况下，我们执行以下步骤：
```cpp
for (int i = 0; i < n; ++i) {
	temp = top[i];
	top[i] = left[i];
	left[i] = bottom[i];
	bottom[i] = right[i];
	right[i] = temp;
}
```
我们在每一层上执行这样的交换，从最外层开始向内工作。或者也可以从内层开始向外工作。此算法的Java代码如下：
```java
class Solution {
    public void rotate(int[][] matrix) {
        if (matrix == null || matrix.length == 0 || matrix.length != matrix[0].length) return;
        int n = matrix.length;
        for (int layer = 0; layer < n / 2; ++layer) { //层数为n/2
            int first = layer; 
            int last = n - 1 - layer;
            for (int i = first; i < last; ++i) { //每层的范围
                int offset = i - first;
                int top = matrix[first][i]; //从最外层开始
                matrix[first][i] = matrix[last - offset][first]; //左侧边缘替换上侧边缘
                matrix[last - offset][first] = matrix[last][last - offset]; //下侧边缘替换左侧边缘
                matrix[last][last - offset] = matrix[i][last]; //右侧边缘替换下侧边缘
                matrix[i][last] = top; //上侧边缘替换右侧边缘
            }
        }
    }
}
```
效率如下：
```java
执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：38.8 MB, 在所有 Java 提交中击败了66.62% 的用户
```
用C++写的从内层向外层工作的代码如下：
```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        if (matrix.size() <= 1 || matrix[0].size() <= 1) return;
        int n = matrix.size();
        for (int layer = n / 2 - 1; layer >= 0; --layer) { //对n/2每层
            int firstRC = layer, lastRC = n - 1 - layer; //每层的最小行号/列号,最大行号/列号
            for (int i = firstRC; i < lastRC; ++i) { //每层的一侧数据量
                int top = matrix[firstRC][i]; //每层的上侧
                int offset = i - firstRC;
                matrix[firstRC][i] = matrix[lastRC - offset][firstRC]; //上侧替换为左侧
                matrix[lastRC - offset][firstRC] = matrix[lastRC][lastRC - offset];   //左侧替换为下侧  
                matrix[lastRC][lastRC - offset] = matrix[i][lastRC]; //下侧替换为右侧
                matrix[i][lastRC] = top; //右侧替换为上侧
            }
        } 
    }
};
```
空间效率大大提高：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7 MB, 在所有 C++ 提交中击败了48.42% 的用户
```
