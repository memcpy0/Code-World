Given a directed acyclic graph of `N` nodes. Find all possible paths from node 0 to node `N-1` , and return them in any order.

The graph is given as follows:  the nodes are `0, 1, ..., graph.length - 1` .  `graph[i]` is a list of all nodes `j` for which the edge `(i, j)` exists.

 

**Example 1:**
<img src="https://img-blog.csdnimg.cn/20201002125858529.png#pic_center" width="20%">

```swift
Input: graph = [[1,2],[3],[3],[]]
Output: [[0,1,3],[0,2,3]]
Explanation: There are two paths: 0 -> 1 -> 3 and 0 -> 2 -> 3.
```

**Example 2:**
<img src="https://img-blog.csdnimg.cn/2020100212591166.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="23%">

```swift
Input: graph = [[4,3,1],[3,2,4],[3],[4],[]]
Output: [[0,4],[0,3,4],[0,1,3,4],[0,1,2,3,4],[0,1,4]]
```

**Example 3:**

```swift
Input: graph = [[1],[]]
Output: [[0,1]]
```

**Example 4:**

```swift
Input: graph = [[1,2,3],[2],[3],[]]
Output: [[0,1,2,3],[0,2,3],[0,3]]
```

**Example 5:**

```swift
Input: graph = [[1,3],[2],[3],[]]
Output: [[0,1,2,3],[0,3]]
```

 

**Constraints:**
-  The number of nodes in the graph will be in the range `[2, 15]` .
- You can print different paths in any order, but you should keep the order of nodes inside one path.
 

题意：给出一个有 `n` 个结点的有向无环图，找出所有从 `0` 到 `n - 1` 的路径。

---
### 思路
简单的DFS，加上回溯即可：
```cpp
class Solution {
public:
    vector<vector<int>> ans;
    vector<int> temp;
    vector<bool> vis;
    void dfs(const vector<vector<int>>& graph, int u, int e) {
        vis[u] = true;
        temp.push_back(u);
        if (u == e) {
            ans.push_back(temp);
            vis[u] = false; //回溯
            temp.pop_back();
            return;
        }
        for (const int &v : graph[u]) 
            if (!vis[v]) 
                dfs(graph, v, e);
        temp.pop_back(); //回溯
        vis[u] = false;
    }
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        if (graph.empty() || graph[0].empty()) return ans;
        vis.resize(graph.size(), false);
        dfs(graph, 0, graph.size() - 1); //0~n-1
        return ans;
    }
};
```
提交结果如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了79.38% 的用户
内存消耗：12 MB, 在所有 C++ 提交中击败了58.82% 的用户
```
