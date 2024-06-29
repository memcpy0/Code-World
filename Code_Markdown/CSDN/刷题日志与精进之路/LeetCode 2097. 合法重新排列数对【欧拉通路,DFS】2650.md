> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的二维整数数组 `pairs` ，其中 `pairs[i] = [starti, endi]` 。如果 `pairs` 的一个重新排列，满足对每一个下标 `i` （ `1 <= i < pairs.length` ）都有 `endi-1 == starti` ，那么我们就认为这个重新排列是 `pairs` 的一个 **合法重新排列** 。

请你返回 **任意一个** `pairs` 的合法重新排列。

**注意：** 数据保证至少存在一个 `pairs` 的合法重新排列。

**示例 1：**
```js
输入：pairs = [[5,1],[4,5],[11,9],[9,4]]
输出：[[11,9],[9,4],[4,5],[5,1]]
解释：
输出的是一个合法重新排列，因为每一个 endi-1 都等于 starti 。
end0 = 9 == 9 = start1 
end1 = 4 == 4 = start2
end2 = 5 == 5 = start3
```
**示例 2：**
```js
输入：pairs = [[1,3],[3,2],[2,1]]
输出：[[1,3],[3,2],[2,1]]
解释：
输出的是一个合法重新排列，因为每一个 endi-1 都等于 starti 。
end0 = 3 == 3 = start1
end1 = 2 == 2 = start2
重新排列后的数组 [[2,1],[1,3],[3,2]] 和 [[3,2],[2,1],[1,3]] 都是合法的。
```
**示例 3：**
```js
输入：pairs = [[1,2],[1,3],[2,1]]
输出：[[1,2],[2,1],[1,3]]
解释：
输出的是一个合法重新排列，因为每一个 endi-1 都等于 starti 。
end0 = 2 == 2 = start1
end1 = 1 == 1 = start2
```
**提示：**
- `1 <= pairs.length <= 10^5`
- `pairs[i].length == 2`
- `0 <= starti, endi <= 10^9`
- `starti != endi`
- `pairs` 中不存在一模一样的数对。
- 至少 **存在** 一个合法的 `pairs` 重新排列。

---
### 解法 欧拉路径+DFS
如果我们把数组 $pairs$ 中出现的每个数看成一个节点，$(\textit{start}_i, \textit{end}_i)$ 看成从 $\textit{start}_i$ 到 $\textit{end}_i$ 的一条有向边，那么 $pairs$ 的一个合法排列就对应着：
- 从节点 $\textit{pairs}[0][0]$ 开始；
- 依次经过 $\textit{pairs}[0][1], \textit{pairs}[1][1], \cdots, \textit{pairs}[n-1][1]$

的一条路径，其中 $n$ 是数组 $pairs$ 的长度。**这条路径经过了图上的每一条边恰好一次**，是一条「欧拉通路」，**因此我们的目标就是找出图上的任意一条欧拉通路**。

对于本题而言，首先需要找到欧拉通路的起始节点：
- 如果图中所有节点的入度和出度都相等，那么从任意节点开始都存在欧拉通路；
- 如果图中存在一个节点的出度比入度恰好多 $1$ ，另一个节点的入度恰好比出度多 $1$ ，那么**欧拉通路必须从前一个节点开始，到后一个节点结束**。
- 除此之外的有向图都不存在欧拉通路。

本题保证了至少存在一个合法排列，因此图已经是上述的两种情况之一。当我们确定起始节点后，就可以使用DFS求解欧拉通路了。如果我们得到的欧拉通路为：
$$v_1, v_2, v_3, \cdots, v_n, v_{n+1}$$
那么 $[[v_1, v_2], [v_2, v_3], \cdots, [v_n, v_{n+1}]]$ 就是一个合法排列。
```cpp
class Solution {
public:
    vector<vector<int>> validArrangement(vector<vector<int>>& pairs) {
        // 存储图
        unordered_map<int, vector<int>> edges;
        // 存储入度和出度
        unordered_map<int, int> deg;
        for (const auto& p: pairs) {
            edges[p[0]].push_back(p[1]);
            ++deg[p[0]], --deg[p[1]];
        }
        // 深度优先搜索（Hierholzer算法）求解欧拉通路
        vector<vector<int>> ans;
        function<void(int)> dfs = [&](int u) {
            while (!edges[u].empty()) {
                int v = edges[u].back();
                edges[u].pop_back(); // 删除一条边
                dfs(v);
                ans.push_back({u, v});
            }
        };     
        // 寻找起始节点
        for (const auto& [x, occ]: deg) // 如果有节点出度比入度恰好多 1，那么只有它才能是起始节点
            if (occ == 1) {
                dfs(x);
                break;
            }
        if (ans.empty()) dfs(pairs[0][0]);
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 nnn 是数组 $pairs$ 的长度。图中有不超过 $n+1$ 个节点和 $n$ 条边，因此求解欧拉通路需要的时间为 $O(n)$ 。
- 空间复杂度：$O(n)$ ，即为存储图需要使用的空间。

求解欧拉通路使用DFS，可以参考「[OI Wiki — 欧拉图](https://oi-wiki.org/graph/euler/)」 ，一般使用 $\text{Hierholzer}$ 算法求解欧拉通路，与欧拉回路或欧拉通路有关的题目：
- 「332. 重新安排行程」
- 「753. 破解保险箱」