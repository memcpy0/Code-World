> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

There is an infrastructure of <code>n</code> cities with some number of <code>roads</code> connecting these cities. Each <code>roads[i] = [a<sub>i</sub>, b<sub>i</sub>]</code> indicates that there is a bidirectional road between cities <code>a<sub>i</sub></code> and <code>b<sub>i</sub></code>.

The <strong>network rank</strong><em> </em>of <strong>two different cities</strong> is defined as the total number of&nbsp;<strong>directly</strong> connected roads to <strong>either</strong> city. If a road is directly connected to both cities, it is only counted <strong>once</strong>.

The <strong>maximal network rank </strong>of the infrastructure is the <strong>maximum network rank</strong> of all pairs of different cities.

Given the integer <code>n</code> and the array <code>roads</code>, return <em>the <strong>maximal network rank</strong> of the entire infrastructure</em>.

<strong>Example 1:</strong>
 <img style="width: 292px; height: 172px;" src="https://assets.leetcode.com/uploads/2020/09/21/ex1.png" alt="">
```cpp 
Input: n = 4, roads = [[0,1],[0,3],[1,2],[1,3]]
Output: 4
Explanation: The network rank of cities 0 and 1 is 4 as there are 4 roads that are connected to either 0 or 1. The road between 0 and 1 is only counted once.
```
<strong>Example 2:</strong> 
 <img style="width: 292px; height: 172px;" src="https://assets.leetcode.com/uploads/2020/09/21/ex2.png" alt=""> 
```cpp Input: n = 5, roads = [[0,1],[0,3],[1,2],[1,3],[2,3],[2,4]]
Output: 5
Explanation: There are 5 roads that are connected to cities 1 or 2.
```
<strong>Example 3:</strong>
```cpp Input: n = 8, roads = [[0,1],[1,2],[2,3],[2,4],[5,6],[5,7]]
Output: 5
Explanation: The network rank of 2 and 5 is 5. Notice that all the cities do not have to be connected.
```
<strong>Constraints:</strong>
<ul>
	<li><code>2 &lt;= n &lt;= 100</code></li>
	<li><code>0 &lt;= roads.length &lt;= n * (n - 1) / 2</code></li>
	<li><code>roads[i].length == 2</code></li>
	<li><code>0 &lt;= a<sub>i</sub>, b<sub>i</sub>&nbsp;&lt;= n-1</code></li>
	<li><code>a<sub>i</sub>&nbsp;!=&nbsp;b<sub>i</sub></code></li>
	<li>Each&nbsp;pair of cities has <strong>at most one</strong> road connecting them.</li>
</ul>
题意：`n` 座城市和一些连接这些城市的道路 `roads` 共同构成了一个基础设施网络。 每个 <code>roads[i] = [a<sub>i</sub>, b<sub>i</sub>]</code> 都表示在城市 <code>a<sub>i</sub></code> 和 <code>b<sub>i</sub></code> 之间有一条双向道路。

两座不同城市构成的 <strong>城市对</strong> 的 <strong>网络秩</strong> 定义为：与这两座城市 <strong>直接</strong> 相连的道路总数。每对城市之间 <strong>最多只有一条</strong> 道路直接相连。如果存在一条道路直接连接这两座城市，则这条道路只计算 <strong>一次</strong> 。

整个基础设施网络的 <strong>最大网络秩</strong> 是所有不同城市对中的 <strong>最大网络秩</strong> 。给出整数 <code>n</code> 和数组 <code>roads</code>，返回整个基础设施网络的 <strong>最大网络秩</strong> 。
 
---
枚举每一对城市，计算和它们连接的道路总数（网络秩）——即**两个城市的度之和**。如果有一条边直接连接着这两座城市，**道路总数减一**。要求的是所有城市对中最大的网络秩。以例2为例：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303170958854.png)

### 解法1 枚举+邻接表/矩阵
用邻接表来存储这些城市（顶点）和它们之间的道路（双向边），一个城市的邻接点数即为它的度数。只是稍慢一点，**需要用一个额外的循环**检查两座城市之间是否有直通道路。
```cpp
class Solution {
public:
    int maximalNetworkRank(int n, vector<vector<int>>& roads) {
        vector<vector<int>> adjList(n);
        for (int i = 0; i < roads.size(); ++i) {
            int ai = roads[i][0], bi = roads[i][1];
            adjList[ai].push_back(bi);
            adjList[bi].push_back(ai);
        }
        int maxRank = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int tempRank = adjList[i].size() + adjList[j].size();
                for (const int &v : adjList[i]) if (v == j) --tempRank;
                maxRank = max(tempRank, maxRank);
            }
        }
        return maxRank; 
    }
}; 
```
提交后效率如下：
```cpp
执行用时：220 ms, 在所有 C++ 提交中击败了50.61% 的用户
内存消耗：30.6 MB, 在所有 C++ 提交中击败了47.92% 的用户
```
我们知道，从邻接表易知顶点的度数（入度或者出度），不容易判断两顶点是否有边直接连接。而邻接矩阵易判断两顶点之间是否有边，但难以知道一个顶点的度数，也浪费存储空间……由于本题数据量小，可以**使用邻接矩阵**高效判断两不同城市之间是否有道路，然后需要**使用一个度数数组**计算顶点度数， 两者相结合得到更高效的解法。代码如下：
```cpp
class Solution {
public:
    int maximalNetworkRank(int n, vector<vector<int>>& roads) {
        vector<int> degrees(n);
        vector<vector<bool>> hasEdge(n, vector<bool>(n));
        for (const vector<int>& road : roads) {
            int a = road[0], b = road[1];
            hasEdge[a][b] = hasEdge[b][a] = true;
            ++degrees[a], ++degrees[b];
        }
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                int rank = degrees[i] + degrees[j] - (hasEdge[i][j] ? 1 : 0);
                if (rank > ans) ans = rank;
            }
        }
        return ans;
    }
};
```
---
### 解法2 枚举优化（贪心）
我们可以对解法一中的方法继续优化。从网络秩的计算公式 `degrees[i] + degrees[j] - hasEdge[i][j]` 来看，网络秩要想达到最大，那么 `degrees[i]` 和 `degrees[j]` 都最大，并且 `hasEdge[i][j]` **最好**为0。即最大网络秩一定存在于 `degrees` 的最大值 `first` 和次大值 `second` 对应的城市之间（**最大度数+最大度数+无直接边** > **最大度数+最大度数+有直接边** > **最大度数+次大度数+无直接边**），==只要记录这些城市，枚举这些城市对，就能减少枚举复杂度==。这是一种空间换时间的策略。相连道路数为 `first` 和 `second` 可能不止一个城市，因此**应记录对应的所有城市**。

为什么说**只需考虑度数为最大值与次大值的城市，其余城市无须考虑**？设 $first$ 表示所有节点中度数的最大值，$second$ 表示所有节点中度数的次大值，原因如下：
- 已知最大值 $first$ 与次大值 $second$ ，则此时可知，当前**最差情况**下，假设这两城市存在连接，则最大的网络秩为 $first+second−1$ ；
- 假设存在度数比 $second$ 小的城市 $x$ ，则 $degree[x] <  second$ ，此时含有 $x$ 构成的城市对的最大网络秩不超过 $degree[x]+first$ ，此时一定满足 $degree[x]+first≤second+first$ ；

综上可以得出结论，**选择最大或者次大度数的城市一定是最优的**。我们可以求出度数为 $first$ 的城市集合 `firstArr` ，同时求出度数为  $second$ 的城市集合 `secondArr` 。设城市的总数量为 $n$ ，道路的总数量为 $m$ ，集合 `firstArr` 的数量为 $x$ ，则此时该集合可以构造的城市对数量为 $\dfrac{x(x-1)}{2}$ ，分以下几种情况来讨论:
- 如果 $x=1$ ，此时我们必须选择 `firstArr` 中唯一的城市，**另一个城市只能在 `secondArr` 中选择**，枚举 `secondArr` 中的每个城市(`secondArr` 可能只对应一座或多座城市），找到最大的网络秩即可，此时需要的时间复杂度为 $O(n)$ ；
- 如果 $x>1$ 时，那么最大网络秩一定在 `firstArr` 的城市对中产生。分类讨论如下：
	- 如果满足 $\binom{x}{2} > m$ 时，此时集合 `firstArr` 一定存在一对城市，他们之间没有道路连接，此时最大的网络秩即为 $2×first$ ；
	- 如果满足 $\binom{x}{2} \le m$ 时，此时枚举集合 `firstArr` 中所有不同的城市对即可，**不需要再考虑次大的城市集合** `secondArr` ，因为此时一定满足 $2 \times \textit{first} - 1 \ge \textit{first} + \textit{second} > 2 \times \textit{second}$ ，此时时间复杂度不超过 $O(\dfrac{x(x-1)}{2}) \le O(m)$ ；

下图橙色部分就是最大道路数组成的城市对，只需要遍历这些城市对即可。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303180116746.png)

```cpp
class Solution {
public:
    int maximalNetworkRank(int n, vector<vector<int>>& roads) {
        vector<vector<bool>> connect(n, vector<bool>(n, false)); // 任意两个城市之间是否存在直接相连道路
        vector<int> degree(n); // 与每个城市直接相连的道路数(顶点度数)
        for (auto road : roads) {
            int x = road[0], y = road[1];
            connect[x][y] = connect[y][x] = true;
            ++degree[x], ++degree[y];
        } 
        int first = -1, second = -2; // 最大相连数 // 次大相连数   
        vector<int> firstArr, secondArr; // 相连道路数为最大、次大值的所有城市
        for (int i = 0; i < n; ++i) {
            if (degree[i] > first) {
                second = first;
                secondArr = firstArr;
                first = degree[i];
                firstArr.clear();
                firstArr.emplace_back(i);
            } else if (degree[i] == first) {
                firstArr.emplace_back(i);
            } else if (degree[i] > second){
                secondArr.clear();
                second = degree[i];
                secondArr.emplace_back(i);
            } else if (degree[i] == second) {
                secondArr.emplace_back(i);
            }
        }
        // 最大值只有一个元素,枚举所有次大值城市(可能一个或多个),网络秩=最大值+次大值-两个城市相连道路数
        if (firstArr.size() == 1) { // 最大值的城市个数
            int u = firstArr[0];
            for (int v : secondArr)
                if (!connect[u][v])
                    return first + second; 
            return first + second - 1;
        } else { // 最大值不止一个元素,枚举所有最大值的城市对,网络秩=2*最大值两个城市相连道路数
            int m = roads.size();
            if (firstArr.size() * (firstArr.size() - 1) / 2 > m) { // 存在一对城市没有直接相连道路
                return first * 2;
            }
            for (int u: firstArr)
                for (int v: firstArr)
                    if (u != v && !connect[u][v])
                        return first * 2; 
            return first * 2 - 1;
        }
    }
};
```
复杂度分析：
- 时间复杂度：$O(n + m)$ ，其中 $n$ 表示城市数，$m$ 表示城市之间的道路总数。计算城市度数需要 $O(m)$ ，找到城市中最大度数和次大度数城市集合需要 $O(n)$ ，计算城市对中最大网络秩需要 $O(m)$ ，因此总的时间复杂度为 $O(m + n)$ 。
- 空间复杂度：$O(n^2)$ 。记录图中所有的城市之间的联通关系，需要 $O(n^2)$ ，记录所有节点的度需要 $O(n)$ ，记录最大度数与次大度数的城市集合需要 $O(n)$ ，因此总的空间复杂度为 $O(n^2)$ 。