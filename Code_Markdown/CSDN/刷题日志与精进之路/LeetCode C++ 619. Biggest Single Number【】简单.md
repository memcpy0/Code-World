Table `my_numbers` contains many numbers in column **num** including duplicated ones. Can you write a SQL query to find the biggest number, which only appears once.
```haskell
+---+
|num|
+---+
| 8 |
| 8 |
| 3 |
| 3 |
| 1 |
| 4 |
| 5 |
| 6 | 
```

For the sample data above, your query should return the following result:

```haskell
+---+
|num|
+---+
| 6 |
```

**Note:** If there is no such number, just output `null`. 

题意：写一个 `SQL` 查询语句，找到只出现过一次的数字中，最大的一个数字吗？
