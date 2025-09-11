<div class="notranslate"><p>There are&nbsp;<code>n</code>&nbsp;computers numbered from&nbsp;<code>0</code>&nbsp;to&nbsp;<code>n-1</code>&nbsp;connected by&nbsp;ethernet cables&nbsp;<code>connections</code>&nbsp;forming a network where&nbsp;<code>connections[i] = [a, b]</code>&nbsp;represents a connection between computers&nbsp;<code>a</code>&nbsp;and&nbsp;<code>b</code>. Any computer&nbsp;can reach any other computer directly or indirectly through the network.</p>

<p>Given an initial computer network <code>connections</code>. You can extract certain cables between two directly connected computers, and place them between any pair of disconnected computers to make them directly connected. Return the <em>minimum number of times</em> you need to do this in order to make all the computers connected. If it's not possible, return -1.&nbsp;</p>

 
<p><strong>Example 1:</strong></p>

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201209004633319.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```swift
Input: n = 4, connections = [[0,1],[0,2],[1,2]]
Output: 1
Explanation: Remove cable between computer 1 and 2 and place between computers 1 and 3.
```

<p><strong>Example 2:</strong></p>

<p><strong><img style="width: 660px; height: 167px;" src="https://assets.leetcode.com/uploads/2020/01/02/sample_2_1677.png" alt=""></strong></p>

```swift
Input: n = 6, connections = [[0,1],[0,2],[0,3],[1,2],[1,3]]
Output: 2
```

<p><strong>Example 3:</strong></p>

```swift
Input: n = 6, connections = [[0,1],[0,2],[0,3],[1,2]]
Output: -1
Explanation: There are not enough cables.
```

<p><strong>Example 4:</strong></p>

```swift
Input: n = 6, connections = [[0,1],[0,2],[0,3],[1,2]]
Output: -1
Explanation: There are not enough cables.
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 10^5</code></li>
	<li><code>1 &lt;= connections.length &lt;= min(n*(n-1)/2, 10^5)</code></li>
	<li><code>connections[i].length == 2</code></li>
	<li><code>0 &lt;= connections[i][0], connections[i][1]&nbsp;&lt; n</code></li>
	<li><code>connections[i][0] != connections[i][1]</code></li>
	<li>There are no repeated connections.</li>
	<li>No two computers are connected by more than one cable.</li>
</ul>
</div>

题意：给出计算机网络的初始布线 `connections`，可以拔开任意两台直连计算机之间的线缆，并用它连接一对未直连的计算机。计算并返回使所有计算机都连通所需的最少操作次数。如果不可能，则返回 `-1` 。

---
### 解法 DFS
```cpp
class Solution {
private:
    void dfs(const vector<vector<int>>& graph, int u, vector<bool>& vis) {
        vis[u] = true;
        for (int i = 0; i < graph[u].size(); ++i) {
            int v = graph[u][i];
            if (!vis[v]) dfs(graph, v, vis);
        }
    }
public:
    int makeConnected(int n, vector<vector<int>>& connections) {
        int edges = connections.size(), components = 0;
        if (edges < n - 1) return -1; //边太少
        vector<vector<int>> graph(n);
        vector<bool> vis(n);
        for (int i = 0; i < edges; ++i) {
            graph[connections[i][0]].push_back(connections[i][1]);
            graph[connections[i][1]].push_back(connections[i][0]);
        }
        for (int i = 0; i < n; ++i) { //计算连通向量的个数
            if (!vis[i]) {
                dfs(graph, i, vis);
                ++components;
            }
        }
        return components - 1;
    }
};
```
运行效率如下：
```cpp
执行用时：312 ms, 在所有 C++ 提交中击败了36.76% 的用户
内存消耗：50.8 MB, 在所有 C++ 提交中击败了18.93% 的用户
```
