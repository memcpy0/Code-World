<p>There are <code>n</code> cities numbered from <code>1</code> to <code>n</code>. You are given an array <code>edges</code> of size <code>n-1</code>, where <code>edges[i] = [u<sub>i</sub>, v<sub>i</sub>]</code> represents a bidirectional edge between cities <code>u<sub>i</sub></code> and <code>v<sub>i</sub></code>. There exists a unique path between each pair of cities. In other words, the cities form a <strong>tree</strong>.</p>

<p>A <strong>subtree</strong> is a subset of cities where every city is reachable from every other city in the subset, where the path between each pair passes through only the cities from the subset. Two subtrees are different if there is a city in one subtree that is not present in the other.</p>

<p>For each <code>d</code> from <code>1</code> to <code>n-1</code>, find the number of subtrees in which the <strong>maximum distance</strong> between any two cities in the subtree is equal to <code>d</code>.</p>

<p>Return <em>an array of size</em> <code>n-1</code> <em>where the </em><code>d<sup>th</sup></code><em> </em><em>element <strong>(1-indexed)</strong> is the number of subtrees in which the <strong>maximum distance</strong> between any two cities is equal to </em><code>d</code>.</p>

<p><strong>Notice</strong>&nbsp;that&nbsp;the <strong>distance</strong> between the two cities is the number of edges in the path between them.</p>

 
<p><strong>Example 1:</strong></p>

<p><strong><img style="width: 161px; height: 181px;" src="https://assets.leetcode.com/uploads/2020/09/21/p1.png" alt=""></strong></p>

```cpp
Input: n = 4, edges = [[1,2],[2,3],[2,4]]
Output: [3,4,0]
Explanation:
The subtrees with subsets {1,2}, {2,3} and {2,4} have a max distance of 1.
The subtrees with subsets {1,2,3}, {1,2,4}, {2,3,4} and {1,2,3,4} have a max distance of 2.
No subtree has two nodes where the max distance between them is 3.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: n = 2, edges = [[1,2]]
Output: [1]
```


<p><strong>Example 3:</strong></p>

```cpp
Input: n = 3, edges = [[1,2],[2,3]]
Output: [2,1]
```


<p><strong>Constraints:</strong></p>

<ul>
	<li><code>2 &lt;= n &lt;= 15</code></li>
	<li><code>edges.length == n-1</code></li>
	<li><code>edges[i].length == 2</code></li>
	<li><code>1 &lt;= u<sub>i</sub>, v<sub>i</sub> &lt;= n</code></li>
	<li>All pairs <code>(u<sub>i</sub>, v<sub>i</sub>)</code> are distinct.</li>
</ul>

题意：给出&nbsp;<code>n</code>&nbsp;个城市，编号为从&nbsp;<code>1</code> 到&nbsp;<code>n</code>&nbsp;。同时给出一个大小为&nbsp;<code>n-1</code>&nbsp;的数组&nbsp;<code>edges</code>&nbsp;，其中&nbsp;<code>edges[i] = [u<sub>i</sub>, v<sub>i</sub>]</code>&nbsp;表示城市&nbsp;<code>u<sub>i</sub></code>&nbsp;和&nbsp;<code>v<sub>i</sub></code><sub>&nbsp;</sub>之间有一条双向边。题目保证任意城市之间只有唯一的一条路径，&nbsp;<code>(u<sub>i</sub>, v<sub>i</sub>)</code>&nbsp;所表示的边互不相同。换句话说，所有城市形成了一棵&nbsp;<strong>树</strong>&nbsp;。

<p>一棵&nbsp;<strong>子树</strong>&nbsp;是城市的一个子集，且子集中任意城市之间可以通过子集中的其他城市和边到达。两个子树被认为不一样的条件是至少有一个城市在其中一棵子树中存在，但在另一棵子树中不存在。</p>

<p>对于&nbsp;<code>d</code>&nbsp;从&nbsp;<code>1</code> 到&nbsp;<code>n-1</code>&nbsp;，请你找到城市间&nbsp;<strong>最大距离</strong>&nbsp;恰好为 <code>d</code>&nbsp;的所有子树数目。返回一个大小为&nbsp;<code>n-1</code>&nbsp;的数组，其中第<em>&nbsp;</em><code>d</code><em>&nbsp;</em>个元素（<strong>下标从 1 开始</strong>）是城市间 <strong>最大距离</strong> 恰好等于&nbsp;<code>d</code>&nbsp;的子树数目。<strong>请注意</strong>，两个城市间距离定义为它们之间需要经过的边的数目。</p>

 
 
 Iterate through every possible subtree by doing a bitmask on which vertices to include. How can you determine if a subtree is valid (all vertices are connected)?

To determine connectivity, count the number of reachable vertices starting from any included vertex and only traveling on edges connecting 2 vertices in the subtree. The count should be the same as the number of 1s in the bitmask.


The diameter is basically the maximum distance between any two nodes. Root the tree at a vertex. The answer is the max of the heights of the two largest subtrees or the longest diameter in any of the subtrees.
