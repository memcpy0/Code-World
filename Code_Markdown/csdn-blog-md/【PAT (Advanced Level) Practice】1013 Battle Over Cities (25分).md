It is vitally important to have all the cities connected by highways in a war. If a city is occupied by the enemy, all the highways from/toward that city are closed. We must know immediately if we need to repair any other highways to keep the rest of the cities connected. Given the map of cities which have all the remaining highways marked, you are supposed to tell the number of highways need to be repaired, quickly.

For example, if we have 3 cities and 2 highways connecting $city_1​-city_2​$ and $city_1-city_3$. Then if $city_1$​​ is occupied by the enemy, we must have 1 highway repaired, that is the highway $city_2​-city_3$. 
### Input Specification:

Each input file contains one test case. Each case starts with a line containing 3 numbers $N (<1000)$, $M$ and $K$, which are the total number of cities, the number of remaining highways, and the number of cities to be checked, respectively. Then $M$ lines follow, each describes a highway by 2 integers, which are the numbers of the cities the highway connects. The cities are numbered from 1 to $N$. Finally there is a line containing $K$ numbers, which represent the cities we concern.
### Output Specification:

For each of the $K$ cities, output in a line the number of highways need to be repaired if that city is lost.
### Sample Input:

```swift
3 2 3
1 2
1 3
1 2 3
```

### Sample Output:

```swift
1
0
0
```

题意：PAT的出题人语文老师死得早吧？？
在战争中，将所有的城市连接起来是至关重要的。如果一座城市被敌人占领，所有进出该城市的公路都将关闭。我们必须立即知道是否需要修复其他高速公路，以保持其他城市之间的联系。在地图上标出了所有剩下的高速公路后，你就应该很快地说出需要维修的高速公路的数量。


例如，如果我们有3个城市和2条公路连接$city_1 -city_2 $和$city_1-city_3$。然后如果$city_1被敌人占领，我们必须修复1条公路，即高速公路$city_2 -city_3。

# # #输入规格:


每个输入文件包含一个测试用例。每个案例以一行包含3个数字$N(<1000)$、$M$和$K$的行开始，分别是城市总数、剩余公路数和待检查城市数。然后是$M$线，每条线用两个整数描述公路，这两个整数是公路连接的城市的数量。城市编号从1到$N$。最后有一行包含$K$数字，表示我们所关心的城市。

# # #输出规范:


对于每一个$K$的城市，输出一列如果城市失去了需要修复的高速公路数量。

---
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxv = 1010;
int vis[maxv], n, lost;
vector<int> G[maxv];
void dfs(int v) {
	vis[v] = true;
	for (int i = 0; i < G[v].size(); i++) {
		int u = G[v][i];
		if (vis[u] == false) dfs(u);
	}
}
int trave() {
	memset(vis, 0, sizeof(vis));
	vis[lost] = 1; //标记这个结点已经被敌人占据, 
	//因此不会访问这个结点及边 
	int comps = 0;
	for (int i = 1; i <= n; i++) {
		if (vis[i] == false) {
			comps++;
			dfs(i);
		}
	}
	return comps;
}
int main() {
	int m, k, u, v;
	scanf("%d%d%d", &n, &m, &k); 
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &u, &v); //从1编号  
		G[u].push_back(v);
		G[v].push_back(u); //无向图 
	}
	while (k--) {
		scanf("%d", &lost);
		//该城市被攻陷了
		printf("%d\n", trave() - 1);  
	}
	return 0;
}
```
