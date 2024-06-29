Table: `Tweets`

```cpp
+----------------+---------+
| Column Name    | Type    |
+----------------+---------+
| tweet_id       | int     |
| content        | varchar |
+----------------+---------+
tweet_id is the primary key for this table.
This table contains all the tweets in a social media app.
```
 

Write an SQL query to find the IDs of the invalid tweets. The tweet is invalid if the number of characters used in the content of the tweet is **strictly greater** than `15`.

Return the result table **in any order**.

The query result format is in the following example:

 

```cpp
Tweets table:
+----------+----------------------------------+
| tweet_id | content                          |
+----------+----------------------------------+
| 1        | Vote for Biden                   |
| 2        | Let us make America great again! |
+----------+----------------------------------+

Result table:
+----------+
| tweet_id |
+----------+
| 2        |
+----------+
Tweet 1 has length = 14. It is a valid tweet.
Tweet 2 has length = 32. It is an invalid tweet.
```
题意：写一个 SQL 查询，找出无效的推特。

---
### 解法 `SELECT` 和 `LENGTH`
MySQL中计算字段的长度，有两个函数：
- `LENGTH`：一个用来获取字符串长度的内置函数，其中一个汉字算作三个字符，一个数字或字母算作一个字符
- `CHAR_LENGTH`：不管汉字还是数字或者是字母，都算是一个字符

```sql
SELECT tweet_id
FROM Tweets
WHERE LENGTH(content) > 15;
```
运行结果如下：
```cpp
执行用时：435 ms, 在所有 MySQL 提交中击败了100.00% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```
