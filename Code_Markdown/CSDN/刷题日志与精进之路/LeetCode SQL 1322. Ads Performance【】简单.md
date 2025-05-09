<p>Table: <code>Ads</code></p>

```haskell
+---------------+---------+
| Column Name   | Type    |
+---------------+---------+
| ad_id         | int     |
| user_id       | int     |
| action        | enum    |
+---------------+---------+
(ad_id, user_id) is the primary key for this table.
Each row of this table contains the ID of an Ad, the ID of a user and the action taken by this user regarding this Ad.
The action column is an ENUM type of ('Clicked', 'Viewed', 'Ignored').
```

 
 
<p>A company is running Ads and wants to calculate the performance of each Ad.</p>

<p>Performance of the Ad is measured using&nbsp;Click-Through Rate (CTR) where:</p>

<p><img style="width: 600px; height: 75px;" src="https://assets.leetcode.com/uploads/2020/01/17/sql1.png" alt=""></p>

<p>Write an SQL query to find the <code>ctr</code> of each Ad.</p>

<p><strong>Round</strong> <code>ctr</code>&nbsp;to 2 decimal points. <strong>Order</strong> the result table by <code>ctr</code>&nbsp;in descending order&nbsp;and by&nbsp;<code>ad_id</code>&nbsp;in ascending order in case of a tie.</p>

<p>The query result format is in the following example:</p>

 
```haskell
Ads table:
+-------+---------+---------+
| ad_id | user_id | action  |
+-------+---------+---------+
| 1     | 1       | Clicked |
| 2     | 2       | Clicked |
| 3     | 3       | Viewed  |
| 5     | 5       | Ignored |
| 1     | 7       | Ignored |
| 2     | 7       | Viewed  |
| 3     | 5       | Clicked |
| 1     | 4       | Viewed  |
| 2     | 11      | Viewed  |
| 1     | 2       | Clicked |
+-------+---------+---------+
Result table:
+-------+-------+
| ad_id | ctr   |
+-------+-------+
| 1     | 66.67 |
| 3     | 50.00 |
| 2     | 33.33 |
| 5     | 0.00  |
+-------+-------+
for ad_id = 1, ctr = (2/(2+1)) * 100 = 66.67
for ad_id = 2, ctr = (1/(1+2)) * 100 = 33.33
for ad_id = 3, ctr = (1/(1+1)) * 100 = 50.00
for ad_id = 5, ctr = 0.00, Note that ad_id = 5 has no clicks or views.
Note that we don't care about Ignored Ads.
Result table is ordered by the ctr. in case of a tie we order them by ad_id
```
题意：一家公司正在运营这些广告并想计算每条广告的效果。广告效果用点击通过率（Click-Through Rate：CTR）来衡量，写一条SQL语句来查询每一条广告的 `ctr` 。
