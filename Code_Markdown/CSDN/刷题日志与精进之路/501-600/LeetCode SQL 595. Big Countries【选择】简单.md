There is a table `World`

```swift
+-----------------+------------+------------+--------------+---------------+
| name            | continent  | area       | population   | gdp           |
+-----------------+------------+------------+--------------+---------------+
| Afghanistan     | Asia       | 652230     | 25500100     | 20343000      |
| Albania         | Europe     | 28748      | 2831741      | 12960000      |
| Algeria         | Africa     | 2381741    | 37100000     | 188681000     |
| Andorra         | Europe     | 468        | 78115        | 3712000       |
| Angola          | Africa     | 1246700    | 20609294     | 100990000     |
+-----------------+------------+------------+--------------+---------------+
```

A country is big if it has an area of bigger than 3 million square km or a population of more than 25 million.

Write a SQL solution to output big countries' name, population and area.

For example, according to the above table, we should output:

```swift
+--------------+-------------+--------------+
| name         | population  | area         |
+--------------+-------------+--------------+
| Afghanistan  | 25500100    | 652230       |
| Algeria      | 37100000    | 2381741      |
+--------------+-------------+--------------+
```
题意：如果一个国家的面积超过 `300` 万平方公里，或者人口超过 `2500` 万，那么这个国家就是大国家。编写一个 `SQL` 查询，输出表中所有大国家的名称、人口和面积。

---
### 解法 `select`
MySQL代码如下：
```sql 
select name, population, area
from World
where area > 3000000 or population > 25000000
```
效率如下：
```cpp
执行用时：206 ms, 在所有 MySQL 提交中击败了60.84% 的用户
内存消耗：0 B, 在所有 MySQL 提交中击败了100.00% 的用户
```
