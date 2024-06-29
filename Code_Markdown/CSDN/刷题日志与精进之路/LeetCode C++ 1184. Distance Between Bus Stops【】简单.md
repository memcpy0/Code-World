<p>A bus&nbsp;has <code>n</code> stops numbered from <code>0</code> to <code>n - 1</code> that form&nbsp;a circle. We know the distance between all pairs of neighboring stops where <code>distance[i]</code> is the distance between the stops number&nbsp;<code>i</code> and <code>(i + 1) % n</code>.</p>

<p>The bus goes along both directions&nbsp;i.e. clockwise and counterclockwise.</p>

<p>Return the shortest distance between the given&nbsp;<code>start</code>&nbsp;and <code>destination</code>&nbsp;stops.</p>

 
<p><strong>Example 1:</strong></p>

<p><img style="width: 388px; height: 240px;" src="https://assets.leetcode.com/uploads/2019/09/03/untitled-diagram-1.jpg" alt=""></p>

<pre><strong>Input:</strong> distance = [1,2,3,4], start = 0, destination = 1
<strong>Output:</strong> 1
<strong>Explanation:</strong> Distance between 0 and 1 is 1 or 9, minimum is 1.</pre>

 

<p><strong>Example 2:</strong></p>

<p><img style="width: 388px; height: 240px;" src="https://assets.leetcode.com/uploads/2019/09/03/untitled-diagram-1-1.jpg" alt=""></p>

<pre><strong>Input:</strong> distance = [1,2,3,4], start = 0, destination = 2
<strong>Output:</strong> 3
<strong>Explanation:</strong> Distance between 0 and 2 is 3 or 7, minimum is 3.
</pre>

 
<p><strong>Example 3:</strong></p>

<p><img style="width: 388px; height: 240px;" src="https://assets.leetcode.com/uploads/2019/09/03/untitled-diagram-1-2.jpg" alt=""></p>

<pre><strong>Input:</strong> distance = [1,2,3,4], start = 0, destination = 3
<strong>Output:</strong> 4
<strong>Explanation:</strong> Distance between 0 and 3 is 6 or 4, minimum is 4.
</pre>

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n&nbsp;&lt;= 10^4</code></li>
	<li><code>distance.length == n</code></li>
	<li><code>0 &lt;= start, destination &lt; n</code></li>
	<li><code>0 &lt;= distance[i] &lt;= 10^4</code></li>
</ul>



题意：环形公交路线上有 `n` 个站，按次序从 `0` 到 `n - 1` 进行编号。已知每一对相邻公交站之间的距离，`distance[i]` 表示编号为 `i` 的车站和编号为 `(i + 1) % n` 的车站之间的距离。环线上的公交车都可以按顺时针和逆时针的方向行驶。返回乘客从出发点 `start` 到目的地 `destination` 之间的最短距离。 
