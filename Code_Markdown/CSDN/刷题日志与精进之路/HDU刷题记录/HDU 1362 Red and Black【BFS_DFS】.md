 ### Problem Description
There is a **rectangular** room, covered with **square tiles**. Each tile is colored **either red or black**. A man is **standing on a black tile**. From a tile, he can move to one of **four adjacent tiles**. But he can't move on red tiles, he can move only on black tiles.

Write a program to count the number of black tiles which he can **reach** by repeating the moves described above.
 

**Input**
The input consists of multiple data sets. A data set starts with a line containing two positive integers `W` and `H`; `W` and ` H ` are the numbers of tiles in the x- and y- directions, respectively. ` W` and `H` are **not more than 20**.

   There are `H` more lines in the data set, each of which includes `W` characters. Each character represents the color of a tile as follows.

'.' - a black tile
'#' - a red tile
'@' - a man on a black tile(appears exactly once in a data set)
 

**Output**
For each data set, your program should output a line which contains the number of tiles he can reach from the initial tile (**including itself**).
 

**Sample Input**
```swfit
6 9
....#.
.....#
......
......
......
......
......
#@...#
.#..#.
11 9
.#.........
.#.#######.
.#.#.....#.
.#.#.###.#.
.#.#..@#.#.
.#.#####.#.
.#.......#.
.#########.
...........
11 6
..#..#..#..
..#..#..#..
..#..#..###
..#..#..#@.
..#..#..#..
..#..#..#..
7 7
..#.#..
..#.#..
###.###
...@...
###.###
..#.#..
..#.#..
0 0
```
 

**Sample Output**
```swift
45
59
6
13
```
题意：一个人站在一个长方形的房间里，瓷砖是方形的，有红黑两色，这个人站在黑色瓷砖上，位置以'@'表示，其他的黑色瓷砖用'.'表示，红色瓷砖用'#'表示，人可以上下左右移动到相邻的黑色瓷砖上。题目要求我们求出这个人从初始位置能够到达的黑色瓷砖的数量。

思路：暴力搜索整个解空间，用BFS和DFS。可以作为【搜索技术】的入门题。

当然，可能有点难度的一点，就是**图论的建模**，这道题只需把每块瓷砖视作一个图的一个顶点就可以了，用每块瓷砖的坐标表示。从起点出发，看起点所在的联通分量有多少个顶点。

还有一个问题就是，如何从一个点移动到上下左右去？可以用“四连通”的写法。
```
Move[][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}
```
### 解决方法一：BFS 
为了讲得清楚，画图，以四行五列的房间为例。BFS从起点扩散开，像一颗石子丢进池塘，激起的波浪会一层层扩散到整个空间，而且扩散按照从近到远的顺序。用队列处理这个扩散的过程很清晰：
![在这里插入图片描述](https://img-blog.csdnimg.cn/202002231612259.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

```cpp
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 25;
char room[maxn][maxn];
int Move[][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // 左上右下
#define check(x, y) ((x < w && x >= 0) && (y < h && y >= 0))
int dx, dy;
struct point { int x, y; };
int w, h;

int bfs() {
    queue<point> q;
    int num = 1; // 记录可走的黑色瓷砖数量
    q.push(point{dx, dy}); // 从起点开始 
	room[dy][dx] = '#'; // 把起点堵住防止重复计算, 其实可以不写, 因为起点是'@'而非'.' 
    while (!q.empty()) {
        point t = q.front(); q.pop();
        for (int i = 0; i < 4; ++i) {
            int tx = t.x + Move[i][0], ty = t.y + Move[i][1];
            if (check(tx, ty) && room[ty][tx] == '.') {
                ++num; // 可走的路多了一条
                q.push(point{tx, ty});
                room[ty][tx] = '#'; // 防止回退 
            }
        }
    }
    return num;
}

int main() {
    while (~scanf("%d %d", &w, &h)) {
        if (w == 0 && h == 0) break;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                cin >> room[i][j];
                if (room[i][j] == '@') { //找到起点坐标
                    dx = j;
                    dy = i;
                }
            }
        }
        printf("%d\n", bfs());
    }
    return 0;
}
```
### 解决方法二：DFS+递归
也可以**用栈模拟，不过这样就和队列+BFS的写法几乎完全一样了**。过程描述如下：
（1）在初始位置，num = 1，标记这个位置已经被走过；
（2）左、上、右、下4个方向，按照顺序选一个能走的方向，走一步；
（3）在新的位置，num++，标记这个位置已经被走过；
（4）继续上诉过程；
（5）如果无路可走，回退到上一步，换个方向再走；
（6）继续上诉过程，直到结束。

画图如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200223163137405.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```cpp
#include <iostream>
#include <cstdio>
#define isLegal(x, y) (x >= 0 && x < w && y >= 0 && y < h)
using namespace std;
const int maxn = 25; 
int h, w;
int Move[][4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; //x, y 左上右下 
char room[maxn][maxn];
int x, y; //起点的坐标
int num = 0; //可以行走的黑色瓷砖的数量
 
void dfs(int x, int y) {
	room[y][x] = '#'; //标记这个位置，表示已经走过了 
	++num;
	for (int i = 0; i < 4; ++i) { //在左上右下四个方向顺时针深搜
		int nextX = x + Move[i][0], nextY = y + Move[i][1];
		if (isLegal(nextX, nextY) && room[nextY][nextX] == '.') //选一个能走的方向
			dfs(nextX, nextY); 
	}
}

int main() {
	while (~scanf("%d%d", &w, &h)) { //宽和高 列和行
		if (!w && !h) break;
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				cin >> room[i][j];
				if (room[i][j] == '@') x = j, y = i; //找到起点坐标
			}
		}
		dfs(x, y);
		cout << num << endl;
		num = 0;
	}
	return 0; 
} 
```
