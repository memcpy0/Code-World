> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given a list of&nbsp;<code>preferences</code>&nbsp;for&nbsp;<code>n</code>&nbsp;friends, where <code>n</code> is always <strong>even</strong>.</p>

<p>For each person <code>i</code>,&nbsp;<code>preferences[i]</code>&nbsp;contains&nbsp;a list of friends&nbsp;<strong>sorted</strong> in the <strong>order of preference</strong>. In other words, a friend earlier in the list is more preferred than a friend later in the list.&nbsp;Friends in&nbsp;each list are&nbsp;denoted by integers from <code>0</code> to <code>n-1</code>.</p>

<p>All the friends are divided into pairs.&nbsp;The pairings are&nbsp;given in a list&nbsp;<code>pairs</code>,&nbsp;where <code>pairs[i] = [x<sub>i</sub>, y<sub>i</sub>]</code> denotes <code>x<sub>i</sub></code>&nbsp;is paired with <code>y<sub>i</sub></code> and <code>y<sub>i</sub></code> is paired with <code>x<sub>i</sub></code>.</p>

<p>However, this pairing may cause some of the friends to be unhappy.&nbsp;A friend <code>x</code>&nbsp;is unhappy if <code>x</code>&nbsp;is paired with <code>y</code>&nbsp;and there exists a friend <code>u</code>&nbsp;who&nbsp;is paired with <code>v</code>&nbsp;but:</p>

<ul>
	<li><code>x</code>&nbsp;prefers <code>u</code>&nbsp;over <code>y</code>,&nbsp;and</li>
	<li><code>u</code>&nbsp;prefers <code>x</code>&nbsp;over <code>v</code>.</li>
</ul>

<p>Return <em>the number of unhappy friends</em>.</p>

 
<p><strong>Example 1:</strong></p>

```bash
Input: n = 4, preferences = [[1, 2, 3], [3, 2, 0], [3, 1, 0], [1, 2, 0]], pairs = [[0, 1], [2, 3]]
Output: 2
Explanation:
Friend 1 is unhappy because:
- 1 is paired with 0 but prefers 3 over 0, and
- 3 prefers 1 over 2.
Friend 3 is unhappy because:
- 3 is paired with 2 but prefers 1 over 2, and
- 1 prefers 3 over 0.
Friends 0 and 2 are happy. 
```
 

<p><strong>Example 2:</strong></p>

```bash
Input: n = 2, preferences = [[1], [0]], pairs = [[1, 0]]
Output: 0
Explanation: Both friends 0 and 1 are happy.
```
 
<p><strong>Example 3:</strong></p>

```bash
Input: n = 4, preferences = [[1, 3, 2], [2, 3, 0], [1, 3, 0], [0, 2, 1]], pairs = [[1, 3], [0, 2]]
Output: 4
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>2 &lt;= n &lt;= 500</code></li>
	<li><code>n</code>&nbsp;is even.</li>
	<li><code>preferences.length&nbsp;== n</code></li>
	<li><code>preferences[i].length&nbsp;== n - 1</code></li>
	<li><code>0 &lt;= preferences[i][j] &lt;= n - 1</code></li>
	<li><code>preferences[i]</code>&nbsp;does not contain <code>i</code>.</li>
	<li>All values in&nbsp;<code>preferences[i]</code>&nbsp;are unique.</li>
	<li><code>pairs.length&nbsp;== n/2</code></li>
	<li><code>pairs[i].length&nbsp;== 2</code></li>
	<li><code>x<sub>i</sub> != y<sub>i</sub></code></li>
	<li><code>0 &lt;= x<sub>i</sub>, y<sub>i</sub>&nbsp;&lt;= n - 1</code></li>
	<li>Each person is contained in <strong>exactly one</strong> pair.</li>
</ul>

题意：给你一份 <code>n</code> 位朋友的亲近程度列表，其中 <code>n</code> 总是 <strong>偶数</strong> 。对每位朋友 <code>i</code>，<code>preferences[i]</code> 包含一份 <strong>按亲近程度从高</strong><strong>到低排列</strong> 的朋友列表。换句话说，排在列表前面的朋友与 <code>i</code> 的亲近程度比排在列表后面的朋友更高。每个列表中的朋友均以 <code>0</code> 到 <code>n-1</code> 之间的整数表示。</p>

<p>所有的朋友被分成几对，配对情况以列表 <code>pairs</code> 给出，其中 <code>pairs[i] = [x<sub>i</sub>, y<sub>i</sub>]</code> 表示 <code>x<sub>i</sub></code> 与 <code>y<sub>i</sub></code> 配对，且 <code>y<sub>i</sub></code> 与 <code>x<sub>i</sub></code> 配对。但是，这样的配对情况可能会使得其中部分朋友感到不开心。在 <code>x</code> 与 <code>y</code> 配对且 <code>u</code> 与 <code>v</code> 配对的情况下，如果同时满足下述两个条件，<code>x</code> 就会不开心：</p>

<ul>
	<li><code>x</code> 与 <code>u</code> 的亲近程度胜过 <code>x</code> 与 <code>y</code>，且</li>
	<li><code>u</code> 与 <code>x</code> 的亲近程度胜过 <code>u</code> 与 <code>v</code></li>
</ul>

<p>返回 <strong>不开心的朋友的数目</strong> 。</p> 

---
### 解法 二维数组+模拟
我们可以遍历每个朋友 `x` ，`[x, y]` 配对；然后内部遍历 `preferences[x]` 列表中的、从最亲近到最不亲近的、朋友 `x` 对其亲近程度大于 `y` 的所有朋友 `u` ，`[u, v]` 配对；要想知道 `x` 是否不开心，就要比较 `x, u` 的亲近程度与 `x, y` 的亲近程度、**`u, x` 的亲近程度与 `u, v` 的亲近程度**——此处仅需要比较后者，原因不用说明。

如何比较呢？如果遍历 `preferences[u]` 列表 ，找到其中 `x, v` 的索引位置……整个算法的时间复杂度就到了 $O(n^3)$ ，绝对通不过OJ。为此，**可以将 `preferences` 转变为一个类似邻接矩阵的结构，用二维数组 `ranks[][]` 或哈希表套哈希表（不太必要）来存储**。

存储什么值呢？利用 `preferences[i]` 按照亲近程度从高到低排序，如果 `i -> j` 比 `i -> k` 更亲近，可以对 `j, k` 在 `preferences[i]` 列表中的下标进行转换，得到亲近得分并满足 `ranks[i][j] < ranks[i][k]` ，从而允许 $O(1)$ 时间的比较。**注意一个朋友只能被统计一次**。

相应代码如下：
```cpp
class Solution {
public:
    int unhappyFriends(int n, vector<vector<int>>& prefers, vector<vector<int>>& pairs) {
        int ans = 0, pairsArr[n], ranks[n][n];
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < n - 1; ++j) 
                ranks[i][prefers[i][j]] = j; //i对v的亲近程度,值越小越亲近
        for (const vector<int> &p : pairs) pairsArr[p[0]] = p[1], pairsArr[p[1]] = p[0];
        for (int x = 0; x < n; ++x) { //遍历所有朋友i
            int y = pairsArr[x]; //x<->y
            for (int j = 0; j < n - 1 && prefers[x][j] != y; ++j) { //从最亲近到最不亲近遍历x的朋友
                int u = prefers[x][j], v = pairsArr[u]; //u<->v; 此处x与u的亲近程度胜过x与y
                if (ranks[u][x] < ranks[u][v]) { //比较u与x的亲近程度、u与v的亲近程度,判断x是否不开心
                    ++ans; 
                    break; //及时退出,避免重复计算人数
                }
            }
        }
        return ans;
    }
};
```
时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(n^2)$ 。运行效率如下：

```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了96.67% 的用户
内存消耗：23.9 MB, 在所有 C++ 提交中击败了88.33% 的用户
```
当然也可以遍历所有配对的二元组合，对于 `pairs[i]` 遍历所有 `j > i` 的 `pairs[j]` ，令 `x = pairs[i][0], y = pairs[i][1], u = pairs[j][0], v = pairs[j][1]` ，然后**一次性检查四个朋友的两两组合 `(x, u), (x, v), (y, u), (y, v)` 是否不开心**——其实也应该发现了，**检查一个朋友 `x` 不开心的条件**（`x` 与 `u` 的亲近度胜过 `x` 与 `y` 且 `u` 与 `x` 的亲近度胜过 `u` 与 `v`），**也能显示出 `u` 是否不开心**（`u` 与 `x` 的亲近度胜过 `u` 与 `v` 且 `x` 与 `u` 的亲近度胜过 `x` 与 `y` ），于是**一次检查能得出有两个人是否一起不开心**。具体代码如下：
```cpp
class Solution {
private:
    int prefer[510][510] = {0}, ans = 0;
    vector<bool> vis;
    void twoFriendsUnhappy(int x, int y, int u, int v) {
        if (prefer[x][u] < prefer[x][y] && prefer[u][x] < prefer[u][v]) { //x,u都不开心
            if (!vis[x]) ++ans, vis[x] = true;
            if (!vis[u]) ++ans, vis[u] = true;
        }
    }
public:
    int unhappyFriends(int n, vector<vector<int>>& preferences, vector<vector<int>>& pairs) {
        for (int i = 0; i < n; ++i) //i和preferences[i][j]的亲近程度为j,越小越亲近 
            for (int j = 0; j < n - 1; ++j)  
                prefer[i][preferences[i][j]] = j; 
        vis.resize(n);
        for (int i = 0; i < pairs.size(); ++i) {
            for (int j = i + 1; j < pairs.size(); ++j) {
                int x = pairs[i][0], y = pairs[i][1], u = pairs[j][0], v = pairs[j][1];
                twoFriendsUnhappy(x, y, u, v);
                twoFriendsUnhappy(x, y, v, u);
                twoFriendsUnhappy(y, x, u, v);
                twoFriendsUnhappy(y, x, v, u);
            }
        }
        return ans;
    }
};
```
时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(n^2)$ 。运行效率如下，突然跑得快了一些：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了98.82% 的用户
内存消耗：23.9 MB, 在所有 C++ 提交中击败了85.06% 的用户
```
