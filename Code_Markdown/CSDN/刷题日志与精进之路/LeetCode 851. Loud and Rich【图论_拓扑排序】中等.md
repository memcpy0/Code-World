> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>There is a group of <code>n</code> people labeled from <code>0</code> to <code>n - 1</code> where each person has a different amount of money and a different level of quietness.</p>

<p>You are given an array <code>richer</code> where <code>richer[i] = [a<sub>i</sub>, b<sub>i</sub>]</code> indicates that <code>a<sub>i</sub></code> has more money than <code>b<sub>i</sub></code> and an integer array <code>quiet</code> where <code>quiet[i]</code> is the quietness of the <code>i<sup>th</sup></code> person. All the given data in richer are <strong>logically correct</strong> (i.e., the data will not lead you to a situation where <code>x</code> is richer than <code>y</code> and <code>y</code> is richer than <code>x</code> at the same time).</p>

<p>Return <em>an integer array </em><code>answer</code><em> where </em><code>answer[x] = y</code><em> if </em><code>y</code><em> is the least quiet person (that is, the person </em><code>y</code><em> with the smallest value of </em><code>quiet[y]</code><em>) among all people who definitely have equal to or more money than the person </em><code>x</code>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: richer = [[1,0],[2,1],[3,1],[3,7],[4,3],[5,3],[6,3]], quiet = [3,2,5,4,6,1,7,0]
Output: [5,5,2,5,4,5,6,7]
Explanation: 
answer[0] = 5.
Person 5 has more money than 3, which has more money than 1, which has more money than 0.
The only person who is quieter (has lower quiet[x]) is person 7, but it is not clear if they have more money than person 0.
answer[7] = 7.
Among all people that definitely have equal to or more money than person 7 (which could be persons 3, 4, 5, 6, or 7), the person who is the quietest (has lower quiet[x]) is person 7.
The other answers can be filled out with similar reasoning.
```
<p><strong>Example 2:</strong></p>

```cpp
Input: richer = [], quiet = [0]
Output: [0]
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == quiet.length</code></li>
	<li><code>1 &lt;= n &lt;= 500</code></li>
	<li><code>0 &lt;= quiet[i] &lt; n</code></li>
	<li>All the values of <code>quiet</code> are <strong>unique</strong>.</li>
	<li><code>0 &lt;= richer.length &lt;= n * (n - 1) / 2</code></li>
	<li><code>0 &lt;= a<sub>i</sub>, b<sub>i</sub> &lt; n</code></li>
	<li><code>a<sub>i </sub>!= b<sub>i</sub></code></li>
	<li>All the pairs of <code>richer</code> are <strong>unique</strong>.</li>
	<li>The observations in <code>richer</code> are all logically consistent.</li>
</ul>

题意：有一组 <code>n</code> 个人作为实验对象，从 <code>0</code> 到 <code>n - 1</code> 编号，其中每个人都有不同数目的钱，以及不同程度的安静值（quietness）。为了方便起见，我们将编号为 <code>x</code> 的人简称为 "person <code>x</code> "。

<p>给你一个数组 <code>richer</code> ，其中 <code>richer[i] = [a<sub>i</sub>, b<sub>i</sub>]</code> 表示 person <code>a<sub>i</sub></code> 比 person <code>b<sub>i</sub></code> 更有钱。另给你一个整数数组 <code>quiet</code> ，其中 <code>quiet[i]</code> 是 person <code>i</code> 的安静值。<code>richer</code> 中所给出的数据 <strong>逻辑自洽</strong>（也就是说，在 person <code>x</code> 比 person <code>y</code> 更有钱的同时，不会出现 person <code>y</code> 比 person <code>x</code> 更有钱的情况 ）。</p>

<p>现在，返回一个整数数组 <code>answer</code> 作为答案，其中 <code>answer[x] = y</code> 的前提是，在所有拥有的钱肯定不少于 person <code>x</code> 的人中，person <code>y</code> 是最安静的人（也就是安静值 <code>quiet[y]</code> 最小的人）。</p>


---
> 祝大家在喧嚣的世界里富有而宁静。

暴力做法是：使用邻接表建图，对每组 <code>richer[i] = [a<sub>i</sub>, b<sub>i</sub>]</code> ，添加一条从 `b` 到 `a` 的有向边（没钱指向有钱）。然后遍历每个节点 `i` ，并依次以其为起点进行BFS，在所有比节点 `i` 更有钱的人中找到最安静的人。由于时间复杂度比较大，会TLE：
```cpp

class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        vector<int> g[n];
        for (int i = 0, m = richer.size(); i < m; ++i) {
            int u = richer[i][0], v = richer[i][1];
            g[v].push_back(u); // u比v更有钱
        }
        vector<int> ans;
        queue<int> q;
        vector<bool> vis(n);
        for (int i = 0; i < n; ++i) {
            while (!q.empty()) q.pop();
            fill(vis.begin(), vis.end(), false);
            int idx = -1;
            q.push(i);
            while (!q.empty()) {
                int u = q.front(); q.pop();
                if (idx == -1 || quiet[u] < quiet[idx]) idx = u;
                for (int j = 0, sz = g[u].size(); j < sz; ++j) {
                    int v = g[u][j];
                    if (!vis[v]) q.push(v);
                }
            }
            ans.push_back(idx);
        }
        return ans;
    }
};
```
### 解法1 记忆化搜索


---
### 解法2 拓扑排序
