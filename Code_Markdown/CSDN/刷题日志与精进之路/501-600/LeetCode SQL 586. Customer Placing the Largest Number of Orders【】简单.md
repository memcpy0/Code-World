<p>Query the <b>customer_number</b> from the <b><i>orders</i></b> table for the customer who has placed the largest number of orders.</p>

<p>It is guaranteed that exactly one customer will have placed more orders than any other customer.</p>

<p>The <b><i>orders</i></b> table is defined as follows:</p>

```haskell
| Column            | Type      |
|-------------------|-----------|
| order_number (PK) | int       |
| customer_number   | int       |
| order_date        | date      |
| required_date     | date      |
| shipped_date      | date      |
| status            | char(15)  |
| comment           | char(200) |
```

 

<p><b>Sample Input</b></p>

```haskell
| order_number | customer_number | order_date | required_date | shipped_date | status | comment |
|--------------|-----------------|------------|---------------|--------------|--------|---------|
| 1            | 1               | 2017-04-09 | 2017-04-13    | 2017-04-12   | Closed |         |
| 2            | 2               | 2017-04-15 | 2017-04-20    | 2017-04-18   | Closed |         |
| 3            | 3               | 2017-04-16 | 2017-04-25    | 2017-04-20   | Closed |         |
| 4            | 3               | 2017-04-18 | 2017-04-28    | 2017-04-25   | Closed |         | 
```

<p><b>Sample Output</b></p>

```haskell
| customer_number |
|-----------------|
| 3               | 
```

<p><b>Explanation</b></p>

```haskell
The customer with number '3' has two orders, which is greater than either customer '1' or '2' because each of them  only has one order. 
So the result is customer_number '3'.
```
 
<p><i><b>Follow up:</b> What if more than one customer have the largest number of orders, can you find all the customer_number in this case?</i></p>

 

题意：在表 `orders` 中找到订单数最多客户对应的 `customer_number` 。
