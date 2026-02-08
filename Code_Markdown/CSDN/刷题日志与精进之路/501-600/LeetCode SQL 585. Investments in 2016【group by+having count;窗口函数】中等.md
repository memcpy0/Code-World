> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

`Insurance` 表：
```sql
+-------------+-------+
| Column Name | Type  |
+-------------+-------+
| pid         | int   |
| tiv_2015    | float |
| tiv_2016    | float |
| lat         | float |
| lon         | float |
+-------------+-------+
pid 是这张表的主键。
表中的每一行都包含一条保险信息，其中：
pid 是投保人的投保编号。
tiv_2015 是该投保人在 2015 年的总投保金额，tiv_2016 是该投保人在 2016 年的总投保金额。
lat 是投保人所在城市的纬度。题目数据确保 lat 不为空。
lon 是投保人所在城市的经度。题目数据确保 lon 不为空。
```
请你编写一个 SQL 查询，报告 2016 年 (`tiv_2016`) 所有满足下述条件的投保人的投保金额之和：
- 他在 2015 年的投保额 (`tiv_2015`) 至少跟一个其他投保人在 2015 年的投保额相同。
- 他所在的城市必须与其他投保人都不同（也就是说 (`lat, lon`) 不能跟其他任何一个投保人完全相同）。

`tiv_2016` 四舍五入的 **两位小数** 。

查询结果格式如下例所示。

**示例：**
```sql
输入：
Insurance 表：
+-----+----------+----------+-----+-----+
| pid | tiv_2015 | tiv_2016 | lat | lon |
+-----+----------+----------+-----+-----+
| 1   | 10       | 5        | 10  | 10  |
| 2   | 20       | 20       | 20  | 20  |
| 3   | 10       | 30       | 20  | 20  |
| 4   | 10       | 40       | 40  | 40  |
+-----+----------+----------+-----+-----+
输出：
+----------+
| tiv_2016 |
+----------+
| 45.00    |
+----------+
解释：
表中的第一条记录和最后一条记录都满足两个条件。
tiv_2015 值为 10 与第三条和第四条记录相同，且其位置是唯一的。
第二条记录不符合任何一个条件。其 tiv_2015 与其他投保人不同，并且位置与第三条记录相同，这也导致了第三条记录不符合题目要求。
因此，结果是第一条记录和最后一条记录的 tiv_2016 之和，即 45 。
```
---
### 解法1 子查询
本题的要求比较多，对每个投保人，先检查：
- 他在 2015 年的投保额 (`tiv_2015`) **至少跟一个**其他投保人在 2015 年的投保额**相同**。
- 他所在的城市必须与其他投保人**都不同**（也就是说 (`lat, lon`) 不能跟其他任何一个投保人完全相同）。

然后报告 2016 年 (`tiv_2016`) 所有满足这两个条件的投保人的投保金额之和。

要判断一个人 $x$ 的投保额是否和其他投保人（至少一个）相同，有几种办法。第一是：**子查询中，筛选出不等于 ${x.pid}$ 的其他人在 2015 的投保额，然后判断 $x$ 的投保额 $tiv\_2015$ 是否在这些投保额中**。
```sql
select ...
from Insurance as A
where tiv_2015 in (
    select tiv_2015 from Insurance as B
    where A.pid != B.pid
)
```
另外一种是，**子查询中使用 `group by` 和 `having count`** ，筛选出那些出现次数超过 $1$ 的投保额。
```sql
select ...
from Insurance as A
where tiv_2015 in (
    select tiv_2015 from Insurance as B
    group by tiv_2015
    having count(*) > 1
)
```
这两种「检查至少跟其他一个相同」的写法，还能用来判断「都不同」。不过这里，**我们需要连接 `LAT` 和 `LON` 为一个整体，以表示坐标信息**。
> 注意：这两条要求需要不分顺序同时满足，所以如果想要先用 `LAT` 来筛选一遍数据，然后再用 `LON` 来筛选，会得到错误的结果。

```sql
(lat, lon) not in (
    select lat, lon from Insurance as B
    where A.pid != B.pid
)

-- 或者写成
concat(lat, lon) in (
    select concat(lat, lon) from Insurance
    group by lat, lon
    having count(*) = 1
)
```
答案如下：
```mysql
select round(sum(tiv_2016), 2) as tiv_2016
from Insurance as A
where tiv_2015 in (
    select tiv_2015 from Insurance as B
    where A.pid != B.pid
) and (lat, lon) not in (
    select lat, lon from Insurance as B
    where A.pid != B.pid
);

select round(sum(tiv_2016), 2) as tiv_2016
from Insurance as A
where A.tiv_2015 in (
    select tiv_2015
    from insurance
    group by TIV_2015
    having count(*) > 1
) and concat(lat, lon) in (
    select concat(lat, lon) from Insurance
    group by lat, lon
    having count(*) = 1
);
```
---
### 解法2 窗口函数
对 `tiv_2015` 和 `(lat, lon)` 分别使用窗口函数，对它们先分组 `partition` 然后计算每组的数量：
```mysql
select round(sum(tiv_2016), 2) as tiv_2016
from (
    select tiv_2016,
        count(1) over (partition by tiv_2015) as tiv_2015_count,
        count(1) over (partition by lat, lon) as location_count
    from Insurance
) tmp
where tiv_2015_count <> 1 and location_count = 1
```