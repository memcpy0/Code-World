<p>You have a list of points in the plane. Return the area of the largest triangle that can be formed by any 3 of the points.</p>
<strong>Example:</strong>

```css
Input: points = [[0,0],[0,1],[1,0],[0,2],[2,0]]
Output: 2
Explanation: 
The five points are show in the figure below. The red triangle is the largest.
```

 

<p><img style="height:328px; width:400px" src="https://s3-lc-upload.s3.amazonaws.com/uploads/2018/04/04/1027.png" width="55%" alt=""></p>

<p><strong>Notes: </strong></p>

<ul>
	<li><code>3 &lt;= points.length &lt;= 50</code>.</li>
	<li>No points will be duplicated.</li>
	<li>&nbsp;<code>-50 &lt;= points[i][j] &lt;= 50</code>.</li>
	<li>Answers within&nbsp;<code>10^-6</code>&nbsp;of the true value will be accepted as correct.</li>
</ul>
 
题意：给定包含多个点的集合，从其中取三个点组成三角形，返回能组成的最大三角形的面积。
