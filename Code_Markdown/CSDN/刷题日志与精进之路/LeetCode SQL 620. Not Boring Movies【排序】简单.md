
X city opened a new cinema, many people would like to go to this cinema. The cinema also gives out a poster indicating the movies’ ratings and descriptions.

Please write a SQL query to output movies with an odd numbered ID and a description that is not 'boring'. Order the result by rating.

 

For example, table `cinema`:

```swift
+---------+-----------+--------------+-----------+
|   id    | movie     |  description |  rating   |
+---------+-----------+--------------+-----------+
|   1     | War       |   great 3D   |   8.9     |
|   2     | Science   |   fiction    |   8.5     |
|   3     | irish     |   boring     |   6.2     |
|   4     | Ice song  |   Fantacy    |   8.6     |
|   5     | House card|   Interesting|   9.1     |
+---------+-----------+--------------+-----------+
```

For the example above, the output should be:

```swift
+---------+-----------+--------------+-----------+
|   id    | movie     |  description |  rating   |
+---------+-----------+--------------+-----------+
|   5     | House card|   Interesting|   9.1     |
|   1     | War       |   great 3D   |   8.9     |
+---------+-----------+--------------+-----------+
```

题意：编写一个 `SQL` 查询，找出所有影片描述为非 `boring` (不无聊)的并且 `id` 为奇数的影片，结果按等级 `rating` 排列。

---
### 解法 `select`
MySQL代码如下：
```sql
select id, movie, description, rating
from cinema
where id mod 2 = 1 and description != "boring"
order by rating desc;
```
执行效果如下：
```sql
执行用时：158 ms, 在所有 MySQL 提交中击败了75.62% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```
