
```cpp
Time Limit: 5000/2000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)
Total Submission(s): 28651    Accepted Submission(s): 7845
```

### Problem Description
Farmer John has been informed of the location of a **fugitive** cow and wants to catch her immediately. He **starts** at a point N (0 ≤ N ≤ 100,000) on **a number line** and the cow is at a point K (0 ≤ K ≤ 100,000) on the same number line. Farmer John has two modes of transportation: walking and teleporting.

* Walking: FJ can move from any point `X` to the points `X - 1` or `X + 1` in a **single** minute
* Teleporting: FJ can move from any point `X` to the point `2 × X` in a **single** minute.

If the cow, unaware of its pursuit, **does not move at all**, how long does it take for Farmer John to retrieve it?
 
Input
Line 1: Two **space-separated** integers: N and K
 
Output
Line 1: The **least** amount of time, in minutes, it takes for Farmer John to catch the fugitive cow.
 

Sample Input
```
5 17
```
 

Sample Output
```
4
```
Hint
The **fastest** way for Farmer John to reach the fugitive cow is to move along the following path:`5-10-9-18-17`, which takes 4 minutes

---

题意：一头牛逃跑了，跑到了$K$处，牛不会动。人从$N$处追这头牛，在一条数轴上行走。人可以移动到$N-1$，$N+1$和$2*N$处，每次移动消耗一分钟。要求最少的追到牛的时间。


思路：在我看来，这道题用来做**BFS入门**很合适，与之前做过的“Red And Black”相比，这个可以说是一维的，是一个一维的遍历。然后有三种情况，分别是前进一步，后退一步，前进二倍的步数。因此也可以说是在遍历一个`三叉树`。由于是无权最短路，因此用BFS是最合适的。 

 以题目的示例为例：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020030200324432.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)注意：
- 第一点，这里不会只输入一个$N \quad K$，HDU一向都是在一个程序中测试多个用例，我一开始没注意到；
- 由于每个结点扩展出三个结点，因此遍历的复杂度飞速上升；我们必须用边界剪去一些“**树枝**”，不然会超过空间。

剪枝：
- 扩展时，用一个$vis$数组记录，**不能扩展出已经走过的节点**，即不能回退，以免陷入无效的重复扩展和可能的循环；
- 如果输入的$N \gt K$，那么只有往后走一条路，因此只需要$N - K$分钟；
- 如果当前的$val - 1 \geq 0$，才扩展$val - 1$结点；
- 如果当前的$val + 1 \leq 100,000$，才扩展$val + 1$结点；
- 如果当前的$val * 2 \leq 100,000$，才扩展$2 * val$结点。

最终，我们最多只会扩展出$100,000$左右个结点，大大降低了复杂度。

代码如下：
这里我没有使用传统的BFS写法，使用一个$struct$包裹一个表示层数的变量，而是每次直接一个$for$循环将下一层可扩展的结点压入队列中。因此找到牛的时候需要一次性跳出两重循环，所以用了$goto$，不用也可以，不过多写几句。
```cpp
#include <bits/stdc++.h> 
using namespace std;
const int maxn = 100100;
bool vis[maxn] = {false}; 

int main() {
	int n, k;
	queue<int> q;
	while (~scanf("%d %d", &n, &k)) { 
		if (n > k) {
			printf("%d\n", n - k);
			continue;
		}
		int num = -1;
		memset(vis, 0, sizeof(vis));
		q.push(n);
		vis[n] = true;
		while (!q.empty()) {
			int size = q.size();
			++num;
			for (int i = 0; i < size; ++i) {  
				int v = q.front(); q.pop(); 
				if (v == k) {
					printf("%d\n", num);
					goto end;
				}
				if (v - 1 >= 0 && !vis[v - 1]) q.push(v - 1), vis[v - 1] = true;
				if (v + 1 <= 100000 && !vis[v + 1]) q.push(v + 1), vis[v + 1] = true;
				if (v != 0 && 2 * v <= 100000 && !vis[2 * v]) q.push(2 * v), vis[2 * v] = true;
			}
		}
		end: 
		while (!q.empty()) q.pop();
	}
	return 0;
}
```


