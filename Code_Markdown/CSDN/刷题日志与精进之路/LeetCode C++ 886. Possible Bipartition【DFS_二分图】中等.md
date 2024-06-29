<p>Given a set of <code>N</code>&nbsp;people (numbered <code>1, 2, ..., N</code>), we would like to split everyone into two groups of <strong>any</strong> size.</p>

<p>Each person may dislike some other people, and they should not go into the same group.&nbsp;</p>

<p>Formally, if <code>dislikes[i] = [a, b]</code>, it means it is not allowed to put the people numbered <code>a</code> and <code>b</code> into the same group.</p>

<p>Return <code>true</code>&nbsp;if and only if it is possible to split everyone into two groups in this way.</p>

 <p><strong>Example 1:</strong></p>

```clike
Input: N = 4, dislikes = [[1,2],[1,3],[2,4]]
Output: true
Explanation: group1 [1,4], group2 [2,3]
```

 
 <p><strong>Example 2:</strong></p>

```clike
Input: N = 4, dislikes = [[1,2],[1,3],[2,4]]
Output: true
Explanation: group1 [1,4], group2 [2,3]
```

 
 <p><strong>Example 3:</strong></p>

```clike
Input: N = 5, dislikes = [[1,2],[2,3],[3,4],[4,5],[1,5]]
Output: false
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= N &lt;= 2000</code></li>
	<li><code>0 &lt;= dislikes.length &lt;= 10000</code></li>
	<li><code>dislikes[i].length == 2</code></li>
	<li><code>1 &lt;= dislikes[i][j] &lt;= N</code></li>
	<li><code>dislikes[i][0] &lt; dislikes[i][1]</code></li>
	<li>There does not exist <code>i != j</code> for which <code>dislikes[i] == dislikes[j]</code>.</li>
</ul>

题意：给定一组 `N` 人（编号为 `1, 2, ..., N`）， 我们想把所有人分进任意大小的两组。一个人可能不喜欢其他人，那么他们不应该属于同一组。形式上，如果 `dislikes[i] = [a, b]` ，表示不允许将编号为 `a` 和 `b` 的人归入同一组。

当可以用这种方法分成两组时，返回 `true` ；否则返回 `false` 。

---
### 解法 二分图匹配
使用简单的DFS，即可判断是否是二分图 `Bipartite Graph` :
```cpp
class Solution {
private:
    vector<vector<int>> g;
    vector<int> colors; //-1为初始值,表示没有访问,0是黑,1是白
    bool dfs(int u, int color) {
        colors[u] = color; 
        for (const int &v : g[u]) {
            if (colors[v] == -1) { //没有访问过这一点
                if (dfs(v, color ^ 1) == false) 
                    return false;
            } else if (colors[v] == colors[u]) return false; //访问过这一点,但是同色
        }
        return true;
    }
public:
    bool possibleBipartition(int N, vector<vector<int>>& dislikes) {
        g.resize(N + 1);
        colors.resize(N + 1, -1);
        for (const vector<int>& dis : dislikes) {
            g[dis[0]].push_back(dis[1]);
            g[dis[1]].push_back(dis[0]);
        }
        for (int i = 1; i <= N; ++i) if (colors[i] == -1) if (dfs(i, 0) == false) return false;
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：216 ms, 在所有 C++ 提交中击败了81.64% 的用户
内存消耗：63.3 MB, 在所有 C++ 提交中击败了59.18% 的用户
```
