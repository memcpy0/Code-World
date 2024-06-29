

Given an **undirected graph**, return true if and only if it is **bipartite**.

Recall that `a graph is bipartite if we can split it's set of nodes into two independent subsets A and B such that every edge in the graph has one node in A and another node in B`.

The graph is given in the following form: `graph[i]` is a list of indexes `j` for which the edge between nodes `i` and `j` exists.  Each node is an integer between `0` and `graph.length - 1`.  There are no self edges or parallel edges: `graph[i]` **does not** contain `i`, and it **doesn't** contain any element twice.

Example 1:
```
Input: [[1,3], [0,2], [1,3], [0,2]]
Output: true
Explanation: 
The graph looks like this:
0----1
|    |
|    |
3----2
We can divide the vertices into two groups: {0, 2} and {1, 3}.
```
Example 2:
```
Input: [[1,2,3], [0,2], [0,1,3], [0,2]]
Output: false
Explanation: 
The graph looks like this:
0----1
| \  |
|  \ |
3----2
We cannot find a way to divide the set of nodes into two independent subsets.
```
Note:

-  `graph` will have length in range `[1, 100]`.
 -  `graph[i]` will contain integers in range `[0, graph.length - 1]`.
 -  `graph[i]` will not contain i or duplicate values.
 -  The graph is **undirected**: if any element `j` is in `graph[i]`, then `i` will be in `graph[j]`.

题意：判断一个图是否是二分图。给出的Graph是一个邻接表，数据范围在[1, 100]，无向无权图，没有自环边和重复边，是一个简单图。

思路：这是一个简单的二分图判断问题，也是二分图判断的模板题。解决方法在于染色，无论DFS还是BFS都可以。

解释：首先，**二分图是指我们可以将它的所有顶点分成两个独立子集，使得图中的每一条边的一个端点在顶点子集A，另一端点在顶点子集B中**。二分图常用于匹配问题。我们判断二分图的方法在于染色，图中每一条边的端点所属的阵营不同，我们就给两个不同的阵营染上不同的颜色，假设一种是黑色，另一种是白色。

从起点s开始，染上黑色，然后给这个点的所有邻接点v染上白色(这些边的两个端点都不同了)，然后再给所有邻接点v的邻接点染上白色，以此类推，最终图中顶点非黑即白，**每一条边**的两个端点颜色都不同。这样就是二分图。如果在染色的过程中，发现有一个邻接点(在同一条边上)已经访问过，但是边的两个端点颜色相同...说明发生了矛盾，不能正确的染色，因此不是二分图。

**搜索节点时，需要考虑图是非连通的情况**。二分图要求**图中的每个联通分量都是二分的**，如果有一个联通分量不是二分的，则整个图非二分图。**只有一个顶点时总是二分的**。

方法一：BFS。起点(第一层)染黑色，下一层染白色，每层与上一层和下一层的颜色都不同。

执行用时 :28 ms, 在所有 C++ 提交中击败了47.53% 的用户
内存消耗 :11.6 MB, 在所有 C++ 提交中击败了10.79%的用户
```cpp
class Solution {
public:
    int colors[110] = {0};  
    bool bfs(int s, int color, const vector<vector<int>>& graph) {
        queue<int> q;
        q.push(s);
        colors[s] = color; //黑色

        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (const auto w : graph[v]) {
                if (!colors[w]) {
                    q.push(w);
                    colors[w] = -colors[v];
                } else if (colors[w] == colors[v]) //已经访问过且颜色相同, 不可二分
                    return false;
            }
        }
        return true;
    } 
    bool isBipartite(vector<vector<int>>& graph) { 
        for (int i = 0; i < graph.size(); ++i)  
            if (!colors[i]) 
                if (!bfs(i, -1, graph)) return false; 
        return true;
    }
}
```

方法二：DFS。对每个未着色节点，从该节点开始深度优先搜索着色。每个邻接点都可以通过当前节点着相反的颜色。如果存在当前点和邻接点颜色相同，则着色失败。

执行用时 :32 ms, 在所有 C++ 提交中击败了29.88% 的用户
内存消耗 :11.3 MB, 在所有 C++ 提交中击败了25.73%的用户
```cpp
class Solution {
public:
    int colors[110] = {0};  //-1 黑色, 1 白色 非黑即白
    bool dfs(int v, int color, const vector<vector<int>>& graph) {
        colors[v] = color;
        for (const auto w : graph[v]) {
            if (!colors[w]) { //没有访问过这一点
                if (!dfs(w, -color, graph)) return false;
            } else if (colors[w] == color) { //访问过这一点, 但是颜色相同, 即这一条边的两端都在同一个子集中, 不可二分
                return false;
            }
        }
        return true;
    }
    bool isBipartite(vector<vector<int>>& graph) { 
        bool isBipart = true;
        for (int i = 0; i < graph.size(); ++i) {
            if (!colors[i]) {
                if (!dfs(i, -1, graph)) { //起点用黑色染色
                    isBipart = false;
                    break;
                }
            }
        }
        return isBipart;
    }
};
```
方法二的进化版：用栈模拟函数调用写DFS。通过始终保持一个栈，不进行函数调用，能够节省一些空间和时间。**BFS应该也能这样写**。

执行用时 :20 ms, 在所有 C++ 提交中击败了92.94% 的用户
内存消耗 :11.3 MB, 在所有 C++ 提交中击败了21.58%的用户
```cpp
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        vector<int> colors(graph.size(), 0);
        queue<int> q; //反复使用栈

        for (int i = 0; i < graph.size(); ++i) {
            if (colors[i] != 0) continue; //跳过已经染色的结点
            while (!q.empty()) q.pop();

            q.push(i);
            colors[i] = -1; //黑色
            while (!q.empty()) {
                int v = q.front(); q.pop();
                for (const auto w : graph[v]) {
                    if (!colors[w]) {
                        q.push(w);
                        colors[w] = -colors[v];
                    } else if (colors[w] == colors[v])
                        return false;
                }
            }
        }
        return true;
    }
}
```
