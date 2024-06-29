<p><b>Description</b></p>

<p>Given three tables: <code>salesperson</code>, <code>company</code>, <code>orders</code>.<br>
Output all the <b>names</b> in the table <code>salesperson</code>, who didn’t have sales to company 'RED'.</p>

<p><b>Example</b><br>
<b>Input</b></p>

<p>Table: <code>salesperson</code></p>

```haskell
+----------+------+--------+-----------------+-----------+
| sales_id | name | salary | commission_rate | hire_date |
+----------+------+--------+-----------------+-----------+
|   1      | John | 100000 |     6           | 4/1/2006  |
|   2      | Amy  | 120000 |     5           | 5/1/2010  |
|   3      | Mark | 65000  |     12          | 12/25/2008|
|   4      | Pam  | 25000  |     25          | 1/1/2005  |
|   5      | Alex | 50000  |     10          | 2/3/2007  |
+----------+------+--------+-----------------+-----------+
```

The table <code>salesperson</code> holds the salesperson information. Every salesperson has a <b>sales_id</b> and a <b>name</b>.

<p>Table: <code>company</code></p>

```haskell
+---------+--------+------------+
| com_id  |  name  |    city    |
+---------+--------+------------+
|   1     |  RED   |   Boston   |
|   2     | ORANGE |   New York |
|   3     | YELLOW |   Boston   |
|   4     | GREEN  |   Austin   |
+---------+--------+------------+
```


The table <code>company</code> holds the company information. Every company has a <b>com_id</b> and a <b>name</b>.

<p>Table: <code>orders</code></p>

```haskell
+----------+------------+---------+----------+--------+
| order_id | order_date | com_id  | sales_id | amount |
+----------+------------+---------+----------+--------+
| 1        |   1/1/2014 |    3    |    4     | 100000 |
| 2        |   2/1/2014 |    4    |    5     | 5000   |
| 3        |   3/1/2014 |    1    |    1     | 50000  |
| 4        |   4/1/2014 |    1    |    4     | 25000  |
+----------+----------+---------+----------+--------+
```


The table <code>orders</code> holds the sales record information, salesperson and customer company are represented by <b>sales_id</b> and <b>com_id</b>.

<p><b>output</b></p>

```haskell
+------+
| name | 
+------+
| Amy  | 
| Mark | 
| Alex |
+------+
```

<p><b>Explanation</b></p>
<p>According to order '3' and '4' in table <code>orders</code>, it is easy to tell only salesperson 'John' and 'Pam' have sales to company 'RED',<br>
so we need to output all the other <b>names</b> in the table <code>salesperson</code>.</p>


题意：给定 3 个表：`salesperson`，`company`，`orders`。输出所有表 `salesperson` 中，没有向公司 `'RED'` 销售任何东西的销售员。
 


