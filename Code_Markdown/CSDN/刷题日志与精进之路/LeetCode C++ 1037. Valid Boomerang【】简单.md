A *boomerang* is a set of 3 points that are all distinct and **not** in a straight line.

Given a list of three points in the plane, return whether these points are a boomerang.

**Example 1:**

```haskell
Input: [[1,1],[2,3],[3,2]]
Output: true
```

**Example 2:**

```haskell
Input: [[1,1],[2,2],[3,3]]
Output: false
```
**Note:**
- `points.length == 3`
- `points[i].length == 2`
- `0 <= points[i][j] <= 100`

题意：回旋镖定义为一组三个点，这些点各不相同且不在一条直线上。给出平面上三个点组成的列表，判断这些点是否可以构成回旋镖。

