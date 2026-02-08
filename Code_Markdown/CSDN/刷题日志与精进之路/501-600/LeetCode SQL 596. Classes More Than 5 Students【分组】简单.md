

There is a table `courses` with columns: **student** and **class**. Please list out all classes which have more than or equal to 5 students. For example, the table:

```swift
+---------+------------+
| student | class      |
+---------+------------+
| A       | Math       |
| B       | English    |
| C       | Math       |
| D       | Biology    |
| E       | Math       |
| F       | Computer   |
| G       | Math       |
| H       | Math       |
| I       | Math       |
+---------+------------+
```

Should output:

```swift
+---------+
| class   |
+---------+
| Math    |
+---------+
```

**Note:** The students should not be counted duplicate in each course.

题意：有一个 `courses` 表 ，有 `student` (学生) 和 `class` (课程)。列出所有**超过或等于** `5` 名学生的课。


---
### 解法1 分组
MySQL代码如下：
```sql
select class
from courses
group by class having count(distinct(student)) >= 5
```
效率如下：
```sql
执行用时：240 ms, 在所有 MySQL 提交中击败了40.47% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```

