> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Table: `Product`
```mysql
+--------------+---------+
| Column Name  | Type    |
+--------------+---------+
| product_id   | int     |
| product_name | varchar |
| unit_price   | int     |
+--------------+---------+
Product_id是该表的主键。
该表的每一行显示每个产品的名称和价格。
```

Table: `Sales`
```mysql
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| seller_id   | int     |
| product_id  | int     |
| buyer_id    | int     |
| sale_date   | date    |
| quantity    | int     |
| price       | int     |
+------ ------+---------+
这个表没有主键，它可以有重复的行。
product_id 是 Product 表的外键。
该表的每一行包含关于一个销售的一些信息。
```
编写一个SQL查询，报告`2019年春季`才售出的产品。即**仅**在 `2019-01-01` 至 `2019-03-31` （含）之间出售的商品。以 **任意顺序** 返回结果表。查询结果格式如下所示。

**示例 1:**
**输入：**
```mysql
Product table:
+------------+--------------+------------+
| product_id | product_name | unit_price |
+------------+--------------+------------+
| 1          | S8           | 1000       |
| 2          | G4           | 800        |
| 3          | iPhone       | 1400       |
+------------+--------------+------------+
`Sales` table:
+-----------+------------+----------+------------+----------+-------+
| seller_id | product_id | buyer_id | sale_date  | quantity | price |
+-----------+------------+----------+------------+----------+-------+
| 1         | 1          | 1        | 2019-01-21 | 2        | 2000  |
| 1         | 2          | 2        | 2019-02-17 | 1        | 800   |
| 2         | 2          | 3        | 2019-06-02 | 1        | 800   |
| 3         | 3          | 4        | 2019-05-13 | 2        | 2800  |
+-----------+------------+----------+------------+----------+-------+
```
**输出：**
```mysql
+-------------+--------------+
| product_id  | product_name |
+-------------+--------------+
| 1           | S8           |
+-------------+--------------+
```
**解释:**
id为1的产品仅在2019年春季销售。
id为2的产品在2019年春季销售，但也在2019年春季之后销售。
id 3的产品在2019年春季之后销售。
我们只退回产品1，因为它是2019年春季才销售的产品。

---
### 解法1 表连接+子查询+ `not in`
首先，从 `Sales` 表中找出不在2019-01-01到2019-03-31这段时间销售的产品ID：
```mysql
select product_id
from Sales
where sale_date < '2019-01-01' or sale_date > '2019-03-31';
```
然后，对 `Product` 中的产品，判断其 `product_id` 是否不属于上者：
```mysql
select product_id, product_name
from Product
where product_id not in (
    select product_id
    from Sales
    where sale_date < '2019-01-01' or sale_date > '2019-03-31'
);
```
此外，如果一个产品未曾销售过，那么 `Sales` 表中就没有它的数据，则 `not in` 返回的结果恒为真。我们可用表连接来判断该产品是否销售过，如果销售过，那么它将出现在连接后的表中。
```mysql 
select distinct Product.product_id, Product.product_name
from Product, Sales
where Product.product_id = Sales.product_id and Sales.product_id not in (
    select Sales.product_id
    from Sales
    where Sales.sale_date < '2019-01-01' or Sales.sale_date > '2019-03-31'
);
```
---
### 解法2 `left join` + `count` 的条件计数
题目要求“**仅**在2019-01-01至2019-03-31之间出售的商品”，翻译过来就是“所有售出日期都在这个时间内”，也就是“**在这个时间内的订单数量等于该商品的总订单数量**”，这样就不难写出如下语句：
```mysql
select sales.product_id as product_id, product.product_name as product_name
from sales left join product on sales.product_id = product.product_id -- 保留左表为NULL的记录
group by product_id
having count(sale_date between '2019-01-01' and '2019-03-31' or null) = count(*);
```
值得注意的是 `count` 的条件用法。举个例子 `count(age > 20 or null)` 这个语句，里面 `or null` 必须加，否则就等价于 `count(*)` 了，要么就是写作 `sum(age > 20)` 也可以。因为 `age > 20` 返回的是 `true` 或 `false` ，而 `count` 对 `true, false` 都是会计数一次的，只有NULL不会被计数。所以这个 `age > 20 or null` 表达的是不大于20就为NULL，这样就不会被 `count` 计数。相当于没有销售订单的不会被计数。

 