> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两棵二叉树 `root` 和 `subRoot` 。检验 `root` 中是否包含和 `subRoot` 具有相同结构和节点值的子树。如果存在，返回 `true` ；否则，返回 `false` 。

二叉树 `tree` 的一棵子树包括 `tree` 的某个节点和这个节点的所有后代节点。`tree` 也可以看做它自身的一棵子树。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/04/28/subtree1-tree.jpg)
```cpp
输入：root = [3,4,5,1,2], subRoot = [4,1,2]
输出：true
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2021/04/28/subtree2-tree.jpg)
```rust
输入：root = [3,4,5,1,2,null,null,null,null,0], subRoot = [4,1,2]
输出：false
```
**提示：**
- `root` 树上的节点数量范围是 `[1, 2000]`
- `subRoot` 树上的节点数量范围是 `[1, 1000]`
- `-10^4 <= root.val <= 10^4`
- `-10^4 <= subRoot.val <= 10^4`

---
### 解法1 嵌套DFS暴力匹配（剪枝最优化）
 相当于回顾了 `LeetCode 100. 相同的树` 这一题。我们先序DFS遍历 `s` 的所有结点，对以当前结点为根的每个子树，都和 `t` 进行**树是否相同**的判断。如何判断一个节点的子树是否和 t 相等呢，我们又需要做一次DFS来检查，即让两个指针一开始先指向该节点和 `t` 的根，然后「同步移动」两根指针来「同步遍历」这两棵树，判断**对应位置**是否相等。
```cpp
class Solution {
public:
    bool isSubtree(TreeNode* s, TreeNode* t) {
        if (s == nullptr) return false; //s遍历完了,t给定为非空树,所以返回false
        return isSameTree(s, t) || isSubtree(s->left, t) || isSubtree(s->right, t); //短路运算符,有一个为真,返回真
    }
    //LeetCode 100题 相同的树的那个函数
    bool isSameTree(TreeNode *s, TreeNode *t) {
        if (!s && !t) return true;  //两子树相同
        if (!s || !t || s->val != t->val) return false; //有一个不为空
        return isSameTree(s->left, t->left) && isSameTree(s->right, t->right);
    }
};
```
```java
class Solution {
    public boolean isSubtree(TreeNode s, TreeNode t) {
        return dfs(s, t);
    }

    public boolean dfs(TreeNode s, TreeNode t) {
        if (s == null) {
            return false;
        }
        return check(s, t) || dfs(s.left, t) || dfs(s.right, t);
    }

    public boolean check(TreeNode s, TreeNode t) {
        if (s == null && t == null) {
            return true;
        }
        if (s == null || t == null || s.val != t.val) {
            return false;
        }
        return check(s.left, t.left) && check(s.right, t.right);
    }
}
```
```go
func isSubtree(s *TreeNode, t *TreeNode) bool {
    if s == nil {
        return false
    }
    return check(s, t) || isSubtree(s.Left, t) || isSubtree(s.Right, t)
}

func check(a, b *TreeNode) bool {
    if a == nil && b == nil {
        return true
    }
    if a == nil || b == nil {
        return false
    }
    if a.Val == b.Val {
        return check(a.Left, b.Left) && check(a.Right, b.Right)
    }
    return false
}
```
**复杂度分析**
- 时间复杂度：对于每一个 $s$ 上的点，都需要做一次深度优先搜索来和 $t$ 匹配，匹配一次的时间代价是 $O(|\ t\ |)$ ，那么总的时间代价就是 $O(∣s∣×∣t∣)$ 。故渐进时间复杂度为 $O(∣s∣×∣t∣)$。
- 空间复杂度：假设 $s$ 深度为 $d_s$ ​，$t$ 的深度为 $d_t$ ，任意时刻栈空间的最大使用代价是 $O(\max \{d_s,d_t\})$ })。故渐进空间复杂度为 $O(\max \{d_s , d_t \})$ 。

此处**优化**的空间是：**深度相同是两棵树相等的必要不充分条件**。那么以下方为例，只用比较 `subRoot` 和两个红框中的子树是否相等即可。

![Screenshot 2024-07-27 at 6.59.11 PM.png](https://pic.leetcode.cn/1722078013-DbjxhR-Screenshot%202024-07-27%20at%206.59.11%20PM.png)

我们可以先计算出 `subRoot` 的深度，再在一次DFS中计算 `root` 各棵子树的深度，**仅**在子树深度和 `subRoot` 深度一致时比较是否相等即可。

因为所需比较的 `root` 子树是不重叠的，且每次比较中对 `root` 子树和 `subRoot` 的探索耗时都是相同的，所以复杂度和 KMP 解法相同。
```cpp
class Solution {
private:
    int subRootDepth = -1;
    bool rootContainsSubRoot = false;
    
    // 判断 a b 两棵树是否相等
    bool equals(TreeNode* a, TreeNode* b) {
        if(a == nullptr && b == nullptr) return true;
        if(a == nullptr || b == nullptr) return false;
        return a->val == b->val && equals(a->left, b->left) && equals(a->right, b->right);
    }

    int getDepth(TreeNode* node, TreeNode* subRoot) {
        if(rootContainsSubRoot || !node) return 0;
        int depth = max(getDepth(node->left, subRoot), getDepth(node->right, subRoot)) + 1;

        // subRootDepth 的初值为 -1，depth >= 0
        // 当 depth == subRootDepth 时，说明如果 subRoot 的深度已经计算得出
        // 再看 node 和 subRoot 两棵树是否相等
        rootContainsSubRoot |= depth == subRootDepth && equals(node, subRoot);

        return depth;
    }
    
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        subRootDepth = getDepth(subRoot, subRoot);
        getDepth(root, subRoot);
        return rootContainsSubRoot;
    }
};
```
**复杂度分析**：
- 时间复杂度：$O(m+n)$ 
    - 当 $m<n$ 时深度不会匹配，不会使用 `equals` 。先遍历完 `subRoot` ，再和 `root` 进行比较，递归不会提前截止，时间复杂度为 $Θ(n)$ 或者说 $O(n+m)$
    - 当 $m≥n$ 时会使用 `equals` 来比较多次，但所用到的 `root` 多棵子树是不重叠的。记所需比较的子树的节点数为 $T_1,\ T_2 ,\ ...,\ T_k$ ，那么多次比较用到的节点数总和为 $∑_{i=1}^k  O(\min(T_i ,\ n))≤O(m)$ 。再考虑递归可能提前截止，导致不用探索完 `root` ，时间复杂度为 $O(m)$ 
- 空间复杂度：$O(m+n)$

考虑上述解法是先计算出 `subRoot` 深度的，我们可以考虑针对 `root` 深度 $<$ `subRoot` 深度的情况**优化**。先**同时**对 `root` 和 `subRoot` 进行BFS，
- 如果 `root` **先**遍历完毕，则说明 `subRoot` 更深，返回 `false` 即可。此时只需 $\Theta (m)$ 时间。
- 否则说明 `root` 深度 $≥$ `subRoot` 深度，则**可利用得到的 `subRoot` 深度**。此后和解法二一致。因此需要 $O(m)$ 时间。
- 总体只需要 $O(m)$ 时间。
```cpp
class Solution {            
private:
    int subRootDepth = 0;
    bool rootContainsSubRoot = false;

    bool equals(TreeNode* a, TreeNode* b) {
        if (a == nullptr && b == nullptr) return true;
        if (a == nullptr || b == nullptr) return false;
        return a->val == b->val && equals(a->left, b->left) && equals(a->right, b->right);
    }

    // 在子树深度和 subRoot 相同时判断相等
    int getDepth(TreeNode* node, TreeNode* subRoot) {
        if(rootContainsSubRoot || !node) return 0;
        int depth = max(getDepth(node->left, subRoot), getDepth(node->right, subRoot)) + 1;
        rootContainsSubRoot |= depth == subRootDepth && equals(node, subRoot);
        return depth;
    }

public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        // 同时对 root 和 subRoot bfs
        queue<TreeNode*> q1;
        queue<TreeNode*> q2;
        if (root != nullptr)
            q1.push(root);
        if (subRoot != nullptr)
            q2.push(subRoot);

        while (!q1.empty() && !q2.empty()) {
            subRootDepth++;

            int size = q1.size();
            for (int i = 0; i < size; ++i) {
                TreeNode* node = q1.front();
                q1.pop();
                if (node->left != nullptr)
                    q1.push(node->left);
                if (node->right != nullptr)
                    q1.push(node->right);
            }

            size = q2.size();
            for (int i = 0; i < size; ++i) {
                TreeNode* node = q2.front();
                q2.pop();
                if (node->left != nullptr)
                    q2.push(node->left);
                if (node->right != nullptr)
                    q2.push(node->right);
            }
        }

        // 如果 subRoot 更深则截止，否则已经计算出 subRootDepth
        if (!q2.empty()) return false;

        getDepth(root, subRoot);
        return rootContainsSubRoot;
    }
};
```

---
### 解法2 DFS序列上做串匹配
这个方法需要我们先了解一个「小套路」：**一棵子树上的点在深度优先搜索序列（即先序遍历）中是连续的**。了解了这个「小套路」之后，我们可以确定解决这个问题的方向就是：把 $s$ 和 $t$ 先转换成深度优先搜索序列，然后看 $t$ 的深度优先搜索序列是否是 $s$ 的深度优先搜索序列的「**子串**」。

这样做正确吗？ 假设 $s$ 由两个点组成，$1$ 是根，$2$ 是 $1$ 的左孩子；$t$ 也由两个点组成，$1$ 是根，$2$ 是 $1$ 的右孩子。这样一来 $s$ 和 $t$ 的深度优先搜索序列相同，可是 $t$ 并不是 $s$ 的某一棵子树。由此可见 **「$s$ 的深度优先搜索序列包含 $t$ 的深度优先搜索序列」是「$t$ 是 $s$ 子树」的必要不充分条件** ，所以单纯这样做是不正确的。

为了解决这个问题，我们可以引入两个空值 `lNull` 和 `rNull` ，当一个节点的左孩子或者右孩子为空的时候，就插入这两个**空值**，这样深度优先搜索序列就唯一对应一棵树。处理完之后，就可以通过判断「$s$ 的深度优先搜索序列包含 $t$ 的深度优先搜索序列」来判断答案。

在判断「$s$ 的深度优先搜索序列包含 $t$ 的深度优先搜索序列」的时候，可以暴力匹配，也可以使用 KMP 或者 Rabin-Karp 算法，**在使用 Rabin-Karp 算法的时候，要注意串中可能有负值**。

这里给出用 KMP 判断的代码实现。
```cpp
class Solution {
private:
    int null = INT_MAX;
    void getDfsOrder(TreeNode* s, vector<int> &order) {
        if (!s) {
            order.push_back(null);
            return;
        }
        order.push_back(s->val);
        getDfsOrder(s->left, order);
        getDfsOrder(s->right, order);
    }
    bool kmp(vector<int>& sOrder, vector<int>& tOrder) {
        int tLen = tOrder.size();
        vector<int> next(tLen, -1); // next[0]=-1
        for (int i = -1, j = 0; j + 1 < tLen; ) {
            if (i == -1 || tOrder[i] == tOrder[j]) {
                next[++j] = ++i; // next[1] / next[j+1] = 0
            } else {
                i = next[i];
            }
        }
        // 检查是否子串
        int i = 0, j = 0;
        for (int sLen = sOrder.size(); i < sLen && j < tLen; ) {
            if (j == -1 || sOrder[i] == tOrder[j]) ++i, ++j;
            else j = next[j];
        }
        return j >= tLen;
    }
public:
    bool isSubtree(TreeNode* s, TreeNode* t) {
        vector<int> sOrder, tOrder;
        getDfsOrder(s, sOrder);
        getDfsOrder(t, tOrder);
        return kmp(sOrder, tOrder);
    }
};
```
```java
class Solution {
    List<Integer> sOrder = new ArrayList<Integer>();
    List<Integer> tOrder = new ArrayList<Integer>();
    int maxElement, lNull, rNull;

    public boolean isSubtree(TreeNode s, TreeNode t) {
        maxElement = Integer.MIN_VALUE;
        getMaxElement(s);
        getMaxElement(t);
        lNull = maxElement + 1;
        rNull = maxElement + 2;

        getDfsOrder(s, sOrder);
        getDfsOrder(t, tOrder);

        return kmp();
    }

    public void getMaxElement(TreeNode t) {
        if (t == null) {
            return;
        }
        maxElement = Math.max(maxElement, t.val);
        getMaxElement(t.left);
        getMaxElement(t.right);
    }

    public void getDfsOrder(TreeNode t, List<Integer> tar) {
        if (t == null) {
            return;
        }
        tar.add(t.val);
        if (t.left != null) {
            getDfsOrder(t.left, tar);
        } else {
            tar.add(lNull);
        }
        if (t.right != null) {
            getDfsOrder(t.right, tar);
        } else {
            tar.add(rNull);
        }
    }

    public boolean kmp() {
        int sLen = sOrder.size(), tLen = tOrder.size();
        int[] fail = new int[tOrder.size()];
        Arrays.fill(fail, -1);
        for (int i = 1, j = -1; i < tLen; ++i) {
            while (j != -1 && !(tOrder.get(i).equals(tOrder.get(j + 1)))) {
                j = fail[j];
            }
            if (tOrder.get(i).equals(tOrder.get(j + 1))) {
                ++j;
            }
            fail[i] = j;
        }
        for (int i = 0, j = -1; i < sLen; ++i) {
            while (j != -1 && !(sOrder.get(i).equals(tOrder.get(j + 1)))) {
                j = fail[j];
            }
            if (sOrder.get(i).equals(tOrder.get(j + 1))) {
                ++j;
            }
            if (j == tLen - 1) {
                return true;
            }
        }
        return false;
    }
}
```
```go
func isSubtree(s *TreeNode, t *TreeNode) bool {
    maxEle := math.MinInt32
    getMaxElement(s, &maxEle)
    getMaxElement(t, &maxEle)
    lNull := maxEle + 1;
    rNull := maxEle + 2;

    sl, tl := getDfsOrder(s, []int{}, lNull, rNull), getDfsOrder(t, []int{}, lNull, rNull)
    return kmp(sl, tl)
}

func kmp(s, t []int) bool {
    sLen, tLen := len(s), len(t)
    fail := make([]int, sLen)
    for i := 0; i < sLen; i++ {
        fail[i] = -1
    }
    for i, j := 1, -1; i < tLen; i++ {
        for j != -1 && t[i] != t[j+1] {
            j = fail[j]
        }
        if t[i] == t[j+1] {
            j++
        }
        fail[i] = j
    }

    for i, j := 0, -1; i < sLen; i++ {
        for j != -1 && s[i] != t[j+1] {
            j = fail[j]
        }
        if s[i] == t[j+1] {
            j++
        }
        if j == tLen - 1 {
            return true
        }
    }
    return false
}

func getDfsOrder(t *TreeNode, list []int, lNull, rNull int) []int {
    if t == nil {
        return list
    }
    list = append(list, t.Val)
    if t.Left != nil {
        list = getDfsOrder(t.Left, list, lNull, rNull)
    } else {
        list = append(list, lNull)
    }

    if t.Right != nil {
        list = getDfsOrder(t.Right, list, lNull, rNull)
    } else {
        list = append(list, rNull)
    }
    return list
} 

func getMaxElement(t *TreeNode, maxEle *int) {
    if t == nil {
        return
    }
    if t.Val > *maxEle {
        *maxEle = t.Val
    }
    getMaxElement(t.Left, maxEle)
    getMaxElement(t.Right, maxEle)
}
```
复杂度分析：
- 时间复杂度：遍历两棵树得到深度优先搜索序列的时间代价是 $O(∣s∣+∣t∣)$ ，在匹配的时候，如果使用暴力匹配，时间代价为 $O(∣s∣×∣t∣)$ ，使用 KMP 或 Rabin-Karp 进行串匹配的时间代价都是 $O(∣s∣+∣t∣)$ 。由于这里的代码使用 KMP 实现的，所以渐进时间复杂度为  $O(∣s∣+∣t∣)$ 。
- 空间复杂度：这里保存了两个深度优先搜索序列，还计算了 $∣t∣$ 长度的 $fail$ 数组，辅助空间的总代价为 $O(∣s∣+∣t∣)$ ，任意时刻栈空间的最大使用代价是 $O(\max \{d_s,\ d_t \})$ ，由于 $\max \{d_s,\ d_t \}=O(∣s∣+∣t∣)$ ，故渐进空间复杂度为 $O(∣s∣+∣t∣)$ 。

---
### 解法3 树哈希
既然每个子树都可遍历为一个DFS序列，而DFS序列又可以类比为字符串，字符串又可以计算哈希值，类似 Rabin-Karp 的思路来匹配……不如我们直接把子树映射为一个数。

考虑==把每个子树都映射成一个唯一的数，如果 $t$ 对应的数字和 $s$ 中任意一个子树映射的数字相等，则 $t$ 是 $s$ 的某一棵子树==。如何映射呢？我们可以定义这样的哈希函数：
$$f_o =v_o + 31\times f_l \times p(s_l) + 179 \times f_r \times p(s_r)$$
这里 $f_x$ 表示节点 $x$ 的哈希值，**$s_x$ 表示节点 $x$ 对应的子树大小**，$v_x$ 代表节点 $x$ 的 $val$ ，$p(n)$ 表示第 $n$ 个素数，$o$ 表示当前节点，$l$ 和 $r$ 分别表示左右孩子。这个式子的意思是：**当前节点 $o$ 的哈希值等于这个点的 $val$ 加上 $31$ 倍左子树的哈希值乘以第 $s_l$ 个素数，再加上 $179$ 倍右子树的哈希值乘以第 $s_r$ 个素数**。这里的 $31$ 和 $179$ 这两个数字只是为了区分左右子树，你可以自己选择你喜欢的权值。

**这样做为什么可行呢**？回到我们的初衷，我们希望把每个子树都映射成一个唯一的数，这样真的能够确保唯一吗？实际上未必。但是我们在这个哈希函数中考虑到每个点的 $val$ 、子树哈希值、子树大小以及左右子树的不同权值，所有这些因素共同影响一个点的哈希值，所以出现冲突的几率较小，一般我们可以忽略。当然你也可以设计你自己的哈希函数，只要考虑到这些因素，就可以把冲突的可能性设计得比较小。

可是如果还是出现了冲突怎么办呢？ 我们可以设计两个哈希函数 $f_1$​ 和 $f_2$ ，用这两个哈希函数生成第三个哈希函数，比如 $f=f_1+f_2$ 、$f=f_1 ×f_2$  等等，这样可以进一步缩小冲突，如果 $f_1$  的冲突概率是 $P_1$ ，$f_2$ 的冲突概率是 $P_2$ ，那么 $f$ 的冲突概率就是 $P_1 ×P_2$ ，理论上已经非常小了，这就是「双哈希」。当然，为了减少冲突，你也可以设计「三哈希」、「四哈希」等，可是这样编程的复杂度就会增加。实际上，一般情况下，只要运气不是太差，一个哈希函数就足够了。

我们可以用「**埃氏筛法**」或者「**欧拉筛法**」求出前 $\arg\ π(\max \{∣s∣,∣t∣ \})$ 个素数（其中 $π(x)$ 表示 $x$ 以内素数个数，$\arg\ π(x)$ 为它的反函数，表示有多少以内包含 $x$ 个素数，这个映射是不唯一的，我们取最小值），然后深度优先搜索计算哈希值，最后比较 $s$ 的所有子树是否有和 $t$ 相同的哈希值即可。
```cpp
class Solution {
public:
    static constexpr int MAX_N = 1000 + 5;
    static constexpr int MOD = int(1E9) + 7;

    bool vis[MAX_N];
    int p[MAX_N], tot;
    void getPrime() {
        vis[0] = vis[1] = 1; tot = 0;
        for (int i = 2; i < MAX_N; ++i) {
            if (!vis[i]) p[++tot] = i;
            for (int j = 1; j <= tot && i * p[j] < MAX_N; ++j) {
                vis[i * p[j]] = 1;
                if (i % p[j] == 0) break;
            }
        }
    }

    struct Status {
        int f, s; // f 为哈希值 | s 为子树大小
        Status(int f_ = 0, int s_ = 0) 
            : f(f_), s(s_) {}
    };

    unordered_map <TreeNode *, Status> hS, hT;

    void dfs(TreeNode *o, unordered_map <TreeNode *, Status> &h) {
        h[o] = Status(o->val, 1);
        if (!o->left && !o->right) return;
        if (o->left) {
            dfs(o->left, h);
            h[o].s += h[o->left].s;
            h[o].f = (h[o].f + (31LL * h[o->left].f * p[h[o->left].s]) % MOD) % MOD;
        }
        if (o->right) {
            dfs(o->right, h);
            h[o].s += h[o->right].s;
            h[o].f = (h[o].f + (179LL * h[o->right].f * p[h[o->right].s]) % MOD) % MOD;
        }
    }

    bool isSubtree(TreeNode* s, TreeNode* t) {
        getPrime();
        dfs(s, hS);
        dfs(t, hT);

        int tHash = hT[t].f;
        for (const auto &[k, v]: hS) {
            if (v.f == tHash) {
                return true;
            }
        } 

        return false;
    }
};
```
```java
class Solution {
    static final int MAX_N = 1005;
    static final int MOD = 1000000007;
    boolean[] vis = new boolean[MAX_N];
    int[] p = new int[MAX_N];
    int tot;
    Map<TreeNode, int[]> hS = new HashMap<TreeNode, int[]>();
    Map<TreeNode, int[]> hT = new HashMap<TreeNode, int[]>();

    public boolean isSubtree(TreeNode s, TreeNode t) {
        getPrime();
        dfs(s, hS);
        dfs(t, hT);

        int tHash = hT.get(t)[0];
        for (Map.Entry<TreeNode, int[]> entry : hS.entrySet()) {
            if (entry.getValue()[0] == tHash) {
                return true;
            }
        }

        return false;
    }

    public void getPrime() {
        vis[0] = vis[1] = true;
        tot = 0;
        for (int i = 2; i < MAX_N; ++i) {
            if (!vis[i]) {
                p[++tot] = i;
            }
            for (int j = 1; j <= tot && i * p[j] < MAX_N; ++j) {
                vis[i * p[j]] = true;
                if (i % p[j] == 0) {
                    break;
                }
            }
        }
    }

    public void dfs(TreeNode o, Map<TreeNode, int[]> h) {
        h.put(o, new int[]{o.val, 1});
        if (o.left == null && o.right == null) {
            return;
        }
        if (o.left != null) {
            dfs(o.left, h);
            int[] val = h.get(o);
            val[1] += h.get(o.left)[1];
            val[0] = (int) ((val[0] + (31L * h.get(o.left)[0] * p[h.get(o.left)[1]]) % MOD) % MOD);
        }
        if (o.right != null) {
            dfs(o.right, h);
            int[] val = h.get(o);
            val[1] += h.get(o.right)[1];
            val[0] = (int) ((val[0] + (179L * h.get(o.right)[0] * p[h.get(o.right)[1]]) % MOD) % MOD);
        }
    }
}
```
**复杂度分析**：
- 时间复杂度：筛选素数（此处为欧拉筛）的时间代价是 $O(\arg\ π(\max \{∣s∣,∣t∣ \}))$ ，对于 $10^6$ 以下的 $x$ ，一般有 $\arg π(x)<15x$ ，也就是在 $15x$ 个自然数里一定能找到 $x$ 个素数，所以这里可以认为它比线性稍微慢一点。深度优先搜索求解和循环比较的时间代价是 $O(∣s∣+∣t∣)$ 。故渐进时间复杂度为 $O(\arg π(\max \{∣s∣,∣t∣\})+∣s∣+∣t∣)=O(\arg π(\max\{∣s∣,∣t∣\}))$ 。
- 空间复杂度：这里用了哈希表来记录每个点的哈希值和子树大小，空间代价是 $O(∣s∣+∣t∣)$ ，筛选素数的 $vis$ 数组的空间代价为 $O(\arg π(\max\{∣s∣,∣t∣\}))$ ，任意时刻栈空间的最大使用代价是 $O(\max \{d_s,\ d_t \})$ ，故渐进空间复杂度为 $O(\arg π(\max \{∣s∣,∣t∣ \}))$ 。
