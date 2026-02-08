 Write a `SQL` query to get the **nth highest** salary from the Employee table.
	
	+----+--------+
	| Id | Salary |
	+----+--------+
	| 1  | 100    |
	| 2  | 200    |
	| 3  | 300    |
	+----+--------+

For example, given the above Employee table, the **nth highest** salary where `n = 2` is $200$ . If there is no nth highest salary, then the query should return `null` .

	+------------------------+
	| getNthHighestSalary(2) |
	+------------------------+
	| 200                    |
	+------------------------+
	

题意：找到第 $n$ 高的薪资。

思路：前面就写过了。

代码：
```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  SET N = N - 1;
  RETURN ( 
      SELECT 
      (SELECT DISTINCT Salary
      FROM Employee
      ORDER BY Salary DESC
      LIMIT N, 1)
  );
END 
```

当然，还有其他方法，这里不多写了。
