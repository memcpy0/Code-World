@[toc]
# 1. 图的遍历

> 深搜广搜日神仙。

图的基本特征是点和边，基本算法BFS和DFS用来遍历和处理点与边的关系。可以说，图论的大部分内容都和它们有关。

特别是DFS，用递归搜索图，变化多端，用途广泛——如拓扑排序、强连通分量等算法，都基于DFS。
## 1.1 DFS
DFS 全称是 [Depth First Search](https://en.wikipedia.org/wiki/Depth-first_search)，中文名是深度优先搜索，是一种用于遍历或搜索树或图的算法。所谓深度优先，就是说每次都尝试向更深的节点走。该算法讲解时常常与 BFS 并列，但**两者除了都能遍历图的连通块以外，用途完全不同**，很少有能混用两种算法的情况。

DFS 常常用来指代用递归函数实现的搜索，但**实际上两者并不一样**。有关该类搜索思想请参阅 [DFS（搜索）](../search/dfs.md).

下面是DFS的示例，用 `vector` 邻接表存图。当然，DFS的模板多种多样，用自己熟悉的就可以了。
```cpp
vector<int> G[N];					//G[u][i]: 结点u邻接的第i个结点
int vis[N];						//点的访问标志,vis[i]=1表示访问过
							//vis[i]=0表示没有访问;
						        //-1则表示正在访问中,有时会用到
bool dfs(int u) {					//以u为起点开始DFS搜索
    vis[u] = 1;						//在本次递归中被访问到
    { ...; return true; }				//出现目标状态,正确返回
    { ...; return false; }				//做相应处理,返回错误
    	
    for (int i = 0; i < G[u].size(); ++i) {		//u的邻接点有G[u].size()个
        int v = G[u][i];		//第i个邻接点v
        if (!vis[v])					//没有访问过
            return dfs(v);				//递归访问过邻接点v
    }		
    { ...; } 		//事后处理返回true/false
}
```
## 过程
DFS 最显著的特征在于其 **递归调用自身**。同时与 BFS 类似，DFS 会对其访问过的点打上访问标记，在遍历图时跳过已打过标记的点，以确保 **每个点仅访问一次**。符合以上两条规则的函数，便是**广义上的 DFS**。

具体地说，DFS 大致结构如下：
```js
DFS(v) // v 可以是图中的一个顶点，也可以是抽象的概念，如 dp 状态等。
  在 v 上打访问标记
  for u in v 的相邻节点
    if u 没有打过访问标记 then
      DFS(u)
    end
  end
end
```
以上代码只包含了 DFS 必需的主要结构。实际的 DFS 会在以上代码基础上加入一些代码，利用 DFS 性质进行其他操作。
## 性质
该算法通常的时间复杂度为 $O(n+m)$，空间复杂度为 $O(n)$ ，其中 $n$ 表示点数，$m$ 表示边数。注意空间复杂度包含了栈空间，栈空间的空间复杂度是 $O(n)$ 的。在**平均 $O(1)$ 遍历一条边的条件下**才能达到此时间复杂度，例如用前向星或邻接表存储图；如果用邻接矩阵则不一定能达到此复杂度。
> 备注：目前大部分算法竞赛（包括 NOIP、大部分省选以及 CCF 举办的各项赛事）都支持 **无限栈空间**，即：栈空间不单独限制，但总内存空间仍然受题面限制。但大部分操作系统会对栈空间做额外的限制，因此在本地调试时需要一些方式来取消栈空间限制。
> -   在 Windows 上，通常的方法是在 **编译选项** 中加入 `-Wl,--stack=1000000000`，表示将栈空间限制设置为 1000000000 字节。
> -   在 Linux 上，通常的方法是在运行程序前 **在终端内** 执行 `ulimit -s unlimited`，表示栈空间无限。每个终端只需执行一次，对之后每次程序运行都有效。
## 实现
以链式前向星为例：（和上方伪代码每行一一对应）

=== "C++"

```cpp
void dfs(int u) {
  vis[u] = 1;
  for (int i = head[u]; i; i = e[i].x) {
    if (!vis[e[i].t]) {
      dfs(v);
    }
  }
}
```

=== "Python"
```python
def dfs(u):
    vis[u] = True
    while i:
        i = head[u]
        if vis[e[i].t] == False:
            dfs(v)
        i = e[i].x
```
### DFS 序列
DFS 序列是指 DFS 调用过程中访问的节点编号的序列。我们发现，**每个子树都对应 DFS 序列中的连续一段（一段区间）**。

### 括号序列
DFS 进入某个节点的时候记录一个左括号 `(`，退出某个节点的时候记录一个右括号 `)`。**每个节点会出现两次**。相邻两个节点的深度相差 1。
### 一般图上 DFS
对于非连通图，只能访问到起点所在的连通分量。

对于连通图，DFS 序列通常不唯一。
注：树的 DFS 序列也是不唯一的。
### DFS树
在 DFS 过程中，通过记录每个节点从哪个点访问而来，可以建立一个树结构，称为 DFS 树。DFS 树是原图的一个生成树。

[DFS 树](./scc.md#dfs-生成树) 有很多性质，比如可以用来求 [强连通分量](./scc.md)。

## 1.2 BFS
BFS 全称是 [Breadth First Search](https://en.wikipedia.org/wiki/Breadth-first_search)，中文名是宽度优先搜索，也叫广度优先搜索。是图上最基础、最重要的搜索算法之一。

所谓宽度优先。就是每次都尝试访问同一层的节点。如果同一层都访问完了，再访问下一层。

这样做的结果是，BFS 算法找到的路径是==从起点开始的 **最短** 合法路径==。换言之，这条路径所包含的**边数最小**。

在 BFS 结束时，每个节点都是通过**从起点到该点的最短路径**访问的。

算法过程可以看做是图上火苗传播的过程：最开始只有起点着火了，在每一时刻，有火的节点都向它相邻的所有节点传播火苗。
## 实现
下文中 C++ 与 Python 的代码实现是基于链式前向星的存图方式，其实现可参考 [图的存储](./save.md) 页面。

=== "伪代码"
```text
bfs(s) {
  q = new queue()
  q.push(s), visited[s] = true
  while (!q.empty()) {
    u = q.pop()
    for each edge(u, v) {
      if (!visited[v]) {
        q.push(v)
        visited[v] = true
      }
    }
  }
}
```
=== "C++"

```cpp
void bfs(int u) {
  while (!Q.empty()) Q.pop();
  Q.push(u);
  vis[u] = 1;
  d[u] = 0;
  p[u] = -1;
  while (!Q.empty()) {
    u = Q.front();
    Q.pop();
    for (int i = head[u]; i; i = e[i].nxt) {
      if (!vis[e[i].to]) {
        Q.push(e[i].to);
        vis[e[i].to] = 1;
        d[e[i].to] = d[u] + 1;
        p[e[i].to] = u;
      }
    }
  }
}

void restore(int x) {
  vector<int> res;
  for (int v = x; v != -1; v = p[v]) {
    res.push_back(v);
  }
  std::reverse(res.begin(), res.end());
  for (int i = 0; i < res.size(); ++i) printf("%d", res[i]);
  puts("");
}
```

=== "Python"

```python
from queue import Queue

def bfs(u):
    Q = Queue()
    Q.put(u)
    vis[u] = True
    d[u] = 0
    p[u] = -1
    while Q.qsize() != 0:
        u = Q.get()
        while i:
            i = head[u]
            if vis[e[i].to] == False:
                Q.put(e[i].to)
                vis[e[i].to] = True
                d[e[i].to] = d[u] + 1
                p[e[i].to] = u
            i = e[i].nxt

def restore(x):
    res = []
    v = x
    while v != -1:
        res.append(v)
        v = p[v]
    res.reverse()
    for i in range(0, len(res)):
        print(res[i])
```
具体来说，我们用一个队列 Q 来记录要处理的节点，然后开一个布尔数组 `vis[]` 来标记是否已经访问过某个节点。
1. 开始的时候，我们将所有节点的 `vis` 值设为 0，表示没有访问过；然后把起点 s 放入队列 Q 中并将 `vis[s]` 设为 1。
2. 之后，我们每次从队列 Q 中取出队首的节点 u，然后把与 u 相邻的所有节点 v 标记为已访问过并放入队列 Q。
3. 循环直至当队列 Q 为空，表示 BFS 结束。

在 BFS 的过程中，也可以记录一些额外的信息。例如上述代码中，d 数组用于记录起点到某个节点的最短距离（要经过的最少边数），p 数组记录是从哪个节点走到当前节点的。
- 有了 d 数组，可以方便地得到起点到一个节点的距离。
- 有了 p 数组，可以方便地还原出起点到一个点的最短路径。上述代码中的 `restore` 函数使用该数组依次输出从起点到节点 x 的最短路径所经过的节点。

时间复杂度 $O(n + m)$

空间复杂度 $O(n)$（`vis` 数组和队列）
## open-closed 表
在实现 BFS 的时候，==本质上我们把未被访问过的节点放在一个称为 open 的容器中，而把已经访问过了的节点放在一个称为 closed 容器中==。
## 在树/图上 BFS
### BFS 序列
类似 DFS 序列，BFS 序列是指在 BFS 过程中访问的节点编号的序列。
### 一般图上 BFS
如果原图不连通，只能访问到从起点出发能够到达的点。

BFS 序列通常也不唯一。

类似的我们也可以定义 BFS 树：==在 BFS 过程中，通过记录每个节点从哪个点访问而来，可以建立一个树结构，即为 BFS 树==。
## 应用
-   在一个无权图上求从起点到其他所有点的最短路径。
-   在 $O(n+m)$ 时间内求出**所有连通块**。（我们只需要从每个没有被访问过的节点开始做 BFS，显然每次 BFS 会走完一个连通块）
-   如果把一个游戏的动作看做是状态图上的一条边（一个转移），那么 BFS 可以用来找到**在游戏中从一个状态到达另一个状态所需要的最小步骤**。
-   在一个有向无权图中**找最小环**。（从每个点开始 BFS，在我们即将抵达一个之前访问过的点开始的时候，就知道遇到了一个环。==图的最小环是每次 BFS 得到的最小环的平均值==。）
-   找到**一定在 $(a, b)$ 最短路上的边**。（分别从 a 和 b 进行 BFS，得到两个 d 数组。之后对每一条边 $(u, v)$，如果 $d_a[u]+1+d_b[v]=d_a[b]$，则说明该边在最短路上）
-   找到**一定在 $(a, b)$ 最短路上的点**。（分别从 a 和 b 进行 BFS，得到两个 d 数组。之后对每一个点 v，如果 $d_a[u]+d_b[v]=d_a[b]$，则说明该点在最短路上）
-   找到**一条长度为偶数的最短路**。（我们需要==构造一个新图，把每个点拆成两个新点==，原图的边 $(u, v)$ 变成 $((u, 0), (v, 1))$ 和 $((u, 1), (v, 0))$。对新图做 BFS，**$(s, 0)$ 和 $(t, 0)$ 之间的最短路**即为所求）
-   在一个**边权为 0/1 的图上**求最短路，见下方双端队列 BFS。
## 双端队列 BFS
如果你不了解双端队列 `deque` 的话，请参阅 [deque 相关章节](../../lang/csl/sequence-container/#deque)。

==双端队列 BFS 又称 0-1 BFS==。
### 适用范围
边权值为可能有，也可能没有（由于 BFS 适用于权值为 1 的图，所以一般权值是 0 或 1），或者**能够转化为这种边权值的最短路问题**。

例如在走迷宫问题中，你可以花 1 个金币走 5 步，也可以不花金币走 1 步，这就可以用 0-1 BFS 解决。
### 实现
一般情况下，==我们把没有权值的边扩展到的点放到队首，有权值的边扩展到的点放到队尾==。这样即可保证==像普通 BFS 一样整个队列队首到队尾**权值单调不下降**==。

下面是伪代码：
```cpp
while (队列不为空) {
  int u = 队首;
  弹出队首;
  for (枚举 u 的邻居) {
    更新数据
    if (...)
      添加到队首;
    else
      添加到队尾;
  }
}
```
### 例题
### [Codeforces 173B](http://codeforces.com/problemset/problem/173/B)
一个 $n \times m$ 的图，现在有一束激光从左上角往右边射出，每遇到 '#'，你可以选择光线往四个方向射出，或者什么都不做，问最少需要多少个 '#' 往四个方向射出才能使光线在第 $n$ 行往右边射出。

此题目正解不是 0-1 BFS，但是适用 0-1 BFS，减小思维强度，赛时许多大佬都是这么做的。

做法很简单，一个方向射出不需要花费（0），而往四个方向射出需要花费（1），然后直接来就可以了。
#### 代码
```cpp
#include <bits/stdc++.h>
using namespace std;

#define INF (1 << 29)
int n, m;
char grid[1001][1001];
int dist[1001][1001][4];
int fx[] = {1, -1, 0, 0};
int fy[] = {0, 0, 1, -1};
deque<int> q;  // 双端队列

void add_front(int x, int y, int dir, int d) {  // 向前方加
  if (d < dist[x][y][dir]) {
    dist[x][y][dir] = d;
    q.push_front(dir);
    q.push_front(y);
    q.push_front(x);
  }
}

void add_back(int x, int y, int dir, int d) {  // 向后方加
  if (d < dist[x][y][dir]) {
    dist[x][y][dir] = d;
    q.push_back(x);
    q.push_back(y);
    q.push_back(dir);
  }
}

int main() {
  cin >> n >> m;
  for (int i = 0; i < n; i++) cin >> grid[i];

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      for (int k = 0; k < 4; k++) dist[i][j][k] = INF;

  add_front(n - 1, m - 1, 3, 0);

  while (!q.empty()) {  // 具体搜索的过程，可以参考上面写的题解
    int x = q[0], y = q[1], dir = q[2];
    q.pop_front();
    q.pop_front();
    q.pop_front();
    int d = dist[x][y][dir];
    int nx = x + fx[dir], ny = y + fy[dir];
    if (nx >= 0 && nx < n && ny >= 0 && ny < m)
      add_front(nx, ny, dir, d);  // 判断条件
    if (grid[x][y] == '#')
      for (int i = 0; i < 4; i++)
        if (i != dir) add_back(x, y, i, d + 1);
  }
  if (dist[0][0][3] == INF)
    cout << -1 << endl;
  else
    cout << dist[0][0][3] << endl;
  return 0;
}
```
双端队列 BFS：
-   [CF1063B. Labyrinth](https://codeforces.com/problemset/problem/1063/B)
-   [CF173B. Chamber of Secrets](https://codeforces.com/problemset/problem/173/B)
-   [「BalticOI 2011 Day1」打开灯泡 Switch the Lamp On](https://loj.ac/p/2632)
## 优先队列 BFS
优先队列，相当于一个二叉堆，STL 中提供了 [`std::priority_queue`](../lang/csl/container-adapter.md)，可以方便我们使用优先队列。

在基于优先队列的 BFS 中，我们每次从队首取出代价最小的结点进行进一步搜索。容易证明这个贪心思想是正确的，因为从这个结点开始扩展的搜索，一定不会更新原来那些代价更高的结点。换句话说，其余那些代价更高的结点，我们不回去考虑更新它。

当然，每个结点可能会被入队多次，只是每次入队的代价不同。当该结点第一次从优先队列中取出，以后便无需再在该结点进行搜索，直接忽略即可。所以，优先队列的 BFS 当中，每个结点只会被处理一次。

相对于普通队列的 BFS，时间复杂度多了一个 $\log n$，毕竟要维护这个优先队列嘛。不过普通 BFS 有可能每个结点入队、出队多次，时间复杂度会达到 $O(n^2)$，不是 $O(n)$。所以优先队列 BFS 通常还是快的。

诶？这怎么听起来这么像堆优化的 [Dijkstra](./shortest-path.md#dijkstra-算法) 算法呢？事实上，堆优化 Dijkstra 就是优先队列 BFS。
## 习题
-   [「NOIP2017」奶酪](https://uoj.ac/problem/332)
## 参考
<https://cp-algorithms.com/graph/breadth-first-search.html>


---
# 2. 图的连通性

在用BFS和DFS遍历一张图的时候，我们同时也解决了图的连通性问题。为了理解**DFS在图的连通性**中的应用，我们先要理解这些概念：

1. 无向图 `G=(V, {E})` 中，如果从顶点 `v` 到顶点 `v'` 有路径，则称 `v, v'` 是连通的。如果图中**任意两个顶点** $v_i, v_j \in V$ ，$v_i, v_j$ 都是连通的，则称 `G` 是**连通图** `Connected Graph` 。
	
	无向图中的**极大连通子图**被称为**连通分量**，注意**极大连通子图**或者说**连通分量**的概念：
	- 要是**子图**；
	- 子图要是**连通**的；
	- 连通子图中含有**极大顶点数**；
	- 含有**极大顶点数**的**连通子图**包含**依附于这些顶点的所有边**。
	<p></p>
	
	如下，图1不是连通图，因为存在 `E,F` 这两个顶点；图2是连通图，其顶点 `A,B,C,D` 相互之间都是连通的，所以它本身是连通图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200730170446402.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
	但是图1作为无向非连通图，有2个**连通分量** `Connected Component` ，即图2和图3，图4虽然是图1的连通子图，但是它不满足**极大顶点数**，因此不是图1的无向图的**连通分量**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020073017132848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

	
2. 有向图 `G=(V, {E})` 中，如果对于**每一对** $v_i, v_j \in V, v_i \ne v_j$ ，从 $v_i \to v_j$ 和从 $v_j \to v_i$ 都存在路径，则称 `G` 是**强连通图**。

	有向图中的**极大强连通子图**被称为**强连通分量**，同样注意这个概念：
	- 要是**子图**；
	- 子图要是**强连通**的；
	- 强连通子图中含有**极大顶点数**；
	- 含有**极大顶点数**的**强连通子图**包含**依附于这些顶点的所有边**。
	<p></p>
	
	下图中，图1不是强连通图，因为顶点 `A` 到 `D` 存在路径，而 `D` 到 `A` 不存在路径。图2是强连通图，而且图2是图1的**极大强连通子图**，即**强连通分量**：![在这里插入图片描述](https://img-blog.csdnimg.cn/20200730172936763.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
3. 一个**连通图**的**生成树**是一个**极小连通子图**，它**含有图中全部顶点，但是只有足以构成一棵树的** `n - 1` **条边**。

	如图1，它是一个连通图，但是不是生成树。我们去掉构成环的两条边后，如图2、图3都是一棵生成树。不过，有 `n - 1` 条边不一定是生成树，如图4：
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20200730174246562.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

	可知，如果一个简单图有 `n` 个顶点和 `< n - 1` 条边，则是非连通图；如果它 `> n - 1` 条边，必定构成一个环，因为这条边让它依附的两个顶点间有了第二条路径。如果是生成树，则有 `n - 1` 条边，反之不然。 

4. 如果一个有向图**恰好有一个顶点的入度是 `0` ，其他顶点的入度均为 `1`** ，则是一棵**有向树**，即只有一个顶点没有直接前驱，其他顶点的直接前驱只有1个。
	
	一个**有向图**的**生成森林**由若干棵**有向树**构成，**含有全部顶点，但是只有能够构成若干棵不相交的有向树的弧**。 

	如图1，是一个有向图，去掉一些边后，就成了两棵有向树，它们就是图1有向图的生成森林：
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20200730175418806.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---

## (1) 求某个点的连通性

对需要的点执行 `dfs(v)` ，可以找到它连通的所有点，例如找图1中 `e` 点的连通性，执行 `dfs(e)` ，访问过程见图2结点上面的数字，顺序是 `ebdca` ：





递归返回的结果见结点下面划线的数字，顺序是 `acdbe` 。虚线指向的结点表示不再访问，因为前面已经访问过了。



# (2) 用 `dfs()` 处理所有点

题目中经常需要处理所有的点，也可以用 `dfs()` 实现。做法是：**想象一个虚拟结点  `v` ，它连接了所有的顶点**，然后，我们在主程序中这样 `dfs()` ：

```cpp
for (int i = 0; i < n; ++i) dfs(i);
```

就好像 `v` 和所有顶点都是邻接点一样。这种思想还会出现在 `johnson` 算法中。



这部分内容必须**彻底掌握**，这是后面内容的基础！




