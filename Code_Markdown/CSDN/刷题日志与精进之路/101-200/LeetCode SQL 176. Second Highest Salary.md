@[toc]
# 1. 题目
Write a `SQL` query to get the **second highest** salary from the Employee table.
	
	+----+--------+
	| Id | Salary |
	+----+--------+
	| 1  | 100    |
	| 2  | 200    |
	| 3  | 300    |
	+----+--------+

For example, given the above `Employee` table, the query should return `200` as the **second highest** salary. If there is no second highest salary, then the query should return `null`.

	+---------------------+
	| SecondHighestSalary |
	+---------------------+
	| 200                 |
	+---------------------+


题意：这个题要求找出第二高的薪资数为多少。

# 2. 思路和代码
## (1) 找不是最大数的最大数
思路：找出最高的工资数；然后找出所有 $<$ 最高工资数的 `Salary` ，再从中找到最高的 `Salary` —— 这就是 `SecondHighestSalary` 。 

SQL代码，用到了不相干子查询：
```sql
# Write your MySQL query statement below
select max(Salary) as SecondHighestSalary 
from Employee 
where Salary in 
    (select Salary 
     from Employee
     where Salary < all
        (select max(Salary)
         from Employee 
        )
     );
```
可以简化一下，减少一层子查询：
```sql
select max(Salary) as SecondHighestSalary
from Employee
where Salary <> 
    (select max(Salary) 
     from Employee
     );
```
## (2) 排序并索引第二高薪资

思路：MySQL支持 `LIMIT` 语句来选取指定的条数数据。可以将不同的薪资按照降序排序，然后用 `LIMIT` 得到第二高的薪资。

### a. limit + 嵌套子查询
-    `limit y` 分句表示：读取 `y` 条数据

-    `limit x, y` 分句表示：跳过 `x` 条数据，读取 `y` 条数据

 -   `limit y offset x` 分句表示：跳过 `x` 条数据，读取 `y` 条数据

可以在排序后跳过 $1$ 条数据，读取 $1$ 条数据，这就是第二条数据。

这个方案的优点在于：可以扩展到查找第 $k$ 高的数据上；但是，这个方案也有几个问题：
1. 如果有重复值，比如最大值有几个，就得不到想要的结果；
2. 如果只有一个值，就是最大值，没有第二最高工资，就会被判断为错误答案。它会返回 `[]` 而不是 `[null]`。

为此，我们可以写成如下形式：
- 用 `distinct` 对指定属性列去重；
- 将查询的结果作为临时表，在外面再套一层查询

```sql
select
    (select distinct Salary
    from Employee
    order by Salary desc
    limit 1 offset 1
    )  as SecondHighestSalary;
```
### b. limit + ifnull
如果不这样写，还可以用 `ifnull` 和 `limit` 子句的结合。

MYSQL中 存在着 `isnull,ifnull,nullif` ：
- `isnull(expr)` 的用法： 如 `expr` 为 `null` ，那么 `isnull()` 的返回值为 $1$，否则返回值为 $0$。与之相比， 使用 `=` 判断是否为 `null` 值通常是错误的。
- `nullif(expr1, expr2)` 的用法：如果 `expr1` 和 `expr2` 相等则返回空值，不相等则返回 `expr1` 的结果。`expr1, expr2` 应该是相同数据类型或能隐含转换成相同数据类型。
- `ifnull(expr1,expr2)` 的用法：假如 `expr1` 不为 `null` ，则 `ifnull` 的返回值为 `expr1`；否则其返回值为 `expr2` 。`ifnull` 的返回值是数字或是字符串，具体情况取决于其所使用的语境。

代码如下：
```sql
select ifnull (
    (select distinct Salary
     from Employee
     order by Salary desc
     limit 1 offset 1), 
     null) as SecondHighestSalary;
```
# 3. 拓展——找第N高？
就用第二种方法就好了。
