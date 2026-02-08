

The `Employee` table holds all employees including their managers. Every employee has an `Id`, and there is also a column for the manager `Id`.

```swift
+----+-------+--------+-----------+
| Id | Name  | Salary | ManagerId |
+----+-------+--------+-----------+
| 1  | Joe   | 70000  | 3         |
| 2  | Henry | 80000  | 4         |
| 3  | Sam   | 60000  | NULL      |
| 4  | Max   | 90000  | NULL      |
+----+-------+--------+-----------+
```

Given the `Employee` table, write a SQL query that finds out employees who earn more than their managers. For the above table, Joe is the only employee who earns more than **his manager**.

```swift
+----------+
| Employee |
+----------+
| Joe      |
+----------+
```

题意：给定 `Employee` 表，编写一个SQL查询，该查询可以获取收入超过他们经理的员工的姓名。

---
### 解法1 连接
由于每个普通员工都属于某个经理的手下，因此判断其收入，应该是与其所属经理的收入相比较。为此，需要使用**条件连接**操作（这里是自连接）：
```sql
SELECT a.Name As Employee
FROM Employee AS a, Employee AS b
WHERE a.Managerid = b.Id And a.Salary > b.Salary;
```
提交后效率如下：
```cpp
执行用时：272 ms, 在所有 MySQL 提交中击败了74.89% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```
---
### 解法2 子表查询
```sql
SELECT Name As Employee
FROM Employee AS a
WHERE Salary > 
    (SELECT Salary 
        FROM Employee
        where Id = a.Managerid);
```
虽然可以这样写，但是效率太低了，每查一次都要执行后面的子表查询：
```cpp
执行用时：791 ms, 在所有 MySQL 提交中击败了8.31% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```
