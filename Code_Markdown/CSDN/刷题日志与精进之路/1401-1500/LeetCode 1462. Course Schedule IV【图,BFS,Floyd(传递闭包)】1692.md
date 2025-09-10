> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309111904800.png)


你总共需要上 `numCourses` 门课，课程编号依次为 `0` 到 `numCourses-1` 。你会得到一个数组 `prerequisite` ，其中 `prerequisites[i] = [ai, bi]` 表示如果你想选 `bi` 课程，你 **必须** 先选 `ai` 课程。

- 有的课会有直接的先修课程，比如如果想上课程 `1` ，你必须先上课程 `0` ，那么会以 `[0,1]` 数对的形式给出先修课程数对。

先决条件也可以是 **间接** 的。如果课程 `a` 是课程 `b` 的先决条件，课程 `b` 是课程 `c` 的先决条件，那么课程 `a` 就是课程 `c` 的先决条件。

你也得到一个数组 `queries` ，其中 `queries[j] = [uj, vj]`。对于第 `j` 个查询，您应该回答课程 `uj` 是否是课程 `vj` 的先决条件。

返回一个布尔数组 `answer` ，其中 `answer[j]` 是第 `j` 个查询的答案。

**示例 1：**

![](https://assets.leetcode.com/uploads/2021/05/01/courses4-1-graph.jpg)
```js
输入：numCourses = 2, prerequisites = [[1,0]], queries = [[0,1],[1,0]]
输出：[false,true]
解释：课程 0 不是课程 1 的先修课程，但课程 1 是课程 0 的先修课程。
```
**示例 2：**
```js
输入：numCourses = 2, prerequisites = [], queries = [[1,0],[0,1]]
输出：[false,false]
解释：没有先修课程对，所以每门课程之间是独立的。
```
**示例 3：**

![](https://assets.leetcode.com/uploads/2021/05/01/courses4-3-graph.jpg)

```js
输入：numCourses = 3, prerequisites = [[1,2],[1,0],[2,0]], queries = [[1,0],[1,2]]
输出：[true,true]
```
**提示：**
- `2 <= numCourses <= 100`
- `0 <= prerequisites.length <= (numCourses * (numCourses - 1) / 2)`
- `prerequisites[i].length == 2`
- `0 <= ai, bi <= n - 1`
- `ai != bi`
- 每一对 `[ai, bi]` 都 **不同**
- 先修课程图中没有环。
- `1 <= queries.length <= 10^4`
- `0 <= ui, vi <= n - 1`
- `ui != vi`

---
### 解法1 多次BFS
矩阵范围很小，用邻接表存图，同时用 `reachable[][] ` 二维数组存储可达性矩阵。然后对每门课程，BFS遍历和标记其后续课程，得到可达性矩阵。最后，对每个询问，$O(1)$ 访问可达性矩阵得到答案。
```cpp
class Solution {
public:
    vector<bool> checkIfPrerequisite(int numCourses, vector<vector<int>>& prerequisites, vector<vector<int>>& queries) {
        vector<int> g[numCourses];
        for (const vector<int> &pre : prerequisites) g[pre[0]].push_back(pre[1]);
        vector<vector<bool>> reachable(numCourses, vector<bool>(numCourses, false));
        for (int i = 0; i < numCourses; ++i) {
            queue<int> q;
            vector<bool> vis(numCourses, false);
            q.push(i);
            vis[i] = true;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                reachable[i][u] = true; //由i可达u
                for (const int &v : g[u]) {
                    if (!vis[v]) {
                        vis[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        vector<bool> ans;
        for (const vector<int> &q : queries) ans.push_back(reachable[q[0]][q[1]]);
        return ans;
    }
};
```
---
### 解法2 Floyd计算传递闭包
运用Floyd算法的思想进行动态规划（可以归约到01矩阵乘法，使用矩阵快速幂和快速矩阵乘法，最优时间复杂度可以达到 $O(\dfrac{n^3}{\log n})$ ）：
```cpp
class Solution {
public:
    vector<bool> checkIfPrerequisite(int numCourses, vector<vector<int>>& prerequisites, vector<vector<int>>& queries) {
        vector<vector<bool>> reachable(numCourses, vector<bool>(numCourses, false));
        for (const vector<int> &pre : prerequisites) reachable[pre[0]][pre[1]] = true; //直接用邻接矩阵存储图
        for (int k = 0; k < numCourses; ++k) {
            for (int i = 0; i < numCourses; ++i) {
                if (!reachable[i][k]) continue; //剪枝,i不可达k
                for (int j = 0; j < numCourses; ++j)
                    reachable[i][j] = reachable[i][j] || reachable[k][j]; //i可达j或者i通过k可达j
            }
        }
        vector<bool> ans;
        for (const vector<int> &q : queries) ans.push_back(reachable[q[0]][q[1]]);
        return ans;
    }
};
```