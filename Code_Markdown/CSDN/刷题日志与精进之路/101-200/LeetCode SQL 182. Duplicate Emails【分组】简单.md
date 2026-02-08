Write a SQL query to find all duplicate emails in a table named `Person`.

```swift
+----+---------+
| Id | Email   |
+----+---------+
| 1  | a@b.com |
| 2  | c@d.com |
| 3  | a@b.com |
+----+---------+
```

For example, your query should return the following for the above table:

```swift
+---------+
| Email   |
+---------+
| a@b.com |
+---------+
```

**Note:** All emails are in lowercase.


题意：写一个SQL查询，找到表中所有重复的电子邮箱。

---
### 解法 分组
分组后借助 `HAVING` 子句：
```sql
select Email
from Person
group by Email having count(Id) >= 2
```
提交后效率如下：
```cpp
执行用时：298 ms, 在所有 MySQL 提交中击败了36.83% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```
