Given a directed graph, design an algorithm to find out whether there is a route between two nodes.

**Example1:**

```cpp
Input: n = 3, graph = [[0, 1], [0, 2], [1, 2], [1, 2]], start = 0, target = 2
Output: true
```

**Example2:**

```cpp
Input: n = 5, graph = [[0, 1], [0, 2], [0, 4], [0, 4], [0, 1], [1, 3], [1, 4], [1, 3], [2, 3], [3, 4]], start = 0, target = 4
Output true
```
**Note:**
-  `0 <= n <= 100000`
- All node numbers are within the range `[0, n]`.
- There might be self cycles and duplicated edges.

题意：给定一个有向图，设计一个算法，找出两个节点之间是否存在一条路径。

---
### 解法1 DFS
构建一个邻接表，然后从 `start` 开始深度优先搜索，如果能够到达 `target` ，就返回 `true` 。具体代码如下：
```cpp
class Solution { 
private:
    vector<vector<int>> g;
    vector<bool> vis;
    bool dfs(int s, int e) {
        if (vis[s]) return false;
        vis[s] = true; //访问这个顶点
        if (s == e) return true;
        for (const int &v : g[s]) if (dfs(v, e)) return true;
        return false;
    }
public:
    bool findWhetherExistsPath(int n, vector<vector<int>>& graph, int start, int target) {
        g.resize(n);
        vis.resize(n);
        for (const vector<int>& edge : graph) g[edge[0]].push_back(edge[1]);
        return dfs(start, target);
    }
};
```
运行效率如下：
```cpp
执行用时：228 ms, 在所有 C++ 提交中击败了99.45% 的用户
内存消耗：64.1 MB, 在所有 C++ 提交中击败了86.64% 的用户
```
---
### 解法2 BFS
从 `start` 开始广度优先搜索，如果能够到达 `target` ，就返回 `true` 。具体代码如下： 
```cpp
class Solution { 
private:
    vector<vector<int>> g;
    vector<bool> vis;
public:
    bool findWhetherExistsPath(int n, vector<vector<int>>& graph, int start, int target) {
        g.resize(n);
        vis.resize(n);
        for (const vector<int>& edge : graph) g[edge[0]].push_back(edge[1]);
        queue<int> q;
        q.push(start);
        vis[start] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == target) return true;
            for (const int &v : g[u]) {
                if (vis[v] == false) {
                    q.push(v);
                    vis[v] = true;
                }
            }
        }
        return false;
    }
};
```
运行效率如下：
```cpp
执行用时：252 ms, 在所有 C++ 提交中击败了98.76% 的用户
内存消耗：64.7 MB, 在所有 C++ 提交中击败了80.58% 的用户
```
