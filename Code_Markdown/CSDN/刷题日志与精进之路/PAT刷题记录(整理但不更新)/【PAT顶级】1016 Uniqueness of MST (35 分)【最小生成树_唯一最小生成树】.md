

Given any weighted undirected graph, there exists at least one minimum spanning tree (MST) if the graph is connected. Sometimes the MST may not be unique though. Here you are supposed to calculate the minimum total weight of the MST, and also tell if it is unique or not.
## Input Specification:

Each input file contains one test case. Each case starts with a line containing 2 numbers N ($≤ 500$), and M, which are the total number of vertices, and the number of edges, respectively. Then M lines follow, each describes an edge by 3 integers:

```css
V1 V2 Weight
```

where `V1` and `V2` are the two ends of the edge (the vertices are numbered from 1 to N), and `Weight` is the positive weight on that edge. It is guaranteed that the total weight of the graph will not exceed $2^{30}$​​.
## Output Specification:

For each test case, first print in a line the total weight of the minimum spanning tree if there exists one, or else print `No MST` instead. Then if the MST exists, print in the next line `Yes` if the tree is unique, or `No` otherwise. There there is no MST, print the number of connected components instead.
## Sample Input 1:

```clike
5 7
1 2 6
5 1 1
2 3 4
3 4 3
4 1 7
2 4 2
4 5 5
```

## Sample Output 1:

```clike
11
Yes
```

## Sample Input 2:

```clike
4 5
1 2 1
2 3 1
3 4 2
4 1 2
3 1 3
```

## Sample Output 2:

```clike
4
No
```

## Sample Input 3:

```clike
5 5
1 2 1
2 3 1
3 4 2
4 1 2
3 1 3
```

## Sample Output 3:

```clike
No MST
2
```

---
很快有时间了就更新题意和解题思路：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 510, inf = 0x3f3f3f3f;
struct node { int to, w; };
vector<node> g[maxn];
int n, m, v1, v2, w, d[maxn], cnt[maxn]; //d[i]为顶点i到MST的最短距离,cnt[i]为MST到顶点i有几条最短边
bool vis[maxn];
void dfs(int u) {
    vis[u] = true;
    for (const node &tmp : g[u]) if (!vis[tmp.to]) dfs(tmp.to);
}
int comps() { //从1到N
    int nums = 0;
    memset(vis, false, sizeof(vis));
    for (int i = 1; i <= n; ++i) if (!vis[i]) { dfs(i); ++nums; }
    return nums;
}
int prim(int s) {
    memset(d, inf, sizeof(d));
    memset(vis, false, sizeof(vis));
    d[s] = 0;
    cnt[s] = 1; 
    int mstWeight = 0;
    for (int i = 1; i <= n; ++i) {
        int u = -1, minDist = inf;
        for (int j = 1; j <= n; ++j) 
            if (!vis[j] && d[j] < minDist) minDist = d[j], u = j;
        if (minDist == inf) break; //没有点可以纳入MST中
        vis[u] = true; //访问该点
        mstWeight += d[u];
        for (int j = 0; j < g[u].size(); ++j) {
            int v = g[u][j].to, w = g[u][j].w;
            if (vis[v]) continue;
            if (w < d[v]) d[v] = w, cnt[v] = cnt[u];
            else if (w == d[v]) cnt[v] += cnt[u];
        }
    }
    return mstWeight;
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d", &v1, &v2, &w);
        g[v1].push_back(node{v2, w});
        g[v2].push_back(node{v1, w});
    }
    int components = comps();
    if (components > 1) { printf("No MST\n%d\n", components); return 0; } //没有MST
    bool isMSTUnique = true;
    int mstWeight = prim(1);
    for (int i = 1; i <= n; ++i) if (cnt[i] != 1) { isMSTUnique = false; break; }
    printf("%d\n%s\n", mstWeight, isMSTUnique ? "Yes" : "No");
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522191028358.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

