> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You have `n` gardens, labeled from `1` to `n`, and an array `paths` where `paths[i] = [xi, yi]` describes a bidirectional path between garden `xi` to garden `yi`. In each garden, you want to plant one of 4 types of flowers.

All gardens have **at most 3** paths coming into or leaving it.

Your task is to choose a flower type for each garden such that, for any two gardens connected by a path, they have different types of flowers.

Return _**any** such a choice as an array_ `answer`_, where_ `answer[i]` _is the type of flower planted in the_ `(i+1)th` _garden. The flower types are denoted_ `1`_,_ `2`_,_ `3`_, or_ `4`_. It is guaranteed an answer exists._

**Example 1:**
```java
Input: n = 3, paths = [[1,2],[2,3],[3,1]]
Output: [1,2,3]
Explanation:
Gardens 1 and 2 have different types.
Gardens 2 and 3 have different types.
Gardens 3 and 1 have different types.
Hence, [1,2,3] is a valid answer. Other valid answers include [1,2,4], [1,4,2], and [3,2,1].
```
**Example 2:**
```java
Input: n = 4, paths = [[1,2],[3,4]]
Output: [1,2,1,2]
```
**Example 3:**
```java
Input: n = 4, paths = [[1,2],[2,3],[3,4],[4,1],[1,3],[2,4]]
Output: [1,2,3,4]
```
**Constraints:**
-   `1 <= n <= 10^4`
-   `0 <= paths.length <= 2 * 10^4`
-   `paths[i].length == 2`
-   `1 <= xi, yi <= n`
-   `xi != yi`
-   Every garden has **at most 3** paths coming into or leaving it.

题意：有 `n` 个花园，按从 `1` 到 `n` 标记。另有数组 `paths` ，其中 `paths[i] = [xi, yi]` 描述了花园 `xi` 到花园 `yi` 的双向路径。在每个花园中，你打算种下四种花之一。另外，所有花园 **最多** 有 **3** 条路径可以进入或离开。需要为每个花园选择一种花，使得通过路径相连的任何两个花园中的花的种类互不相同。

以数组形式返回 **任一** 可行的方案作为答案 `answer` ，其中 `answer[i]` 为在第 `(i+1)` 个花园中种植的花的种类。花的种类用  1、2、3、4 表示。保证存在答案。

---
### 解法 模拟+暴力
出题人可能是受到四色定理的启发出的题。问题相当于用 $4$ 种颜色给图中的每个节点染色，要求相邻节点颜色不同。而「所有花园最多有 $3$ 条路径可以进入或离开」，**这相当于图中每个点的度数至多为 $3$ ，那么只要选一个和邻居不同的颜色即可**。

哈希表（数组）实现：
```cpp
class Solution { 
public:
    vector<int> gardenNoAdj(int n, vector<vector<int>>& paths) {
        vector<vector<int>> g(n);
        vector<int> ans(n);
        for (vector<int> &v : paths) {
            g[v[0] - 1].push_back(v[1] - 1); // 从0开始
            g[v[1] - 1].push_back(v[0] - 1);
        } 
        for (int i = 0; i < n; ++i) { // 给结点i选择颜色
            bool used[5]{};
            for (int j: g[i]) used[ans[j]] = true;
            while (used[++ans[i]]) ;
        }
        return ans;
    }
}; 
``` 
位运算实现： 我们需要找到 $used$ 从低到高第一个 $0$ 的位置，即第一个未使用的颜色，这需要我们计算 $used$ 取反后的尾零个数。例如 $\textit{used}=10111_{(2)}$ ​，取反后变为 $1000_{(2)}$ （实际前导零也取反了，但不影响计算），尾零个数为 $3$ ，这恰好就是从低位到高位第一个 $0$ 的位置。 
```cpp
class Solution { 
public:
    vector<int> gardenNoAdj(int n, vector<vector<int>>& paths) {
        vector<vector<int>> g(n);
        vector<int> ans(n);
        for (vector<int> &v : paths) {
            g[v[0] - 1].push_back(v[1] - 1); // 从0开始
            g[v[1] - 1].push_back(v[0] - 1);
        } 
        for (int i = 0; i < n; ++i) { // 给结点i选择颜色
            int used = 1; // 由于颜色是1~4,把0加入mask保证下面不会算出0
            for (int j: g[i]) used |= 1 << ans[j];
            ans[i] = __builtin_ctz(~used);
        }
        return ans;
    }
}; 
```
复杂度分析：
- 时间复杂度：$O(n+m)$ ，其中 $m$ 为 $paths$ 的长度。
- 空间复杂度：$O(n+m)$ 。
