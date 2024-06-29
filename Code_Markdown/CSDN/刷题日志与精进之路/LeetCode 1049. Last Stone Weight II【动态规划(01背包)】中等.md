<p>You are given an array of integers <code>stones</code> where <code>stones[i]</code> is the weight of the <code>i<sup>th</sup></code> stone.</p>

<p>We are playing a game with the stones. On each turn, we choose any two stones and smash them together. Suppose the stones have weights <code>x</code> and <code>y</code> with <code>x &lt;= y</code>. The result of this smash is:</p>

<ul>
	<li>If <code>x == y</code>, both stones are destroyed, and</li>
	<li>If <code>x != y</code>, the stone of weight <code>x</code> is destroyed, and the stone of weight <code>y</code> has new weight <code>y - x</code>.</li>
</ul>

<p>At the end of the game, there is <strong>at most one</strong> stone left.</p>

<p>Return <em>the smallest possible weight of the left stone</em>. If there are no stones left, return <code>0</code>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: stones = [2,7,4,1,8,1]
Output: 1
Explanation:
We can combine 2 and 4 to get 2, so the array converts to [2,7,1,8,1] then,
we can combine 7 and 8 to get 1, so the array converts to [2,1,1,1] then,
we can combine 2 and 1 to get 1, so the array converts to [1,1,1] then,
we can combine 1 and 1 to get 0, so the array converts to [1], then that's the optimal value. 
```

<p><strong>Example 2:</strong></p>

```cpp
Input: stones = [31,26,33,21,40]
Output: 5
```

<p><strong>Example 3:</strong></p>

```cpp
Input: stones = [1,2]
Output: 1
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= stones.length &lt;= 30</code></li>
	<li><code>1 &lt;= stones[i] &lt;= 100</code></li>
</ul>

题意：有一堆石头，用整数数组&nbsp;<code>stones</code> 表示。其中&nbsp;<code>stones[i]</code> 表示第 <code>i</code> 块石头的重量。每一回合，从中选出<strong>任意两块石头</strong>，然后将它们一起粉碎。假设石头的重量分别为&nbsp;<code>x</code> 和&nbsp;<code>y</code>，且&nbsp;<code>x &lt;= y</code>。那么粉碎的可能结果如下：</p>

<ul>
	<li>如果&nbsp;<code>x == y</code>，那么两块石头都会被完全粉碎；</li>
	<li>如果&nbsp;<code>x != y</code>，那么重量为&nbsp;<code>x</code>&nbsp;的石头将会完全粉碎，而重量为&nbsp;<code>y</code>&nbsp;的石头新重量为&nbsp;<code>y-x</code>。</li>
</ul>

<p>最后，<strong>最多只会剩下一块 </strong>石头。返回此石头 <strong>最小的可能重量 </strong>。如果没有石头剩下，就返回 <code>0</code>。</p>

---
### 解法 动态规划
这一题乍一看很难想到解法，即使看了标签「动态规划」，也不一定能有思路。不过仔细看了一下题目，发现**可对任意两颗石子进行操作，并放回石子重量的差值**——和一般的石子合并问题不同。

因此，想要得到最优解，操作顺序应该为：
$$[(s_a, s_b), (s_c, s_d), \dots, (s_i, s_j)]$$
其中 $a, b, ...$ 表示的是石子编号，这一顺序不代表石子重量，且编号间不具有大小关系。

**简单情况是不考虑放回操作**。解法是将石子分为两堆，开始都是空堆；而后每次将此时**重量较大**的石子放入第一堆，将**重量小于等于前者**的石子放入第二堆。最终的结果是，第一堆石子重量之和减去第二堆石子重量之和。于是，将第一堆命名为「**正号堆**」，其中的石子重量在最终运算中用**加号**计算；第二堆命名为「**负号堆**」，其中的石子重量在最终运算中**用负号**计算。举一个例子：
```cpp
stones = [1,3,7,9]
不放回：9和7抵消，3和1抵消，结果为(9-7)+(3-1)=9+3+(-7)+(-1)=4
```
> 其实**不放回的情况下**，贪心是最简单的，先从大到小排序，然后每次将最重和次重的石子抵消，抵消差值之和一定最小。**这一做法的正确性不难验证**。

进一步的，考虑放回操作。
