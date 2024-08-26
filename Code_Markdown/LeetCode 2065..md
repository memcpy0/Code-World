> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一张 **无向** 图，图中有 `n` 个节点，节点编号从 `0` 到 `n - 1` （**都包括**）。同时给你一个下标从 **0** 开始的整数数组 `values` ，其中 `values[i]` 是第 `i` 个节点的 **价值** 。同时给你一个下标从 **0** 开始的二维整数数组 `edges` ，其中 `edges[j] = [uj, vj, timej]` 表示节点 `uj` 和 `vj` 之间有一条需要 `timej` 秒才能通过的无向边。最后，给你一个整数 `maxTime` 。

**合法路径** 指的是图中任意一条从节点 `0` 开始，最终回到节点 `0` ，且花费的总时间 **不超过** `maxTime` 秒的一条路径。你可以访问一个节点任意次。一条合法路径的 **价值** 定义为路径中 **不同节点** 的价值 **之和** （每个节点的价值 **至多** 算入价值总和中一次）。

请你返回一条合法路径的 **最大** 价值。

**注意：** 每个节点 **至多** 有 **四条** 边与之相连。

**示例 1：**

![](https://assets.leetcode.com/uploads/2021/10/19/ex1drawio.png)

```cpp
输入：values = [0,32,10,43], edges = [[0,1,10],[1,2,15],[0,3,10]], maxTime = 49
输出：75
解释：
一条可能的路径为：0 -> 1 -> 0 -> 3 -> 0 。总花费时间为 10 + 10 + 10 + 10 = 40 <= 49 。
访问过的节点为 0 ，1 和 3 ，最大路径价值为 0 + 32 + 43 = 75 。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2021/10/19/ex2drawio.png)
```cpp
输入：values = [5,10,15,20], edges = [[0,1,10],[1,2,10],[0,3,10]], maxTime = 30
输出：25
解释：
一条可能的路径为：0 -> 3 -> 0 。总花费时间为 10 + 10 = 20 <= 30 。
访问过的节点为 0 和 3 ，最大路径价值为 5 + 20 = 25 。
```
**示例 3：**
![](https://assets.leetcode.com/uploads/2021/10/19/ex31drawio.png)
```cpp
输入：values = [1,2,3,4], edges = [[0,1,10],[1,2,11],[2,3,12],[1,3,13]], maxTime = 50
输出：7
解释：
一条可能的路径为：0 -> 1 -> 3 -> 1 -> 0 。总花费时间为 10 + 13 + 13 + 10 = 46 <= 50 。
访问过的节点为 0 ，1 和 3 ，最大路径价值为 1 + 2 + 4 = 7 。
```
**示例 4：**
**![](https://assets.leetcode.com/uploads/2021/10/21/ex4drawio.png)**

```cpp
输入：values = [0,1,2], edges = [[1,2,10]], maxTime = 10
输出：0
解释：
唯一一条路径为 0 。总花费时间为 0 。
唯一访问过的节点为 0 ，最大路径价值为 0 。
```
**提示：**
- `n == values.length`
- `1 <= n <= 1000`
- `0 <= values[i] <= 10^8`
- `0 <= edges.length <= 2000`
- `edges[j].length == 3`
- `0 <= uj < vj <= n - 1`
- `10 <= timej, maxTime <= 100`
- `[uj, vj]` 所有节点对 **互不相同** 。
- 每个节点 **至多有四条** 边。
- 图可能不连通。

---
### 解法1 暴力搜索
根据题目的数据范围，路径至多有 $\dfrac{maxTime}{\min(time_j)} \le \dfrac{100}{10} = 10$ 条边，这意味着搜索树最多只有 $11$ 层；同时，题目保证每个节点至多有 $4$ 条边与其相连，因此搜索树的每个节点至多有 $4$ 个节点，所以本题可以视作在一棵层数最多为 $11$ 的四叉树上递归，这可以在时间限制内跑完。

递归到下一个节点之前，如果发现消耗的时间超过了 $maxTime$ ，则不递归。
```cpp
class Solution {
public:
    int maximalPathQuality(vector<int>& values, vector<vector<int>>& edges, int max_time) {
        int n = values.size();
        vector<vector<pair<int, int>>> g(n);
        for (auto& e : edges) {
            int x = e[0], y = e[1], t = e[2];
            g[x].emplace_back(y, t);
            g[y].emplace_back(x, t);
        }
        int ans = 0;
        vector<int> vis(n);
        vis[0] = true;
        auto dfs = [&](auto&& dfs, int x, int sum_time, int sum_value) -> void {
            if (x == 0) { // 节点回到0的时候计算已经积累的价值和
                ans = max(ans, sum_value); // 注意这里没有return，还可继续走
            }
            for (auto& [y, t] : g[x]) {
                if (sum_time + t > max_time) {
                    continue;
                }
                if (vis[y]) { // 当前路径中已经访问过了
                    dfs(dfs, y, sum_time + t, sum_value);
                } else {
                    vis[y] = true; // 每个节点的价值至多算入价值总和一次
                    dfs(dfs, y, sum_time + t, sum_value + values[y]);
                    vis[y] = false; // 恢复现场
                }
            }
        };
        dfs(dfs, 0, 0, values[0]); // 0的价值
        return ans;
    }
};
```
```rust
impl Solution {
    pub fn maximal_path_quality(values: Vec<i32>, edges: Vec<Vec<i32>>, max_time: i32) -> i32 {
        let n = values.len();
        let mut g = vec![vec![]; n];
        for e in edges {
            let x = e[0] as usize;
            let y = e[1] as usize;
            let t = e[2];
            g[x].push((y, t));
            g[y].push((x, t));
        }

        fn dfs(x: usize, sum_time: i32, sum_value: i32, vis: &mut Vec<bool>, g: &Vec<Vec<(usize, i32)>>, values: &Vec<i32>, max_time: i32) -> i32 {
            let mut ans = if x == 0 { sum_value } else { 0 };
            for &(y, t) in &g[x] {
                if sum_time + t > max_time {
                    continue;
                }
                if vis[y] {
                    ans = ans.max(dfs(y, sum_time + t, sum_value, vis, g, values, max_time));
                } else {
                    vis[y] = true;
                    // 每个节点的价值至多算入价值总和中一次
                    ans = ans.max(dfs(y, sum_time + t, sum_value + values[y], vis, g, values, max_time));
                    vis[y] = false; // 恢复现场
                }
            }
            ans
        }

        let mut vis = vec![false; n];
        vis[0] = true;
        dfs(0, 0, values[0], &mut vis, &g, &values, max_time)
    }
}
```
```python
class Solution:
    def maximalPathQuality(self, values: List[int], edges: List[List[int]], max_time: int) -> int:
        n = len(values)
        g = [[] for _ in range(n)]
        for x, y, t in edges:
            g[x].append((y, t))
            g[y].append((x, t))

        def dfs(x: int, sum_time: int, sum_value: int) -> None:
            if x == 0:
                nonlocal ans
                ans = max(ans, sum_value)
                # 注意这里没有 return，还可以继续走
            for y, t in g[x]:
                if sum_time + t > max_time:
                    continue
                if vis[y]:
                    dfs(y, sum_time + t, sum_value)
                else:
                    vis[y] = True
                    # 每个节点的价值至多算入价值总和中一次
                    dfs(y, sum_time + t, sum_value + values[y])
                    vis[y] = False  # 恢复现场

        ans = 0
        vis = [False] * n
        vis[0] = True
        dfs(0, 0, values[0])
        return ans
```
```java
public class Solution {
    public int maximalPathQuality(int[] values, int[][] edges, int maxTime) {
        int n = values.length;
        List<int[]>[] g = new ArrayList[n];
        Arrays.setAll(g, i -> new ArrayList<>());
        for (int[] e : edges) {
            int x = e[0];
            int y = e[1];
            int t = e[2];
            g[x].add(new int[]{y, t});
            g[y].add(new int[]{x, t});
        }

        boolean[] vis = new boolean[n];
        vis[0] = true;
        return dfs(0, 0, values[0], vis, g, values, maxTime);
    }

    private int dfs(int x, int sumTime, int sumValue, boolean[] vis, List<int[]>[] g, int[] values, int maxTime) {
        int res = x == 0 ? sumValue : 0;
        for (int[] e : g[x]) {
            int y = e[0];
            int t = e[1];
            if (sumTime + t > maxTime) {
                continue;
            }
            if (vis[y]) {
                res = Math.max(res, dfs(y, sumTime + t, sumValue, vis, g, values, maxTime));
            } else {
                vis[y] = true;
                // 每个节点的价值至多算入价值总和中一次
                res = Math.max(res, dfs(y, sumTime + t, sumValue + values[y], vis, g, values, maxTime));
                vis[y] = false; // 恢复现场
            }
        }
        return res;
    }
}
```
```go
func maximalPathQuality(values []int, edges [][]int, maxTime int) (ans int) {
    n := len(values)
    type edge struct{ to, time int }
    g := make([][]edge, n)
    for _, e := range edges {
        x, y, t := e[0], e[1], e[2]
        g[x] = append(g[x], edge{y, t})
        g[y] = append(g[y], edge{x, t})
    }

    vis := make([]bool, n)
    vis[0] = true
    var dfs func(int, int, int)
    dfs = func(x, sumTime, sumValue int) {
        if x == 0 {
            ans = max(ans, sumValue)
            // 注意这里没有 return，还可以继续走
        }
        for _, e := range g[x] {
            y, t := e.to, e.time
            if sumTime+t > maxTime {
                continue
            }
            if vis[y] {
                dfs(y, sumTime+t, sumValue)
            } else {
                vis[y] = true
                // 每个节点的价值至多算入价值总和中一次
                dfs(y, sumTime+t, sumValue+values[y])
                vis[y] = false // 恢复现场
            }
        }
    }
    dfs(0, 0, values[0])
    return ans
}
```
```js
var maximalPathQuality = function(values, edges, maxTime) {
    const n = values.length;
    const g = Array.from({length: n}, () => []);
    for (const [x, y, t] of edges) {
        g[x].push([y, t]);
        g[y].push([x, t]);
    }

    let ans = 0;
    const vis = Array(n).fill(false);
    vis[0] = true;
    function dfs(x, sumTime, sumValue) {
        if (x === 0) {
            ans = Math.max(ans, sumValue);
            // 注意这里没有 return，还可以继续走
        }
        for (const [y, t] of g[x]) {
            if (sumTime + t > maxTime) {
                continue;
            }
            if (vis[y]) {
                dfs(y, sumTime + t, sumValue);
            } else {
                vis[y] = true;
                // 每个节点的价值至多算入价值总和中一次
                dfs(y, sumTime + t, sumValue + values[y]);
                vis[y] = false; // 恢复现场
            }
        }
    }
    dfs(0, 0, values[0]);
    return ans;
};
```
复杂度分析：
- 时间复杂度：$O(n+m+4)$ ，其中 $n$ 是 $values$ 的长度，$m$ 是 $edges$ 的长度，$k= \dfrac{maxTime}{\min{(time_j)}}$  。
- 空间复杂度：$O(n+m+k)$ 。注意递归会消耗 $O(k)$ 的栈空间。

---
### 解法2 暴力搜索+最短路剪枝
这一做法很简单，用Dijkstra算法预处理起点 $0$ 到每个节点的最短路距离，也是**每个节点到 $0$ 的最短路距离**。

递归到下个节点前，先判断下个节点走最短路的前提下、能否在 $maxTime$ 时间内回到起点 $0$ ，若不能则不递归。

这个做法在上述做法上进行了**常数优化**，实际运行时间变快了。
```python
class Solution:
    def maximalPathQuality(self, values: List[int], edges: List[List[int]], max_time: int) -> int:
        n = len(values)
        g = [[] for _ in range(n)]
        for x, y, t in edges:
            g[x].append((y, t))
            g[y].append((x, t))

        # Dijkstra 算法
        dis = [inf] * n
        dis[0] = 0
        h = [(0, 0)]
        while h:
            dx, x = heappop(h)
            if dx > dis[x]:  # x 之前出堆过
                continue
            for y, d in g[x]:
                new_dis = dx + d
                if new_dis < dis[y]:
                    dis[y] = new_dis  # 更新 x 的邻居的最短路
                    heappush(h, (new_dis, y))

        def dfs(x: int, sum_time: int, sum_value: int) -> None:
            if x == 0:
                nonlocal ans
                ans = max(ans, sum_value)
                # 注意这里没有 return，还可以继续走
            for y, t in g[x]:
                # 相比方法一，这里多了 dis[y]
                if sum_time + t + dis[y] > max_time:
                    continue
                if vis[y]:
                    dfs(y, sum_time + t, sum_value)
                else:
                    vis[y] = True
                    # 每个节点的价值至多算入价值总和中一次
                    dfs(y, sum_time + t, sum_value + values[y])
                    vis[y] = False  # 恢复现场

        ans = 0
        vis = [False] * n
        vis[0] = True
        dfs(0, 0, values[0])
        return ans
```
```java
public class Solution {
    public int maximalPathQuality(int[] values, int[][] edges, int maxTime) {
        int n = values.length;
        List<int[]>[] g = new ArrayList[n];
        Arrays.setAll(g, i -> new ArrayList<>());
        for (int[] e : edges) {
            int x = e[0];
            int y = e[1];
            int t = e[2];
            g[x].add(new int[]{y, t});
            g[y].add(new int[]{x, t});
        }

        // Dijkstra 算法
        int[] dis = new int[n];
        Arrays.fill(dis, Integer.MAX_VALUE);
        dis[0] = 0;
        PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[0] - b[0]);
        pq.add(new int[]{0, 0});
        while (!pq.isEmpty()) {
            int[] p = pq.poll();
            int dx = p[0];
            int x = p[1];
            if (dx > dis[x]) { // x 之前出堆过
                continue;
            }
            for (int[] e : g[x]) {
                int y = e[0];
                int newDis = dx + e[1];
                if (newDis < dis[y]) {
                    dis[y] = newDis; // 更新 x 的邻居的最短路
                    pq.offer(new int[]{newDis, y});
                }
            }
        }

        boolean[] vis = new boolean[n];
        vis[0] = true;
        return dfs(0, 0, values[0], vis, g, values, maxTime, dis);
    }

    private int dfs(int x, int sumTime, int sumValue, boolean[] vis, List<int[]>[] g, int[] values, int maxTime, int[] dis) {
        int res = x == 0 ? sumValue : 0;
        for (int[] e : g[x]) {
            int y = e[0];
            int t = e[1];
            // 相比方法一，这里多了 dis[y]
            if (sumTime + t + dis[y] > maxTime) {
                continue;
            }
            if (vis[y]) {
                res = Math.max(res, dfs(y, sumTime + t, sumValue, vis, g, values, maxTime, dis));
            } else {
                vis[y] = true;
                // 每个节点的价值至多算入价值总和中一次
                res = Math.max(res, dfs(y, sumTime + t, sumValue + values[y], vis, g, values, maxTime, dis));
                vis[y] = false; // 恢复现场
            }
        }
        return res;
    }
}
```
```cpp

```