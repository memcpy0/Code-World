

We have a list of `points` on the plane.  Find the `K` closest points to the origin `(0, 0)`.

(Here, the distance between two points on a plane is the Euclidean distance.)

You may return the answer in any order.  The answer is guaranteed to be unique (except for the order that it is in.)

 

**Example 1:**

```swift
Input: points = [[1,3],[-2,2]], K = 1
Output: [[-2,2]]
Explanation: 
The distance between (1, 3) and the origin is sqrt(10).
The distance between (-2, 2) and the origin is sqrt(8).
Since sqrt(8) < sqrt(10), (-2, 2) is closer to the origin.
We only want the closest K = 1 points from the origin, so the answer is just [[-2,2]].
```

**Example 2:**

```swift
Input: points = [[3,3],[5,-1],[-2,4]], K = 2
Output: [[3,3],[-2,4]]
(The answer [[-2,4],[3,3]] would also be accepted.)
```

 

**Note:**
- `1 <= K <= points.length <= 10000`
-  `-10000 < points[i][0] < 10000`
-  `-10000 < points[i][1] < 10000`

题意：有一个由平面上的点组成的列表 `points` 。需要从中找出 `K` 个距离原点 `(0, 0)` 最近的点。可以按任何顺序返回答案。除了点坐标的顺序之外，答案确保是唯一的。

---
### 解法1 `sort`
这是最简单的解法，其他解法日后有时间再更新——我们将这些点按照**离原点的距离**从近到远排序，然后返回前 `K` 个点：
```cpp
class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int K) {
        sort(points.begin(), points.end(), [&](const vector<int>& a, const vector<int>& b) {
            int distA = a[0] * a[0] + a[1] * a[1];
            int distB = b[0] * b[0] + b[1] * b[1];
            return distA < distB;
        });
        return vector<vector<int>>(points.begin(), points.begin() + K);
    }
};
```
执行效率如下：
```cpp
执行用时：424 ms, 在所有 C++ 提交中击败了45.76% 的用户
内存消耗：38.6 MB, 在所有 C++ 提交中击败了75.22% 的用户
```
