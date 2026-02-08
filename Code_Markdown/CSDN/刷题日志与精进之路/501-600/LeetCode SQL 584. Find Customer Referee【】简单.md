Given a table `customer` holding customers information and the referee.

```haskell
+------+------+-----------+
| id   | name | referee_id|
+------+------+-----------+
|    1 | Will |      NULL |
|    2 | Jane |      NULL |
|    3 | Alex |         2 |
|    4 | Bill |      NULL |
|    5 | Zack |         1 |
|    6 | Mark |         2 |
+------+------+-----------+
```

Write a query to return the list of customers **NOT** referred by the person with id `'2'` .

For the sample data above, the result is:

```haskell
+------+
| name |
+------+
| Will |
| Jane |
| Bill |
| Zack |
+------+
```
题意：给定表 `customer` ，里面保存了所有客户信息和他们的推荐人。写一个查询语句，返回一个编号列表，列表中编号的推荐人的编号都**不是** `2` 。
