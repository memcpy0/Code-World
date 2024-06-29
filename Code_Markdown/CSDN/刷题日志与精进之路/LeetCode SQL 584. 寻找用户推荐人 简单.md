> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定表 `customer` ，里面保存了所有客户信息和他们的推荐人。

```sql
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

写一个查询语句，返回一个客户列表，列表中客户的推荐人的编号都 **不是** 2。

对于上面的示例数据，结果为：

```sql
+------+
| name |
+------+
| Will |
| Jane |
| Bill |
| Zack |
+------+
```

---
### 解法 三值逻辑
> 本地可见[[../基础课程和实践项目/数据库系统/【数据库系统】第一部分 数据库基础(3) 关系数据库标准语言SQL#3.6 空值的处理]]。

题目虽简，包含的知识点也不少。
> SQL的很多保留字都被归类为**谓词**，例如 `>, <> , =` 等比较谓词，以及 `BETWEEN, LIKE, IN, IS NULL` 等。**谓词是一种特殊的函数，返回值是真值**。真值可能是 `true, false, unknown` 三者之一（都是小写！），**因为SQL是三值逻辑，所以有三种真值**。
> 
> 需要注意的是，NULL不是值也不是变量，**SQL中对NULL使用比较和判等谓词得到的结果是 `unknown`** ，即对NULL使用这些谓词是没有意义的。而 `IS NULL` 是**一个**谓词，看做 `IS_NULL` 或许更合适。
> 
> 还需注意的是，**查询出的结果只包含令where子句的判断结果为 `true` 的行，不包括判断结果为 `false` 和 `unknown` 的行**。

这题的客户可能推荐人编号不是2，还有可能没有推荐人，即对应为NULL。因此用 `OR` + `IS NULL` 。注意，`name` 可能有重名。
```mysql
select name
from customer
where referee_id is null or referee_id != 2;
```
或者用**子查询**查出「推荐人编号为2的所有客户ID」的集合，然后 `NOT IN` 判断哪些客户ID不在该集合中：
```mysql
select name
from customer
where id not in (
	select id
	from customer 
	where referee_id = 2
);
```
> 注意：如果集合中有NULL，则 `NOT IN` 可能查不到值。比如 `WHERE id NOT IN (1, 2, NULL)` ，看起来好像是 `id` 不为 `1, 2` 也不为NULL，但实际上可以写成 `WHERE id != 1 AND id != 2 AND id != NULL` ，而 `id != NULL` 为 `unknown` ，所以整个 `AND` 连接的表达式结果也为 `unknown` 。
> 
> 结论：**如果 `NOT IN` 用到的子查询表中、被选择的列上存在NULL，则SQL整体的查询结果永远是空**！为了解决烦人的NULL，最好在表里添加NOT NULL约束来尽力排除NULL；或者加一个函数，把NULL置为一个值，即 `IFNULL(referee_id, '1')` 。

还可用 `UNION ALL` ，用 `UNION` 则会自动去重：
```mysql
select name
from customer
where referee_id != 2
union all
select name
from customer
where referee_id is null;
```
最后，还可使用 `exists` ，先查出 `referee_id` 的数据，再用 `exists` 。注意语法，**`exists` 需要关联表**。
```mysql
select name
from customer C
where not exists (
    select name
    from customer C1
    where C1.id = C.id and C1.referee_id = 2
);
```

