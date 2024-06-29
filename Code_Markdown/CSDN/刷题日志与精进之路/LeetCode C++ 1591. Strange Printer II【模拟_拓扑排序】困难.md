 <div class="content__1Y2H"><div class="notranslate"><p>There is a strange printer with the following two special requirements:</p>
<ul>
	<li>On each turn, the printer will print a solid rectangular pattern of a single color on the grid. This will cover up the existing colors in the rectangle.</li>
	<li>Once the printer has used a color for the above operation, <strong>the same color cannot be used again</strong>.</li>
</ul>

<p>You are given a <code>m x n</code> matrix <code>targetGrid</code>, where <code>targetGrid[row][col]</code> is the color in the position <code>(row, col)</code> of the grid.</p>

<p>Return <code>true</code><em> if it is possible to print the matrix </em><code>targetGrid</code><em>,</em><em> otherwise, return </em><code>false</code>.</p>
 
<p><strong>Example 1:</strong></p>

<p><img style="width: 413px; height: 138px;" src="https://assets.leetcode.com/uploads/2020/08/15/sample_1_1929.png" width="60%" alt=""></p>

```cpp
Input: targetGrid = [[1,1,1,1],[1,2,2,1],[1,2,2,1],[1,1,1,1]]
Output: true
```

<p><strong>Example 2:</strong></p>

<p><img style="width: 483px; height: 290px;" src="https://assets.leetcode.com/uploads/2020/08/15/sample_2_1929.png" width="60%" alt=""></p>

```cpp
Input: targetGrid = [[1,1,1,1],[1,1,3,3],[1,1,3,4],[5,5,1,4]]
Output: true
```

<p><strong>Example 3:</strong></p>

```cpp
Input: targetGrid = [[1,2,1],[2,1,2],[1,2,1]]
Output: false
Explanation: It is impossible to form targetGrid because it is not allowed to print the same color in different turns.
```

<p><strong>Example 4:</strong></p>

```cpp
Input: targetGrid = [[1,1,1],[3,1,3]]
Output: false
```
<p><strong>Constraints:</strong></p>
<ul>
	<li><code>m == targetGrid.length</code></li>
	<li><code>n == targetGrid[i].length</code></li>
	<li><code>1 &lt;= m, n &lt;= 60</code></li>
	<li><code>1 &lt;= targetGrid[row][col] &lt;= 60</code></li>
</ul> 

题意：给出一个奇特的打印机，它的打印规则如下：
- 每次操作时，打印机会使用同一种颜色打印一个矩形（矩形中的所有格子都使用这种颜色），每次打印都会覆盖矩形对应格子中原本的颜色；
- 一旦矩阵根据上一规则使用了一种颜色，就不能再在后面使用这种颜色。

给出一个目标矩阵 `targetGrid` ，它的大小为 <code>m &times; n</code>，判断使用初始没有任何颜色的同样大小的矩阵，是否能够根据上述规则打印成目标矩阵。

---
### 思路1 逆序模拟
比赛的时候没有做出来。这道题说起来应该是一道模拟题，或者说是贪心题。如果从初始矩阵顺着推断，几乎不可能做出这道题；所以破解之处在于，既然已经有了目标矩阵 `targetGrid` ，那么我们是否能够还原回原来的初始矩阵？

如果当前一种颜色出现在某个 ***完整矩形*** 里，我们就可以认为这种颜色是最后打印的颜色之一，比如下图中，颜色 `4, 5` 都出现完整矩形中，那么我们就可以先还原掉颜色 `4` 或者颜色 `5`（最后打印这两种颜色）。还原后，我们就回到了 `Print with color 3` 这种状态，**注意：颜色 `4` 或颜色 `5` 所在的位置，下面是什么颜色不重要，反正它们会被覆盖掉**。这样虽然最终状态下，颜色 `3` 只占据了 `3` 个格子，而 `targetGrid[2][3] = 4` ，但是无所谓了，我们可以把这个格子也认为是 `3` ，于是颜色 `3` 的矩形就完整了，我们还原掉颜色 `3` 的矩形。最后我们还原掉颜色 `1` 的矩形，得到初始矩阵。从而能够判断，可以从初始矩阵打印出目标矩阵：
<img src="https://img-blog.csdnimg.cn/20200923233822276.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="55%">

这道题就是模拟这个倒过来的过程，也就是说，我们先删除最上层的颜色矩形，然后逐层删除没有被其他颜色覆盖的颜色矩形……emmm，说是这样说，算法如何实现还是一个问题。有些重要的点写在下面：
- 对于一种颜色，我们需要知道矩形的范围，寻找它的左边界、右边界、上边界和下边界，或者说两个坐标 `(l, u), (r, b)` ——左上坐标和右下坐标。我们还原这个范围的颜色矩形就可以了。
- 如果判断矩形是否完整？我们已经得到了矩形的范围，那么标准大小就等于 `(r - l + 1) * (b - u + 1)` 个格子，如果矩形的实际格子（没有被其他颜色覆盖的格子）数目等于标准大小，说明矩形是完整的。

算法的具体过程如下：
- 先找到一个未被还原的、完整的颜色矩形，它的标准大小等于实际大小；
- 设置这个颜色矩形已经被还原，然后对矩形中的所有格子，如果它在其他未被还原的颜色格子内部，就解锁这个格子（它可以被其他矩形使用）。最后设置这个格子已经被解锁；
- 这样持续循环，直到某一轮中，找不到能够还原的矩形，就退出循环。
- 循环所有颜色矩形，如果还有矩形无法还原，就返回 `false` 。都能被还原，表示能够回到初始矩形，返回 `true` 。

实际代码如下：
```cpp
const int MAXM = 65;
const int MAXN = 65;

struct Color {
    bool enabled = false; 
    int l, r, u, b; //颜色矩阵的范围(l,u),(r,b)
    int cnt, size;  //颜色矩阵的标准大小,实际大小(标准大小减去被覆盖的格子数)
    Color() {} 
    Color(int x, int y) {  //初始化颜色的范围和大小
        u = b = x; l = r = y; 
        size = 1; cnt = 0; //size==cnt时就认为这个矩形是完整的,可以被还原
        enabled = true;    //是否已经遇到(true表示已经遇到); 是否已经被还原(false表示已经被还原,之后这个矩形的格子可以随意使用)
    }
    void update(int x, int y) { //更新矩形范围和标准大小
        l = min(l, y); r = max(r, y);
        u = min(u, x); b = max(b, x);
        size = (r - l + 1) * (b - u + 1);
    }
    bool inner(int x, int y) { //判断坐标是否在颜色矩形中
        if (u <= x && x <= b && l <= y && y <= r) return true;
        return false;
    }
} color[MAXM];
int rea[MAXM][MAXN];

class Solution {
public:
    bool isPrintable(vector<vector<int>>& targetGrid) {
        int m = targetGrid.size(), n = targetGrid[0].size();
        for (int i = 1; i <= 60; ++i) {
            color[i].enabled = false; //所有颜色开始都没有遇到
            color[i].cnt = 0;         //所有颜色矩形的实际格子数量开始都是0
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                //Init
                rea[i][j] = false;             //全部初始化为false
                //Color
                int c = targetGrid[i][j];
                if (color[c].enabled == false) //之前没有遇到这种颜色
                    color[c] = Color(i, j);    //初始化这种颜色矩形的范围和大小
                else color[c].update(i, j);    //之前遇到过这种颜色,更新其范围和标准大小
                ++color[c].cnt;                //更新这种颜色矩形中的实际格子数量
            }
        }
        bool flag = false;
        do {
            flag = false;
            for (int c = 1; c <= 60 && !flag; ++c) { //循环所有颜色
                //如果颜色矩形已经被还原或不是完整矩形,就跳过
                if (color[c].enabled == false || color[c].size > color[c].cnt) continue; 
                color[c].enabled = false; //还原这个颜色矩形
                flag = true; //设置为true,表示还原过矩形(和冒泡排序提前退出的标志一样)
                for (int i = color[c].u; i <= color[c].b; ++i) {     //对这种颜色的上下范围
                    for (int j = color[c].l; j <= color[c].r; ++j) { //对这种颜色的左右范围
                        if (rea[i][j]) continue;                     //如果已经被解锁,不需要再被解锁,就跳过
                        //用这个格子,解锁其他颜色矩形的格子
                        for (int nc = 1; nc <= 60; ++nc) {           //再次循环所有颜色矩形
                            if (color[nc].enabled == false) continue;   //如果这一颜色矩形已经被还原,跳过
                            if (color[nc].inner(i, j)) ++color[nc].cnt; //如果这个格子被包括在其他未还原的矩形的范围内
                            //修改其他颜色矩形的实际格子数量
                        }
                        rea[i][j] = true; //这一格子已经被解锁
                    }
                }
                //还原了某一矩形
                //printf("Color %d %d %d\n", c, color[c].size, color[c].cnt);
            }
        } while (flag); //没有矩形要还原时直接退出
        for (int c = 1; c <= 60; ++c) if (color[c].enabled) return false; //如果还有颜色未被还原,返回false
        return true;
    }
};
```
效率如下：
```cpp
执行用时：68 ms, 在所有 C++ 提交中击败了94.19% 的用户
内存消耗：13.4 MB, 在所有 C++ 提交中击败了94.96% 的用户
```

---
### 思路2 拓扑排序
这一道题目也可以将问题转换为有向图。在 `targetGrid` 中，首先求出每个矩形的范围。然后再次扫描矩阵，如果某个颜色点 `B` 存在于矩形 `A` 中（以颜色代表矩形），则颜色矩形 `A` 应该先被打印，后打印 `B` ，即存在一条 `A->B` 的有向边。这样，如果一个颜色矩形 `T` 跨越了多种颜色矩形的范围，`T` 就必须等待所有在它下层的颜色矩形打印后再被打印。据此建立一张有向图。

采用邻接表，其中每个顶点 `color` 表示一种颜色矩形，它的有向邻接边表示 `color` 先被打印，邻接点的颜色矩形后被打印。于是，对邻接表进行拓扑排序，判断这张有向图是否是有向无环图，就可以判断是否可以从初始矩阵打印出目标矩阵。

代码如下：
```cpp
class Solution {
public:
    vector<list<int>> adjList;
    int top[61], bottom[61], left[61], right[61]; //上下左右
    bool isPrintable(vector<vector<int>>& targetGrid) {
        int color, m = targetGrid.size(), n = targetGrid[0].size();
        for (int i = 1; i <= 60; ++i) {
            top[i] = left[i] = 61;
            bottom[i] = right[i] = 0;
        }
        //遍历图中的像素,求出每种颜色的矩形范围
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                color = targetGrid[r][c];
                top[color] = min(top[color], r);
                bottom[color] = max(bottom[color], r);
                left[color] = min(left[color], c);
                right[color] = max(right[color], c);
            }
        }
        adjList.resize(61); //邻接表建立有向图
        vector<vector<bool>> haveEdge(61, vector<bool>(61, false)); //判断是否有边,避免重复添加
        vector<int> indegrees(61, 0); //入度数组
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                color = targetGrid[r][c];
                for (int cl = 1; cl <= 60; ++cl) {
                    if (color != cl && !haveEdge[cl][color] &&
                        r >= top[cl] && r <= bottom[cl] && 
                        c <= right[cl] && c >= left[cl]) { //这个格子在这种颜色矩形的范围内
                        haveEdge[cl][color] = true;
                        ++indegrees[color];
                        adjList[cl].push_back(color);
                    }
                }
            }
        }
        //进行拓扑排序,如果最后存在入度不为零的颜色,说明存在环,不能打印
        queue<int> q;
        for (int i = 1; i <= 60; ++i) if (indegrees[i] == 0) q.push(i);
        while (!q.empty()) {
            color = q.front(); q.pop();
            for (int v : adjList[color]) {
                --indegrees[v];
                if (indegrees[v] == 0) q.push(v);
            }
        }
        for (int i = 1; i <= 60; ++i) if (indegrees[i] > 0) return false;
        return true;
    }
};
```
比起模拟来说，效率低许多，但好歹能过，而且程序更加清晰：
```cpp
执行用时：380 ms, 在所有 C++ 提交中击败了19.09% 的用户
内存消耗：20.7 MB, 在所有 C++ 提交中击败了20.58% 的用户
```
