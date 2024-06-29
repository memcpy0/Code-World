![在这里插入图片描述](https://img-blog.csdnimg.cn/20210519032948748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)**输入格式**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210519032958952.png)
**输出格式**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210519033036819.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
**题意翻译**
初始时从左到右有 $n$ 个木块，编号为 $0 \ldots n-1$ ，要求实现下列四种操作：
- `move a onto b` : 把 `a` 和 `b` 上方的木块归位，然后把 `a` 放到 `b` 上面。
-  `move a over b` : 把 `a` 上方的木块归位，然后把 `a` 放在 `b` 所在木块堆的最上方。
 -   `pile a onto b` : 把 `b` 上方的木块归位，然后把 `a` 及以上的木块坨到 `b` 上面。
  -  `pile a over b` : 把 `a` 及以上的木块坨到 `b` 的上面。
-    一组数据的结束标志为 `quit` ，如果有非法指令（如 `a` 与 `b` 在同一堆），无需处理。

输出:所有操作输入完毕后，从左到右，从下到上输出每个位置的木块编号。

**输入输出样例**
输入 #1 
```clike
10
move 9 onto 1
move 8 over 1
move 7 over 1
move 6 over 1
pile 8 over 6
pile 8 over 5
move 2 over 1
move 4 over 9
quit
```

输出 #1

```clike
0: 0
1: 1 9 2 4
2:
3: 3
4:
5: 5 8 7 6
6:
7:
8:
9:
```

---
### 解法 模拟
这道题目的题意有点复杂，不过也不难懂。如果对4种操作分别写一个函数，暴力解这道题，其实也是可行的。不过通过观察，我们抽象出了两种共同的操作——归位和移动，然后以此实现四种指令。代码如下，其中使用了 `pos[maxn][2]` 记录木块当前所在的堆位置和木块在堆中的位置，方便查找。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 30;
int n, a, b, pos[maxn][2]; //pos[i][0]记录木块i的堆位置,pos[i][1]记录了木块i在堆pos[i][0]中的位置 
char act[5], how[5];
vector<int> blcs[maxn]; //blocks
void blocksReturn(int x) { //归位函数,将x所在的堆中x上方的木块全部放回原位 
	int blcPos = pos[x][0];
	for (int i = blcs[blcPos].size() - 1; blcs[blcPos][i] != x; --i) { //这里可用可不用木块x在堆中的位置 
		int val = blcs[blcPos].back(); 
		blcs[blcPos].pop_back();
		pos[val][0] = val, pos[val][1] = blcs[val].size(); //放回原来的堆中 
		blcs[val].push_back(val);
	}
} 
void putOn(int x, int y) { //将x所在的堆中x及上方的木块(如果有的话)整体放到y所在的木块堆上 
	int xBlcPos = pos[x][0], yBlcPos = pos[y][0], xPos = pos[x][1]; //查询x的堆位置和x在堆中的位置 
	for (int i = xPos; i < blcs[xBlcPos].size(); ++i) {
		int val = blcs[xBlcPos][i];
		pos[val][0] = yBlcPos, pos[val][1] = blcs[yBlcPos].size();
		blcs[yBlcPos].push_back(val); //放到y所在的木块堆上
	}
	while (blcs[xBlcPos].size() > xPos) blcs[xBlcPos].pop_back(); //真正丢弃元素 
} 
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) { blcs[i].push_back(i); pos[i][0] = i, pos[i][1] = 0; }
	while (~scanf("%s", act)) {
		if (strcmp(act, "quit") == 0) break;
		scanf("%d %s %d", &a, how, &b);
		if (a == b || pos[a][0] == pos[b][0]) continue; //非法指令 
		if (strcmp(act, "move") == 0) { 
			if (strcmp(how, "onto") == 0) { //move a onto b
				blocksReturn(a), blocksReturn(b); //把a和b上方的木块全部归位
				putOn(a, b); //此时a和b都在各自木块堆的最上方; 把a放到b上 
			} else { //move a over b
				blocksReturn(a); //把a上方的木块归位
				putOn(a, b); //此时a在木块堆的最上方; 把a放到b所在木块堆的最上方 
			}
		} else if (strcmp(act, "pile") == 0) {
			if (strcmp(how, "onto") == 0) { //pile a onto b
				blocksReturn(b); //把b上方的木块归位
				putOn(a, b); //此时b在木块堆的最上方; 把a及上方的木块放到b上 
			} else { //pile a over b
				putOn(a, b); //把a及上方的木块放到b所在木块堆的最上方 
			}
		} 
	}  
	for (int i = 0; i < n; ++i) {
		printf("%d:", i);
		for (int j = 0; j < blcs[i].size(); ++j) printf(" %d", blcs[i][j]);
		printf("\n");
	}
	return 0;
} 
```
此时的代码已经可以AC了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210519035800493.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
不过还有更近一步简化的空间。比如说用归位和移动实现四种指令时，发现存在 `move` 时就必须要归位 `a` 上方的木块，存在 `onto` 时必须要归位 `b` 上方的木块，因此简化的 `main()` 函数如下，其他代码一样：
```cpp
//......
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) { blcs[i].push_back(i); pos[i][0] = i, pos[i][1] = 0; }
	while (~scanf("%s", act)) {
		if (strcmp(act, "quit") == 0) break;
		scanf("%d %s %d", &a, how, &b);
		if (a == b || pos[a][0] == pos[b][0]) continue; //非法指令 
		if (strcmp(act, "move") == 0) blocksReturn(a); //把a上方的木块归位
		if (strcmp(how, "onto") == 0) blocksReturn(b); //把b上方的木块归位 
		putOn(a, b); //把a及上方的木块放到b所在木块堆的最上方 
	}  
	for (int i = 0; i < n; ++i) {
		printf("%d:", i);
		for (int j = 0; j < blcs[i].size(); ++j) printf(" %d", blcs[i][j]);
		printf("\n");
	}
	return 0;
} 
```
如果使用链表（比如说静态链表），`putOn()` 函数的效率还可以进一步提高，无论是搬运一个木块还是一堆木块，时间复杂度都可以达到 $O(1)$ 。
