> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

一场比赛中共有 `n` 支队伍，按从 `0` 到  `n - 1` 编号。每支队伍也是 **有向无环图（DAG）** 上的一个节点。

给你一个整数 `n` 和一个下标从 **0** 开始、长度为 `m` 的二维整数数组 `edges` 表示这个有向无环图，其中 `edges[i] = [ui, vi]` 表示图中存在一条从 `ui` 队到 `vi` 队的有向边。

从 `a` 队到 `b` 队的有向边意味着 `a` 队比 `b` 队 **强** ，也就是 `b` 队比 `a` 队 **弱** 。

在这场比赛中，如果不存在某支强于 `a` 队的队伍，则认为 `a` 队将会是 **冠军** 。

如果这场比赛存在 **唯一** 一个冠军，则返回将会成为冠军的队伍。否则，返回 `-1` _。_

**注意**：
- **环** 是形如 `a1, a2, ..., an, an+1` 的一个序列，且满足：节点 `a1` 与节点 `an+1` 是同一个节点；节点 `a1, a2, ..., an` 互不相同；对于范围 `[1, n]` 中的每个 `i` ，均存在一条从节点 `ai` 到节点 `ai+1` 的有向边。
- **有向无环图** 是不存在任何环的有向图。

**示例 1：**
![](https://assets.leetcode.com/uploads/2023/10/19/graph-3.png)
```js
输入：n = 3, edges = [[0,1],[1,2]]
输出：0
解释：1 队比 0 队弱。2 队比 1 队弱。所以冠军是 0 队。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2023/10/19/graph-4.png)
```js
输入：n = 4, edges = [[0,2],[1,3],[1,2]]
输出：-1
解释：2 队比 0 队和 1 队弱。3 队比 1 队弱。但是 1 队和 0 队之间不存在强弱对比。所以答案是 -1 。
```
**提示：**
- `1 <= n <= 100`
- `m == edges.length`
- `0 <= m <= n * (n - 1) / 2`
- `edges[i].length == 2`
- `0 <= edge[i][j] <= n - 1`
- `edges[i][0] != edges[i][1]`
- 生成的输入满足：如果 `a` 队比 `b` 队强，就不存在 `b` 队比 `a` 队强
- 生成的输入满足：如果 `a` 队比 `b` 队强，`b` 队比 `c` 队强，那么 `a` 队比 `c` 队强

---
### 解法 是否恰好有一个入度为 $0$ 的点
本质上是看是否恰好有一个入度为 $0$ 的点。这一题我们可以统计入度，但也**不必实际计算度数**。

对每个节点，判断它是否出现在 $\textit{edges}[i][1]$中。**如果恰好有一个节点没有出现，说明没有可以击败它的队伍**，返回这个节点的编号。否则返回 $-1$ 。
```cpp
class Solution {
public:
    int findChampion(int n, vector<vector<int>>& edges) {
        vector<int> weak(n);
        for (auto &e : edges) weak[e[1]] = true;
        int ans = -1;
        for (int i = 0; i < n; ++i) {
            if (!weak[i]) {
                if (ans != -1) return -1; // 有多个入度为0的点
                ans = i;
            }
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n+m)$ ，其中 $m$ 为 $\textit{edges}$ 的长度。
- 空间复杂度：$\mathcal{O}(n)$ 。
https://leetcode.cn/problems/find-champion-ii/solutions/2513144/bu-zai-edgesi1-zhong-de-wei-yi-bian-hao-44ur6/