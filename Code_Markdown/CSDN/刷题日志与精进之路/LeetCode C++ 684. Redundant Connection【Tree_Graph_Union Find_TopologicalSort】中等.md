In this problem, a tree is an **undirected** graph that is connected and has no cycles.

The given input is a graph that started as a tree with `N` nodes (with distinct values `1, 2, ..., N` ), with one additional edge added. The added edge has two different vertices chosen from `1` to `N` , and was not an edge that already existed.

The resulting graph is given as a 2D-array of `edges` . Each element of edges is a pair `[u, v]` with `u < v` , that represents an undirected edge connecting nodes `u` and `v` .

Return an edge that can be removed so that the resulting graph is a tree of `N` nodes. If there are multiple answers, return the answer that occurs last in the given 2D-array. The answer edge `[u, v]` should be in the same format, with `u < v` .

**Example 1:**
```cpp
Input: [[1,2], [1,3], [2,3]]
Output: [2,3]
Explanation: The given undirected graph will be like this:
  1
 / \
2 - 3
```

**Example 2:**

```cpp
Input: [[1,2], [2,3], [3,4], [1,4], [1,5]]
Output: [1,4]
Explanation: The given undirected graph will be like this:
5 - 1 - 2
    |   |
    4 - 3
```
**Note:**
- The size of the input 2D-array will be between `3` and `1000` .
- Every integer represented in the 2D-array will be between `1` and `N` , where `N` is the size of the input array.


<font color="red">**Update (2017-09-26):**</font>
We have overhauled the problem description + test cases and specified clearly the graph is an ***undirected*** graph. For the ***directed*** graph follow up please see `Redundant Connection II` ). We apologize for any inconvenience caused. 

题意：本题中，树指的是一个连通且无环的无向图。输入一个图，该图由一个有着 `N` 个节点 (节点值不重复 `1, 2, ..., N` ) 的树及**一条附加的边**构成。附加的边的两个顶点包含在 `1` 到 `N` 中间，这条附加的边不属于树中已存在的边。

输入的图是一个以边组成的二维数组。每一个边的元素是一对 `[u, v]` ，满足 `u < v` ，表示连接顶点 `u` 和 `v` 的无向图的边。**返回一条可以删去的边**，使得结果图是一个有着 `N` 个节点的树。如果有多个答案，则**返回二维数组中最后出现的边**。答案边 `[u, v]` 应满足相同的格式 `u < v` 。

以示例一为例，无向图是一个有着 `3` 个节点的图，我们可以返回 `[1,2], [1,3], [2,3]` 中的任何一条边，把它删去就得到一个 `3` 个节点的树。最后我们返回的是二维数组中最后出现的边 `[2,3]` 。 
 

 
---
思路1：总结过图中环的判断和输出，就知道这道题有很多种做法。最简单的就是并查集，在最小生成树的 `Kruskal` 算法中，就是用它判断环的存在。思路是：**如果一个边的两个顶点不属于同一个集合，就合并；否则两个顶点属于一个已经连通的集合，意味着形成了一个环**。如果只是任意输出一条要删除的边，则输出环上的任何一条边都可以；但是这里要输出最后出现的边，换句话说，就是使得环成形的最后一条边。代码如下：
```cpp
class Solution {
public: 
    vector<int> father;
    int find(int x) {
        return father[x] < 0 ? x : father[x] = find(father[x]);
    }
    void merge(int x, int y) {
        if (father[x] < father[y]) {
            father[x] += father[y];
            father[y] = x;
        } else {
            father[y] += father[x];
            father[x] = y;
        }
    }
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        father.resize(edges.size() + 1, -1); 
        for (int i = 0; i < edges.size(); ++i) {
            int r1 = find(edges[i][0]), r2 = find(edges[i][1]);
            if (r1 == r2) return edges[i]; //如果这条边的两个端点都在一个联通集合中,说明存在环
            else merge(r1, r2);
        }
        return vector<int>();
    }
};
```
---
思路2：用拓扑排序判断这个无向图中环的存在。具体做法见代码：
```cpp
class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        //使用拓扑排序
        const int N = edges.size();
        vector<int> degrees(N + 1, 0); //所有顶点的度默认为0
        vector<vector<int>> adjlist(N + 1); //邻接表
        for (int i = 0; i < N; ++i) {
            int u = edges[i][0], v = edges[i][1];
            adjlist[u].push_back(v);   //建立邻接表
            adjlist[v].push_back(u);
            ++degrees[u];
            ++degrees[v];
        }
        queue<int> q; //存储所有度为1的顶点
        //无向图中度为1,实质上是有向图中入度为0出度为1的顶点
        for (int i = 1; i <= N; ++i)
            if (degrees[i] == 1) q.push(i);
        while (!q.empty()) { 
            int u = q.front(); q.pop();
            for (int j = 0; j < adjlist[u].size(); ++j) {
                int v = adjlist[u][j];
                --degrees[v];
                if (degrees[v] == 1) q.push(v);
            } 
        }
        //从后往前判断,如果不存在环,拓扑排序后所有点的度数都是0
        //如果一对边的度有大于1的,那说明,这对边有问题,需要去掉
        for (int i = N - 1; i >= 0; --i) 
            if (min(degrees[edges[i][0]], degrees[edges[i][1]]) > 1) 
                return edges[i];
        return vector<int>();
    } 
};
```
