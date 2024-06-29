> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>You are given a <strong>0-indexed</strong> 2D array <code>grid</code> of size <code>2 x n</code>, where <code>grid[r][c]</code> represents the number of points at position <code>(r, c)</code> on the matrix. Two robots are playing a game on this matrix.</p>

<p>Both robots initially start at <code>(0, 0)</code> and want to reach <code>(1, n-1)</code>. Each robot may only move to the <strong>right</strong> (<code>(r, c)</code> to <code>(r, c + 1)</code>) or <strong>down </strong>(<code>(r, c)</code> to <code>(r + 1, c)</code>).</p>

<p>At the start of the game, the <strong>first</strong> robot moves from <code>(0, 0)</code> to <code>(1, n-1)</code>, collecting all the points from the cells on its path. For all cells <code>(r, c)</code> traversed on the path, <code>grid[r][c]</code> is set to <code>0</code>. Then, the <strong>second</strong> robot moves from <code>(0, 0)</code> to <code>(1, n-1)</code>, collecting the points on its path. Note that their paths may intersect with one another.</p>

<p>The <strong>first</strong> robot wants to <strong>minimize</strong> the number of points collected by the <strong>second</strong> robot. In contrast, the <strong>second </strong>robot wants to <strong>maximize</strong> the number of points it collects. If both robots play <strong>optimally</strong>, return <em>the <b>number of points</b> collected by the <strong>second</strong> robot.</em></p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 388px; height: 103px;" src="https://assets.leetcode.com/uploads/2021/09/08/a1.png" alt="">

```cpp
Input: grid = [[2,5,4],[1,5,1]]
Output: 4
Explanation: The optimal path taken by the first robot is shown in red, and the optimal path taken by the second robot is shown in blue.
The cells visited by the first robot are set to 0.
The second robot will collect 0 + 0 + 4 + 0 = 4 points.
```

 
<p><strong>Example 2:</strong></p>
<img style="width: 384px; height: 105px;" src="https://assets.leetcode.com/uploads/2021/09/08/a2.png" alt="">

```cpp
Input: grid = [[3,3,1],[8,5,2]]
Output: 4
Explanation: The optimal path taken by the first robot is shown in red, and the optimal path taken by the second robot is shown in blue.
The cells visited by the first robot are set to 0.
The second robot will collect 0 + 3 + 1 + 0 = 4 points.
```

 
<p><strong>Example 3:</strong></p>
<img style="width: 493px; height: 103px;" src="https://assets.leetcode.com/uploads/2021/09/08/a3.png" alt="">

```cpp
Input: grid = [[1,3,1,15],[1,3,3,1]]
Output: 7
Explanation: The optimal path taken by the first robot is shown in red, and the optimal path taken by the second robot is shown in blue.
The cells visited by the first robot are set to 0.
The second robot will collect 0 + 1 + 3 + 3 + 0 = 7 points.
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>grid.length == 2</code></li>
	<li><code>n == grid[r].length</code></li>
	<li><code>1 &lt;= n &lt;= 5 * 10<sup>4</sup></code></li>
	<li><code>1 &lt;= grid[r][c] &lt;= 10<sup>5</sup></code></li>
</ul>

题意：给你一个下标从 <strong>0</strong> 开始的二维数组 <code>grid</code> ，数组大小为 <code>2 x n</code> ，其中 <code>grid[r][c]</code> 表示矩阵中 <code>(r, c)</code> 位置上的点数。现在有两个机器人正在矩阵上参与一场游戏。

<p>两个机器人初始位置都是 <code>(0, 0)</code> ，目标位置是 <code>(1, n-1)</code> 。每个机器人只会 <strong>向右</strong> (<code>(r, c)</code> 到 <code>(r, c + 1)</code>) 或 <strong>向下 </strong>(<code>(r, c)</code> 到 <code>(r + 1, c)</code>) 。</p>

<p>游戏开始，<strong>第一个</strong> 机器人从 <code>(0, 0)</code> 移动到 <code>(1, n-1)</code> ，并收集路径上单元格的全部点数。对于路径上所有单元格 <code>(r, c)</code> ，途经后 <code>grid[r][c]</code> 会重置为 <code>0</code> 。然后，<strong>第二个</strong> 机器人从 <code>(0, 0)</code> 移动到 <code>(1, n-1)</code> ，同样收集路径上单元的全部点数。注意，它们的路径可能会存在相交的部分。</p>

<p><strong>第一个</strong> 机器人想要打击竞争对手，使 <strong>第二个</strong> 机器人收集到的点数 <strong>最小化</strong> 。与此相对，<strong>第二个</strong> 机器人想要 <strong>最大化</strong> 自己收集到的点数。两个机器人都发挥出自己的 <strong>最佳水平</strong>&nbsp;的前提下，返回 <strong>第二个</strong> 机器人收集到的 <strong>点数</strong> <em>。</em></p> 



---
### 解法 前缀和
我太蠢了，周赛时竟然没有想出来这道题的做法，虽然周赛一结束就想通了……只能说，当时我拐进动态规划的死角中出不来了，虽然类似这种题目——在一个矩阵中从左上角到右下角，只能向下或向右行走，最大化地收集分数，都是用动态规划解决的。

不过本题很显然有点特殊，矩阵只有两行；先走的第一个机器人要让第二个机器人收集的点数最少，且它走过的路径上的单元都会置零；后走的第二个机器人要最大化自己的点数。于是一个事实“显而易见”，**第一个机器人从 `(0, 0)` 移动到 `(1, n - 1)` ，只能向下或向右移动……那就只有 `n` 条可选的路径**，即先往右走一段，再向下一格，再往右一直走到目的地。

要让第二个机器人收集的点数最小化，说明第一个机器人要走这 `n` 条路径之一——假设它在 `(0, i)` 格下移，则 `(0, 0)` 到 `(0, i)` 全部置零，`(1, i)` 到 `(1, n - 1)` 全部置零，而**第二个机器人为了最大化收集点数，就只有两条可选的路径**：要么先向右走到 `(0, n - 1)` 再下移到 `(1, n - 1)` 、要么先下移到 `(1, 0)` 再往右走到 `(1, n - 1)` ，收集的点数要么从 `(0, i + 1)` 到 `(0, n - 1)` （设为 `a` ）、要么从 `(1, 0)` 到 `(1, i - 1)`（设为 `b` ） ，取决于两者之间的最大值即 `max(a, b)` 。
![在这里插入图片描述](https://img-blog.csdnimg.cn/fbef1f66eb2448f689601ba683a35fe1.png)
因此，第一个机器人要下移的位置需要使 `max(a, b)` 最小化，之后第二个机器人收集的点数即为此时的 `max(a, b)` 。如果暴力计算这 `n` 条路径走后各自的 `max(a, b)` ，时间复杂度为 $O(n^2)$ 。通过预处理出 `grid[0][0]` 到 `grid[0][n - 1]` 、`grid[1][0]` 到 `grid[1][n - 1]` 的前缀和，可以优化算法复杂度到 $O(n)$ 时间和 $O(n)$ 空间：
```cpp
//C++ version
class Solution {
public:
    long long gridGame(vector<vector<int>>& grid) {
        using ll = long long;
        int n = grid[0].size();
        ll ans = numeric_limits<ll>::max();
        vector<ll> up(n + 1), down(n + 1);
        for (int i = 0; i < n; ++i) {
            up[i + 1] = up[i] + grid[0][i];
            down[i + 1] = down[i] + grid[1][i];
        }
        for (int i = 0; i < n; ++i) //假设从(0,i)点下移
            ans = min(ans, max(up.back() - up[i + 1], down[i] - down[0]));
        return ans;
    }
};
//执行用时：136 ms, 在所有 C++ 提交中击败了58.46% 的用户
//内存消耗：69.2 MB, 在所有 C++ 提交中击败了45.21% 的用户
```
仔细思索，其实还能优化一下空间。如下代码的时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    long long gridGame(vector<vector<int>>& grid) {
        using ll = long long;
        int n = grid[0].size();
        ll ans = numeric_limits<ll>::max(), upsum = 0, down = 0;
        for (int i = 0; i < n; ++i) upsum += grid[0][i];
        for (int i = 0; i < n; ++i) { //假设从(0,i)点下移
            upsum -= grid[0][i];
            ans = min(ans, max(upsum, down));
            down += grid[1][i];
        }
        return ans;
    }
};
//执行用时：112 ms, 在所有 C++ 提交中击败了99.30% 的用户
//内存消耗：61.5 MB, 在所有 C++ 提交中击败了77.48% 的用户
```
