<p>You are given a tree with&nbsp;<code>n</code>&nbsp;nodes numbered from&nbsp;<code>0</code>&nbsp;to&nbsp;<code>n-1</code>&nbsp;in the form of a parent array where <code>parent[i]</code>&nbsp;is the parent of node <code>i</code>. The root of the tree is node <code>0</code>.</p>

<p>Implement the function&nbsp;<code>getKthAncestor</code><code>(int node, int k)</code>&nbsp;to return the <code>k</code>-th ancestor of the given&nbsp;<code>node</code>. If there is no such ancestor, return&nbsp;<code>-1</code>.</p>

<p>The&nbsp;<em>k-th&nbsp;</em><em>ancestor</em>&nbsp;of a tree node is the <code>k</code>-th node&nbsp;in the path&nbsp;from that node to the root.</p>
 
<p><strong>Example:</strong></p>

<p><strong><img style="width: 396px; height: 262px;" src="https://assets.leetcode.com/uploads/2019/08/28/1528_ex1.png" alt=""></strong></p>

```clike
Input:
["TreeAncestor","getKthAncestor","getKthAncestor","getKthAncestor"]
[[7,[-1,0,0,1,1,2,2]],[3,1],[5,2],[6,3]]

Output:
[null,1,0,-1]

Explanation:
TreeAncestor treeAncestor = new TreeAncestor(7, [-1, 0, 0, 1, 1, 2, 2]);

treeAncestor.getKthAncestor(3, 1);  // returns 1 which is the parent of 3
treeAncestor.getKthAncestor(5, 2);  // returns 0 which is the grandparent of 5
treeAncestor.getKthAncestor(6, 3);  // returns -1 because there is no such ancestor
```

 
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= k &lt;=&nbsp;n &lt;= 5*10^4</code></li>
	<li><code>parent[0] == -1</code>&nbsp;indicating that&nbsp;<code>0</code>&nbsp;is the root node.</li>
	<li><code>0 &lt;= parent[i] &lt; n</code>&nbsp;for all&nbsp;<code>0 &lt;&nbsp;i &lt; n</code></li>
	<li><code>0 &lt;= node &lt; n</code></li>
	<li>There will be at most <code>5*10^4</code> queries.</li>
</ul>


题意：给你一棵树，树上有 `n` 个节点，按从 `0` 到 `n-1` 编号。树以父节点数组的形式给出，其中 `parent[i]` 是节点 `i` 的父节点。树的根节点是编号为 `0` 的节点。

请你设计并实现 `getKthAncestor(int node, int k)` 函数，函数返回节点 `node` 的第 `k` 个祖先节点。如果不存在这样的祖先节点，返回 `-1` 。树节点的第 `k` 个祖先节点是从该节点到根节点路径上的第 `k` 个节点。
 
---
### 解法 动态规划+倍增法
这道题倒是不难，不过实现了这个类之后，就可以用 $O(\log n)$ 的时间复杂度计算两个节点的最近公共祖先，只要先进行一次 $O(n\log n)$ 时间复杂度的预处理。
```cpp
class TreeAncestor {
private:
    int depth[50001]; //记录某个节点处在的层级
    int Log2[50001]; //Log2[i]计算log2(i)
    int father[50001][20]; //father[i][j]存储节点i的2^j级祖先
public: //n个节点从0~n-1
    TreeAncestor(int n, vector<int>& parent) {
        depth[0] = 0;
        Log2[0] = Log2[1] = 0;
        for (int i = 2; i <= n; ++i) Log2[i] = Log2[i / 2] + 1; 
        memset(father, -1, sizeof(father)); //初始化为-1 
        for (int i = 1; i < n; ++i) {
            father[i][0] = parent[i]; //存储节点i的2^0=1级祖先,即其父节点
            depth[i] = depth[parent[i]] + 1; //计算某个节点的深度 
        }
        for (int i = 0; i < n; ++i) { 
            for (int j = 1; j <= Log2[depth[i]]; ++j) //只用处理Log2[depth[i]]级祖先
                father[i][j] = father[father[i][j - 1]][j - 1];
        }
    }
    // int getKthAncestor(int node, int k) { //返回k级祖先(迭代写法)
    //     if (k == 0) return node; //0级祖先就是自己
    //     while (k) {
    //         int z = Log2[k];
    //         node = father[node][z]; 
    //         if (node == -1) return -1; //不存在这样的祖先
    //         k -= (1 << z);
    //     }
    //     return node; 
    // }
    int getKthAncestor(int node, int k) { //返回k级祖先
        if (k == 0 || node == -1) return node; //0级祖先就是自己; 或者不存在这样的祖先
        int z = Log2[k];
        return getKthAncestor(father[node][z], k - (1 << z));
    }    
};
```
运行效率如下：
```cpp
执行用时：312 ms, 在所有 C++ 提交中击败了97.34% 的用户
内存消耗：142.1 MB, 在所有 C++ 提交中击败了10.62% 的用户
```
