It is vitally important to have all the cities connected by highways in a war. If a city is conquered by the enemy, all the highways from/toward that city will be closed. To keep the rest of the cities connected, we must repair some highways with the minimum cost. On the other hand, if losing a city will cost us too much to rebuild the connection, we must pay more attention to that city.

Given the map of cities which have all the destroyed and remaining highways marked, you are supposed to point out the city to which we must pay the most attention.

## Input Specification:
Each input file contains one test case. Each case starts with a line containing 2 numbers `N` (≤500), and `M` , which are the total number of cities, and the number of highways, respectively. Then `M` lines follow, each describes a highway by 4 integers: `City1 City2 Cost Status` where `City1` and `City2` are the numbers of the cities the highway connects (the cities are numbered from 1 to `N`), `Cost` is the effort taken to repair that highway if necessary, and `Status` is either 0, meaning that highway is destroyed, or 1, meaning that highway is in use.

Note: It is guaranteed that the whole country was connected before the war.

## Output Specification:

For each test case, just print in a line the city we must protest the most, that is, it will take us the maximum effort to rebuild the connection if that city is conquered by the enemy.

In case there is more than one city to be printed, output them in increasing order of the city numbers, separated by one space, but no extra space at the end of the line. In case there is no need to repair any highway at all, simply output 0.

## Sample Input 1:

```clike
4 5
1 2 1 1
1 3 1 1
2 3 1 0
2 4 1 1
3 4 1 0
```

## Sample Output 1:

```clike
1 2
```

## Sample Input 2:

```clike
4 5
1 2 1 1
1 3 1 1
2 3 1 0
2 4 1 1
3 4 2 1
```

## Sample Output 2:

```clike
0
```


---
现在先把代码放上来，很快有时间了就更新题意、解法。

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 510, inf = 0x3f3f3f3f;
struct node { int to, cost, stat; };
vector<node> g[maxn];
bool vis[maxn];
int n, m, a, b, co, st, c[maxn], maxMSTCost = 0; //c[i]为点i到生成树顶点集合的最小花费
int prim(int s) {
    int mstCostSum = 0, cnt = 0;
    c[s] = 0;
    for (int i = 1; i <= n; ++i) {
        int u = -1, minCost = inf;
        for (int j = 1; j <= n; ++j) 
            if (!vis[j] && c[j] < minCost) u = j, minCost = c[j];
        if (minCost == inf) break; //两个INT_MAX,如果遇到割点被攻占,分成不同的连通分量,此时有的点无法访问为inf<INT_MAX,这里不会退出
        vis[u] = true; //这个城市被确认
        ++cnt;
        mstCostSum += minCost; //最小生成树又确认一个顶点,加上其离生成树顶点集合的最小cost
        for (int j = 0; j < g[u].size(); ++j) {
            int v = g[u][j].to, stat = g[u][j].stat, cost = stat ? 0 : g[u][j].cost; //由u到v的道路还能用
            if (!vis[v] && cost < c[v]) c[v] = cost;
        }
    }
    return cnt == n - 1 ? mstCostSum : inf;
}
int main() { 
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d%d", &a, &b, &co, &st);
        g[a].push_back({b, co, st});
        g[b].push_back({a, co, st});
    }
    vector<int> ans;
    for (int i = 1; i <= n; ++i) { //整个国家的城市开始都是连通的
        //令城市i被攻占,前往或者离开城市i的道路会关闭,为使剩下的城市连通,需要修复一些道路
        //修复所需要的成本越大,我们对城市i的事先关注越大
        memset(vis, false, sizeof(vis));
        memset(c, inf, sizeof(c));
        vis[i] = true;
        int tot = prim(i % n + 1);
        if (tot > maxMSTCost) maxMSTCost = tot, ans = vector<int>{i};
        else if (tot == maxMSTCost) ans.push_back(i);
    }
    if (!maxMSTCost) { printf("0"); return 0; }
    for (int i = 0; i < ans.size(); ++i) printf("%d%c", ans[i], i + 1 != ans.size() ? ' ' : '\n');
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522185816895.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

