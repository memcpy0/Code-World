
A pupil Tim gets homework to identify whether three line segments could possibly form a triangle.

 
However, this assignment is very heavy because there are hundreds of records to calculate.

 
Could you help Tim by writing a query to judge whether these three sides can form a triangle, assuming table `triangle` holds the length of the three sides `x`, `y` and `z`.

 

```haskell
| x  | y  | z  |
|----|----|----|
| 13 | 15 | 30 |
| 10 | 20 | 15 |
```

For the sample data above, your query should return the follow result:

```haskell
| x  | y  | z  | triangle |
|----|----|----|----------|
| 13 | 15 | 30 | No       |
| 10 | 20 | 15 | Yes      |
```

题意：一个小学生的作业是判断三条线段是否能形成一个三角形。然而，这个作业非常繁重，因为有几百组线段需要判断。假设表 `triangle` 保存了所有三条线段的长度 `x`、`y`、`z` ，写一个查询语句，来判断每组 `x`、`y`、`z` 是否可以组成一个三角形？ 
