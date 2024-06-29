It is vitally important to have all the cities connected by highways in a war. If a city is occupied by the enemy, all the highways from/toward that city are closed. We must know immediately if we need to repair any other highways to keep the rest of the cities connected. Given the map of cities which have all the remaining highways marked, you are supposed to tell the number of highways need to be repaired, quickly.

For example, if we have 3 cities and 2 highways connecting city​1​​-city​2​​ and city​1​​-city​3​​. Then if city​1​​ is occupied by the enemy, we must have 1 highway repaired, that is the highway city​2​​-city​3​​.
## Input Specification:

Each input file contains one test case. Each case starts with a line containing 3 numbers $N$ ($<1000$), $M$ and $K$ , which are the total number of cities, the number of remaining highways, and the number of cities to be checked, respectively. Then $M$ lines follow, each describes a highway by 2 integers, which are the numbers of the cities the highway connects. The cities are numbered from 1 to $N$ . Finally there is a line containing $K$ numbers, which represent the cities we concern.

## Output Specification:

For each of the $K$ cities, output in a line the number of highways need to be repaired if that city is lost.

## Sample Input:

```cpp
3 2 3
1 2
1 3
1 2 3
```

## Sample Output:

```cpp
1
0
0
```

---
### 解法1 DFS求连通分量数
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
int n, m, k, a, b;
vector<int> g[maxn];
bool vis[maxn];
void dfs(int u) {
    vis[u] = true;
    for (const int &v : g[u]) if (!vis[v]) dfs(v);
} 
int main() {
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d", &a, &b);
        g[a].push_back(b);
        g[b].push_back(a);
    }
    for (int i = 0; i < k; ++i) {
        scanf("%d", &a);
        memset(vis, false, sizeof(vis));
        vis[a] = true; //占领这座城市,相当于这一点已经被访问,我们不会再次访问和经过这一点
        int comps = 0;
        for (int i = 1; i <= n; ++i) if (!vis[i]) { ++comps; dfs(i); }
        printf("%d\n", comps - 1);
    }
    return 0;
}
```
---
### 解法2 并查集求连通分量数
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e3 + 10;
vector<int> g[maxn];
int father[maxn], n, m, k, t, a, b;
int find(int x) { 
    if (father[x] < 0) return x;
    return father[x] = find(father[x]);
}
void merge(int x, int y) {
    if (father[x] < father[y]) father[x] += father[y], father[y] = x;
    else father[y] += father[x], father[x] = y;
}
int comps() {
    for (int u = 1; u <= n; ++u) {
        if (u == t) continue; //不能从城市t出发
        for (const int &v : g[u]) {
            if (v == t) continue; //不能到达城市t
            int r1 = find(u), r2 = find(v);
            if (r1 != r2) merge(r1, r2);
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; ++i) if (i != t && father[i] < 0) ++ans; //不是被攻占的城市
    return ans;
}
int main() {
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d", &a, &b);
        g[a].push_back(b);
        g[b].push_back(a);
    }
    for (int i = 0; i < k; ++i) {
        memset(father, -1, sizeof(father));
        scanf("%d", &t); //城市t被攻占
        printf("%d\n", comps() - 1); //要重建的路等于连通分量个数-1
    }
    return 0;
}
```
