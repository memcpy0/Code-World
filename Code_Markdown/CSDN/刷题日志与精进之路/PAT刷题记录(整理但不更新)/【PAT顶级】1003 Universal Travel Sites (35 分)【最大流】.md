

After finishing her tour around the Earth, CYLL is now planning a universal travel sites development project. After a careful investigation, she has a list of capacities of all the satellite transportation stations in hand. To estimate a budget, she must know the minimum capacity that a planet station must have to guarantee that every space vessel can dock and download its passengers on arrival.
## Input Specification:

Each input file contains one test case. For each case, the first line contains the names of the source and the destination planets, and a positive integer `N` ($≤500$). Then `N` lines follow, each in the format: `source[i] destination[i] capacity[i] ` where `source[i]` and `destination[i]` are the names of the satellites and the two involved planets, and `capacity[i] > 0` is the maximum number of passengers that can be transported at one pass from `source[i]` to `destination[i]` . Each name is a string of 3 uppercase characters chosen from {A-Z}, e.g., ZJU.

Note that the satellite transportation stations have no accommodation facilities for the passengers. Therefore none of the passengers can stay. Such a station will not allow arrivals of space vessels that contain more than its own capacity. It is guaranteed that the list contains neither the routes to the source planet nor that from the destination planet.

## Output Specification:

For each test case, just print in one line the minimum capacity that a planet station must have to guarantee that every space vessel can dock and download its passengers on arrival.
## Sample Input: 
```clike
EAR MAR 11
EAR AAA 300
EAR BBB 400
AAA BBB 100
AAA CCC 400
AAA MAR 300
BBB DDD 400
AAA DDD 400
DDD AAA 100
CCC MAR 400
DDD CCC 200
DDD MAR 300
```

## Sample Output:

```clike
700
```

---
很快有时间了就更新题意和解法，还有更优的代码如Dinic算法。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1000, inf = 0x3f3f3f3f;
int graph[maxn][maxn], pre[maxn], tot = 0, n; //tot为节点数,n为边数
int bfs(int s, int t) {
    int flow[maxn] = {0};
    memset(pre, -1, sizeof(pre));
    flow[s] = inf; pre[s] = 0; //初始化起点
    queue<int> q; q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == t) break;
        for (int i = 1; i <= tot; ++i) { //BFS所有的邻接点
            if (i != s && graph[u][i] > 0 && pre[i] == -1) {
                pre[i] = u; //记录路径
                q.push(i);
                flow[i] = min(flow[u], graph[u][i]); //更新节点流量(计算增广路上的最小流量)
            }
        }
    }
    return pre[t] == -1 ? -1 : flow[t]; //没有找到新的增广路就返回-1,否则返回这个增广路的流量
}
int max_flow(int s, int t) {
    int MaxFlow = 0;
    while (true) {
        int flow = bfs(s, t); //执行一次BFS,找到一条增广路,返回路径的流量
        if (flow == -1) break; //不存在新的增广路,结束
        int cur = t; 
        while (cur != s) { //一直沿着路径回溯到起点
            int father = pre[cur]; //pre[]记录路径上的前一个点
            graph[father][cur] -= flow; //更新残留网络,正向边减流量
            graph[cur][father] += flow; //更新残留网络,反向边加流量
            cur = father;
        }
        MaxFlow += flow;
    }
    return MaxFlow;
}

unordered_map<string, int> rec;
string a, b;
int c;
int main() {
    cin >> a >> b >> n;
    rec[a] = ++tot, rec[b] = ++tot; 
    for (int i = 0; i < n; ++i) {
        cin >> a >> b >> c;
        if (rec.find(a) == rec.end()) rec[a] = ++tot;
        if (rec.find(b) == rec.end()) rec[b] = ++tot;
        int u = rec[a], v = rec[b]; 
        graph[u][v] = c;
    } 
    printf("%d\n", max_flow(1, 2)); //起点1,终点2
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522190529966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

