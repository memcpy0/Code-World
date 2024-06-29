> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两组点，其中第一组中有 `size1` 个点，第二组中有 `size2` 个点，且 `size1 >= size2` 。

任意两点间的连接成本 `cost` 由大小为 `size1 x size2` 矩阵给出，其中 `cost[i][j]` 是第一组中的点 `i` 和第二组中的点 `j` 的连接成本。**如果两个组中的每个点都与另一组中的一个或多个点连接，则称这两组点是连通的。** 换言之，第一组中的每个点必须至少与第二组中的一个点连接，且第二组中的每个点必须至少与第一组中的一个点连接。

返回连通两组点所需的最小成本。

**示例 1：**

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/09/20/ex1.jpg)

```js
输入：cost = [[15, 96], [36, 2]]
输出：17
解释：连通两组点的最佳方法是：
1--A
2--B
总成本为 17 。
```
**示例 2：**

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/09/20/ex2.jpg)

```js
输入：cost = [[1, 3, 5], [4, 1, 1], [1, 5, 3]]
输出：4
解释：连通两组点的最佳方法是：
1--A
2--B
2--C
3--A
最小成本为 4 。
请注意，虽然有多个点连接到第一组中的点 2 和第二组中的点 A ，但由于题目并不限制连接点的数目，所以只需要关心最低总成本。
```
**示例 3：**
```js
输入：cost = [[2, 5, 1], [3, 4, 7], [8, 1, 2], [6, 2, 4], [3, 8, 8]]
输出：10
```
**提示：**
- `size1 == cost.length`
- `size2 == cost[i].length`
- `1 <= size1, size2 <= 12`
- `size1 >= size2`
- `0 <= cost[i][j] <= 100`

---
相似题目（状压 DP）：
- 1879. 两个数组最小的异或值之和
- 2172. 数组的最大与和
- 1125. 最小的必要团队
- 2305. 公平分发饼干
- 1494. 并行课程 II
- LCP 53. 守护太空城
- 1986. 完成任务的最少工作时间段

> 以后可以补两个解法，时间复杂度O(n^3)的二分图匹配，或者网络流min(O(n ^ 3), O(n * n * m * log(m)))。由于牵涉到数据转换，转换后可能是稀疏图，m可能远小于n。
### 解法 记忆化搜索
#### 1. 寻找子问题
下文中 $n=\textit{size}_1$ ，$m=\textit{size}_2$ 。假设 $n=5,m=3$ 。第一组的点编号为 $0,1,2,3,4$ ，第二组的点编号为 $0,1,2$ 。

**用「枚举选哪个」来思考**。考虑第一组的最后一个点（$4$），可以枚举它和第二组的 $0,1,2$ 相连。假设和第二组的 $1$ 相连，那么问题变成：「第一组的 $0,1,2,3$ 和第二组的 $0,1,2$ 相连，且第二组的 $0,2$ 未被连接时，最小成本是多少」。然后考虑第一组的点 $3$ 和第二组的哪个相连（可以连之前连过的点），接着考虑第一组的点 $2$，点 $1$，最后是点 $0$ 。

第一组的点全部连完后，==第二组的某些点可能未被连接，这些点可以去第一组找个成本最小的点连上==。

上述做法**枚举了第一组的每个点连接第二组的所有情况，并在最后用贪心的思路处理第二组剩余没有连的点**，所以一定可以算出（枚举出）最优解。
#### 2. 状态定义与状态转移方程
根据上面的讨论，定义 $\textit{dfs}(i,j)$ 表示第一组的 $0,1,\cdots,i$ 和第二组的 $0,1,\cdots,m-1$ 相连，且第二组的集合 $j$ 中元素未被连接时，最小成本是多少。

枚举第一组的点 $i$ 和第二组的 $0,1,\cdots,m-1$ 其中一个点相连，取最小值，即
$$\textit{dfs}(i,j) =\min_{k=0}^{m-1} \textit{dfs}(i-1,j\setminus\{k\}) + \textit{cost}[i][k]$$
​其中 $j\setminus\{k\}$ 表示从集合 $j$ 中去掉元素 $k$ 后的集合。注意，如果 $k$ 不在 $j$ 中，那么 $j$ 不变。

递归边界：**第一组点已经连接完了**，我们设集合 $j$ 中第二组的点 $x$ 与第一组的点连接时，最小成本是 $\textit{minCost}[x]$ ，那么有
$$\textit{dfs}(-1,j) = \sum_{k\in j} \textit{minCost}[k]$$
递归入口：$\textit{dfs}(n-1,\{0,1,\cdots,m-1\})$ 。

代码实现时，$minCost$ 数组可以提前预处理出来。
#### 3. 记忆化搜索
下文用 $x-y$ 表示第一组的点 $x$ 连第二组的点 $y$ 。

假设 $n=5,m=3$ 。由于无论是先 $4-0$ 再 $3-1$ ，还是先 $4-1$ 再 $3-0$ ，都会递归到 $\textit{dfs}(2,\{2\})$ 这个状态上。一叶知秋，整个递归中有大量重复递归调用（递归入参相同）。由于递归函数没有副作用，同样的入参无论计算多少次，算出来的结果都是一样的，因此可以用记忆化搜索来优化：
- 如果一个状态（递归入参）是第一次遇到，那么可以在返回前，把状态及其结果记到一个 $\textit{memo}$ 数组（或哈希表）中。
- 如果一个状态不是第一次遇到，那么直接返回 $\textit{memo}$ 中保存的结果。

> 问：能不能枚举第二组的点，去连接第一组的点？
> 答：也可以，但这样做的时间复杂度是 $\mathcal{O}(nm2^n)$ ，相比 $\mathcal{O}(nm2^m)$ 更慢。注意本题 $n\ge m$ 。

```cpp
class Solution {
public:
    int connectTwoGroups(vector<vector<int>>& cost) {
        int n = cost.size(), m = cost[0].size();
        vector<int> minCost(m, INT_MAX);
        for (int j = 0; j < m; ++j)
            for (auto &c : cost)
                minCost[j] = min(minCost[j], c[j]);

        vector<vector<int>> memo(n, vector<int>(1 << m, INT_MAX));
        function<int(int, int)> dfs = [&](int i, int j) -> int {
            if (i < 0) {
                int ans = 0;
                for (int k = 0; k < m; ++k)
                    if (j >> k & 1) // 第二组的点集中k未连接
                        ans += minCost[k]; // 去第一组找个成本最小的点连接
                return ans;
            }
            int &ans = memo[i][j]; // 引用
            if (ans != INT_MAX) return ans; // 之前算过了
            for (int k = 0; k < m; ++k) // 第一组的点i与第二组的点k
                ans = min(ans, dfs(i - 1, j & ~(1 << k)) + cost[i][k]);
            return ans;
        };
        return dfs(n - 1, (1 << m) - 1);
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(nm2^m)$，其中 $n$ 和 $m$ 分别为 $\textit{cost}$ 的行数和列数。动态规划的时间复杂度 == 状态个数 $\times$ 单个状态的计算时间。本题中状态个数等于 $\mathcal{O}(n2^m)$ ，单个状态的计算时间为 $\mathcal{O}(m)$ ，所以总的时间复杂度为 $\mathcal{O}(nm2^m)$ 。
- 空间复杂度：$\mathcal{O}(n2^m)$。

---
### 解法2 递推
去掉递归中的「递」，只保留「归」的部分，即自底向上计算。通用做法：
- $\textit{dfs}$ 改成 $f$ 数组；
- 递归改成循环；
- 递归边界改成 $f$ 的初始状态。相当于原来是用递归去计算每个状态，现在是用循环去计算每个状态。

具体来说，$f[i][j]$ 的含义和状态转移方程和上面的记忆化搜索是一样的，即：
$$f[i][j] =\min_{k=0}^{m-1} f[i-1][j\setminus\{k\}] + \textit{cost}[i][k]$$
但当 $i=0$ 时，等号右边会出现负数下标。或者说，这种定义方式没有状态能表示递归边界。

解决办法：在 $f$ 数组的上边加一排，把原来的 $f[i]$ 改成 $f[i+1]$ ，$f[i-1]$ 改成 $f[i]$ 。此时 $f[0][j]$ 就对应着 $\textit{dfs}(-1,j)$ 。

修改后的递推式为
$$f[i+1][j] =\min_{k=0}^{m-1} f[i][j\setminus\{k\}] + \textit{cost}[i][k]$$ 
注意只需要把 $f$ 中的 $i$ 加一，$\textit{cost}$ 中的 $i$ 不受影响。初始值 $$f[0][j]= \sum\limits_{k\in j} \textit{minCost}[k]$$
答案为 $f[n][\{0,1,\cdots,m-1\}]$ 。
```cpp
class Solution {
public:
    int connectTwoGroups(vector<vector<int>>& cost) {
        int n = cost.size(), m = cost[0].size();
        vector<int> minCost(m, INT_MAX);
        for (int j = 0; j < m; ++j)
            for (auto &c : cost)
                minCost[j] = min(minCost[j], c[j]);

        vector<vector<int>> f(n + 1, vector<int>(1 << m));
        for (int j = 0; j < 1 << m; ++j) 
            for (int k = 0; k < m; ++k)
                if (j >> k & 1) // 第二组的点k未连接
                    f[0][j] += minCost[k]; //去第一组找个成本最小的点连接
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < 1 << m; ++j) {
                int ans = INT_MAX;
                for (int k = 0; k < m; ++k) // 第一组的点i与第二组的点k连接
                    ans = min(ans, f[i][j & ~(1 << k)] + cost[i][k]);
                f[i + 1][j] = ans;
            }
        }
        return f[n][(1 << m) - 1];
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(nm2^m)$，其中 $n$ 和 $m$ 分别为 $\textit{cost}$ 的行数和列数。动态规划的时间复杂度 == 状态个数 $\times$ 单个状态的计算时间。本题中状态个数等于 $\mathcal{O}(n2^m)$ ，单个状态的计算时间为 $\mathcal{O}(m)$ ，所以总的时间复杂度为 $\mathcal{O}(nm2^m)$ 。
- 空间复杂度：$\mathcal{O}(n2^m)$。

#### 两个优化
由于 $f[i+1]$ 只和 $f[i]$ 有关，因此可以仿照 0-1 背包，优化掉第一个维度，只用一个长为 $2^m$ 的一维数组。

去掉第一个维度后，考虑 $f$ 数组初始值的计算，**还可以进一步优化**。

假设现在算出了 $f[00],f[01],f[10],f[11]$（这里用二进制数表示集合），那么结合 $\textit{minCost}[2]$ ，可以算出
$$\begin{aligned}
f[100] = f[00] + \textit{minCost}[2]\\ f[101] = f[01] + \textit{minCost}[2]\\ f[110] = f[10] + \textit{minCost}[2]\\ f[111] = f[11] + \textit{minCost}[2]\end{aligned}$$
这样就得到了 $f[0]$ 到 $f[111]$ 。按照同样的方式，结合 $\textit{minCost}[3]$ ，可以算出 $f[0]$ 到 $f[1111]$ 。依此类推。

这样每个初始值只需要 $\mathcal{O}(1)$ 的时间就能递推算出来。
```cpp
class Solution {
public:
    int connectTwoGroups(vector<vector<int>>& cost) {
        int n = cost.size(), m = cost[0].size();
        int f[1 << m];
        f[0] = 0;
        for (int j = 0; j < m; ++j) {
            int mn = INT_MAX;
            for (auto &c : cost) mn = min(mn, c[j]);
            int bit = 1 << j;
            for (int mask = 0; mask < bit; ++mask)
                f[bit | mask] = f[mask] + mn;
        }
        for (auto &row : cost) {
            for (int j = (1 << m) - 1; j >= 0; --j) {
                int ans = INT_MAX;
                for (int k = 0; k < m; ++k) // 第一组的点i与第二组的点k
                    ans = min(ans, f[j & ~(1 << k)] + row[k]);
                f[j] = ans;
            }
        }
        return f[(1 << m) - 1];
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(nm2^m)$ ，其中 $n$ 和 $m$ 分别为 $\textit{cost}$ 的行数和列数。计算 $f$ 数组的初始值，循环次数为 $2^0+2^1+2^2\cdots+2^{m-1} = 2^m-1$ ，这比优化前的 $m2^m$ 要快。动态规划的时间复杂度 = 状态个数 $\times$ 单个状态的计算时间。本题中状态个数等于 $O(n2^m)$ ，单个状态的计算时间为 $\mathcal{O}(m)$ ，所以总的时间复杂度为 $\mathcal{O}(nm2^m)$ 。
- 空间复杂度：$\mathcal{O}(2^m)$ 。
