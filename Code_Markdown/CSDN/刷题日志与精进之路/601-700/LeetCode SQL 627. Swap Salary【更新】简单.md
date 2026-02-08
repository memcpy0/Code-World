Given a table `salary`, such as the one below, that has m=male and f=female values. Swap all f and m values (i.e., change all f values to m and vice versa) with a **single update statement** and no intermediate temp table.

Note that you must write a single **update** statement, DO NOT write any **select** statement for this problem.

 

**Example:**

```swift
| id | name | sex | salary |
|----|------|-----|--------|
| 1  | A    | m   | 2500   |
| 2  | B    | f   | 1500   |
| 3  | C    | m   | 5500   |
| 4  | D    | f   | 500    |
```

After running your **update** statement, the above `salary` table should have the following rows:

```swift
| id | name | sex | salary |
|----|------|-----|--------|
| 1  | A    | f   | 2500   |
| 2  | B    | m   | 1500   |
| 3  | C    | f   | 5500   |
| 4  | D    | m   | 500    |
```

 题意：交换所有的 `f` 和 `m` 值。要求只使用一个 **Update** 语句，不要编写任何 **Select** 语句，并且没有中间的临时表。 


----
### 解法 `Update` 的 `set` 分句和 `if` 三元表达式
MySQL代码如下：
```sql
update salary
set sex = if (sex = 'm', 'f', 'm');
```
提交后效果如下：
```sql
执行用时：197 ms, 在所有 MySQL 提交中击败了43.00% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```
