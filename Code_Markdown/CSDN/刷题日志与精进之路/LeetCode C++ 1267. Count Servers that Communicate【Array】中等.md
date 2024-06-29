<p>You are given a map of a server center, represented as a <code>m * n</code> integer matrix&nbsp;<code>grid</code>, where 1 means that on that cell there is a server and 0 means that it is no server. Two servers are said to communicate if they are on the same row or on the same column.<br>
<br>
Return the number of servers&nbsp;that communicate with any other server.</p>

 
<p><strong>Example 1:</strong></p>

<p><img style="width: 202px; height: 203px;" src="https://assets.leetcode.com/uploads/2019/11/14/untitled-diagram-6.jpg" alt=""></p>
 

```clike
Input: grid = [[1,0],[0,1]]
Output: 0
Explanation: No servers can communicate with others.
```

 
<p><strong>Example 2:</strong></p>

<p><strong><img style="width: 203px; height: 203px;" src="https://assets.leetcode.com/uploads/2019/11/13/untitled-diagram-4.jpg" alt=""></strong></p>

```clike
Input: grid = [[1,0],[1,1]]
Output: 3
Explanation: All three servers can communicate with at least one other server.
```

 
<p><strong>Example 3:</strong></p>

<p><img style="width: 443px; height: 443px;" src="https://assets.leetcode.com/uploads/2019/11/14/untitled-diagram-1-3.jpg" alt=""></p>

```clike
Input: grid = [[1,1,0,0],[0,0,1,0],[0,0,1,0],[0,0,0,1]]
Output: 4
Explanation: The two servers in the first row can communicate with each other. The two servers in the third column can communicate with each other. The server at right bottom corner can't communicate with any other server.
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>m == grid.length</code></li>
	<li><code>n == grid[i].length</code></li>
	<li><code>1 &lt;= m &lt;= 250</code></li>
	<li><code>1 &lt;= n &lt;= 250</code></li>
	<li><code>grid[i][j] == 0 or 1</code></li>
</ul>


题意：这里有一幅服务器分布图，服务器的位置标识在 `m * n` 的整数矩阵网格 `grid` 中，`1` 表示单元格上有服务器，`0` 表示没有。如果两台服务器位于同一行或者同一列，我们就认为它们之间可以进行通信。请你统计并返回能够与至少一台其他服务器进行通信的服务器的数量。 

---
### 解法1 暴力
对于每个矩阵元素，如果有服务器，就遍历同行或者同列，看有没有其他的服务器，如果有，能和其他服务器进行通信的服务器的数量就加一：
```cpp
class Solution {
public:
    int countServers(vector<vector<int>>& grid) {
        int ans = 0, m = grid.size(), n = grid[0].size(); 
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                bool flag = grid[i][j];
                for (int k = 0; flag && k < m; ++k) if (k != i && grid[k][j]) { flag = false; ++ans; }
                for (int k = 0; flag && k < n; ++k) if (k != j && grid[i][k]) { flag = false; ++ans; }
            }
        }
        return ans;
    }
};
```
$O(n^3)$ 解法，运行效率如下：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了63.90% 的用户
内存消耗：21.6 MB, 在所有 C++ 提交中击败了86.10% 的用户
```
---
### 解法2 存储每行/每列的元素和
我们可以存储每行和每列的元素和，然后双重循环遍历服务器，看同行或者同列的服务器数量是否 `> 1` ，是则该服务器必然能够和其他服务器通信：
```cpp
class Solution {
public:
    int countServers(vector<vector<int>>& grid) {
        int ans = 0, m = grid.size(), n = grid[0].size();
        vector<int> rowsum(m), colsum(n);
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j) 
                if (grid[i][j]) ++rowsum[i], ++colsum[j];
        for (int i = 0; i < m; ++i) 
            for (int j = 0; j < n; ++j) 
                //这个判断保证当前服务器的同行或同列必然有至少一个服务器和当前服务器连通
                if (grid[i][j] && (rowsum[i] > 1 || colsum[j] > 1)) ++ans;
        return ans;
    }
}; 
```
$O(n^2)$ 解法，运行效率如下：
```cpp 
执行用时：52 ms, 在所有 C++ 提交中击败了95.72% 的用户
内存消耗：21.7 MB, 在所有 C++ 提交中击败了80.21% 的用户
```
