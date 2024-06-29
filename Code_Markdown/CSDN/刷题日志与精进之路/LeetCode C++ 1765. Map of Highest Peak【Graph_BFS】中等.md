<p>You are given an integer matrix <code>isWater</code> of size <code>m x n</code> that represents a map of <strong>land</strong> and <strong>water</strong> cells.</p>

<ul>
	<li>If <code>isWater[i][j] == 0</code>, cell <code>(i, j)</code> is a <strong>land</strong> cell.</li>
	<li>If <code>isWater[i][j] == 1</code>, cell <code>(i, j)</code> is a <strong>water</strong> cell.</li>
</ul>

<p>You must assign each cell a height in a way that follows these rules:</p>

<ul>
	<li>The height of each cell must be non-negative.</li>
	<li>If the cell is a <strong>water</strong> cell, its height must be <code>0</code>.</li>
	<li>Any two adjacent cells must have an absolute height difference of <strong>at most</strong> <code>1</code>. A cell is adjacent to another cell if the former is directly north, east, south, or west of the latter (i.e., their sides are touching).</li>
</ul>

<p>Find an assignment of heights such that the maximum height in the matrix is <strong>maximized</strong>.</p>

<p>Return <em>an integer matrix </em><code>height</code><em> of size </em><code>m x n</code><em> where </em><code>height[i][j]</code><em> is cell </em><code>(i, j)</code><em>'s height. If there are multiple solutions, return <strong>any</strong> of them</em>.</p>

 
<p><strong>Example 1:</strong></p>

<p><strong><img style="width: 220px; height: 219px;" src="https://assets.leetcode.com/uploads/2021/01/10/screenshot-2021-01-11-at-82045-am.png" alt=""></strong></p>

```cpp
Input: isWater = [[0,1],[0,0]]
Output: [[1,0],[2,1]]
Explanation: The image shows the assigned heights of each cell.
The blue cell is the water cell, and the green cells are the land cells. 
```

<p><strong>Example 2:</strong></p>

<p><strong><img style="width: 300px; height: 296px;" src="https://assets.leetcode.com/uploads/2021/01/10/screenshot-2021-01-11-at-82050-am.png" alt=""></strong></p>

```cpp
Input: isWater = [[0,0,1],[1,0,0],[0,0,0]]
Output: [[1,1,0],[0,1,1],[1,2,2]]
Explanation: A height of 2 is the maximum possible height of any assignment.
Any height assignment that has a maximum height of 2 while still meeting the rules will also be accepted. 
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>m == isWater.length</code></li>
	<li><code>n == isWater[i].length</code></li>
	<li><code>1 &lt;= m, n &lt;= 1000</code></li>
	<li><code>isWater[i][j]</code> is <code>0</code> or <code>1</code>.</li>
	<li>There is at least <strong>one</strong> water cell.</li>
</ul>

题意：给你一个大小为&nbsp;<code>m x n</code>&nbsp;的整数矩阵&nbsp;<code>isWater</code>&nbsp;，它代表了一个由 <strong>陆地</strong>&nbsp;和 <strong>水域</strong>&nbsp;单元格组成的地图。

<ul>
	<li>如果&nbsp;<code>isWater[i][j] == 0</code>&nbsp;，格子&nbsp;<code>(i, j)</code>&nbsp;是一个 <strong>陆地</strong>&nbsp;格子。</li>
	<li>如果&nbsp;<code>isWater[i][j] == 1</code>&nbsp;，格子&nbsp;<code>(i, j)</code>&nbsp;是一个 <strong>水域</strong>&nbsp;格子。</li>
</ul>

<p>需要按照如下规则给每个单元格安排高度：</p>

<ul>
	<li>每个格子的高度都必须是非负的。</li>
	<li>如果一个格子是 <strong>水域</strong>&nbsp;，那么它的高度必须为 <code>0</code>&nbsp;。</li>
	<li>任意相邻的格子高度差 <strong>至多</strong>&nbsp;为 <code>1</code>&nbsp;。当两个格子在正东、南、西、北方向上相互紧挨着，就称它们为相邻的格子。（也就是说它们有一条公共边）</li>
</ul>

<p>找到一种安排高度的方案，使得矩阵中的最高高度值&nbsp;<strong>最大</strong>&nbsp;。即返回一个大小为&nbsp;<code>m x n</code>&nbsp;的整数矩阵 <code>height</code>&nbsp;，其中 <code>height[i][j]</code>&nbsp;是格子 <code>(i, j)</code>&nbsp;的高度。如果有多种解法，请返回&nbsp;<strong>任意一个</strong>&nbsp;。</p>

---
### 解法 多源BFS
容易想到，每个格子的高度就是离其最近的水域的距离。具体做法是将所有水域的位置加入同一个队列中，然后统一BFS。而非分别对每个水域进行BFS，再取格子的最小值，这样太慢了：
```cpp
class Solution {
public:
    vector<vector<int>> highestPeak(vector<vector<int>>& isWater) {
        queue<int> q;
        int m = isWater.size(), n = isWater[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (isWater[i][j] == 1) { isWater[i][j] = 0; q.push(i * 10000 + j); }
                else isWater[i][j] = -1;
            }
        }
        int Move[][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        while (!q.empty()) {
            int x = q.front() / 10000, y = q.front() % 10000; q.pop();
            for (int i = 0; i < 4; ++i) {
                int tx = x + Move[i][0], ty = y + Move[i][1];
                if (tx >= 0 && tx < m && ty >= 0 && ty < n && isWater[tx][ty] == -1) {
                    q.push(tx * 10000 + ty); 
                    isWater[tx][ty] = isWater[x][y] + 1;
                }
            }
        }
        return isWater;
    }
};
```
运行效率如下：
```cpp
执行用时：360 ms, 在所有 C++ 提交中击败了75.65% 的用户
内存消耗：98.3 MB, 在所有 C++ 提交中击败了97.17% 的用户
```
