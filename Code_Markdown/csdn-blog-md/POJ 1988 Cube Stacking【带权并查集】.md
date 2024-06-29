**Description**
Farmer John and Betsy are playing a game with `N (1 <= N <= 30,000)` identical cubes labeled `1` through `N`. They start with `N` stacks, each containing a single cube. Farmer John asks Betsy to perform `P (1<= P <= 100,000)` operation. There are two types of operations:
moves and counts.
* In a move operation, Farmer John asks Bessie to move the stack containing cube `X` **on top of** the stack containing cube `Y` .
* In a count operation, Farmer John asks Bessie to count the number of cubes on the stack with cube `X` that are under the cube `X` and report that value.

Write a program that can verify the results of the game.

**Input**
* Line `1` : A single integer, `P`

* Lines `2..P+1` : Each of these lines describes a legal operation. Line `2` describes the first operation, etc. Each line begins with a `'M'` for a move operation or a `'C'` for a count operation. For move operations, the line also contains two integers: `X` and `Y` . For count operations, the line also contains a single integer: `X` .

Note that the value for `N` does not appear in the input file. No move operation will request a move a stack onto itself.

**Output**
Print the output from each of the count operations in the same order as the input file.

**Sample Input**
```cpp
6
M 1 6
C 1
M 2 4
M 2 6
C 3
C 4
```
**Sample Output**
```cpp
1
0
2
```

题意：给定 `N` 个方块，排成一行，将它们编号 `1` 到 `N` 。再给出 `P` 个操作：
- `M i j` 表示将 `i` 所在的那一堆移到 `j` 所在那一堆的顶部。
- `C i` 表示一个询问，询问 `i` 下面有多少个方块。

需要写一个程序来完成这些操作。

---
思路：简单的带权并查集，和银河英雄传说类似，可以参考黑书。其实题目并不难，只是最开始看错了题目。

我们设 `father[i]` 是每个方块所在位置的最底下方块的编 号，`height[i]` 是每个方块之下有多少个方块（开始赋成 `0` ）。每次读入合并操作 `Y` 和 `X` 时：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200831190036638.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
首先找到 `X` 和 `Y` 的父亲（最底下的点），同时更新：`f[fy]=fx` 。但是很快问题就来了——我们怎么更新 `f[fy]` 的高度呢？我们不能获知左边堆里最上面一层的 `h[i]` !

可以开个数组 `size[i]` ，表示 `i` 所在的堆的大小（仅当 `i` 是底层的方格时用到）。初始化时是 `1` 。进行如图的操作时，`height[fy] += size[fx]`(写成 `=` 也可以) ，同时 `size[fx] += size[fy]` 。

既然右侧堆中最底层的元素已经转移了，那么它上面的点呢？这里，要用到类似于线段树里的 `lazy-tag` 思想，即**询问时再更新值**。如果询问右侧某一点，我们只需**在它寻找父亲、路径压缩时**，再增加一个修改 `height` 的操作即可。

代码如下：
```cpp
#include <cstdio>
using namespace std;
const int maxn = 3e4 + 10;
int father[maxn], height[maxn], size[maxn];
//father表示并查集,
//height[i]表示木块i之下有多少块木块,
//size表示i所在的集合的木块数目
//每堆最下方的结点是根,father[x]是与x相连的下面结点 
//开始所有的木块都是根结点,高度为0,大小为1 
void init() {
	for (int i = 1; i < maxn; ++i) father[i] = i, height[i] = 0, size[i] = 1;
}
//通过递归查询到根值,确定在当前的栈内每个木块的高度 
int find(int x) {
	if (father[x] == x) return x;
	int t = father[x];      //记录原来的父结点 
	father[x] = find(t);	//路径压缩
	height[x] += height[t]; //木块x的高度建立在父木块的高度上 
	return father[x];		
}
//合并栈,确定上方栈的根值新高度,更新新栈大小 
void merge(int a, int b) {	//把木块a所在的堆x放到木块b所在的堆y上面 
	int x = find(a), y = find(b); 
	if (x == y) return;
	father[x] = y;			//把堆x连到堆y上 
	height[x] += size[y];	//木块x的高度+=堆y的大小(因为放到y的上面) //height[x]=size[y]
	size[y] += size[x];		//堆y的大小+堆x的大小(合并为一堆) 
}

int main() { 
	int p, a, b;
	char c[2];
	scanf("%d", &p); 
	init(); 
	for (int i = 1; i <= p; ++i) {
	 	scanf("%s", c);
	 	if (c[0] == 'M') {
	 		scanf("%d%d", &a, &b);
	 		merge(a, b);
	 	} else if (c[0] == 'C') {
	 		scanf("%d", &a);
	 		find(a);
			printf("%d\n", height[a]);
	 	}		 
	} 
	return 0;
}
```
提交后AC：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200831190746429.png#pic_center)

