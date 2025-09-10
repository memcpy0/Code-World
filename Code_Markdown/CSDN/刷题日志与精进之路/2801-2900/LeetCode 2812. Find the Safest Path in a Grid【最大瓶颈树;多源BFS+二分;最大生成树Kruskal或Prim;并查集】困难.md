> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始、大小为 `n x n` 的二维矩阵 `grid` ，其中 `(r, c)` 表示：
- 如果 `grid[r][c] = 1` ，则表示一个存在小偷的单元格
- 如果 `grid[r][c] = 0` ，则表示一个空单元格

你最开始位于单元格 `(0, 0)` 。在一步移动中，你可以移动到矩阵中的任一相邻单元格，包括存在小偷的单元格。

矩阵中路径的 **安全系数** 定义为：从路径中任一单元格到矩阵中任一小偷所在单元格的 **最小** 曼哈顿距离。

返回所有通向单元格 `(n - 1, n - 1)` 的路径中的 **最大安全系数** 。

单元格 `(r, c)` 的某个 **相邻** 单元格，是指在矩阵中存在的 `(r, c + 1)`、`(r, c - 1)`、`(r + 1, c)` 和 `(r - 1, c)` 之一。

两个单元格 `(a, b)` 和 `(x, y)` 之间的 **曼哈顿距离** 等于 `| a - x | + | b - y |` ，其中 `|val|` 表示 `val` 的绝对值。

**示例 1：**
![](https://assets.leetcode.com/uploads/2023/07/02/example1.png)
```java
输入：grid = [[1,0,0],[0,0,0],[0,0,1]]
输出：0
解释：从 (0, 0) 到 (n - 1, n - 1) 的每条路径都经过存在小偷的单元格 (0, 0) 和 (n - 1, n - 1) 。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2023/07/02/example2.png)
```java
输入：grid = [[0,0,1],[0,0,0],[0,0,0]]
输出：2
解释：
上图所示路径的安全系数为 2：
- 该路径上距离小偷所在单元格（0，2）最近的单元格是（0，0）。它们之间的曼哈顿距离为 | 0 - 0 | + | 0 - 2 | = 2 。
可以证明，不存在安全系数更高的其他路径。
```
**示例 3：**
![](https://assets.leetcode.com/uploads/2023/07/02/example3.png)
```java
输入：grid = [[0,0,0,1],[0,0,0,0],[0,0,0,0],[1,0,0,0]]
输出：2
解释：
上图所示路径的安全系数为 2：
- 该路径上距离小偷所在单元格（0，3）最近的单元格是（1，2）。它们之间的曼哈顿距离为 | 0 - 1 | + | 3 - 2 | = 2 。
- 该路径上距离小偷所在单元格（3，0）最近的单元格是（3，2）。它们之间的曼哈顿距离为 | 3 - 3 | + | 0 - 2 | = 2 。
可以证明，不存在安全系数更高的其他路径。
```
**提示：**
- `1 <= grid.length == n <= 400`
- `grid[i].length == n`
- `grid[i][j]` 为 `0` 或 `1`
- `grid` 至少存在一个小偷

---
同类题目：
- [LeetCode 2812. Find the Safest Path in a Grid](https://leetcode.cn/problems/find-the-safest-path-in-a-grid/)
- [LeetCode 778. Swim in Rising Water](https://leetcode.cn/problems/swim-in-rising-water)
- [LeetCode 1631. 最小体力消耗路径](https://leetcode.cn/problems/path-with-minimum-effort)

这几道题，虽然物理问题不同，但是背后的数学模型几乎完全相同，唯一的区别在于：
- 第 1631 题将**相邻格子的高度差**作为边的权重，是找最小化的 相邻格子最大绝对高度差。
- 第 778 题将**相邻两格子间高度的最大值**作为边的权重，是找最小化的 最大高度
- 第 2812 题将当前方格到最近小偷的距离作为边的权重，是找最大化的 最小距离

> 无向图 $G$ 中 $x$ 到 $y$ 的**最小瓶颈路**是这样的一类简单路径，满足这条路径上的**最大边权**在所有 $x$ 到 $y$ 的简单路径中是**最小**的。
> 根据最小生成树定义，$x$ 到 $y$ 的最小瓶颈路上的最大边权等于**最小生成树上 $x$ 到 $y$ 路径上的最大边权**。虽然最小生成树不唯一，但是每种最小生成树 $x$ 到 $y$ 路径的最大边权相同且为最小值。也就是说，每种最小生成树上的 $x$ 到 $y$ 的路径均为最小瓶颈路。
> <b></b>
> 
> 无向图 $G$ 中 $x$ 到 $y$ 的**最大瓶颈路**是这样的一类简单路径，满足这条路径上的**最小边权**在所有 $x$ 到 $y$ 的简单路径中是**最大**的。
> 根据最小生成树定义，$x$ 到 $y$ 的最大瓶颈路上的最小边权等于**最大生成树上 $x$ 到 $y$ 路径上的最小边权**。虽然最大生成树不唯一，但是每种最大生成树 $x$ 到 $y$ 路径的最小边权相同且为最大值。也就是说，每种最大生成树上的 $x$ 到 $y$ 的路径均为最大瓶颈路。
> <b></b>
> 
> **我们不必实际求出整个最小/大生成树，只需要求出最小/大生成树上 $x$ 到 $y$ 路径上的最大/小边权**。

在看清楚物理问题背后的数学模型后，这几道题都会迎刃而解。
### 解法1 多源BFS+二分
设每个格子的**安全系数**为**最近的小偷到它的距离**，求所有格子的安全系数，就是经典的**多源BFS最短路**，以所有小偷为起点、各做一次BFS，即可算出每个格子的安全系数。求出每个格子的安全系数后，就变成和「[水位上升的泳池游泳](https://leetcode.cn/problems/swim-in-rising-water/description/)」基本一样的问题了

最大化路径的安全系数，则是经典的**瓶颈路模型**。首先二分答案 $x$ ，然后通过一次BFS判断：能否只经过安全系数至少为 $x$ 的格子、从 $(0, 0)$ 走到 $(n - 1, n-1)$ ，因为安全系数最大是 $(n +  n - 1)$ ，所以本题复杂度为 $O(n^2 \log 2n)$ 。

```cpp
class Solution {
public:
    typedef pair<int, int> pii;
    int maximumSafenessFactor(vector<vector<int>>& grid) {
        int n = grid.size();
        // 以所有小偷为起点进行多源BFS
        int d[4][2] = {0, 1, 1, 0, -1, 0, 0, -1};
        int minDist[n][n]; // 最近的小偷到(i,j)的距离
        memset(minDist, -1, sizeof(minDist));
        queue<pii> q;
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (grid[i][j] == 1) {
            q.push(pii(i, j));
            minDist[i][j] = 0;
        }
        // 多源BFS
        while (!q.empty()) {
            pii p = q.front(); q.pop();
            int i = p.first, j = p.second;
            for (int k = 0; k < 4; ++k) {
                int x = i + d[k][0], y = j + d[k][1]; // minDist[x][y]看最近的小偷
                if (x < 0 || x >= n || y < 0 || y >= n || minDist[x][y] >= 0) continue;
                q.push(pii(x, y));
                minDist[x][y] = minDist[i][j] + 1;
            }
        }
        // 通过一次BFS,检查能否只经过安全系数>=lim的格子,从左上到右下角
        auto check = [&](int lim) {
            bool vis[n][n];
            memset(vis, false, sizeof(vis));
            queue<pii> q;
            q.push(pii(0, 0)); vis[0][0] = true;
            while (!q.empty()) {
                pii p = q.front(); q.pop();
                int i = p.first, j = p.second;
                for (int k = 0; k < 4; ++k) {
                    int x = i + d[k][0], y = j + d[k][1];
                    if (x < 0 || y < 0 || x >= n || y >= n || // 已访问或距离太短
                        minDist[x][y] < lim || vis[x][y]) continue;
                    q.push(pii(x, y));
                    vis[x][y] = true;
                }
            }
            return vis[n - 1][n - 1];
        };
        // 二分答案
        int l = 0, r = min(minDist[0][0], minDist[n - 1][n - 1]);
        while (l < r) {
            int mid = (l + r + 1) >> 1;
            if (check(mid)) l = mid;
            else r = mid - 1;
        }
        return l;
    }
};
```
---
### 解法2 多源BFS+最大瓶颈路模型+最大生成树（Prim+堆/Kruskal+边集数组）
根据题意，我们要找的是从左上角到右下角的最优路径，其中最优路径是指**途径的边的最小权重值最大**，然后输出最优路径中的最小权重值。此处的边权为**途径节点间 最小的最近小偷距离**。

根据最大瓶颈路模型，我们可以使用Kruskal最大生成树算法：
1. 先遍历所有的点，将所有的边加入集合，存储的格式为数组 $[a, b, w]$ ，代表编号为 $a$ 的点和编号为 $b$ 的点之间的权重为 $w$（按照题意，$w$ 为两个顶点间更小的那个最近小偷距离）。
2. **对边集集合进行排序，按照 $w$ 进行从大到小排序**。
3. 当我们有了所有排好序的候选边集合之后，对边从前往后处理，选择那些不会出现环的边加入最大生成树中。
4. 每次加入一条边之后，使用并查集来查询左上角点和右下角点是否连通。如果连通，那么**该边的权重即是答案**。

```java
class Solution {
    private class UnionFind {
        private int[] parent;
        public UnionFind(int n) {
            parent = new int[n];
            for (int i = 0; i < n; ++i) parent[i] = i;
        }
        public int find(int x) {
            return x != parent[x] ? parent[x] = find(parent[x]) : x;
        }
        public boolean isConnected(int x, int y) { return find(x) == find(y); }
        public void union(int p, int q) {
            int rp = find(p), rq = find(q);
            if (rp == rq) return;
            parent[rp] = rq;
        }
    }
    public int maximumSafenessFactor(List<List<Integer>> grid) {
        int n = grid.size(), m = grid.get(0).size();
        int len = n * m;
        int[][] d = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        int[][] minDist = new int[n][n]; // 最近的小偷到(i,j)的距离
        for (int[] row : minDist) Arrays.fill(row, -1);
        Queue<int[]> q = new ArrayDeque<>();
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (grid.get(i).get(j) == 1) {
            q.offer(new int[]{i, j});
            minDist[i][j] = 0;
        }
        while (!q.isEmpty()) {
            int[] front = q.poll();
            int i = front[0], j = front[1];
            for (int k = 0; k < 4; ++k) {
                int x = i + d[k][0], y = j + d[k][1]; // minDist[x][y]看最近的小偷
                if (x < 0 || x >= n || y < 0 || y >= n || minDist[x][y] >= 0) continue;
                q.offer(new int[]{x, y});
                minDist[x][y] = minDist[i][j] + 1;
            }
        }

        UnionFind unionFind = new UnionFind(len);
        // 预处理所有无向边 edge存[a,b,w]
        // 虽然我们可以往四个方向移动,但只要对于每个点都添加「向右」和「向下」
        // 两条边的话，其实就已经覆盖了所有边
        List<int[]> edges = new ArrayList<>();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                int index = i * m + j;
                int a, b, w;
                if (i + 1 < n) {
                    a = index; b = (i + 1) * m + j;
                    w = Math.min(minDist[i][j], minDist[i + 1][j]);
                    edges.add(new int[]{a, b, w});
                }
                if (j + 1 < m) {
                    a = index; b = i * m + (j + 1);
                    w = Math.min(minDist[i][j], minDist[i][j + 1]);
                    edges.add(new int[]{a, b, w});
                }
            }
        }
        Collections.sort(edges, (a, b) -> b[2] - a[2]); // 根据w权值降序
        int start = 0, end = len - 1;
        for (int[] edge : edges) {
            int a = edge[0], b = edge[1], w = edge[2];
            unionFind.union(a, b);
            if (unionFind.isConnected(start, end)) return w;
        }
        return 0;
    }
}
```
Kruskal虽然没有求出完整最大生成树，但是对所有边进行了排序。我们可以使用Prim算法+优先队列。

下面将此问题抽象为一张带权有向图，每个单元格为顶点，有指向相邻顶点的有向边，**边的权值为两个顶点中较低的最近小偷距离**。然后用Prim算法，求出最大生成树上左上角的点到右下角的点的路径上的最小边权值。此时不用对所有边进行排序。
```java
class Solution {
    public int maximumSafenessFactor(List<List<Integer>> grid) {
        int n = grid.size(), m = grid.get(0).size();
        int[][] directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        int[][] minDist = new int[n][n]; // 最近的小偷到(i,j)的距离
        for (int[] row : minDist) Arrays.fill(row, -1);
        Queue<int[]> q = new ArrayDeque<>();
        for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if (grid.get(i).get(j) == 1) {
            q.offer(new int[]{i, j});
            minDist[i][j] = 0;
        }
        while (!q.isEmpty()) {
            int[] front = q.poll();
            int i = front[0], j = front[1];
            for (int k = 0; k < 4; ++k) { // minDist[x][y]看最近的小偷
                int x = i + directions[k][0], y = j + directions[k][1];
                if (x < 0 || x >= n || y < 0 || y >= n || minDist[x][y] >= 0) continue;
                q.offer(new int[]{x, y});
                minDist[x][y] = minDist[i][j] + 1;
            }
        }
        
        boolean[][] vis = new boolean[n][m];
        int[][] dist = new int[n][m];
        for (int[] row : dist)
            Arrays.fill(row, Integer.MIN_VALUE);
        dist[0][0] = minDist[0][0];

        // 必须将 先前加入队列时的边权重 加入int[]中
        Queue<int[]> maxHeap = new PriorityQueue<>((a, b) -> b[2] - a[2]);
        maxHeap.offer(new int[]{0, 0, dist[0][0]});

        while (!maxHeap.isEmpty()) { // 找最长的边
            int[] front = maxHeap.poll();
            int x = front[0], y = front[1], d = front[2];
            if (vis[x][y]) continue;
            vis[x][y] = true;
            if (x == n - 1 && y == m - 1) break;
            // 更新
            for (int i = 0; i < 4; ++i) {
                int u = x + directions[i][0], v = y + directions[i][1];
                if (u >= 0 && u < n && v >= 0 && v < m 
                    && !vis[u][v] && // prim算法
                    Math.min(d, minDist[u][v]) > dist[u][v]) {
                        dist[u][v] = Math.min(d, minDist[u][v]);
                        maxHeap.offer(new int[]{u, v, dist[u][v]});
                    }
            }
        }
        return dist[n - 1][m - 1];
    }
}
```
复杂度分析：
- 时间复杂度：$O(2N^2 \log N)$ 。 使用了优先队列的 Prim 算法的时间复杂度是 $O(E \log E)$  ，这里 $E$ 是边数，至多是顶点数的 $4$ 倍，顶点数为 $N^2$  ，因此 $O(E \log E) = O(4N^2 \log N^2) = O(N^2 \log N)$ 。
- 空间复杂度：$O(N^2)$ 。 数组 $vis$ 、$dist$ 的大小为 $O(N^2)$ ，优先队列中保存的边的总数也是 $N^2$ 级别的。

---
### 解法3 多源BFS+倒序枚举答案+并查集
这是灵神的做法，我抄过来了，先膜拜一个 ლ(′◉❥◉｀ლ) ，用Java实现：
1. 从所有 $1$ 出发，写一个多源 BFS，计算出每个格子 $(i,j)$ 到最近的 $1$ 的曼哈顿距离 $\textit{dis}[i][j]$ 。注意题目保证**至少有一个 $1$** 。
2. 答案不会超过 $\textit{dis}[i][j]$ 的最大值，**这最大值的范围不是很大，因此可以倒序枚举答案**，不像 1631. Path With Minimum Effort 题那样消耗的体力可能很多。
3. 如何判断能从左上角走到右下角呢？用并查集——假设答案为 $d$ ，我们可以把所有 $\textit{dis}[i][j]=d$ 的格子与其四周 $\ge d$ 的格子用并查集连起来，在答案为 $d$ 的情况下，这些格子之间是可以互相到达的。
4. 用并查集判断 $(0,0)$ 和 $(n-1,n-1)$ 是否连通，只要连通就立刻返回 $d$ 作为答案。

```java
class Solution {
    private int[] fa;
    private int find(int x) {
        return fa[x] != x ? fa[x] = find(fa[x]) : fa[x];
    }
    public int maximumSafenessFactor(List<List<Integer>> grid) {
        int n = grid.size(), m = grid.get(0).size();
        int[][] directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        var q = new ArrayList<int[]>();
        var dist = new int[n][n];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid.get(i).get(j) > 0) q.add(new int[] {i, j}); // 小偷
                else dist[i][j] = -1;
            }
        }
        // 按照到最近小偷的距离，分层存储
        var groups = new ArrayList<List<int[]>>();
        groups.add(q);

        while (!q.isEmpty()) { // 多源BFS
            var tmp = q;
            q = new ArrayList<>();
            for (var p : tmp) {
                for (var d : directions) {
                    int x = p[0] + d[0], y = p[1] + d[1];
                    if (0 <= x && x < n && 0 <= y && y < n && dist[x][y] < 0) { // 不是小偷
                        q.add(new int[]{x, y});
                        dist[x][y] = groups.size(); // 到最近小偷的距离,一开始为1
                    }
                }
            }
            groups.add(q);
        }
        // 并查集
        fa = new int[n * n];
        for (int i = 0; i < n * n; ++i) fa[i] = i;

        // 多源BFS过程中最后一步会添加一个空集合用来退出循环
        // 要从groups.size()-2开始
        for (int ans = groups.size() - 2; ans > 0; --ans) {
            var g = groups.get(ans);
            for (var p : groups.get(ans)) {
                int i = p[0], j = p[1];
                for (var d : directions) {
                    int x = i + d[0], y = j + d[1];
                    if (0 <= x && x < n && 0 <= y && y < n && dist[x][y] >= dist[i][j])
                        
                        fa[find(x * n + y)] = find(i * n + j);
                }
            }
            if (find(0) == find(n * n - 1)) return ans;
        }
        return 0;
    }
}
```

---
### 错误示例
对我来说的极限剪枝，在比赛时还是有8个测试样例超时通不过：
```cpp
class Solution {
private:
    int n, mn = INT_MAX;
    typedef pair<int, int> pii;
    vector<pii> st;
    int dist[410][410];
    int pds[410][410];
    bool vis[410][410];
    struct pos {
        int x, y, d, pd;
        bool operator<(const pos& b) const {
            return d < b.d;
        }
    };
    int move[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int ans = 0;
    void findPath() {
        priority_queue<pos> pq;
        pq.push(pos{0, 0, mn, mn});
        vis[0][0] = true;
        while (!pq.empty()) {
            pos u = pq.top(); pq.pop();
            int x = u.x, y = u.y, pd = u.pd;
            if (pd <= ans) continue;
            if (x == n - 1 && y == n - 1) {
                ans = max(ans, pd);
                continue;
            }
            for (int i = 0; i < 4; ++i) {
                int a = x + move[i][0], b = y + move[i][1];
                if (a < 0 || a >= n || b < 0 || b >= n) continue;
                pos p = pos {a, b, dist[a][b], min(pd, dist[a][b]) };
                if (p.pd > ans) {
                    if (vis[a][b] == false || p.pd > pds[a][b]) {
                        pq.push(p);
                        vis[a][b] = true;
                        pds[a][b] = p.pd;
                    }
                }
            } 
        }
    }
public:    
    int maximumSafenessFactor(vector<vector<int>>& grid) {
        n = grid.size();
        if (grid[0][0] == 1 || grid[n - 1][n - 1] == 1) return 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 1) {
                    st.push_back(pii(i, j));
                    mn = min(mn, i + j);
                }
            }
        } 
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                vis[i][j] = false;
                pds[i][j] = -1;
                if (grid[i][j] == 0) {
                    int mnDist = INT_MAX;
                    for (int k = 0; k < st.size(); ++k) mnDist = min(mnDist, abs(i - st[k].first) + abs(j - st[k].second));
                    dist[i][j] = mnDist; 
                } else dist[i][j] = 0;
            }
        }
        findPath();
        return ans;
    }
};
```