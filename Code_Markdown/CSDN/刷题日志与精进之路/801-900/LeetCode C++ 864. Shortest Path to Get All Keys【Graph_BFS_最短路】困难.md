<p>We are given a 2-dimensional&nbsp;<code>grid</code>.&nbsp;<code>"."</code> is an empty cell, <code>"#"</code> is&nbsp;a wall, <code>"@"</code> is the starting point, (<code>"a"</code>, <code>"b"</code>, ...) are keys, and (<code>"A"</code>,&nbsp;<code>"B"</code>, ...) are locks.</p>

<p>We start at the starting point, and one move consists of walking one space in one of the 4 cardinal directions.&nbsp; We cannot walk outside the grid, or walk into a wall.&nbsp; If we walk over a key, we pick it up.&nbsp; We can't walk over a lock unless we have the corresponding key.</p>

<p>For some <code>1 &le; K &le; 6</code>, there is exactly one lowercase and one uppercase letter of the first <code>K</code> letters of the English alphabet in the <code>grid</code>.&nbsp; This means that there is exactly one key for each lock, and one lock for each key; and also that the letters used to represent the keys and locks were&nbsp;chosen in the same order as the English alphabet.</p>

<p>Return the lowest number of moves to acquire all keys.&nbsp; If&nbsp;it's impossible, return <code>-1</code>.</p>

<div>
<p><strong>Example 1:</strong></p>

```css
Input: ["@.a.#","###.#","b.A.B"]
Output: 8  
```

<p><strong>Example 2:</strong></p>

```css
Input: ["@..aA","..B#.","....b"]
Output: 6 
```

<p><strong>Note:</strong></p>

<ol>
	<li><code>1 &lt;= grid.length&nbsp;&lt;= 30</code></li>
	<li><code>1 &lt;= grid[0].length&nbsp;&lt;= 30</code></li>
	<li><code>grid[i][j]</code> contains only<code> '.'</code>, <code>'#'</code>, <code>'@'</code>,&nbsp;<code>'a'-'f'</code> and <code>'A'-'F'</code></li>
	<li>The number of keys is in <code>[1, 6]</code>.&nbsp; Each key has a different letter and opens exactly one lock.</li>
</ol>
</div>


题意：给定一个二维网格 `grid` 。`"."` 代表一个空房间，`"#"` 代表一堵墙， `"@"` 是起点，小写字母如（`"a"`, `"b"`, ...）代表钥匙，对应的大写字母如（`"A"`, `"B"`, ...）代表锁。

我们从起点开始出发，一次移动是指向四个基本方向之一行走一个单位空间。不能在网格外面行走，也无法穿过一堵墙。如果途经一个钥匙，我们就把它捡起来。除非我们手里有对应的钥匙，否则无法通过锁。

假设 `K` 为钥匙/锁的个数，且满足 `1 <= K <= 6` ，字母表中的前 `K` 个字母在网格中都有自己对应的一个小写和一个大写字母。换言之，每个锁有唯一对应的钥匙，每个钥匙也有唯一对应的锁。代表钥匙和锁的字母互为大小写并按字母顺序排列。

返回获取所有钥匙所需要的移动的**最少次数**。如果无法获取所有钥匙，返回 `-1` 。

---  
### 解法1 BFS（在普通BFS基础上增加一维）
这道题有一定难度。题目要求我们拿到所有的钥匙（不一定要打开所有的锁），很容易就能想到思路——将给出的二维网格作为一个无向图来遍历求解。然而这一思路中存在一些棘手的难点。例如，有三把钥匙 `a, b, c` 分布如下，我们要拿到钥匙 `a` ，必须先拿到锁 `B, D` 的钥匙 `b, d` ，此前还必须拿到钥匙 `c` 。
```css
"@..c.",
"BD.#C",
"a.Adb"
```
经典的BFS和DFS算法中，为了避免循环搜索，都存在**标记节点已被访问**的步骤。一个节点遍历后，将其标记为已访问，之后就不会重复遍历此节点，保证算法能够正确退出。考虑上面的例子，通过 `c -> C -> b -> d` 后，我们拿到了 `b, c, d` 这三把钥匙，在无向图中这一路径上的所有节点都被标记为**已访问**，于是算法无法按照原路返回，即无法从 `d, b` 再退回到 `c` 。但是要拿到钥匙 `a` ，我们就必须从 `b, d` 再退回到 `c` ，才能够往下拿取钥匙 `a` 。这就产生了一个矛盾。

解决办法：从 `c -> b -> d` 是为了拿取 `b, d` ，拿到 `b, d` 前没有 `b, d` 这两把钥匙；拿到 `b, d` 后返回 `c` 时，多了 `b, d` 这两把钥匙。因此，我们在原有的图节点基础上，添加已拥有的钥匙这一信息，扩充二维图到三维图，就解决了这一矛盾。此时 `d -> b -> c` 在二维图上是按照原路返回，但在三维图上对应的却不是同样的图节点。

我们的图节点为三个整数 `(x, y, k)` ——二维坐标和已拿到的钥匙信息。`(x, y)` 表示**当前的坐标**，`k` 的二进制形式表示**当前获得的钥匙的状态**，即将钥匙对应到整数 `k` 的低六个位，每个比特对应一个钥匙。

队列中存储的节点结构如下：
```cpp
struct node{
    int x, y;
    int k;
    int d; //起始节点@到(x,y,k)的最短距离 
    node(int _x = 0, int _y = 0, int _k = 0, int _d = 0) : 
    	x(_x), y(_y), k(_k), d(_d) {}
};
```
节点标记数组为 `bool vis[x][y][k]` ，值为 `true` 表示图节点 `(x, y, k)` 已遍历过，为 `false` 表示图节点 `(x, y, k)` 没有遍历过。注意起点 `@` 也是可以走的格子。

整个BFS算法的初始节点是 `(x0, y0, 0)` ，`(x0, y0)` 为 `@` 所在的位置，目标节点是**包含所有钥匙**的一个未知节点。我们在三维无向无权图上用BFS算法，求取初始节点到目标节点的最短路径长度。为了方便查看算法的全部步骤，写成伪代码如下：
```cpp
vis(x, y, k) = false;
Q = [] ;
(x0, y0, 0, 0)入Q；
while (Q不空) {
    (x, y, k, d) = Q.front, 出Q;
    if (k包含所有钥匙) 算法结束，返回d;  
    for ((tx, ty) in (x, y)的四周){
        if((tx, ty)在图中 且 grid(tx, ty) != '#') {
            if (grid(tx, ty)是空格. or 起点@) {
				if (vis(tx, ty, k) == false) 访问(tx, ty, k, d + 1);
			} else if (grid(tx, ty)是钥匙a-f) {
				tk = k;
				收集钥匙;
				if (vis(tx, ty, tk) == false) 访问(tx, ty, tk, d + 1);
			} else if (grid(tx, ty)是锁A-F) {
				if (存在对应的钥匙 and vis(tx, ty, k) == false) 访问(tx, ty, k, d + 1);
            }
        }
    }
}
其它返回-1.
```
具体代码如下：
```cpp
#define visit(x, y, k, d) do { vis[x][y][k] = true; q.push(node(x, y, k, d)); } while (false)
class Solution {
private:
    struct node {
        int x, y;
        int k;
        int d;
        node(int _x = 0, int _y = 0, int _k = 0, int _d = 0) :
            x(_x), y(_y), k(_k), d(_d) {}
    };
    int Move[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
public:
    int shortestPathAllKeys(vector<string>& grid) {
        int m = grid.size(), n = grid[0].size(), len = 0; //len为钥匙个数
        char keys[7]; //记录钥匙
        int pos[27]; //记录钥匙在keys[]中的位置,用来区分k中的低六位
        bool vis[31][31][64] = {false}; //标记数组
        queue<node> q;

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) { 
                if (grid[i][j] == '@') { //起点
                    q.push(node(i, j, 0, 0));
                    vis[i][j][0] = true;
                }  else if (grid[i][j] >= 'a' && grid[i][j] <= 'z') { //钥匙
                    keys[len++] = grid[i][j];
                    pos[grid[i][j] - 'a'] = len - 1;
                }
            }
        }
        while (!q.empty()) {
            node temp = q.front(); q.pop();
            if (temp.k + 1 == (1 << len)) return temp.d; //找到所有钥匙,此时k=0b..11,返回最少步数
            for (int i = 0; i < 4; ++i) {
                int tx = temp.x + Move[i][0], ty = temp.y + Move[i][1];
                if (tx >= 0 && tx < m && ty >= 0 && ty < n && grid[tx][ty] != '#') {
                    if (grid[tx][ty] == '.' || grid[tx][ty] == '@') { //是空格和起点
                        if (!vis[tx][ty][temp.k]) visit(tx, ty, temp.k, temp.d + 1); //未访问过的起点或空格状态
                    } else if (grid[tx][ty] >= 'a' && grid[tx][ty] <= 'z') { //是钥匙
                        int tk = temp.k | (1 << pos[grid[tx][ty] - 'a']); //拾取钥匙
                        if (!vis[tx][ty][tk]) visit(tx, ty, tk, temp.d + 1); //未访问过的钥匙状态
                    } else if (grid[tx][ty] >= 'A' && grid[tx][ty] <= 'Z') { //是锁
                        if ((temp.k >> (pos[grid[tx][ty] - 'A'])) & 1) { //已得到对应的钥匙
                            if (!vis[tx][ty][temp.k]) visit(tx, ty, temp.k, temp.d + 1); //未访问过的锁状态
                        }
                    }
                }
            }
        }
        return -1; //无法收集到所有的钥匙
    }
};
```
运行效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了93.47% 的用户
内存消耗：10.5 MB, 在所有 C++ 提交中击败了91.92% 的用户
```

除了这一种方法外，我们还可以暴力枚举钥匙序列的所有排列，计算出最小的移动步数；或者巧妙地使用Dijkstra算法。
