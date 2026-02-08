Table `point` holds the x coordinate of some points on x-axis in a plane, which are all integers.

 
Write a query to find the shortest distance between two points in these points.
 
```haskell
| x   |
|-----|
| -1  |
| 0   |
| 2   |
```
 
The shortest distance is '1' obviously, which is from point '-1' to '0'. So the output is as below:
 
```haskell
| shortest|
|---------|
| 1       |
```

 
**Note:** Every point is unique, which means there is no duplicates in table point.

**Follow-up:** What if all these points have an id and are arranged from the left most to the right most of x axis? 

题意：表 `point` 保存了一些点在 `x` 轴上的坐标，这些坐标都是整数。写一个查询语句，找到这些点中最近两个点之间的距离。
