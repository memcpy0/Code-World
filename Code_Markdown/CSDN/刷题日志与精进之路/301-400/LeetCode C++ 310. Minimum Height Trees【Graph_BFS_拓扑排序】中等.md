A tree is an undirected graph in which any two vertices are connected by exactly one path. In other words, any connected graph without simple cycles is a tree.

Given a tree of `n` nodes labelled from `0` to `n - 1`, and an array of `n - 1` `edges` where `edges[i] = [ai, bi]` indicates that there is an undirected edge between the two nodes `ai` and `bi` in the tree, you can choose any node of the tree as the root. When you select a node `x` as the root, the result tree has height `h`. Among all possible rooted trees, those with minimum height (i.e. `min(h)`)  are called **minimum height trees** (MHTs).

Return *a list of all **MHTs'** root labels.* You can return the answer in **any order**.

The **height** of a rooted tree is the number of edges on the longest downward path between the root and a leaf. 

**Example 1:**
<img src="https://img-blog.csdnimg.cn/2020110219144382.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="55%">
```swift
Input: n = 4, edges = [[1,0],[1,2],[1,3]]
Output: [1]
Explanation: As shown, the height of the tree is 1 when the root is the node with label 1 which is the only MHT.
```

**Example 2:**
<img src="https://img-blog.csdnimg.cn/20201102191452202.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="55%">

```swift
Input: n = 6, edges = [[3,0],[3,1],[3,2],[3,4],[5,4]]
Output: [3,4]
```

**Example 3:**

```swift
Input: n = 1, edges = []
Output: [0]
```

**Example 4:**

```swift
Input: n = 2, edges = [[0,1]]
Output: [0,1]
```

 

**Constraints:**
- <code>1 <= n <= 2 * 10<sup>4</sup></code>
-  `edges.length == n - 1` 
-  `0 <= ai, bi < n`
 -  `ai != bi`
 - All the pairs `(ai, bi)` are distinct.
 - The given input is **guaranteed** to be a **tree** and there will be **no repeated** edges.


题意：对于一个具有树特征的无向图，我们可选择任何一个节点作为根，图因此成为树。在所有可能的树中，具有最小高度的树被称为**最小高度树**。给出这样的一个图，写出一个函数找到所有的最小高度树并返回他们的根节点。

---
### 解法1 对每个顶点BFS
BFS如下，会超时：
```cpp
class Solution {
private:
    vector<vector<int>> adjList;
    int n, minHeight = 0x3fffffff;
    int bfs(int root) {
        queue<int> q;
        q.push(root);
        bitset<20010> vis;
        int curHeight = -1;
        while (!q.empty()) {
            int size = q.size();
            ++curHeight; 
            if (curHeight > minHeight) return curHeight; //剪枝
            for (int i = 0; i < size; ++i) {
                int u = q.front(); q.pop();  
                vis[u] = 1; 
                for (int v : adjList[u])
                    if (!vis[v]) q.push(v);
            }
        }
        minHeight = min(minHeight, curHeight);
        return curHeight;
    }
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        n = edges.size() + 1; //顶点数
        adjList.resize(n);
        for (int i = 0; i < n - 1; ++i) {
            adjList[edges[i][0]].push_back(edges[i][1]);
            adjList[edges[i][1]].push_back(edges[i][0]);
        } 
        vector<int> heightOfEveryVertex;
        for (int i = 0; i < n; ++i)  //对每个顶点BFS 
            heightOfEveryVertex.push_back(bfs(i));  
        vector<int> ans; //具有最小高度树的顶点数组
        for (int i = 0; i < n; ++i)
            if (heightOfEveryVertex[i] == minHeight) ans.push_back(i);
        return ans;
    }
};
```
使用DFS同样会超时。

---
### 解法2 规律
构建邻接表作为图，而后找出叶子节点（度为 $1$ 的节点），不断删除度为1的结点，直到图中节点只剩下 $1$ 个或 $2$ 个，返回剩下的节点。

为什么这种做法是正确的？首先让我们想一想，一张无向无环图中最多有多少个MHT？或者换种问法，一棵树中最多有几个节点能够作为MHT的根节点？答案是 $1$ 个或者 $2$ 个。

如何得到这1个或者2个根节点？我们的做法是，一圈圈地剔除外部节点，直到只剩下一个或者两个节点。这一过程的体现就是**不断删除度为 $1$ 的节点**，就像给一颗洋葱剥皮一样。证明如下：
- 无向无环图 $A$ 在删除度为 $1$ 的节点后，得到无向无环图 $B$ 。可知图 $A$ 构成的最小高度树 $T_A$ ，是在图 $B$ 构成的最小高度树 $T_B$ 之上，接上被删除节点构成的；
- 反证如下：图 $B$ 构成的最小高度树 $T_B$ 如果不是最小高度树，则存在图 $B$ 的最小高度树 $T_B'$ ，$T_B'$ 的高度小于 $T_B$ 。那么 $T_B$ 接上被删除节点后，高度比 $T_A$ 更小，这与 $T_A$ 是最小高度树矛盾。
- 既然图 $A$ 的最小高度树 $T_A$ = 图 $B$ 的最小高度树 $T_B$ + 被删除节点，那么图 $A$ 的最小高度树的根节点与图 $B$ 的最小高度树的根节点**相同**。因为，接上的被删除的节点不能作为根节点，否则比其不作为根节点时高度要高 $1$ 。因此，**寻找图 $A$ 的最小高度树的根节点**，可以变换为**寻找图 $B$ 的最小高度树的根节点。**
- 这一过程，一直进行到只剩下一个节点（**它就是自身的MHT的根节点**），此时该节点就是我们要寻找的、整张图的MHT的根节点；或者剩下两个节点，这两个节点都是对应的MHT的根节点。

具体代码如下：
```cpp
class Solution { 
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        if (edges.empty()) return {0}; //边界情况
        vector<vector<int>> adjList(n);
        vector<int> degrees(n, 0), ans;
        for (int i = 0; i < n - 1; ++i) {
            int u = edges[i][0], v = edges[i][1];
            adjList[u].push_back(v);
            adjList[v].push_back(u);
            ++degrees[u];
            ++degrees[v];
        }  
        for (int i = 0; i < n; ++i) if (degrees[i] == 1) ans.push_back(i);
        while (n > 2) {
            int size = ans.size();
            n -= size; //减去度为1的节点数目
            vector<int> temp;
            for (int i = 0; i < size; ++i) { //每次剥离一层度为1的节点
                int u = ans.back(); ans.pop_back();
                for (int j = 0; j < adjList[u].size(); ++j) { //减去仍然存在的节点的度数
                    int v = adjList[u][j];
                    if (degrees[v] == 1) continue;
                    if (--degrees[v] == 1) temp.push_back(v); 
                }
            }   
            ans = temp;
        }
        return ans;
    }
};
```
实际执行效率如下：
```cpp
执行用时：140 ms, 在所有 C++ 提交中击败了79.95% 的用户
内存消耗：27.4 MB, 在所有 C++ 提交中击败了43.32% 的用户
```
