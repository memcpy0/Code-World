<p>You are given two integers, <code>x</code> and <code>y</code>, which represent your current location on a Cartesian grid: <code>(x, y)</code>. You are also given an array <code>points</code> where each <code>points[i] = [a<sub>i</sub>, b<sub>i</sub>]</code> represents that a point exists at <code>(a<sub>i</sub>, b<sub>i</sub>)</code>. A point is <strong>valid</strong> if it shares the same x-coordinate or the same y-coordinate as your location.</p>

<p>Return <em>the index <strong>(0-indexed)</strong> of the <strong>valid</strong> point with the smallest <strong>Manhattan distance</strong> from your current location</em>. If there are multiple, return <em>the valid point with the <strong>smallest</strong> index</em>. If there are no valid points, return <code>-1</code>.</p>

<p>The <strong>Manhattan distance</strong> between two points <code>(x<sub>1</sub>, y<sub>1</sub>)</code> and <code>(x<sub>2</sub>, y<sub>2</sub>)</code> is <code>abs(x<sub>1</sub> - x<sub>2</sub>) + abs(y<sub>1</sub> - y<sub>2</sub>)</code>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: x = 3, y = 4, points = [[1,2],[3,1],[2,4],[2,3],[4,4]]
Output: 2
Explanation: Of all the points, only [3,1], [2,4] and [4,4] are valid. Of the valid points, [2,4] and [4,4] have the smallest Manhattan distance from your current location, with a distance of 1. [2,4] has the smallest index, so return 2.
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: x = 3, y = 4, points = [[3,4]]
Output: 0
Explanation: The answer is allowed to be on the same location as your current location. 
```

 
<p><strong>Example 3:</strong></p>

```clike
Input: x = 3, y = 4, points = [[2,3]]
Output: -1
Explanation: There are no valid points.
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= points.length &lt;= 10<sup>4</sup></code></li>
	<li><code>points[i].length == 2</code></li>
	<li><code>1 &lt;= x, y, a<sub>i</sub>, b<sub>i</sub> &lt;= 10<sup>4</sup></code></li>
</ul>



题意：给你两个整数&nbsp;<code>x</code> 和&nbsp;<code>y</code>&nbsp;，表示你在一个笛卡尔坐标系下的&nbsp;<code>(x, y)</code>&nbsp;处。同时，在同一个坐标系下给你一个数组&nbsp;<code>points</code>&nbsp;，其中&nbsp;<code>points[i] = [a<sub>i</sub>, b<sub>i</sub>]</code>&nbsp;表示在&nbsp;<code>(a<sub>i</sub>, b<sub>i</sub>)</code>&nbsp;处有一个点。当一个点与你所在的位置有相同的 x 坐标或者相同的 y 坐标时，我们称这个点是 <b>有效的</b>&nbsp;。 

<p>请返回距离你当前位置&nbsp;<strong>曼哈顿距离</strong>&nbsp;最近的&nbsp;<strong>有效</strong>&nbsp;点的下标（下标从 <strong>0</strong> 开始）。如果有多个最近的有效点，请返回下标&nbsp;<strong>最小</strong>&nbsp;的一个。如果没有有效点，请返回&nbsp;<code>-1</code>&nbsp;。</p>

<p>两个点 <code>(x<sub>1</sub>, y<sub>1</sub>)</code>&nbsp;和 <code>(x<sub>2</sub>, y<sub>2</sub>)</code>&nbsp;之间的 <strong>曼哈顿距离</strong>&nbsp;为&nbsp;<code>abs(x<sub>1</sub> - x<sub>2</sub>) + abs(y<sub>1</sub> - y<sub>2</sub>)</code>&nbsp;。</p>

 
---
### 解法 直接遍历
```cpp
class Solution {
public:
    int nearestValidPoint(int x, int y, vector<vector<int>>& points) {
        int n = points.size(), dist = INT_MAX, ans = -1;
        for (int i = 0; i < n; ++i) {
            if (points[i][0] == x && abs(points[i][1] - y) < dist) {
                dist = abs(points[i][1] - y);
                ans = i;
            }
            if (points[i][1] == y && abs(points[i][0] - x) < dist) {
                dist = abs(points[i][0] - x);
                ans = i;
            } 
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：184 ms, 在所有 C++ 提交中击败了80.73% 的用户
内存消耗：58 MB, 在所有 C++ 提交中击败了42.99% 的用户
```
将 `for` 循环中间的四次比较压缩为两次：
```cpp
class Solution {
public:
    int nearestValidPoint(int x, int y, vector<vector<int>>& points) {
        int n = points.size(), dist = INT_MAX, ans = -1;
        for (int i = 0; i < n; ++i) {
            if (points[i][0] == x || points[i][1] == y) {
                int d = abs(points[i][0] - x) + abs(points[i][1] - y);
                if (d < dist) dist = d, ans = i;
            }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：184 ms, 在所有 C++ 提交中击败了80.73% 的用户
内存消耗：57.9 MB, 在所有 C++ 提交中击败了72.50% 的用户
```
