A family hierarchy is usually presented by a pedigree tree. Your job is to count those family members who have no child.
### Input Specification:

Each input file contains one test case. Each case starts with a line containing $0<N<100$ , the number of nodes in a tree, and $M (<N)$ , the number of non-leaf nodes. Then $M$ lines follow, each in the format:
```cpp
ID K ID[1] ID[2] ... ID[K]
```

where `ID` is a two-digit number representing a given non-leaf node, `K` is the number of its children, followed by a sequence of two-digit `ID`'s of its children. For the sake of simplicity, let us fix the root ID to be `01` .

The input ends with $N$ being $0$ . That case must NOT be processed.
### Output Specification:

For each test case, you are supposed to count those family members who have no child **for every seniority level** starting from the root. The numbers must be printed in a line, separated by a space, and there must be no extra space at the end of each line.

The sample case represents a tree with only 2 nodes, where `01` is the root and `02` is its only child. Hence on the root `01` level, there is `0` leaf node; and on the next level, there is `1` leaf node. Then we should output `0 1` in a line.
### Sample Input:

```clike
2 1
01 1 02
```
### Sample Output:
```swift
0 1
```

以前写的代码：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 101;
struct Node {
    int layer;
    vector<int> child;
} node[maxn];
int ans[maxn];

int LevelOrder(int r) {
	int max_level = 1;
    memset(ans, 0, sizeof(ans));
    node[r].layer = 0;
    queue<int> q;
    q.push(r);
    while (!q.empty()) {
        int front = q.front(); q.pop();
        if (node[front].layer + 1 > max_level) max_level++;
        if (node[front].child.size() == 0) ans[node[front].layer]++;
        for (int i = 0; i < node[front].child.size(); i++) {
            int child = node[front].child[i];
            node[child].layer = node[front].layer + 1;
            q.push(child);
        }
    }
    return max_level;
}

int main() {
    int n, nonleaf, id, k, t; //所有结点个数和非叶结点个数
    scanf("%d%d", &n, &nonleaf);
    for (int i = 0; i < nonleaf; i++) {
        scanf("%d%d", &id, &k);
        for (int j = 0; j < k; j++) {
            scanf("%d", &t);
            node[id].child.push_back(t);
        }
    }
    int m = LevelOrder(1);
    for (int i = 0; i < m; i++) {
        if (i > 0) printf(" ");
        printf("%d", ans[i]);
    }
    return 0;
}
```
现在写的代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 110;
int n, m, id, k, child; //n为树的节点数,m为非叶子节点的数目
vector<int> tree[maxn]; 
int main() {
    scanf("%d%d", &n, &m); 
    for (int i = 0; i < m; ++i) {
        scanf("%d%d", &id, &k);  
        for (int j = 0; j < k; ++j) {
            scanf("%d", &child);
            tree[id].push_back(child); 
        }
    }
    queue<int> q;
    q.push(1);
    vector<int> ans;
    while (!q.empty()) {
        int size = q.size(), leafNodes = 0;
        for (int i = 0; i < size; ++i) {
            int t = q.front(); q.pop();
            if (tree[t].empty()) ++leafNodes; //没有孩子的叶子节点+1
            for (const int &v : tree[t]) q.push(v);
        }
        ans.push_back(leafNodes);
    }
    for (int i = 0; i < ans.size(); ++i) 
        printf(" %d" + !i, ans[i]);
    return 0;
}
```
