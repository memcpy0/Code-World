<p>Given an array of meeting time <code>intervals</code>&nbsp;where <code>intervals[i] = [start<sub>i</sub>, end<sub>i</sub>]</code>, determine if a person could attend all meetings.</p>

**Example 1:**

```haskell
Input: intervals = [[0,30],[5,10],[15,20]]
Output: false
```

**Example 2:**

```haskell
Input: intervals = [[7,10],[2,4]]
Output: true
```

**Constraints:**
<ul>
	<li><code>0 &lt;= intervals.length &lt;= 10<sup>4</sup></code></li>
	<li><code>intervals[i].length == 2</code></li>
	<li><code>0 &lt;= start<sub>i</sub> &lt;&nbsp;end<sub>i</sub> &lt;= 10<sup>6</sup></code></li>
</ul>

题意：给定一个会议时间安排的数组 `intervals` ，每个会议时间都会包括开始和结束的时间 `intervals[i] = [starti, endi]` ，判断一个人是否能够参加这里面的全部会议。
