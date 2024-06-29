<p>You have a list of points in the plane. Return the area of the largest triangle that can be formed by any 3 of the points.</p>

 <strong>Example:</strong>

```clike
Input: points = [[0,0],[0,1],[1,0],[0,2],[2,0]]
Output: 2
Explanation: 
The five points are show in the figure below. The red triangle is the largest.
```

 
<p><img style="height:328px; width:400px" src="https://s3-lc-upload.s3.amazonaws.com/uploads/2018/04/04/1027.png" alt=""></p>

<p><strong>Notes: </strong></p>

<ul>
	<li><code>3 &lt;= points.length &lt;= 50</code>.</li>
	<li>No points will be duplicated.</li>
	<li>&nbsp;<code>-50 &lt;= points[i][j] &lt;= 50</code>.</li>
	<li>Answers within&nbsp;<code>10^-6</code>&nbsp;of the true value will be accepted as correct.</li>
</ul>
 
题意：给定包含多个点的集合，从其中取三个点组成三角形，返回能组成的最大三角形的面积。


---
### 解法1 海伦公式
暴力遍历每个三元组，用海伦公式求出三角形面积。时间效率为 $O(n^3)$ ：
```cpp
class Solution {
private:
    double dist(const vector<int>& x, const vector<int>& y) {
        return sqrt((x[0] - y[0]) * (x[0] - y[0]) + (x[1] - y[1]) * (x[1] - y[1]));
    }
public:
    double largestTriangleArea(vector<vector<int>>& points) {
        int n = points.size();
        double maxArea = 0.0;
        for (int i = 0; i < n - 2; ++i) {
            for (int j = i + 1; j < n - 1; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    double a = dist(points[i], points[j]);
                    double b = dist(points[i], points[k]);
                    double c = dist(points[j], points[k]);
                    double p = (a + b + c) / 2;
                    double area = sqrt(p * (p - a) * (p - b) * (p - c));
                    if (area > maxArea) maxArea = area;
                }
            }
        }
        return maxArea;
    }
};
```
运行效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了33.91% 的用户
内存消耗：7.4 MB, 在所有 C++ 提交中击败了42.92% 的用户
```
---
### 解法2 线性代数（行列式）
三个点构成两个共起点的向量，它们可以构成一个平行四边形，其面积等于两个向量构成的矩阵的行列式值的绝对值，而三角形的面积是平行四边形的一半。

因此我们先遍历所有的起点，并选中某个点 `p1` 作为起点，然后遍历选中第一个向量的终点，接着遍历选中第二个向量的终点，接着直接计算面积即可。**不难发现**，这种做法避免了点与点之间距离的计算，**避免了大量的平方和开方操作**，因此有着更好的效率。
```cpp
class Solution { 
public:
    double largestTriangleArea(vector<vector<int>>& points) {
        int n = points.size();
        double maxArea = 0.0, area, x1, y1, x2, y2;
        for (int i = 0; i < n - 2; ++i) { //选起点
            for (int j = i + 1; j < n - 1; ++j) { //第一个向量的终点
                x1 = points[j][0] - points[i][0]; //第一个向量分量计算
                y1 = points[j][1] - points[i][1];
                for (int k = j + 1; k < n; ++k) { //第二个向量的终点
                    x2 = points[k][0] - points[i][0]; //第二个向量分量计算
                    y2 = points[k][1] - points[i][1];
                    //行列式计算面积
                    area = abs(x1 * y2 - x2 * y1);
                    if (area > maxArea) maxArea = area; 
                }
            }
        }
        return maxArea / 2;
    }
}; 
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了90.56% 的用户
内存消耗：7.3 MB, 在所有 C++ 提交中击败了76.82% 的用户
```
