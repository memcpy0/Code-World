<p>Implement the class <code>SubrectangleQueries</code>&nbsp;which receives a <code>rows x cols</code> rectangle as a matrix of integers in the constructor and supports two methods:</p>

<p>1.<code>&nbsp;updateSubrectangle(int row1, int col1, int row2, int col2, int newValue)</code></p>

<ul>
	<li>Updates all values with <code>newValue</code> in the subrectangle whose upper left coordinate is <code>(row1,col1)</code> and bottom right coordinate is <code>(row2,col2)</code>.</li>
</ul>

<p>2.<code>&nbsp;getValue(int row, int col)</code></p>

<ul>
	<li>Returns the current value of the coordinate <code>(row,col)</code> from&nbsp;the rectangle.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

```swift
Input
["SubrectangleQueries","getValue","updateSubrectangle","getValue","getValue","updateSubrectangle","getValue","getValue"]
[[[[1,2,1],[4,3,4],[3,2,1],[1,1,1]]],[0,2],[0,0,3,2,5],[0,2],[3,1],[3,0,3,2,10],[3,1],[0,2]]
Output
[null,1,null,5,5,null,10,5]
Explanation
SubrectangleQueries subrectangleQueries = new SubrectangleQueries([[1,2,1],[4,3,4],[3,2,1],[1,1,1]]);  
// The initial rectangle (4x3) looks like:
// 1 2 1
// 4 3 4
// 3 2 1
// 1 1 1
subrectangleQueries.getValue(0, 2); // return 1
subrectangleQueries.updateSubrectangle(0, 0, 3, 2, 5);
// After this update the rectangle looks like:
// 5 5 5
// 5 5 5
// 5 5 5
// 5 5 5 
subrectangleQueries.getValue(0, 2); // return 5
subrectangleQueries.getValue(3, 1); // return 5
subrectangleQueries.updateSubrectangle(3, 0, 3, 2, 10);
// After this update the rectangle looks like:
// 5   5   5
// 5   5   5
// 5   5   5
// 10  10  10 
subrectangleQueries.getValue(3, 1); // return 10
subrectangleQueries.getValue(0, 2); // return 5
```

<p><strong>Example 2:</strong></p>

```swift
Input
["SubrectangleQueries","getValue","updateSubrectangle","getValue","getValue","updateSubrectangle","getValue"]
[[[[1,1,1],[2,2,2],[3,3,3]]],[0,0],[0,0,2,2,100],[0,0],[2,2],[1,1,2,2,20],[2,2]]
Output
[null,1,null,100,100,null,20]
Explanation
SubrectangleQueries subrectangleQueries = new SubrectangleQueries([[1,1,1],[2,2,2],[3,3,3]]);
subrectangleQueries.getValue(0, 0); // return 1
subrectangleQueries.updateSubrectangle(0, 0, 2, 2, 100);
subrectangleQueries.getValue(0, 0); // return 100
subrectangleQueries.getValue(2, 2); // return 100
subrectangleQueries.updateSubrectangle(1, 1, 2, 2, 20);
subrectangleQueries.getValue(2, 2); // return 20
```

 
 
<p><strong>Constraints:</strong></p> 
<ul>
	<li>There will be at most <code><font face="monospace">500</font></code>&nbsp;operations considering both methods:&nbsp;<code>updateSubrectangle</code> and <code>getValue</code>.</li>
	<li><code>1 &lt;= rows, cols &lt;= 100</code></li>
	<li><code>rows ==&nbsp;rectangle.length</code></li>
	<li><code>cols == rectangle[i].length</code></li>
	<li><code>0 &lt;= row1 &lt;= row2 &lt; rows</code></li>
	<li><code>0 &lt;= col1 &lt;= col2 &lt; cols</code></li>
	<li><code>1 &lt;= newValue, rectangle[i][j] &lt;= 10^9</code></li>
	<li><code>0 &lt;= row &lt; rows</code></li>
	<li><code>0 &lt;= col &lt; cols</code></li>
</ul></div>


题意：实现一个类 `SubrectangleQueries` ，它的构造函数的参数是一个 `rows x cols` 的矩形（这里用整数矩阵表示），并支持以下两种操作：
- 用 `newValue` 更新以 `(row1,col1)` 为左上角且以 `(row2,col2)` 为右下角的子矩形。
- 返回矩形中坐标 `(row,col)` 的当前值。

----
### 解法1 暴力更新
由于题目中的二维数组比较小，所以可以 $O(n^2)$ 更新对应区间的值，$O(1)$ 返回查询值：
```cpp
class SubrectangleQueries {
public:
    vector<vector<int>> rect;
    SubrectangleQueries(vector<vector<int>>& rectangle) { rect = rectangle; }
    
    void updateSubrectangle(int row1, int col1, int row2, int col2, int newValue) {
        for (int i = row1; i <= row2; ++i) for (int j = col1; j <= col2; ++j) rect[i][j] = newValue;
    }
    
    int getValue(int row, int col) {
        return rect[row][col];
    }   
};
```
这种方法的运行效率如下：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了81.54% 的用户
内存消耗：18.8 MB, 在所有 C++ 提交中击败了22.68% 的用户
```
---
### 解法2 常量记录
如果二维数组比较巨大，更新操作多而查询操作较少，就可以 $O(1)$ 记录更新操作，$O(n)$ 返回查询值：
```cpp
class SubrectangleQueries {
public:
    vector<vector<int>> rect;
    vector<array<int, 5>> ops;
    SubrectangleQueries(vector<vector<int>>& rectangle) { rect = rectangle; }

    void updateSubrectangle(int row1, int col1, int row2, int col2, int newValue) {
        ops.push_back(array{row1, col1, row2, col2, newValue});
    }
    
    int getValue(int row, int col) {
        int ans = rect[row][col];
        for (const array<int, 5> &v : ops) 
            if (row >= v[0] && col >= v[1] && row <= v[2] && col <= v[3]) ans = v[4];
        return ans;
    }   
};
```
运行效率如下：
```cpp
执行用时：80 ms, 在所有 C++ 提交中击败了69.34% 的用户
内存消耗：18.8 MB, 在所有 C++ 提交中击败了19.55% 的用户
```
