### Problem Description
有N个比赛队（1<=N<=500），编号依次为1，2，3，。。。。，N进行比赛，比赛结束后，裁判委员会要将所有参赛队伍从前往后依次排名，但现在裁判委员会不能直接获得每个队的比赛成绩，只知道每场比赛的结果，即P1赢P2，用P1，P2表示，排名时P1在P2之前。现在请你编程序确定排名。
 

### Input
输入有若干组，每组中的第一行为二个数N（1<=N<=500），M；其中N表示队伍的个数，M表示接着有M行的输入数据。接下来的M行数据中，每行也有两个整数P1，P2表示即P1队赢了P2队。
 

### Output
给出一个符合要求的排名。输出时队伍号之间有空格，最后一名后面没有空格。

其他说明：符合条件的排名可能不是唯一的，此时要求输出时编号小的队伍在前；输入数据保证是正确的，即输入数据确保一定能有一个符合要求的排名。
 

### Sample Input
```csharp
4 3
1 2
2 3
4 3
```
### Sample Output
```csharp
1 2 4 3
```

 
---
### 解法 求最小字典序的拓扑序列
修改BFS拓扑排序，将普通队列改为优先队列，每次放入入度为0的点，出队编号最小的结点，然后把它的后续结点的入度减一，入度减为0的点再放入优先队列中。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 510;
vector<int> g[maxn], ans;
int n, m, indegrees[maxn], a, b;
int main() {
	while (~scanf("%d%d", &n, &m)) {
		for (int i = 0; i < maxn; ++i) g[i].clear();
		ans.clear();
		memset(indegrees, 0, sizeof(indegrees));
		
		for (int i = 0; i < m; ++i) {
			scanf("%d%d", &a, &b); //a->b
			g[a].push_back(b);
			++indegrees[b];
		}
		priority_queue<int, vector<int>, greater<int>> q;
		for (int i = 1; i <= n; ++i) if (indegrees[i] == 0) q.push(i);
		while (!q.empty()) {
			int u = q.top(); q.pop();
			ans.push_back(u);
			for (const int &v : g[u]) if (--indegrees[v] == 0) q.push(v);
		}
		for (int i = 0; i < ans.size(); ++i) printf(" %d" + !i, ans[i]); 
		printf("\n");
	}
	return 0;
} 
```
