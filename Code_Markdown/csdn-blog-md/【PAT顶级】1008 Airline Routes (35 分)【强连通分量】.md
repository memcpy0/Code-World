

Given a map of airline routes, you are supposed to check if a round trip can be planned between any pair of cities.
## Input Specification:

Each input file contains one test case. For each case, the first line gives two positive integers $N (2≤N≤10^4$​​) and $M$ ($≤6N$), which are the total number of cities (hence the cities are numbered from 1 to $N$) and the number of airline routes, respectively. Then $M$ lines follow, each gives the information of a route in the format of the source city index first, and then the destination city index, separated by a space. It is guaranteed that the source is never the same as the destination.

After the map information, another positive integer $K$ is given, which is the number of queries. Then $K$ lines of queries follow, each contains a pair of distinct cities' indices.
## Output Specification:

For each query, output in a line `Yes` if a round trip is possible, or `No` if not.
## Sample Input:

```go
12 19
3 4
1 3
12 11
5 9
6 2
3 2
10 7
9 1
7 12
2 4
9 5
2 6
12 4
11 10
4 8
8 12
11 8
12 7
1 5
20
11 4
12 7
3 6
2 3
5 3
3 9
4 3
8 3
8 10
10 11
7 8
7 1
9 5
1 9
2 6
3 1
3 12
7 3
6 9
6 8
```

## Sample Output:

```go
Yes
Yes
No
No
No
No
No
No
Yes
Yes
Yes
No
Yes
Yes
Yes
No
No
No
No
No
```

---
很快有时间了就更新题意和解法：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e4 + 10;
int n, m, a, b, k;
vector<int> g[maxn];
stack<int> st;
bool inStack[maxn];
int low[maxn], dfn[maxn], tot, sccnum = 1, scc[maxn]; //scc[i]表示顶点i所在的强连通分量
void tarjan(int u) {
    dfn[u] = low[u] = ++tot;
    st.push(u);
    inStack[u] = true;
    for (const int &v : g[u]) {
        if (!dfn[v]) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (inStack[v]) low[u] = min(low[u], dfn[v]); //在栈上
    }
    if (dfn[u] == low[u]) {
        while (!st.empty()) {
            int t = st.top(); st.pop();
            inStack[t] = false;
            scc[t] = sccnum;
            if (t == u) break;
        }
        ++sccnum; //连通分量数+1
    }
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d", &a, &b);
        g[a].push_back(b);
    }
    for (int i = 1; i <= n; ++i) if (!dfn[i]) tarjan(i); //求出所有的强连通分量
    scanf("%d", &k);
    for (int i = 0; i < k; ++i) {
        scanf("%d%d", &a, &b);
        printf("%s\n", scc[a] == scc[b] ? "Yes" : "No");
    }
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522192546939.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

