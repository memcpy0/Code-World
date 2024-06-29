

There are `n` cities numbered from `1` to `n`. You are given an array `edges` of size `n-1`, where `edges[i] = [ui, vi]` represents a bidirectional edge between cities `ui` and `vi`. There exists a unique path between each pair of cities. In other words, the cities form a **tree**.

A **subtree** is a subset of cities where every city is reachable from every other city in the subset, where the path between each pair passes through only the cities from the subset. Two subtrees are different if there is a city in one subtree that is not present in the other.

For each `d` from `1` to `n-1`, find the number of subtrees in which the **maximum distance** between any two cities in the subtree is equal to `d`.

Return _an array of size_ `n-1` _where the_ `dth` _element **(1-indexed)** is the number of subtrees in which the **maximum distance** between any two cities is equal to_ `d`.

**Notice** that the **distance** between the two cities is the number of edges in the path between them.

**Example 1:**

**![](https://assets.leetcode.com/uploads/2020/09/21/p1.png)**

```java
Input: n = 4, edges = [[1,2],[2,3],[2,4]]
Output: [3,4,0]
Explanation:
The subtrees with subsets {1,2}, {2,3} and {2,4} have a max distance of 1.
The subtrees with subsets {1,2,3}, {1,2,4}, {2,3,4} and {1,2,3,4} have a max distance of 2.
No subtree has two nodes where the max distance between them is 3.
```
**Example 2:**
```java
Input: n = 2, edges = [[1,2]]
Output: [1]
```
**Example 3:**
```java
Input: n = 3, edges = [[1,2],[2,3]]
Output: [2,1]
```
**Constraints:**
-   `2 <= n <= 15`
-   `edges.length == n-1`
-   `edges[i].length == 2`
-   `1 <= ui, vi <= n`
-   All pairs `(ui, vi)` are distinct.

题意：给你 `n` 个城市，编号为从 `1` 到 `n` 。给出一个大小为 `n-1` 的数组 `edges` ，其中 `edges[i] = [ui, vi]` 表示城市 `ui` 和 `vi` 之间有一条双向边。题目保证任意城市之间只有唯一的一条路径。即所有城市形成了一棵 **树** 。

一棵 **子树** 是城市集合的一个子集，且子集中任意城市都可以通过子集中的其他城市和边到达。两个子树被认为不一样的条件是至少有一个城市在其中一棵子树中存在，但在另一棵子树中不存在。

对于 `d` 从 `1` 到 `n-1` ，返回一个大小为 `n-1` 的数组，其中第 `d` 个元素（**下标从 1 开始**）是「子树中城市间 **最大距离** 」恰好等于 `d` 的子树数目。**注意**，两个城市间距离定义为它们之间需要经过的边的数目。

---
### 解法1 二进制枚举+树型DP
前置知识：二叉树的直径。见[543. 二叉树的直径](https://leetcode.cn/problems/diameter-of-binary-tree/)，从这题开始学习树形DP——简单说，就是枚举从 $x$ 往左儿子走的最长链和往右儿子走的最长链，这两条链可能会组成直径。==每个节点都需要返回「往左走的最长链长度」和「往右走的最长链长度」的最大值（实际上就是这个节点的深度）给父节点==，这样父节点才知道往这边走的最长链的长度是多少。枚举二叉树中所有节点作为 $x$ 就能找到答案。

前置知识：树的直径。[见1245. 树的直径]()。这题需要会员，可以做这些相似题目：
- [2246. 相邻字符不同的最长路径](https://leetcode.cn/problems/longest-path-with-different-adjacent-characters/)
- [2538. 最大价值和与最小价值和的差值](https://leetcode.cn/problems/difference-between-maximum-and-minimum-price-sum/)

树的直径可以用两次BFS/DFS或者树形DP，树型DP是在二叉树直径的基础上略作修改。

思路：本题结合了78题和1245题，求这棵子树的直径。需要注意的是，二进制枚举时子集不一定是一棵树，可能是森林（多棵树，多个连通块），==在计算树形DP的同时去统计访问过的点，看看是否与子集相等，只有相等才是一棵树==。
```cpp
class Solution {
public:
    vector<int> countSubgraphsForEachDiameter(int n, vector<vector<int>>& edges) {
        vector<vector<int>> g(n);
        for (auto &e : edges) {
            int x = e[0] - 1, y = e[1] - 1; // 编号改为从0开始
            g[x].push_back(y);
            g[y].push_back(x);
        }
        vector<int> ans(n - 1);
        // 二进制枚举
        int tot = 1 << n;
        for (int mask = 3; mask < tot; ++mask) { // 至少需要两个点
            if ((mask & (mask - 1)) == 0) continue; // 少于两个点被选择,剪去一部分集合
            // 求树的直径
            int vis = 0, diameter = 0;
            function<int(int)> dfs = [&](int x) -> int {
                vis |= 1 << x; // 标记x被访问过
                int dx = 0;
                for (int y : g[x]) {
                    if ((vis >> y & 1) == 0 && mask >> y & 1) { // y没有访问且在mask集合中
                        int dy = dfs(y) + 1;
                        diameter = max(diameter, dy + dx);
                        dx = max(dx, dy);
                    }
                }
                return dx;
            };
            dfs(__builtin_ctz(mask)); // 从一个在mask中的点开始递归
            if (vis == mask)
                ++ans[diameter - 1];
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n 2^n)$ 。枚举子集 $O(2^n)$ ，求直径 $O(n)$  
- 空间复杂度：$O(n)$

---
### 解法2 n次DFS+枚举直径端点+乘法原理
前置知识：树的深度。前置知识：乘法原理。**暴力枚举 $i$ 和 $j$ 作为直径的两个端点 ，那么从 $i$ 到 $j$ 的这条简单路径是直径，这上面的每个点都必须选**。还有哪些点是可以选的？（下面是大佬画的图，只能说大佬太强了）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303130012348.png)
注意：上面的 $x$ 要么选择，然后计算每棵子树方案数的乘积；**要么不选，则不能连接到后面的子树，就只让方案数+1**。且**一开始只选择 $x$ ，也会让方案数+1**。

为了计算树上任意两点的距离 $\textit{dis}$ ，枚举 $i$ 作为树的根，计算 $i$ 到其余点的距离。这通常用BFS来做，但是==对于树来说，任意两点的简单路径是唯一的，所以DFS也可以==。那么通过 $n$ 次DFS，就可以得到树上任意两点的距离了。
> 问：还有类似的「需要避免重复统计」的题目吗？
> 答：例如[15. 三数之和](https://leetcode.cn/problems/3sum/)、[90. 子集 II]([90. 子集 II](https://leetcode.cn/problems/subsets-ii/)等。

```cpp
// n次DFS/BFS+乘法原理
class Solution {
public:
    vector<int> countSubgraphsForEachDiameter(int n, vector<vector<int>>& edges) {
        vector<vector<int>> g(n);
        for (auto &e : edges) {
            int x = e[0] - 1, y = e[1] - 1; // 编号改为从0开始
            g[x].push_back(y);
            g[y].push_back(x);
        }
        int dis[n][n]; memset(dis, 0, sizeof(dis)); // d[i][j]表示树中i到j的距离
        function<void(int, int, int)> dfs = [&](int i, int x, int fa) { // DFS遍历树的写法
            for (int y : g[x]) {
                if (y != fa) {
                    dis[i][y] = dis[i][x] + 1; // 自顶向下
                    dfs(i, y, x); 
                }
            }
        };
        for (int i = 0; i < n; ++i) // 原树是连通的
            dfs(i, i, -1); // 计算i到其余点的距离
        
        function<int(int, int, int, int, int)> dfs2 = [&](int i, int j, int d, int x, int fa) { // 计算以i,j为直径,d为直径长度,fa为x父节点时,x节点及其子树的选择方案数
            // 能递归到这,说明x可选
            int cnt = 1; // 选x但还没选子树
            for (int y : g[x]) {
                if (y != fa &&
                    (dis[i][y] < d || dis[i][y] == d && y > j) &&
                    (dis[j][y] < d || dis[j][y] == d && y > i)
                ) // 满足这些条件就可以选
                    cnt *= dfs2(i, j, d, y, x); // 每棵子树互相独立,乘法原理
            }
            if (dis[i][x] + dis[j][x] > d) // x是可选点,排除了dis[i][x]+dis[j][x]==d的必选点
                ++cnt; // 不选x
            return cnt;
        };

        vector<int> ans(n - 1);
        for (int i = 0; i < n; ++i) // 枚举直径端点
            for (int j = i + 1; j < n; ++j) 
                ans[dis[i][j] - 1] += dfs2(i, j, dis[i][j], i, -1);
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^3)$ 。$O(n^2)$ 计算各点之间的距离、枚举直径端点，$O(n)$ 计算方案数，所以时间复杂度为 $O(n^3)$ 。
- 空间复杂度：$O(n^2)$ 。