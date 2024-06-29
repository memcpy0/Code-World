Table: `Department`

```haskell
+---------------+---------+
| Column Name   | Type    |
+---------------+---------+
| id            | int     |
| revenue       | int     |
| month         | varchar |
+---------------+---------+
(id, month) is the primary key of this table.
The table has information about the revenue of each department per month.
The month has values in ["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"].
```

 

Write an SQL query to reformat the table such that there is a department id column and a revenue column **for each month**.

The query result format is in the following example:

```haskell
Department table:
+------+---------+-------+
| id   | revenue | month |
+------+---------+-------+
| 1    | 8000    | Jan   |
| 2    | 9000    | Jan   |
| 3    | 10000   | Feb   |
| 1    | 7000    | Feb   |
| 1    | 6000    | Mar   |
+------+---------+-------+

Result table:
+------+-------------+-------------+-------------+-----+-------------+
| id   | Jan_Revenue | Feb_Revenue | Mar_Revenue | ... | Dec_Revenue |
+------+-------------+-------------+-------------+-----+-------------+
| 1    | 8000        | 7000        | 6000        | ... | null        |
| 2    | 9000        | null        | null        | ... | null        |
| 3    | null        | 10000       | null        | ... | null        |
+------+-------------+-------------+-------------+-----+-------------+

Note that the result table has 13 columns (1 for the department id + 12 for the months).
```
题意：编写一个 `SQL` 查询来重新格式化表，使得新的表中有一个部门 `id` 列和一些对应 **每个月** 的收入（`revenue`）列。

---
### 解法 `GROUP BY` 、聚合函数和 `IF`
这题中，`(id, month)` 是联合主键，因此按照 `id` 分组后，每个 `id` 组内，不会有相同的月份 `month` ，即每个 `id` 组内每个月份只有一条记录。因此在下面的代码中，**聚合函数**使用 `SUM/MAX/MIN/AVG` 都是一样的效果。然而**没有聚合函数是万万不能的**，但凡使用`GROUP BY` ，前面一定要有聚合函数：
```sql
SELECT 
    DISTINCT id,
    SUM(IF(month = "Jan", revenue, null)) as Jan_Revenue,
    SUM(IF(month = "Feb", revenue, null)) as Feb_Revenue,
    SUM(IF(month = "Mar", revenue, null)) as Mar_Revenue,
    SUM(IF(month = "Apr", revenue, null)) as Apr_Revenue,
    SUM(IF(month = "May", revenue, null)) as May_Revenue,
    SUM(IF(month = "Jun", revenue, null)) as Jun_Revenue,
    SUM(IF(month = "Jul", revenue, null)) as Jul_Revenue,
    SUM(IF(month = "Aug", revenue, null)) as Aug_Revenue,
    SUM(IF(month = "Sep", revenue, null)) as Sep_Revenue,
    SUM(IF(month = "Oct", revenue, null)) as Oct_Revenue,
    SUM(IF(month = "Nov", revenue, null)) as Nov_Revenue,
    SUM(IF(month = "Dec", revenue, null)) as Dec_Revenue
FROM Department 
GROUP BY id
ORDER BY id;
```
运行效率如下：
```sql
执行用时：463 ms, 在所有 MySQL 提交中击败了22.39% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```
