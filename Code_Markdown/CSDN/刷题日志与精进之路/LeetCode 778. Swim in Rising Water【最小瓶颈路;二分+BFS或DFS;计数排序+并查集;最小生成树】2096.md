> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

在一个 `n x n` 的整数矩阵 `grid` 中，每一个方格的值 `grid[i][j]` 表示位置 `(i, j)` 的平台高度。

当开始下雨时，在时间为 `t` 时，水池中的水位为 `t` 。你可以从一个平台游向四周相邻的任意一个平台，但是前提是此时水位必须同时淹没这两个平台。假定你可以瞬间移动无限距离，也就是默认在方格内部游动是不耗时的。当然，在你游泳的时候你必须待在坐标方格里面。

你从坐标方格的左上平台 `(0，0)` 出发。返回 _你到达坐标方格的右下平台 `(n-1, n-1)` 所需的最少时间 。_

**示例 1:**

![](https://assets.leetcode.com/uploads/2021/06/29/swim1-grid.jpg)
```java
输入: grid = [[0,2],[1,3]]
输出: 3
解释:
时间为0时，你位于坐标方格的位置为 `(0, 0)。`
此时你不能游向任意方向，因为四个相邻方向平台的高度都大于当前时间为 0 时的水位。
等时间到达 3 时，你才可以游向平台 (1, 1). 因为此时的水位是 3，坐标方格中的平台没有比水位 3 更高的，所以你可以游向坐标方格中的任意位置
```
**示例 2:**
![](https://assets.leetcode.com/uploads/2021/06/29/swim2-grid-1.jpg)
```java
输入: grid = [[0,1,2,3,4],[24,23,22,21,5],[12,13,14,15,16],[11,17,18,19,20],[10,9,8,7,6]]
输出: 16
解释: 最终的路线用加粗进行了标记。
我们必须等到时间为 16，此时才能保证平台 (0, 0) 和 (4, 4) 是连通的
```
**提示:**
- `n == grid.length`
- `n == grid[i].length`
- `1 <= n <= 50`
- `0 <= grid[i][j] < n^2`
- `grid[i][j]` 中每个值 **均无重复**

---
注意题目中的重要信息：假定你可以 **瞬间移动** 无限距离，**游动不耗时**。当前这个问题就转换成为：找一个时刻 $t$ ，使得这个二维网格上数值 **小于等于** $t$ 的部分，存在一条从左上角到右下角的路径。即：经过了时间 $t$ 以后，可以瞬间从左上角（坐标 $[0, 0]$ ）游到右下角（坐标 $[N - 1, N - 1]$ ）。

相似题目：
- [LeetCode 2812. Find the Safest Path in a Grid](https://leetcode.cn/problems/find-the-safest-path-in-a-grid/)
- [LeetCode 1631. 最小体力消耗路径](https://leetcode.cn/problems/path-with-minimum-effort)
### 解法1 二分+BFS/DFS
这种做法是[LeetCode 2812. Find the Safest Path in a Grid](https://leetcode.cn/problems/find-the-safest-path-in-a-grid/)的完全简化版、在2812中需要先进行一次多源BFS、再来二分答案，也用在[LeetCode 1631. 最小体力消耗路径](https://leetcode.cn/problems/path-with-minimum-effort)中。

根据题目中的描述，给定了 $grid[i][j]$ 的范围是 $[0, n^2 - 1]$ ，所以答案必然落在此范围：
- 如果等待的时间 $t$ 越少，网格上可以游泳的部分就越少，存在从左上角到右下角的一条路径的可能性越小
- 如果等待的时间 $t$ 越多，网格上可以游泳的部分就越多，存在从左上角到右下角的一条路径的可能性越大。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308122004217.png)
这是本问题具有的 **单调性** 。因此可以使用二分查找定位到最短等待时间。

假设最优解为 $l$ 的话（恰好能到达右下角的时间），那么小于 $l$ 的时间无法到达右下角，大于 $l$ 的时间能到达右下角，因此==在以最优解 $l$ 为分割点的数轴上具有**二段性**，可以通过「二分」找到分割点 $l$== 。
> 注意：「二分」的本质是两段性，并非单调性。只要一段满足某个性质，另外一段不满足某个性质，就可以用「二分」。其中 33. 搜索旋转排序数组 是一个很好的说明例子。

具体来说：在区间 $[0, N * N - 1]$ 里猜一个整数，针对这个整数从起点（左上角）开始做一次DFS或BFS：
- ==当小于等于该数值时，如果存在一条从左上角到右下角的路径，说明答案可能是这个数值，也可能更小==；
- 当小于等于该数值时，如果不存在一条从左上角到右下角的路径，说明答案一定比这个数值更大。
- 按照这种方式不断缩小搜索的区间，最终找到最少等待时间。

```cpp
class Solution {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();
        typedef pair<int, int> pii;
        int d[4][2] = {-1, 0, 1, 0, 0, -1, 0, 1};
        auto check = [&](int lim) { // 能否在<=lim时间从左上到右小
            bool vis[n][n]; memset(vis, false, sizeof(vis));
            queue<pii> q;
            if (grid[0][0] <= lim) {
                q.push(pii(0, 0)); vis[0][0] = true;
            }
            while (!q.empty()) {
                pii p = q.front(); q.pop();
                int i = p.first, j = p.second;
                if (i == n - 1 && j == n - 1) return true;
                for (int k = 0; k < 4; ++k) {
                    int x = i + d[k][0], y = j + d[k][1];
                    if (x < 0 || x >= n || y < 0 || y >= n || grid[x][y] > lim || vis[x][y])
                        continue;
                    q.push(pii(x, y));
                    vis[x][y] = true;
                }
            }
            return vis[n - 1][n - 1];
        };
        int l = grid[0][0], r = n * n;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (check(mid)) r = mid;
            else l = mid + 1;
        }
        return l;
    }
};
```
复杂度分析：
- 时间复杂度：$O(N^2 \log N)$ 。 其中 $N$ 是方格的边长。最差情况下进行 $\log N^2$  次二分查找，每一次二分查找最差情况下要遍历所有单元格一次，时间复杂度为 $O(N^2)$ 。总的时间复杂度为 $O(N^2 \log N^2) = O(2N^2 \log N) = O(N^2 \log N)$
- 空间复杂度：$O(N^2)$ 。 数组 $vis$ 的大小为 $N^2$ ，如果使用深度优先遍历，须要使用的栈的大小最多为 $N^2$  ，如果使用广度优先遍历，须要使用的队列的大小最多为 $N^2$ 。

---
### 解法2 计数排序+并查集
关于连通性的问题，如果只问结果，不问具体怎么连起来的，还可以考虑使用并查集。由于题目要找的是最少等待时间，可以模拟下雨的过程，把网格抽象成一个无权图，==每经过一个时刻，**水位不断提高**，就考虑此时和雨水高度相等的单元格，考虑这个单元格的上、下、左、右、四个方向且**高度更低**的单元格，把它们在并查集中进行合并==。
![](https://pic.leetcode-cn.com/1611932131-mvdock-0778%20-%20unionfind.gif)
为了找到高度相近的单元格，需要先进行计数排序。注意 `grid[i][j]` 中每个值 **均无重复** 。
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
    public int swimInWater(int[][] grid) {
        int n = grid.length;
        int len = n * n;
        int[] heightToIndex = new int[len]; // 方格的高度:方格的坐标
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < n; ++j)
                heightToIndex[grid[i][j]] = i * n + j;

        int[][] d = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        UnionFind unionFind = new UnionFind(len);
        for (int i = 0; i < len; ++i) { // 高度从低到高
            int x = heightToIndex[i] / n, y = heightToIndex[i] % n;
            for (int j = 0; j < 4; ++j) {
                int u = x + d[j][0], v = y + d[j][1];
                if (u >= 0 && u < n && v >= 0 && v < n && grid[u][v] <= i)
                    unionFind.union(heightToIndex[i], u * n + v); // 合并
            }
            if (unionFind.isConnected(0, len - 1)) return i; // 连通
        }
        return -1;
    }
}
```
这种做法部分类似[LeetCode 2812. Find the Safest Path in a Grid](https://leetcode.cn/problems/find-the-safest-path-in-a-grid/)的并查集做法——每轮将「到最近小偷的距离为 $x$ 的单元格」与「距离为 $x+1$ 的单元格」合并。

复杂度分析：
- 时间复杂度：$O(N^2 \log N)$ 。 其中 $N$ 是方格的边长。计数排序 $O(N^2)$ ，合并周围、检查起点和终点是否属于同个连通分量 $O(\log N^2)$ 。总的时间复杂度为 $O(N^2+ N^2 \log N^2) = O(N^2 + 2N^2 \log N) = O(N^2 \log N)$ 。
- 空间复杂度：$O(N^2)$ 。 数组 `index` 的大小为 $N^2$ ，并查集底层的长度均为 $N^2$ 。

---
### 解法3 最小瓶颈路模型+最小生成树（Prim+堆/Kruskal+边集数组）
根据题意，我们要找的是从左上角到右下角的最优路径，其中最优路径是指**途径的边的最大权重值最小**，然后输出最优路径中的最大权重值。
> 无向图 $G$ 中 $x$ 到 $y$ 的**最小瓶颈路**是这样的一类简单路径，满足这条路径上的**最大边权**在所有 $x$ 到 $y$ 的简单路径中是**最小**的。
> 根据最小生成树定义，$x$ 到 $y$ 的最小瓶颈路上的最大边权等于**最小生成树上 $x$ 到 $y$ 路径上的最大边权**。虽然最小生成树不唯一，但是每种最小生成树 $x$ 到 $y$ 路径的最大边权相同且为最小值。也就是说，每种最小生成树上的 $x$ 到 $y$ 的路径均为最小瓶颈路。

这道题和第 1631 题，虽然物理问题不同，但是背后的数学模型几乎完全相同，唯一的区别在于第 1631 题将相邻格子的高度差作为边的权重、而本题将**相邻两格子间高度的最大值**作为边的权重。在看清楚物理问题背后的数学模型后，这两道题的物理问题都会迎刃而解。
> 和1631. 最小体力消耗路径几乎是完全一样的思路。**我们不必实际求出整个最小生成树，只需要求出最小生成树上 $x$ 到 $y$ 路径上的最大边权**。

根据最小瓶颈路模型，我们可以使用Kruskal最小生成树算法：
1. 先遍历所有的点，将所有的边加入集合，存储的格式为数组 $[a, b, w]$ ，代表编号为 $a$ 的点和编号为 $b$ 的点之间的权重为 $w$（按照题意，$w$ 为两者的最大高度）。
2. **对边集集合进行排序，按照 $w$ 进行从小到大排序**。
3. 当我们有了所有排好序的候选边集合之后，对边从前往后处理，选择那些不会出现环的边加入最小生成树中。
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
    public int swimInWater(int[][] grid) {
        int n = grid.length;
        int len = n * n;
        int[][] d = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        UnionFind unionFind = new UnionFind(len);

        // 预处理所有无向边
        // edge存[a,b,w]: 代表从a到b所需的时间点为w
        // 虽然我们可以往四个方向移动,但只要对于每个点都添加「向右」和「向下」
        // 两条边的话，其实就已经覆盖了所有边
        List<int[]> edges = new ArrayList<>();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int index = i * n + j;
                int a, b, w;
                if (i + 1 < n) {
                    a = index; b = (i + 1) * n + j;
                    w = Math.max(grid[i][j], grid[i + 1][j]);
                    edges.add(new int[]{a, b, w});
                }
                if (j + 1 < n) {
                    a = index; b = i * n + (j + 1);
                    w = Math.max(grid[i][j], grid[i][j + 1]);
                    edges.add(new int[]{a, b, w});
                }   
            }
        }
        Collections.sort(edges, (a, b) -> a[2] - b[2]); // 根据w权值升序
        // 从「小边」开始添加,当某一条边应用之后,恰好使得「起点」和「结点」联通
        // 那么代表找到了「最短路径」中的「权重最大的边」
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
Kruskal虽然没有求出完整最小生成树，但是对所有边进行了排序。我们可以使用Prim算法+优先队列。

下面将此问题抽象为一张带权有向图，每个单元格为顶点，有指向相邻顶点的有向边，**边的权值为指向顶点的高度**（水位到达该顶点的高度才可游到该顶点）。然后用Prim算法，求出最小生成树上左上角的点到右下角的点的路径上的最大边权。此时不用对所有边进行排序。
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308122020967.png)
Prim算法在示例2的用法如下：
![700](https://pic.leetcode-cn.com/1611935856-wrgFkn-0778%20-%20Dijkstra.gif)
```java
class Solution {
    public int swimInWater(int[][] grid) {
        int n = grid.length;
        Queue<int[]> minHeap = new PriorityQueue<>((a, b) -> a[2] - b[2]);
        minHeap.offer(new int[]{0, 0, grid[0][0]});
        
        boolean[][] vis = new boolean[n][n];
        // dist[i][j]表示 到顶点[i,j] 要等待的最少时间
        int[][] dist = new int[n][n];
        for (int[] row : dist) Arrays.fill(row, n * n);
        dist[0][0] = grid[0][0];

        int[][] directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        while (!minHeap.isEmpty()) { // 找最短的边
            int[] front = minHeap.poll();
            int x = front[0], y = front[1], d = front[2];
            if (vis[x][y]) continue;
            vis[x][y] = true;
            if (x == n - 1 && y == n - 1) return dist[n - 1][n - 1];
            // 更新
            for (int i = 0; i < 4; ++i) {
                int u = x + directions[i][0], v = y + directions[i][1];
                if (u >= 0 && u < n && v >= 0 && v < n 
                    && !vis[u][v] && // prim算法
                    Math.max(d, grid[u][v]) < dist[u][v]) {
                        dist[u][v] = Math.max(d, grid[u][v]);
                        minHeap.offer(new int[]{u, v, dist[u][v]});
                    }
            }
        }
        return -1;
    }
}
```
复杂度分析：
- 时间复杂度：$O(N^2 \log N)$ 。 使用了优先队列的 Prim 算法的时间复杂度是 $O(E \log E)$  ，这里 $E$ 是边数，至多是顶点数的 $4$ 倍，顶点数为 $N^2$  ，因此 $O(E \log E) = O(4N^2 \log N^2) = O(N^2 \log N)$ 。
- 空间复杂度：$O(N^2)$ 。 数组 $vis$ 、$dist$ 的大小为 $O(N^2)$ ，优先队列中保存的边的总数也是 $N^2$ 级别的。