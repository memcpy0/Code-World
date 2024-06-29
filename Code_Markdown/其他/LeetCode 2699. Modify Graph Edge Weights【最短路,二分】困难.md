> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 `n` 个节点的 **无向带权连通** 图，节点编号为 `0` 到 `n - 1` ，再给你一个整数数组 `edges` ，其中 `edges[i] = [ai, bi, wi]` 表示节点 `ai` 和 `bi` 之间有一条边权为 `wi` 的边。

部分边的边权为 `-1`（`wi = -1`），其他边的边权都为 **正** 数（`wi > 0`）。

你需要将所有边权为 `-1` 的边都修改为范围 `[1, 2 * 109]` 中的 **正整数** ，使得从节点 `source` 到节点 `destination` 的 **最短距离** 为整数 `target` 。如果有 **多种** 修改方案可以使 `source` 和 `destination` 之间的最短距离等于 `target` ，你可以返回任意一种方案。

如果存在使 `source` 到 `destination` 最短距离为 `target` 的方案，请你按任意顺序返回包含所有边的数组（包括未修改边权的边）。如果不存在这样的方案，请你返回一个 **空数组** 。

**注意：**你不能修改一开始边权为正数的边。

**示例 1：**
**![](https://assets.leetcode.com/uploads/2023/04/18/graph.png)**
```java
输入：n = 5, edges = [[4,1,-1],[2,0,-1],[0,3,-1],[4,3,-1]], source = 0, destination = 1, target = 5
输出：[[4,1,1],[2,0,1],[0,3,3],[4,3,1]]
解释：上图展示了一个满足题意的修改方案，从 0 到 1 的最短距离为 5 。
```
**示例 2：**
**![](https://assets.leetcode.com/uploads/2023/04/18/graph-2.png)**
```java
输入：n = 3, edges = [[0,1,-1],[0,2,5]], source = 0, destination = 2, target = 6
输出：[]
解释：上图是一开始的图。没有办法通过修改边权为 -1 的边，使得 0 到 2 的最短距离等于 6 ，所以返回一个空数组。
```
**示例 3：**
**![](https://assets.leetcode.com/uploads/2023/04/19/graph-3.png)**
```java
输入：n = 4, edges = [[1,0,4],[1,2,3],[2,3,5],[0,3,-1]], source = 0, destination = 2, target = 6
输出：[[1,0,4],[1,2,3],[2,3,5],[0,3,1]]
解释：上图展示了一个满足题意的修改方案，从 0 到 2 的最短距离为 6 。
```
**提示：**
- `1 <= n <= 100`
- `1 <= edges.length <= n * (n - 1) / 2`
- `edges[i].length == 3`
- `0 <= ai, bi < n`
- `wi = -1` 或者 `1 <= wi <= 10^7`
- `ai != bi`
- `0 <= source, destination < n`
- `source != destination`
- `1 <= target <= 10^9`
- 输入的图是连通图，且没有自环和重边。

---
本题难度较大，在[Zerotrac](https://zerotrac.github.io/leetcode_problem_rating/#/)上本题的分数是2873，是LeetCode周赛的第四题。需要先掌握朴素的 Dijkstra 算法，时间复杂度为 $O(n^2)$ ，其中 $n$ 是图中的节点数量。如果使用优先队列进行优化，时间复杂度为 $O(e \log e)$ ，其中 $e$ 是图中的边的数量，而本题中 $e$ 的范围可以达到 $O(n^2)$ 级别。

下面说明这两种方法，第一种方法只用不修改的朴素 Dijkstra 算法，但时间复杂度较高。第二种方法在 Dijkstra 算法的基础上进行修改，思维难度较高，但时间复杂度较低。

为了叙述方便，下文用 $s,t,D$ 分别表示题目中的起点 $source$ ，终点 $destiation$ 和目标的最短距离 $target$ 。

---
### 解法1 二分查找 + 最短路
给定任意一个图，如果节点 $s$ 到 $t$ 的最短路长度为 $d_{\min}(s, t)$ ​，那么如果给图中**任意一条边**的长度增加 $1$ ，新的最短路长度要么仍然为 $d_{\min}(s, t)$ ，要么为 $d_{\min}(s, t) + 1$ 。
> 证明：如果所有的 $s-t$ 最短路都经过了修改的那条边，那么最短路的长度会增加 $1$ 。否则，存在一条 $s-t$ 最短路没有任何改动，最短路的长度不变。

#### 思路与算法
根据题目的描述，我们可以得到下面的结论：
- 当所有可修改的负权边的长度改为 $1$ 时，$s-t$ 最短路的长度达到最小值；
- 当所有可修改的负权边的长度改为 $2 \times 10^9$ 时，$s−t$ 的最短路长度达到最大值。

然而，**把一条边的长度定为 $D$ 以上的值是没有意义的**，因为：
- 如果我们希望这条边出现在最短路中，那么它的长度一定不可能大于 $D$ ；
- 如果我们不希望这条边出现在最短路中，那么将它的值定为 $D$ ，加上任意一条路径上的边，就会得到大于 $D$ 的路径长度，这样就是合理的。

因此，每一条可修改的边的长度范围为 $[1,D]$ 。此时，构造出下面 $k \times (D-1) + 1$ 种不同的边权情况，其中 **$k$ 是可以修改的边的数量**：
$$\begin{align}
&[1, 1, 1, \cdots, 1] \\
&[2, 1, 1, \cdots, 1] \\
&[3, 1, 1, \cdots, 1] \\
&\cdots \\
&[D, 1, 1, \cdots, 1] \\ 
&[D, 2, 1, \cdots, 1] \\ 
&[D, 3, 1, \cdots, 1] \\ 
&\cdots \\
&[D, D, 1, \cdots, 1] \\
&\cdots \\
&[D, D, D, \cdots, D]
\end{align}$$
即**每次将「第一条可修改的、且长度没有到达 $D$ 的边」的长度增加 $1$** 。==根据上面的证明，相邻两种边权情况对应的最短路长度要么相同，要么增加 $1$== 。因此：
- 这 $k \times (D-1) + 1$ 种不同的边权情况，包含了**最短路长度的最小值到最大值**之间的、所有可能的最短路值，并且==上面构造的边权序列，其最短路的长度是满足单调性的==。
- 为什么每次都选择**第一条可修改的、长度小于 $D$ 的边**？题目中只要求最短路的长度为 $D$ ，修改的边可以是最短路中任意一条，而选择第一条就是**为了让边权序列呈字典序递增、让最短路长度单调递增**。

这样一来，我们就可以设计出如下的算法：
1. 对边权情况为 $[1, 1, 1, \cdots, 1]$ 计算一次最短路。如果最短路的长度大于 $D$ ，那么无解；
2. 对边权情况为 $[D,D,D,⋯ ,D]$ 计算一次最短路。如果最短路的长度小于 $D$ ，那么无解；
3. 判断为边界情况后，**此时答案一定存在**。我们可以==在 $k \times (D-1) + 1$ 种不同的边权情况中进行二分查找==。

class Solution {
public:
    vector<vector<int>> modifiedGraphEdges(int n, vector<vector<int>>& edges, int source, int destination, int target) {
        int k = 0;
        for (const auto& e: edges) {
            if (e[2] == -1) {
                ++k;
            }
        }

        if (dijkstra(source, destination, construct(n, edges, 0, target)) > target) {
            return {};
        }
        if (dijkstra(source, destination, construct(n, edges, static_cast<long long>(k) * (target - 1), target)) < target) {
            return {};
        }

        long long left = 0, right = static_cast<long long>(k) * (target - 1), ans = 0;
        while (left <= right) {
            long long mid = (left + right) / 2;
            if (dijkstra(source, destination, construct(n, edges, mid, target)) >= target) {
                ans = mid;
                right = mid - 1;
            }
            else {
                left = mid + 1;
            }
        }

        for (auto& e: edges) {
            if (e[2] == -1) {
                if (ans >= target - 1) {
                    e[2] = target;
                    ans -= (target - 1);
                }
                else {
                    e[2] = 1 + ans;
                    ans = 0;
                }
            }
        }

        return edges;
    }

    long long dijkstra(int source, int destination, const vector<vector<int>>& adj_matrix) {
        // 朴素的 dijkstra 算法
        // adj_matrix 是一个邻接矩阵
        int n = adj_matrix.size();
        vector<long long> dist(n, INT_MAX / 2);
        vector<int> used(n);
        dist[source] = 0;

        for (int round = 0; round < n - 1; ++round) {
            int u = -1;
            for (int i = 0; i < n; ++i) {
                if (!used[i] && (u == -1 || dist[i] < dist[u])) {
                    u = i;
                }
            }
            used[u] = true;
            for (int v = 0; v < n; ++v) {
                if (!used[v] && adj_matrix[u][v] != -1) {
                    dist[v] = min(dist[v], dist[u] + adj_matrix[u][v]);
                }
            }
        }

        return dist[destination];
    }

    vector<vector<int>> construct(int n, const vector<vector<int>>& edges, long long idx, int target) {
        // 需要构造出第 idx 种不同的边权情况，返回一个邻接矩阵
        vector<vector<int>> adj_matrix(n, vector<int>(n, -1));
        for (const auto& e: edges) {
            int u = e[0], v = e[1], w = e[2];
            if (w != -1) {
                adj_matrix[u][v] = adj_matrix[v][u] = w;
            }
            else {
                if (idx >= target - 1) {
                    adj_matrix[u][v] = adj_matrix[v][u] = target;
                    idx -= (target - 1);
                }
                else {
                    adj_matrix[u][v] = adj_matrix[v][u] = 1 + idx;
                    idx = 0;
                }
            }
        }
        return adj_matrix;
    }
};
复杂度分析

时间复杂度：O((n2+m)(log⁡m+log⁡target))O((n^2 + m)(\log m + \log \textit{target}))O((n 
2
 +m)(logm+logtarget))，其中 mmm 是图中边的数量。二分查找需要进行 O(log⁡(k×(target−1)))O(\log(k \times (\textit{target} - 1)))O(log(k×(target−1))) 次，在最坏情况下，所有的边都是可以修改的边，即 O(log⁡m+log⁡target)O(\log m + \log \textit{target})O(logm+logtarget)。每一次二分查找中需要 O(n2+m)O(n^2+m)O(n 
2
 +m) 的时间构造邻接矩阵，以及 O(n2)O(n^2)O(n 
2
 ) 的时间使用朴素的 Dijkstra 算法计算最短路。

空间复杂度：O(n2)O(n^2)O(n 
2
 )，即为朴素的 Dijkstra 算法中邻接矩阵需要的空间。返回的答案可以直接在给定的数组 edges\textit{edges}edges 上进行修改得到，省去 O(m)O(m)O(m) 的空间。

方法二：动态修改边权的 Dijkstra 算法
思路与算法

在方法一中，我们每次给一条可以修改的边长度增加 111。虽然使用了二分查找进行加速，但这样做效率仍然较低。如何进一步进行优化呢？我们可以产生一个简单的想法：

现在固定所有可以修改的边的长度为 111，再选择其中一条可以修改的边 u−vu-vu−v。s−us-us−u 的最短路长度为 dmin⁡(s,u)d_{\min}(s, u)d 
min
​
 (s,u)，v−tv-tv−t 的最短路长度为 dmin⁡(v,t)d_{\min}(v, t)d 
min
​
 (v,t)，那么将 u−vu-vu−v 的长度修改为 D−dmin⁡(s,u)−dmin⁡(v,t)D - d_{\min}(s, u) - d_{\min}(v, t)D−d 
min
​
 (s,u)−d 
min
​
 (v,t)，这样 s−u−v−ts-u-v-ts−u−v−t 就是一条长度恰好为 DDD 的路径。

然而 s−u−v−ts-u-v-ts−u−v−t 不一定是最短的路径，但我们可以不断重复这个步骤，直到最短路径的长度为 DDD 为止。具体的步骤如下：

每次选择一条可以修改的边 u−vu-vu−v，并且这条边之前没有选择过。修改这条边的长度，使得 s−u−v−ts-u-v-ts−u−v−t 这一条路径的长度恰好为 DDD，其中 s−us-us−u 以及 v−tv-tv−t 的路径是在「没有选择过的边的长度均为 111，选择过的边的长度均为修改后的长度」情况下的最短路径。

可以证明，如果存在满足题目要求的方案，那么上面的做法也一定能得到一种方案：

首先，每条边最多会被选择一次。因为在修改的过程中，s−us-us−u 和 v−tv-tv−t 的最短路径长度都是单调不降的（有其它长度为 111 的边被修改成了长度大于 111），因此如果第二次选择 u−vu-vu−v 这条边，会使得这条边的长度减少，这是没有任何意义的；

其次，如果当所有可以被选择的边都被选择过一次后，s−ts-ts−t 的最短路径长度不为 DDD，那么：

要么初始时（即所有可以修改的边的长度为 111 时）最短路径的长度已经大于 DDD，此时不存在满足题目要求的方案；

要么存在一条 s−ts-ts−t 的长度小于 DDD 的路径，这条路径上不可能有可以修改的边（否则可以修改对应的边使得长度等于 DDD），这就说明这条路径与所有可以修改的边无关，图中的最短路径本来就小于 DDD，此时也不存在满足题目要求的方案。

因此，「s−ts-ts−t 的最短路径长度不为 DDD」只会出现在不存在满足题目要求的方案时。这就说明在重复上述的步骤之后，s−ts-ts−t 的最短路径长度一定为 DDD。

这样一来，我们就可以得到如下的算法：

我们每次选择一条可以修改的边 u−vu-vu−v，并且这条边之前没有选择过；

我们使用 Dijkstra 算法，求出 s−us-us−u 以及 u−vu-vu−v 的最短路 dmin⁡(s,u)d_{\min}(s, u)d 
min
​
 (s,u) 和 dmin⁡(v,t)d_{\min}(v, t)d 
min
​
 (v,t)。如果 dmin⁡(s,u)+dmin⁡(v,t)<Dd_{\min}(s, u) + d_{\min}(v, t) < Dd 
min
​
 (s,u)+d 
min
​
 (v,t)<D，那么将 u−vu-vu−v 的长度修改为 D−dmin⁡(s,u)−dmin⁡(v,t)D - d_{\min}(s, u) - d_{\min}(v, t)D−d 
min
​
 (s,u)−d 
min
​
 (v,t)。

当所有可以修改的边都被选择过后，如果 s−ts-ts−t 的最短路长度为 DDD，说明存在一种满足要求的方案。

由于可以修改的边的数量最多为 O(m)O(m)O(m)，因此上述算法的时间复杂度为 O(mn2)O(mn^2)O(mn 
2
 )，需要继续进行优化。可以发现，该算法的瓶颈在于，当我们修改了 u−vu-vu−v 的长度后，后续选择的边的两部分最短路的值 dmin⁡(s,u)d_{\min}(s, u)d 
min
​
 (s,u) 和 dmin⁡(v,t)d_{\min}(v, t)d 
min
​
 (v,t) 会发生变化，需要重新进行计算。

因此，我们可以考虑在进行 Dijkstra 算法的同时对边进行修改。在 Dijkstra 算法中，当我们遍历到节点 uuu 时，再去修改所有以 uuu 为端点的（且没有修改过的）边，此时就可以保证 dmin⁡(s,u)d_{\min}(s, u)d 
min
​
 (s,u) 的值都是最新的。同时，当我们枚举 uuu 的相邻节点 vvv 时，vvv 到 ttt 的最短路长度 dmin⁡(v,t)d_{\min}(v, t)d 
min
​
 (v,t) 要么与第一次 Dijkstra 算法中计算出的值相同，要么会变成一个非常大的值，使得 dmin⁡(s,u)+dmin⁡(v,t)d_{\min}(s, u) + d_{\min}(v, t)d 
min
​
 (s,u)+d 
min
​
 (v,t) 已经至少为 DDD（证明见下一段）。这样就只需要一次 Dijkstra 算法即可，时间复杂度降低为 O(n2)O(n^2)O(n 
2
 )。

关于 dmin⁡(v,t)d_{\min}(v, t)d 
min
​
 (v,t) 正确性的证明：如果此时的 dmin⁡(v,t)d_{\min}(v, t)d 
min
​
 (v,t) 与第一次 Dijkstra 算法中计算出的值不同，那么说明 vvv 到 ttt 的任意一条原来的（即所有可以修改的边的长度均为 111）最短路中，都有一条边的长度修改为了大于 111 的值。对于任意一条最短路：

如果修改的边的某个端点为 vvv，记这条边为 u′−vu'-vu 
′
 −v，由于 vvv 还没有遍历过，说明 u′u'u 
′
  已经遍历过，但 u′u'u 
′
  在 vvv 到 ttt 的最短路上，而最短路上不可能先经过离 ttt 较近的点 u′u'u 
′
 ，再经过离 ttt 较远的点 vvv，这说明最终的最短路不可能从 sss 到 u′u'u 
′
  到 vvv 再到 ttt，而是会跳过 vvv，因此这种情况并不会出现；

否则，记最短路上的 u′−v′u'-v'u 
′
 −v 
′
  这条边被修改过，并且遍历过的节点是 u′u'u 
′
 。根据 Dijkstra 算法的性质，我们有 dmin⁡(s,u′)≤dmin⁡(s,u)d_{\min}(s, u') \leq d_{\min}(s, u)d 
min
​
 (s,u 
′
 )≤d 
min
​
 (s,u)；根据最短路的性质，我们有 dmin⁡(v′,t)<dmin⁡(v,t)d_{\min}(v', t) < d_{\min}(v, t)d 
min
​
 (v 
′
 ,t)<d 
min
​
 (v,t)，那么 u′−v′u'-v'u 
′
 −v 
′
  这条边被修改成的值就为 V=D−dmin⁡(s,u′)−dmin⁡(v′,t)>D−dmin⁡(s,u)−dmin⁡(v,t)V = D - d_{\min}(s, u') - d_{\min}(v', t) > D - d_{\min}(s, u) - d_{\min}(v, t)V=D−d 
min
​
 (s,u 
′
 )−d 
min
​
 (v 
′
 ,t)>D−d 
min
​
 (s,u)−d 
min
​
 (v,t)，vvv 到 ttt 的路径长度增加了至少 V−1≥D−dmin⁡(s,u)−dmin⁡(v,t)V - 1 \geq D - d_{\min}(s, u) - d_{\min}(v, t)V−1≥D−d 
min
​
 (s,u)−d 
min
​
 (v,t)，即路径长度至少为 dmin⁡′(v,t)=dmin⁡(v,t)+V−1≥D−dmin⁡(s,u)d'_{\min}(v, t) = d_{\min}(v, t) + V - 1 \geq D - d_{\min}(s, u)d 
min
′
​
 (v,t)=d 
min
​
 (v,t)+V−1≥D−d 
min
​
 (s,u)。此时，dmin⁡(s,u)+dmin⁡′(v,t)≥Dd_{\min}(s, u) + d'_{\min}(v, t) \geq Dd 
min
​
 (s,u)+d 
min
′
​
 (v,t)≥D。

因此，如果 dmin⁡(v,t)d_{\min}(v, t)d 
min
​
 (v,t) 与第一次 Dijkstra 算法中计算出的值不同，u−vu-vu−v 这条边本身就没有任何意义了。

最终的算法如下：

首先以 ttt 为起始点进行一次 Dijkstra 算法，此时所有可以修改的边长度均为 111；

随后以 sss 为起始点进行一次 Dijkstra 算法。当遍历到节点 uuu 时，修改所有以 uuu 为端点的边 u−vu-vu−v：

如果不是可以修改的边，或已经修改过，则不进行任何操作；

如果 s−us-us−u 的最短路长度（当前 Dijkstra 算法求出）加上 v−tv-tv−t 的最短路长度（第一次 Dijkstra 算法求出）已经大于 DDD，则 u−vu-vu−v 是一条没有用处的边，将它修改为任意值，例如 DDD；

否则，将边的长度进行修改，值等于 DDD 减去 s−us-us−u 的最短路长度与 v−tv-tv−t 的最短路长度之和，构造出一条长度为 DDD 的路径。

再通过这些边更新到 vvv 的最短路长度；

两次 Dijkstra 算法完成后，如果 s−ts-ts−t 的最短路长度为 DDD，则返回修改后的边，否则无解。

代码

C++
Java
C#
Python3
Golang
JavaScript
C
class Solution {
public:
    vector<vector<int>> modifiedGraphEdges(int n, vector<vector<int>>& edges, int source, int destination, int target) {
        this->target = target;
        vector<vector<int>> adj_matrix(n, vector<int>(n, -1));
        // 邻接矩阵中存储边的下标
        for (int i = 0; i < edges.size(); ++i) {
            int u = edges[i][0], v = edges[i][1];
            adj_matrix[u][v] = adj_matrix[v][u] = i;
        }
        from_destination = dijkstra(0, destination, edges, adj_matrix);
        if (from_destination[source] > target) {
            return {};
        }
        vector<long long> from_source = dijkstra(1, source, edges, adj_matrix);
        if (from_source[destination] != target) {
            return {};
        }
        return edges;
    }

    vector<long long> dijkstra(int op, int source, vector<vector<int>>& edges, const vector<vector<int>>& adj_matrix) {
        // 朴素的 dijkstra 算法
        // adj_matrix 是一个邻接矩阵
        int n = adj_matrix.size();
        vector<long long> dist(n, INT_MAX / 2);
        vector<int> used(n);
        dist[source] = 0;

        for (int round = 0; round < n - 1; ++round) {
            int u = -1;
            for (int i = 0; i < n; ++i) {
                if (!used[i] && (u == -1 || dist[i] < dist[u])) {
                    u = i;
                }
            }
            used[u] = true;
            for (int v = 0; v < n; ++v) {
                if (!used[v] && adj_matrix[u][v] != -1) {
                    if (edges[adj_matrix[u][v]][2] != -1) {
                        dist[v] = min(dist[v], dist[u] + edges[adj_matrix[u][v]][2]);
                    }
                    else {
                        if (op == 0) {
                            dist[v] = min(dist[v], dist[u] + 1);
                        }
                        else {
                            int modify = target - dist[u] - from_destination[v];
                            if (modify > 0) {
                                dist[v] = min(dist[v], dist[u] + modify);
                                edges[adj_matrix[u][v]][2] = modify;
                            }
                            else {
                                edges[adj_matrix[u][v]][2] = target;
                            }
                        }
                    }
                    
                }
            }
        }

        return dist;
    }

private:
    vector<long long> from_destination;
    int target;
};
复杂度分析

时间复杂度：O(n2+m)O(n^2 + m)O(n 
2
 +m)，其中 mmm 是图中边的数量。我们需要 O(n2+m)O(n^2+m)O(n 
2
 +m) 的时间构造邻接矩阵，以及两次 O(n2)O(n^2)O(n 
2
 ) 的时间使用朴素的 Dijkstra 算法计算最短路。

空间复杂度：O(n2)O(n^2)O(n 
2
 )，即为朴素的 Dijkstra 算法中邻接矩阵需要的空间。返回的答案可以直接在给定的数组 edges\textit{edges}edges 上进行修改得到，省去 O(m)O(m)O(m) 的空间。

作者：力扣官方题解
链接：https://leetcode.cn/problems/modify-graph-edge-weights/solutions/2300101/xiu-gai-tu-zhong-de-bian-quan-by-leetcod-66bg/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
