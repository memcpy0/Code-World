> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

有一棵 `n` 个节点的无向树，节点编号为 `0` 到 `n - 1` ，根节点编号为 `0` 。给你一个长度为 `n - 1` 的二维整数数组 `edges` 表示这棵树，其中 `edges[i] = [ai, bi]` 表示树中节点 `ai` 和 `bi` 有一条边。

同时给你一个长度为 `n` 下标从 **0** 开始的整数数组 `values` ，其中 `values[i]` 表示第 `i` 个节点的值。

一开始你的分数为 `0` ，每次操作中，你将执行：
- 选择节点 `i` 。
- 将 `values[i]` 加入你的分数。
- 将 `values[i]` 变为 `0` 。

如果从根节点出发，到任意叶子节点经过的路径上的节点值之和都不等于 0 ，那么我们称这棵树是 **健康的** 。

你可以对这棵树执行任意次操作，但要求执行完所有操作以后树是 **健康的** ，请你返回你可以获得的 **最大分数** 。

**示例 1：**
![](https://assets.leetcode.com/uploads/2023/10/11/graph-13-1.png)
```js
输入：edges = [[0,1],[0,2],[0,3],[2,4],[4,5]], values = [5,2,5,2,1,1]
输出：11
解释：我们可以选择节点 1 ，2 ，3 ，4 和 5 。根节点的值是非 0 的。所以从根出发到任意叶子节点路径上节点值之和都不为 0 。所以树是健康的。你的得分之和为 values[1] + values[2] + values[3] + values[4] + values[5] = 11 。
11 是你对树执行任意次操作以后可以获得的最大得分之和。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2023/10/11/graph-14-2.png)
```js
输入：edges = [[0,1],[0,2],[1,3],[1,4],[2,5],[2,6]], values = [20,10,9,7,4,3,5]
输出：40
解释：我们选择节点 0 ，2 ，3 和 4 。
- 从 0 到 4 的节点值之和为 10 。
- 从 0 到 3 的节点值之和为 10 。
- 从 0 到 5 的节点值之和为 3 。
- 从 0 到 6 的节点值之和为 5 。
所以树是健康的。你的得分之和为 values[0] + values[2] + values[3] + values[4] = 40 。
40 是你对树执行任意次操作以后可以获得的最大得分之和。
```
**提示：**
- `2 <= n <= 2 * 10^4`
- `edges.length == n - 1`
- `edges[i].length == 2`
- `0 <= ai, bi < n`
- `values.length == n`
- `1 <= values[i] <= 10^9`
- 输入保证 `edges` 构成一棵合法的树。

---
**相似题目**
- [337. 打家劫舍 III](https://leetcode.cn/problems/house-robber-iii/)
### 解法 树形DP——正向思考
题目的意思其实是：取树上的数，使得**每条从根到叶子节点的路径**都有一个点没被取到，且使得取到的数的和最大。

为什么这样就一定能满足条件了呢？下面的数据范围告诉我们：每个点的元素本来就是正数值。这样，==只要到叶子节点的每条路径上有一个点没被取到，就可以让每条到叶子节点的路径和为正数==。

对于一棵以 $x$ 为根的子树：
- 留下 $x$ 的值，则每条从子树根 $x$ 到子树叶子节点的路径，都满足题意，因此可以取走子树其他所有节点。
- 取走 $x$ 的值，则**根 $x$ 下的所有子树，都是原问题的子问题**。

因此，我们要使用树形DP。对于一棵子树，我们记录两个数值：一个是该子树中「没有受到保留一个元素约束的情况下」的总权值，另一个是该子树已经满足「每条路径上都有一个点没被取到的情况下」的总权值。

这种情况下，前者的更新直接求和即可；后者的更新考虑两种情况。注意，**「以叶子节点为根的子树」无法取走叶子节点的值**。
```cpp
class Solution {
public:
    long long maximumScoreAfterOperations(vector<vector<int>>& edges, vector<int>& values) {
        using ll = long long;
        int n = values.size();
        vector<int> g[n];
        for (auto &e : edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }

        function<pair<ll, ll>(int, int)> dfs = [&](int u, int p) -> pair<ll, ll> {
            ll sum = 0, dp = 0;
            bool flag = false;
            for (int v : g[u]) {
                if (v == p) continue;
                pair<ll, ll> p = dfs(v, u);
                ll subtreeSum = p.first, subtreeDp = p.second;
                sum += subtreeSum; 
                dp += subtreeDp;
                flag = true;
            }
            if (!flag) return {values[u], 0};
            return { sum + values[u], max(dp + values[u], sum)};
        };
        return dfs(0, -1).second;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$

---
### 解法2 树形DP——正难则反
正难则反，先把所有 $values[i]$ 加到答案中，然后考虑哪些 $values[i]$ 不能选（撤销，不加入答案）。

设当前节点为 $x$ ，计算以 $x$ 为根的子树是健康时，失去的最小分数。那么总答案就是 $values$ 的元素和，减去「以 $0$ 为根的子树是健康时，失去的最小分数」。

用「选或不选」分类讨论：
- 第一种情况：失去 $values[x]$ ，也就是不加入答案，那么 $x$ 的所有子孙节点都可以加入答案，失去的最小分数就是 $\textit{values}[x]$ 。
- 第二种情况：$values[x]$ 加入答案，问题变成「以 $y$ 为根的子树是健康时，失去的最小分数」，这里 $y$ 是 $x$ 的儿子。如果有多个儿子，累加失去的最小分数。

这两种情况取最小值。注意**第一种情况是不会往下递归的，所以当我们递归到叶子的时候，叶子一定不能加入答案**，此时直接返回 $\textit{values}[x]$ 。
```cpp
class Solution {
public:
    long long maximumScoreAfterOperations(vector<vector<int>>& edges, vector<int>& values) {
        using ll = long long;
        int n = values.size();
        vector<int> g[n];
        for (auto &e : edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }
        // dfs(x,fa) 计算以x为根的子树是健康时失去的最小分数
        function<ll(int, int)> dfs = [&](int x, int fa) -> ll {
            ll loss = 0; // 第二种情况
            bool flag = false;
            for (int y : g[x]) {
                if (y == fa) continue;
                loss += dfs(y, x); // 计算以y为根的子树是健康时失去的最小分数
                flag = true;
            }
            if (!flag) return values[x]; // x是叶子时
            return min((ll)values[x], loss); // 两种情况取最小值
        };
        return accumulate(values.begin(), values.end(), 0LL) - dfs(0, -1);
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$