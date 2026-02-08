As an emergency rescue team leader of a city, you are given a special map of your country. The map shows several scattered cities connected by some roads. Amount of rescue teams in each city and the length of each road between any pair of cities are marked on the map. When there is an emergency call to you from some other city, your job is to lead your men to the place as quickly as possible, and at the mean time, call up as many hands on the way as possible.
### Input Specification:

Each input file contains one test case. For each test case, the first line contains 4 positive integers: $N (≤500)$ - the number of cities (and the cities are numbered from $0$ to $N−1$ ), $M$ - the number of roads, $C_​1$​​ and $C_​2$​​ - the cities that you are currently in and that you must save, respectively. The next line contains $N$ integers, where the $i$-th integer is the number of rescue teams in the $i$-th city. Then $M$ lines follow, each describes a road with three integers $c_​1$​​, $c_​2$​​ and $L$ , which are the pair of cities connected by a road and the length of that road, respectively. It is guaranteed that there exists at least one path from $C_​1$​​ to $C_​2$ ​​.
### Output Specification:

For each test case, print in one line two numbers: the number of different shortest paths between $C_1$​​ and $C_2$ ​​, and the maximum amount of rescue teams you can possibly gather. All the numbers in a line must be separated by exactly one space, and there is no extra space allowed at the end of a line.
### Sample Input:

```swift
5 6 0 2
1 2 1 5 3
0 1 1
0 2 2
0 3 1
1 2 1
2 4 1
3 4 1
```

### Sample Output:

```swift
2 4
```

```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 510, inf = 0x3f3f3f3f;
int n, m, s, e;        //n个城市0~n-1,m条道路,起点s,终点e
int c1, c2, len;       //城市1,城市2,距离
int rescueTeams[maxn]; //0~n-1每座城市的急救队数量
int dis[maxn];         //每座城市到起点的距离
bool vis[maxn];        //是否被访问过
struct edge { int to, w; }; //w是距离
vector<edge> lst[maxn]; 
int num[maxn], weight[maxn]; //从s到达顶点i的最短路径的数目,从s到达顶点i的最多急救队数量

void dijkstra(int s, int e) { 
    fill(dis, dis + maxn, inf);
    fill(vis, vis + maxn, false);
    fill(num, num + maxn, 0);
    fill(weight, weight + maxn, 0);
    dis[s] = 0; weight[s] = rescueTeams[s]; num[s] = 1; //初始化起点
    for (int i = 0; i < n; ++i) {
        int minDist = inf, u = -1; //找到距离开始城市最短的城市
        for (int j = 0; j < n; ++j) {
            if (!vis[j] && dis[j] < minDist) { //未访问过
                minDist = dis[j];
                u = j;
            }
        }
        if (u == -1) break; //不用循环
        vis[u] = true; 
        for (int j = 0; j < lst[u].size(); ++j) {
            int v = lst[u][j].to, w = lst[u][j].w; 
            if (vis[v]) continue;      //曾访问过该点
            if (dis[v] > dis[u] + w) { //以u为中介点可以使v到s的距离减少
                dis[v] = dis[u] + w;   //更新距离
                num[v] = num[u];       //继承s到达u的最短路数目
                weight[v] = weight[u] + rescueTeams[v]; //在路径最短的情况下,使得救援队数量最多,更新点权
            } else if (dis[v] == dis[u] + w) {//不用更新距离 
                num[v] += num[u];      //加上新的最短路数目 
                if (weight[u] + rescueTeams[v] > weight[v]) 
                    weight[v] = weight[u] + rescueTeams[v]; //判断是否可以得到更大的点权
            } 
        }
    }
}

int main() {
    scanf("%d%d%d%d", &n, &m, &s, &e);
    for (int i = 0; i < n; ++i) scanf("%d", &rescueTeams[i]);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d", &c1, &c2, &len);
        lst[c1].push_back(edge{c2, len});
        lst[c2].push_back(edge{c1, len});
    } 
    dijkstra(s, e);
    printf("%d %d", num[e], weight[e]);
    return 0;
}
```
使用优先队列的版本如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 510, inf = 0x3f3f3f3f;
int n, m, s, e;        //n个城市0~n-1,m条道路,起点s,终点e
int c1, c2, len;       //城市1,城市2,距离
int rescueTeams[maxn]; //0~n-1每座城市的急救队数量
int dis[maxn];         //每座城市到起点的距离
bool vis[maxn];        //是否被访问过
struct edge { int to, w; }; //w是距离
vector<edge> lst[maxn]; 
struct s_node {
    int to, w;
    s_node() {}
    s_node(int _t, int _w) : to(_t), w(_w) {}
    bool operator<(const s_node& a) const {
        return w > a.w;      
    }
};
int num[maxn], weight[maxn]; //从s到达顶点i的最短路径的数目,从s到达顶点i的最多急救队数量
void dijkstra(int s, int e) { 
    fill(dis, dis + maxn, inf);
    fill(vis, vis + maxn, false);
    fill(num, num + maxn, 0);
    fill(weight, weight + maxn, 0);
    dis[s] = 0; weight[s] = rescueTeams[s]; num[s] = 1;
    priority_queue<s_node> pq;
    pq.push(s_node(s, 0)); //起点
    while (!pq.empty()) {
        s_node t = pq.top(); pq.pop(); //找到距离开始城市最短的城市
        int u = t.to;
        if (vis[u]) continue;
        vis[u] = true; 
        for (int j = 0; j < lst[u].size(); ++j) {
            int v = lst[u][j].to, w = lst[u][j].w; 
            if (vis[v]) continue;      //曾访问过该点
            if (dis[v] > dis[u] + w) { //以u为中介点可以使v到s的距离减少
                dis[v] = dis[u] + w;   //更新距离
                num[v] = num[u];       //继承s到达u的最短路数目
                weight[v] = weight[u] + rescueTeams[v]; //在路径最短的情况下,使得救援队数量最多,更新点权
                pq.push(s_node(v, dis[v]));
            } else if (dis[v] == dis[u] + w) {//不用更新距离 
                num[v] += num[u];      //加上新的最短路数目 
                if (weight[u] + rescueTeams[v] > weight[v]) 
                    weight[v] = weight[u] + rescueTeams[v]; //判断是否可以得到更大的点权
                pq.push(s_node(v, dis[v]));
            } 
        }
    }
}

int main() {
    scanf("%d%d%d%d", &n, &m, &s, &e);
    for (int i = 0; i < n; ++i) scanf("%d", &rescueTeams[i]);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d", &c1, &c2, &len);
        lst[c1].push_back(edge{c2, len});
        lst[c2].push_back(edge{c1, len});
    } 
    dijkstra(s, e);
    printf("%d %d", num[e], weight[e]);
    return 0;
}
```
