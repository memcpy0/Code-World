**Description**
An earthquake takes place in Southeast Asia. The ACM (Asia Cooperated Medical team) have set up a wireless network with the lap computers, but an unexpected aftershock attacked, all computers in the network were all broken. The computers are repaired one by one, and the network gradually began to work again. Because of the hardware restricts, each computer can only directly communicate with the computers that are not farther than d meters from it. But every computer can be regarded as the intermediary of the communication between two other computers, that is to say computer `A` and computer `B` can communicate if computer `A` and computer `B` can communicate directly or there is a computer `C` that can communicate with both `A` and `B` .

In the process of repairing the network, workers can take two kinds of operations at every moment, repairing a computer, or testing if two computers can communicate. Your job is to answer all the testing operations.

**Input**
The first line contains two integers `N` and `d (1 <= N <= 1001, 0 <= d <= 20000)` . Here `N` is the number of computers, which are numbered from `1` to `N` , and `D` is the maximum distance two computers can communicate directly. In the next `N` lines, each contains two integers `xi, yi (0 <= xi, yi <= 10000)` , which is the coordinate of `N` computers. From the `(N+1)-th` line to the end of input, there are operations, which are carried out one by one. Each line contains an operation in one of following two formats:
1. `"O p" (1 <= p <= N)` , which means repairing computer p.
2. `"S p q" (1 <= p, q <= N)` , which means testing whether computer p and q can communicate.

The input will not exceed `300000` lines.

**Output**
For each Testing operation, print `"SUCCESS"` if the two computers can communicate, or `"FAIL" ` if not.

**Sample Input**
```cpp
4 1
0 1
0 2
0 3
0 4
O 1
O 2
O 4
S 1 4
O 3
S 1 4
```
**Sample Output**
```cpp
FAIL
SUCCESS
```

题意：有 `N` 台电脑坏了，两台电脑之间超过距离 `d` 就无法直接通信，但是可以借助其他电脑间接通信。电脑编号从 `1~N` 。然后进行多次操作：维护或者测试。

---
思路：时间限制是10000MS，于是很简单：每维护一台电脑，就标记为维护过，然后加入已维护的电脑集合，让它和**所有维护过的电脑**进行比较，如果距离足够短，就用并查集合并。每一次的测试操作，如果两台电脑在同一个集合中，就说明可以进行通信。

代码如下：
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;
const int maxn = 1010;
int father[maxn], repaired[maxn]; //并查集; 维护过的电脑 
int len = 0, d; //距离d 
struct computer {
	double x, y;
	bool isRepaired;
} c[maxn];

inline void init(int n) { 
	for (int i = 1; i <= n; ++i) {
		father[i] = -1;
		c[i].isRepaired = false; 
	}
}
inline int find(int x) {
    return father[x] < 0 ? x : father[x] = find(father[x]);
}
void merge(int a, int b) {
	int x = find(a), y = find(b);
	if (x == y) return;
	double dist = sqrt((c[a].x - c[b].x) * (c[a].x - c[b].x) + (c[a].y - c[b].y) * (c[a].y - c[b].y));
	if (dist <= d) {
		if (father[x] < father[y]) {
			father[x] += father[y];
			father[y] = x;
		} else {
			father[y] += father[x];
			father[x] = y;
		}
	}
}
inline bool query(int a, int b) { return find(a) == find(b); }

int main() {
	char ch[2];
	int n, p, q;
	scanf("%d%d", &n, &d);
	init(n);
	for (int i = 1; i <= n; ++i)
		scanf("%lf%lf", &c[i].x, &c[i].y);
	while (~scanf("%s%d", ch, &p)) {
		if (ch[0] == 'S') {
			scanf("%d", &q);
			if (!c[p].isRepaired || !c[q].isRepaired || !query(p, q))
				printf("FAIL\n"); 
			else
				printf("SUCCESS\n");
		} 
		else if (ch[0] == 'O') {
			c[p].isRepaired = true;
			repaired[len++] = p;	//记录维护过的电脑的编号 
			for (int i = 0; i < len - 1; ++i) //和其他已经维护过的电脑比较
				merge(repaired[i], p); 
		}
	}
    return 0;
} 
```

用G++提交：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200829195353510.png#pic_center)
改成C++：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020082919555224.png#pic_center)


