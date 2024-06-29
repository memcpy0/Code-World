<p>You are driving a vehicle that&nbsp;has <code>capacity</code> empty seats initially available for passengers.&nbsp; The vehicle <strong>only</strong> drives east (ie. it <strong>cannot</strong> turn around and drive west.)</p>

<p>Given a list of <code>trips</code>, <code>trip[i] = [num_passengers, start_location, end_location]</code>&nbsp;contains information about the <code>i</code>-th trip: the number of passengers that must be picked up, and the locations to pick them up and drop them off.&nbsp; The locations are given as the number of kilometers&nbsp;due east from your vehicle's initial location.</p>

<p>Return <code>true</code> if and only if&nbsp;it is possible to pick up and drop off all passengers for all the given trips.&nbsp;</p>
 

<p><strong>Example 1:</strong></p>

```clike
Input: trips = [[2,1,5],[3,3,7]], capacity = 4
Output: false
```
<p><strong>Example 2:</strong></p>

```clike
Input: trips = [[2,1,5],[3,3,7]], capacity = 5
Output: true
```

 <p><strong>Example 3:</strong></p>

```clike
Input: trips = [[2,1,5],[3,5,7]], capacity = 3
Output: true
```

 <p><strong>Example 4:</strong></p>

```clike
Input: trips = [[3,2,7],[3,7,9],[8,3,9]], capacity = 11
Output: true
```

 <p><strong>Constraints:</strong></p>

<ol>
	<li><code>trips.length &lt;= 1000</code></li>
	<li><code>trips[i].length == 3</code></li>
	<li><code>1 &lt;= trips[i][0] &lt;= 100</code></li>
	<li><code>0 &lt;= trips[i][1] &lt; trips[i][2] &lt;= 1000</code></li>
	<li><code>1 &lt;=&nbsp;capacity &lt;= 100000</code></li>
</ol>

题意：请你根据给出的行程计划表和车子的座位数，来判断你的车是否可以顺利完成接送所有乘客的任务（当且仅当可以在所有给定的行程中接送所有乘客时，返回 `true`，否则请返回 `false`）。


---
### 解法 差分
很简单的题目：
```cpp
class Solution {
public:
    bool carPooling(vector<vector<int>>& trips, int capacity) {
        vector<int> d(1001, 0); //每个站点i上车的乘客数量数组people[]的差分
        for (const vector<int>& trip : trips) d[trip[1]] += trip[0], d[trip[2]] -= trip[0];
        int sum = 0;
        for (int i = 0; i <= 1000; ++i) {
            sum += d[i];
            if (sum > capacity) return false;
        }
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了85.41% 的用户
内存消耗：8.7 MB, 在所有 C++ 提交中击败了59.93% 的用户
```
类似的题目有：
[1854. Maximum Population Year](https://memcpy0.blog.csdn.net/article/details/116765984)
[1109. Corporate Flight Bookings](https://memcpy0.blog.csdn.net/article/details/116857235)
[1589. Maximum Sum Obtained of Any Permutation](https://memcpy0.blog.csdn.net/article/details/108711074)，这一题比较难

另外，这一题还可以用贪心+排序的方法解决。

