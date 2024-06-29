> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an **undirected** tree consisting of `n` vertices numbered from `1` to `n`. A frog starts jumping from the **vertex 1**. In one second, the frog jumps from its current vertex to another **unvisited** vertex if they are directly connected. The frog can not jump back to a visited vertex. In case the frog can jump to several vertices it jumps randomly to one of them with the same probability, otherwise, when the frog can not jump to any **unvisited** vertex it **jumps forever** on the same vertex. 

The edges of the undirected tree are given in the array `edges`, where `edges[i] = [fromi, toi]` means that exists an edge connecting directly the vertices `fromi` and `toi`.

*Return the probability that after `t` seconds the frog is on the vertex `target`*.

**Example 1: 
![](https://assets.leetcode.com/uploads/2021/12/21/frog1.jpg)
 
```java
Input: n = 7, edges = [[1,2],[1,3],[1,7],[2,4],[2,6],[3,5]], t = 2, target = 4
Output: 0.16666666666666666 
Explanation: The figure above shows the given graph. The frog starts at vertex 1, 
jumping with 1/3 probability to the vertex 2 after second 1 and 
then jumping with 1/2 probability to vertex 4 after second 2. 
Thus the probability for the frog is on the vertex 4 after 2 seconds is 
1/3 * 1/2 = 1/6 = 0.16666666666666666.  
```
**Example 2:**
![](https://assets.leetcode.com/uploads/2021/12/21/frog2.jpg)
```ruby
Input: n = 7, edges = [[1,2],[1,3],[1,7],[2,4],[2,6],[3,5]], t = 1, target = 7
Output: 0.3333333333333333
Explanation: The figure above shows the given graph. The frog starts at vertex 1, 
jumping with 1/3 = 0.3333333333333333 probability to the vertex 7 after second 1. 
```
**Example 3:**
```cpp
Input: n = 7, edges = [[1,2],[1,3],[1,7],[2,4],[2,6],[3,5]], t = 20, target = 6
Output: 0.16666666666666666 
```
Constraints:
- `1 <= n <= 100`
- `edges.length == n-1`
-  `edges[i].length == 2`
-  `1 <= edges[i][0], edges[i][1] <= n`
-   `1 <= t <= 50`
-   `1 <= target <= n`
-    Answers within `10^-5` of the actual value will be accepted as correct.

题意：给出一棵**无向树**，由序号为 `1-n` 的 `n` 个顶点组成。一只青蛙从顶点 `1` 开始起跳。每一秒，它从现在的结点跳到另外一个**未曾访问过**而且**直接相连**的顶点。青蛙不能跳回到一个已经访问过的顶点。如果青蛙有几个可以跳跃的点，它会**等概率**地选择一个顶点跳过去；当青蛙没有可以跳跃的下一个顶点时，它会一直在原地跳。

---
### 解法1 DFS
#### 1. 思考
这个题目有点麻烦，但也不是太难，关键在于是否理解清楚了题意。
- 示例1中，青蛙从顶点1开始起跳，经过两秒(2次跳跃)后，它到达顶点4的概率为 `0.16666666666666666` ；
- 示例2中，青蛙从顶点1开始起跳，经过一秒(1次跳跃)后，它到达顶点7的概率为 `0.3333333333333333` ；
- 示例3中，青蛙从顶点1开始起跳，经过20秒(20次跳跃)后，它到达顶点6的概率为 `0.16666666666666666` 。这里青蛙任选一条路径，都可以在20秒内跳完，当它到达了目标顶点后，它就一直原地跳跃直到时间结束。

**更关键的是，思考以下问题**：
- 如果青蛙跳到了一个不含 $target$ 的子树，它还能到达 $target$ 吗？
- 如果当前节点有 $c$ 个儿子，青蛙跳到其中一个儿子的概率是多少？
- 考虑到浮点数在计算时可能会丢失精度，能想到一个尽量避免丢失精度的写法吗？

#### 2. 想法
根据题意，青蛙只能一路向下、无法回头。如果青蛙跳到了一棵不包含 $target$ 的子树，它就无法到达 $target$ 。

如果当前节点有 $c$ 个儿子，青蛙跳到其中一个儿子的概率显然是 $\dfrac{1}{c}$ 。如示例1，第一步有 $\dfrac{1}{3}$ 的概率跳到节点 $2$ ，第二步有 $\dfrac{1}{2}$ 的概率跳到节点 $4$ ，所以 $t = 2$  秒后位于 $target = 4$ 的概率是 $\dfrac{1}{6}$ 。

既然答案是由若干个分子为 $1$ 的分数相乘得到，那么**干脆只把分母相乘**，最后计算下倒数，就可避免过程中因为浮点乘法导致的精度丢失了。另外，==整数的计算效率通常比浮点数的高==。
#### 3. DFS算法
DFS算法从根节点 $1$ 出发，搜索 $target$ 。而且总是由自顶向下和自底向上两种实现方案。
- **自顶向下**是一边「递」，一边把儿子个数 $c$ 乘起来，如果**能在第 $t$ 秒到达 $target$** ，或**小于 $t$ 秒到达 $target$ 且 $target$ 是叶子节点**（此时每次跳跃都留在原地），那么就记录答案为乘积的倒数，同时返回一个布尔值表示递归结束。
- **自底向上**类似，找到 $target$ 后，在「归」的过程中做乘法。这种写法更好，因为只在找到 $target$ 后做乘法，而自顶向下即使在不含 $target$ 的子树中搜索，也会盲目做乘法。

#### 4. 细节
思考下什么情况下算出的乘积最大？考虑构造一棵树，根节点有 $c$ 个儿子，其中**一个儿子**又有 $c$ 个儿子，每层都有 $c$ 个节点，依次类推。==这样算出的乘积约为 $c^{n/c}$ ，当 $n = 100, c = 3$ 时取到最大值，这超过了 $32$ 位整数范围，但在 $64$ 位整数范围内==。为了避免溢出，乘积需要使用 $64$ 位整数存储。
> 此外，可以把节点 $1$ 添加一个 $0$ 号邻居（作为虚拟的DFS过程中的父节点，当然 **$1$ 可能有真实存在的父节点**），从而避免判断当前节点为根节点 $1$ ，也避免了特判 $n = 1$ 的情况。
> 
> 此外，DFS中的时间不是从 $0$ 开始增加到 $t$ ，而是从 $t$ 开始减少到 $0$ ，这样代码中只用和 $0$ 比较，不用和 $t$ 比较，从而**减少了一个DFS之外的变量引入**。

#### 5. 自顶向下实现
```cpp
class Solution {
public:
    double frogPosition(int n, vector<vector<int>> &edges, int t, int target) {
        vector<vector<int>> g(n + 1);
        g[1] = {0}; // 减少额外判断的小技巧
        for (auto &e : edges) {
            int x = e[0], y = e[1];
            g[x].push_back(y);
            g[y].push_back(x); // 建树
        }
        double ans = 0;
        function<bool(int, int, int, long long)> dfs = [&](int x, int fa, int left_t, long long prod) -> bool {
            // t秒后必须在target(恰好到达，或target是叶子节点、停在原地)
            if (x == target && (left_t == 0 || g[x].size() == 1)) {
                ans = 1.0 / prod;
                return true;
            }
            if (x == target || left_t == 0) return false;
            for (int y : g[x]) // 遍历x的儿子y
                if (y != fa && dfs(y, x, left_t - 1, prod * (g[x].size() - 1)))
                    return true; // 找到target就不再递归了
            return false; // 未找到target
        };
        dfs(1, 0, t, 1);
        return ans;
    }
};
```
#### 6. 自底向上实现
```cpp
class Solution {
public:
    double frogPosition(int n, vector<vector<int>> &edges, int t, int target) {
        vector<vector<int>> g(n + 1);
        g[1] = {0}; // 减少额外判断的小技巧
        for (auto &e : edges) {
            int x = e[0], y = e[1];
            g[x].push_back(y);
            g[y].push_back(x); // 建树
        }
        double ans = 0;
        function<long long(int, int, int)> dfs = [&](int x, int fa, int left_t) -> long long {
            // t秒后必须在target(恰好到达，或者target是叶子停在原地)
            if (left_t == 0) return x == target;
            if (x == target) return g[x].size() == 1;
            for (int y : g[x]) { // 遍历x的儿子y
                if (y != fa) { // y不能是父节点
                    auto prod = dfs(y, x, left_t - 1); // 寻找target
                    if (prod) return prod * (g[x].size() - 1); // 乘上儿子个数,并直接返回
                }
            }
            return 0; // 未找到target
        };
        auto prod = dfs(1, 0, t);
        return prod ? 1.0 / prod : 0;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 。每个节点都至多递归一次。
- 空间复杂度：$O(n)$ 。

---
### 解法2 BFS
**无向树，但并不一定是以 $1$ 为根的树**，等价于无向无环图。我们将其当做图来处理，将边集转换为邻接表。然后：
- 使用BFS，每次遍历一层的结点，计算概率。需要注意的是，每次弹出一层的结点，计算其下一次跳跃**可达的、未访问过的顶点**数量 $cnt$ ，其实很简单——==由于是树，当前节点的邻接点必定是儿子+其父亲，减去父亲节点的个数就是儿子节点的个数 $cnt$ ，只有节点 $1$ 需要添加一个虚拟的父节点、方便统一处理==。下个可达顶点的访问概率等于当前顶点的访问概率 $p$ 乘以 $1.0 / cnt$ 。当然，为了避免精度丢失，这里使用 `long long` 记录乘积。 
- 使用 $p$ 数组，记录每个节点的访问概率；
- 当访问到 $target$ 时，我们已经知道了访问到它的概率，但还需要判断是否满足**时间限制**：
	- 如果此时刚好用完全部时间，则返回原概率 $p$ ；
	- 如果此时 $target$ 已经没有了可访问的结点，就会原地跳跃：
		- 如果此时还有时间，则返回原概率 $p$ ；
		- 如果没有时间，则返回 $0$ 概率。
- 如果时间已经用完，就不必继续遍历下去，直接退出。

一个有意思的测试用例：
```java 
3
[[2,1],[3,2]]
1
2
```
代码如下：
```cpp
class Solution {
public:
    double frogPosition(int n, vector<vector<int>>& edges, int t, int target) {
        vector<int> g[110];
        long long p[110] = {0, 1}; // p[1] = 1 
        int fa[110] = {0}; // 父节点 fa[1] = 0
        g[1].push_back(0);
        for (auto &e : edges) {
            g[e[0]].push_back(e[1]);  
            g[e[1]].push_back(e[0]);
        }
        queue<int> q;
        q.push(1); 
        while (!q.empty() && t >= 0) {
            int size = q.size(); 
            for (int i = 0; i < size; ++i) {
                int u = q.front(); q.pop();
                int cnt = g[u].size() - 1;
                if (u == target) {
                    if (t == 0) return 1.0 / p[u]; // 刚好到目标
                    else if (t > 0 && !cnt) return 1.0 / p[u]; // 此时还有时间且u为叶子
                    return 0; // 没有时间了
                } 
                for (auto v : g[u]) {
                    if (v == fa[u]) continue;
                    fa[v] = u; // 在遍历的过程中记录父节点(按照遍历顺序)
                    q.push(v); 
                    p[v] = p[u] * cnt;
                } 
            }
            --t; // 经过1秒 // t=0时说明被弹出的下一层节点就是能到达的最远节点
        }
        return 0;
    }
};  
```

---
### Hack
大佬给出的Hack过程：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305252128237.png)
两个Hack用例：
$2^{32} +95168$ 的测试数据
```java
84
[[1,2],[2,3],[3,4],[4,5],[5,6],[6,7],[7,8],[8,9],[9,10],[10,11],[11,12],[12,13],[13,14],[14,15],[15,16],[1,17],[1,18],[1,19],[1,20],[1,21],[1,22],[1,23],[1,24],[1,25],[1,26],[2,27],[2,28],[2,29],[2,30],[2,31],[2,32],[2,33],[2,34],[2,35],[2,36],[3,37],[3,38],[3,39],[3,40],[3,41],[3,42],[3,43],[3,44],[3,45],[3,46],[4,47],[4,48],[4,49],[4,50],[4,51],[4,52],[5,53],[5,54],[5,55],[5,56],[5,57],[5,58],[6,59],[6,60],[6,61],[6,62],[6,63],[6,64],[7,65],[7,66],[7,67],[7,68],[7,69],[7,70],[8,71],[8,72],[8,73],[8,74],[8,75],[8,76],[9,77],[9,78],[10,79],[11,80],[12,81],[13,82],[14,83],[15,84]]
15
84
```
$2^{32}−49216$ 的测试数据（已提交给官方）
```java
94
[[1,2],[2,3],[3,4],[4,5],[5,6],[6,7],[7,8],[8,9],[9,10],[10,11],[11,12],[12,13],[13,14],[14,15],[15,16],[16,17],[17,18],[1,19],[1,20],[1,21],[1,22],[1,23],[1,24],[1,25],[1,26],[1,27],[1,28],[1,29],[1,30],[1,31],[1,32],[1,33],[1,34],[1,35],[1,36],[2,37],[2,38],[2,39],[2,40],[2,41],[2,42],[2,43],[2,44],[2,45],[2,46],[2,47],[2,48],[2,49],[2,50],[2,51],[2,52],[2,53],[2,54],[3,55],[3,56],[3,57],[3,58],[3,59],[3,60],[3,61],[3,62],[3,63],[3,64],[3,65],[3,66],[3,67],[3,68],[3,69],[3,70],[4,71],[4,72],[4,73],[4,74],[5,75],[5,76],[6,77],[6,78],[7,79],[7,80],[8,81],[8,82],[9,83],[9,84],[10,85],[10,86],[11,87],[11,88],[12,89],[13,90],[14,91],[15,92],[16,93],[17,94]]
17
94
```