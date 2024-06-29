> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

表: `Orders`

```
+-----------------+----------+
| Column Name     | Type     |
+-----------------+----------+
| order_number    | int      |
| customer_number | int      |
+-----------------+----------+
Order_number是该表的主键。
此表包含关于订单ID和客户ID的信息。
```

编写一个SQL查询，为下了 **最多订单** 的客户查找 `customer_number` 。

测试用例生成后， **恰好有一个客户** 比任何其他客户下了更多的订单。

查询结果格式如下所示。

**示例 1:**

**输入:** 
```mysql
Orders 表:
+--------------+-----------------+
| order_number | customer_number |
+--------------+-----------------+
| 1            | 1               |
| 2            | 2               |
| 3            | 3               |
| 4            | 3               |
+--------------+-----------------+
```
**输出:** 
```mysql
+-----------------+
| customer_number |
+-----------------+
| 3               |
+-----------------+
解释:
customer_number 为 '3' 的顾客有两个订单，比顾客 '1' 或者 '2' 都要多，因为他们只有一个订单。
所以结果是该顾客的 customer_number ，也就是 3 。
```
**进阶：** 如果有多位顾客订单数并列最多，你能找到他们所有的 `customer_number` 吗？

---
### 解法1 分步骤+group by+having+子查询
第一步，思考如何输出每个客户的订单数量。可用 `group by` + `count` 聚合函数做到：
```mysql
select customer_number, count(order_number) 
from Orders 
group by customer_number;
```
第二步，找到客户订单的最大数目。这可以在上面的查询基础上，用 `max` 聚合函数求出：
```mysql
select max(sum) 
from (
    select customer_number, count(order_number) as sum
    from Orders
    group by customer_number
) tmp; 
```
第三步，找到订单数量与最大值相等的客户。上个查询求出的结果一定是一个数，因此可以使用判等谓词。**这种写法可以顺便解决进阶要求**——可能存在消费数一样的多个消费者：
```mysql
select customer_number
from Orders 
group by customer_number having count(order_number) = (
	select max(sum) 
	from (
	    select customer_number, count(order_number) as sum
	    from Orders
	    group by customer_number
	) tmp
);
```
或者考虑一下，如果当前客户的订单数量**大于等于**「分组后**所有**组的订单数量」，也是最大值：
```mysql 
select customer_number
from orders
group by customer_number having count(order_number) >= all(
	select count(order_number)
	from orders
	group by customer_number
);
```
本题还存在偷懒的一种写法。即按照消费者 `customer_number` 分组，并统计组内订单数目，再按组内统计数降序排序，最后只要排名为1的 `customer_number` 。注意，我们**可直接在 `order by` 中使用 `count` 计数**，然后用 `limit` 子句返回第一条数据——`limit` 子句第一个参数 `offset` 表示**跳过前面多少行后开始取数据**，第二个参数表示**最多返回多少行的数据**，默认 `offset` 为 $0$（不是 $1$ ）；只使用一个参数时，是指从结果的第一行开始返回的行数。
```mysql
select customer_number
from Orders
group by customer_number
order by count(order_number) desc
limit 0, 1;
```
---
### 解法2 窗口排名函数 `dense_rank`
类似的题目：
- [176. 第二高的薪水](https://leetcode.cn/problems/second-highest-salary/)（窗口总体排名+IFNULL）
- [574.当选者](https://leetcode.cn/problems/project-employees-ii/)（窗口总体排名+计数）
- [578.查询回答率最高](https://leetcode.cn/problems/get-highest-answer-rate-question/)（窗口总体排名+sumif+指标计算）
- [586. 订单最多的客户](https://leetcode.cn/problems/customer-placing-the-largest-number-of-orders/)（窗口总体排名+计数）
- [602. 好友申请 II ：谁有最多的好友](https://leetcode.cn/problems/friend-requests-ii-who-has-the-most-friends/)（窗口总体排名+union all）
- [1076.项目员工II](https://leetcode.cn/problems/project-employees-ii/)（窗口总体排名+计数）
- [1082.销售分析 I](https://leetcode.cn/problems/project-employees-ii/)（窗口总体排名+求和）
- [1112. 每位学生的最高](https://leetcode.cn/problems/highest-grade-for-each-student/)（窗口总体排名）

这里先使用 `customer_number` 进行分组计数，然后**使用 `dense_rank` 开窗函数，里面用 `order by count` 对分组计数进行排名**。最后再嵌套外层查询，获取排名为1的所有消费者。
> `dense_rank` 可以没有 `partition by` ，但不能没有 `order by` 。使用 `partition by` 是组内排名；不使用 `partition by` 是总体排名。

```mysql
select customer_number
from (
    select *,
        dense_rank() over(order by count(order_number) desc) as ranking
    from Orders
    group by customer_number
) tmp
where ranking = 1;
```