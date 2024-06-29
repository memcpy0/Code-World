> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a **tree** (i.e. a connected, undirected graph that has no cycles) **rooted** at node `0` consisting of `n` nodes numbered from `0` to `n - 1`. The tree is represented by a **0-indexed** array `parent` of size `n`, where `parent[i]` is the parent of node `i`. Since node `0` is the root, `parent[0] == -1`.

You are also given a string `s` of length `n`, where `s[i]` is the character assigned to node `i`.

Return _the length of the **longest path** in the tree such that no pair of **adjacent** nodes on the path have the same character assigned to them._

**Example 1:**
![](https://assets.leetcode.com/uploads/2022/03/25/testingdrawio.png)
```java
Input: parent = [-1,0,0,1,1,2], s = "abacbe"
Output: 3
Explanation: The longest path where each two adjacent nodes have different characters in the tree is the path: 0 -> 1 -> 3. The length of this path is 3, so 3 is returned.
It can be proven that there is no longer path that satisfies the conditions. 
```
**Example 2:**
![](https://assets.leetcode.com/uploads/2022/03/25/graph2drawio.png)
```java
Input: parent = [-1,0,0,0], s = "aabc"
Output: 3
Explanation: The longest path where each two adjacent nodes have different characters is the path: 2 -> 0 -> 3. The length of this path is 3, so 3 is returned.
```
**Constraints:**
-   `n == parent.length == s.length`
-   `1 <= n <= 105`
-   `0 <= parent[i] <= n - 1` for all `i >= 1`
-   `parent[0] == -1`
-   `parent` represents a valid tree.
-   `s` consists of only lowercase English letters.

题意：给出一棵 **树** ，根节点是节点 `0` ，由编号从 `0` 到 `n - 1` 的 `n` 个节点组成。用下标从 **0** 开始、长度为 `n` 的数组 `parent` 来表示这棵树，`parent[i]` 是节点 `i` 的父节点，节点 `0` 是根节点，所以 `parent[0] == -1` 。另给一个长度 `n` 的字符串 `s` ，其中 `s[i]` 表示分配给节点 `i` 的字符。

找出路径上任意一对相邻节点都没有分配到相同字符的 **最长路径** ，并返回该路径的长度。

---
### 解法 树型DP+DFS
简单题，无非是树的直径的变体。由于最长路径（即树的直径）中相邻节点不能有「相同字符」，所以「以节点 $x$ 为根的子树中」$x$ 可达的最大距离 $d[x]$ （相当于深度-1），应该等于「所有与节点 $x$ 具有不同字符的子节点 $sub_1,sub_2,\dots$ 」中最大的 $d[sub_i]$ 再加 $1$ 。即从子树 $sub_i$ 转移过来时，仅考虑从满足 $s[x]\ne s[sub_i]$ 的子树 $sub_i$ 转移过来，所以加个if判断就行了。

假设经过 $x$ 和 $x$ 某两个子树的最长链，经过的子树节点是 $sub_i, sub_j$ ，于是该最长链的长度为 $d[sub_i]+1+d[sub_j]+1$ 。枚举所有节点作为 $x$ ，并比较所有最长链的长度，求出最长路径。

由于本题求的是点的个数，所以答案为最长路径的长度加一。
```cpp
class Solution {
private:
    vector<vector<int>> g;
    unordered_map<int, bool> vis;
    int ans = 0;
    vector<int> d;
    void dfs(int u, const string& s) {
        vis[u] = true;
        for (int i = 0, n = g[u].size(); i < n; ++i) {
            int v = g[u][i];
            if (vis[v]) continue;
            dfs(v, s);
            if (s[u] != s[v]) { // d[u]为之前求得的,与节点u具有不同字符的子节点能抵达的最大距离,再加1
                ans = max(ans, d[v] + 1 + d[u]); 
                d[u] = max(d[u], d[v] + 1);
            } 
        }
    }
public:
    int longestPath(vector<int>& parent, string s) {
        int n = parent.size();
        g.resize(n);
        d.resize(n);
        for (int i = 1; i < n; ++i) {
            int p = parent[i];
            g[p].push_back(i);
            g[i].push_back(p);
        }
        dfs(0, s);
        return ans + 1; // 要返回的是最长路径(相邻节点无相同字符)的节点数
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(height)$ 