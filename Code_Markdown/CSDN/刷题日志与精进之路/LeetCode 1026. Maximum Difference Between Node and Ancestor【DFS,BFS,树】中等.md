> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given the `root` of a binary tree, find the maximum value `v` for which there exist **different** nodes `a` and `b` where `v = |a.val - b.val|` and `a` is an ancestor of `b`.

A node `a` is an ancestor of `b` if either: any child of `a` is equal to `b` or any child of `a` is an ancestor of `b`.

**Example 1:**

![](https://assets.leetcode.com/uploads/2020/11/09/tmp-tree.jpg)

```java
Input: root = [8,3,10,1,6,null,14,null,null,4,7,13]
Output: 7
Explanation: We have various ancestor-node differences, some of which are given below :
|8 - 3| = 5
|3 - 7| = 4
|8 - 1| = 7
|10 - 13| = 3
Among all possible differences, the maximum value of 7 is obtained by |8 - 1| = 7.
```
**Example 2:**
![](https://assets.leetcode.com/uploads/2020/11/09/tmp-tree-1.jpg)

```java
Input: root = [1,null,2,null,0,3]
Output: 3
```
**Constraints:**
-   The number of nodes in the tree is in the range `[2, 5000]`.
-   `0 <= Node.val <= 105`

题意：给定二叉树的根节点 `root`，找出存在于 **不同** 节点 `A` 和 `B` 之间的最大值 `V`，其中 `V = |A.val - B.val|`，且 `A` 是 `B` 的祖先。如果 A 的任何子节点之一为 B，或者 A 的任何子节点是 B 的祖先，那么我们认为 A 是 B 的祖先。

---
### 解法 DFS自顶向下  
如果节点 $A$ 在从根节点到节点 $B$ 的**路径**上，则称 $A$ 是 $B$ 的**祖先**节点，称 $B$ 是 $A$ 的**子孙**节点。
> 注：在这个定义中，$B$ 的祖先节点可以是 $B$ 自己。例如示例 1 中 $6$ 的祖先节点自上而下依次为 $8,3,6$ 。
> 注：虽然题目要求「不同节点」，但计算的是最大差值，相同节点算出来的差值为 $0$ ，不影响最大差值。

对于题目给出的公式 $V = |A.\textit{val} - B.\textit{val}|$ ，为了让 $V$ 尽量大，分类讨论：
- 如果 $A.\textit{val} < B.\textit{val}$ ，那么 $A.val$ 越小，$V$ 越大。
- 如果 $A.val≥B.val$ ，那么 $A.val$ 越大，$V$ 越大；

因此，无需记录递归路径中的全部节点值，只需要记录递归路径中的最小值 $mn$ 和最大值 $mx$ 。每递归到一个节点 $B$ ，计算
$$max⁡(∣mn−B.val∣,∣mx−B.val∣)$$
并更新答案的最大值。由于 $\textit{mn}\le B.\textit{val} \le \textit{mx}$ ，上式可化简为 $\max(B.\textit{val}-\textit{mn},\textit{mx}-B.\textit{val})$ 
```java
class Solution {
    private int ans;

    public int maxAncestorDiff(TreeNode root) {
        dfs(root, root.val, root.val);
        return ans;
    }

    private void dfs(TreeNode node, int mn, int mx) {
        if (node == null) return;
        // 虽然题目要求「不同节点」，但相同节点的差值为 0，不会影响最大差值
        // 所以先更新 mn 和 mx，再计算差值也可以
        // 在这种情况下，一定满足 mn <= node.val <= mx
        mn = Math.min(mn, node.val);
        mx = Math.max(mx, node.val);
        ans = Math.max(ans, Math.max(node.val - mn, mx - node.val));
        dfs(node.left, mn, mx);
        dfs(node.right, mn, mx);
    }
}
```
换个角度看问题：==对于一条从根出发向下的路径，我们要计算的实际上是这条路径上任意两点的最大差值==。递归到叶子时，$\textit{mx}$ 是从根到叶子的路径上的最大值，$\textit{mn}$ 是从根到叶子的路径上的最小值，所以 $\textit{mx}-\textit{mn}$ 就是**从根到叶子的路径上任意两点的最大差值**。所以**无需每个节点都去更新答案，而是在递归到终点时才去更新答案**。
```cpp
class Solution {
private:
    int ans = 0;
    void dfs(TreeNode* r, int mx, int mn) {
        if (r == nullptr) {
            ans = max(mx - mn, ans);
            return;
        }
        mx = max(mx, r->val);
        mn = min(mn, r->val); 
        dfs(r->left, mx, mn);
        dfs(r->right, mx, mn);
    }
public:
    int maxAncestorDiff(TreeNode* root) {
        dfs(root, root->val, root->val);
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为二叉树的节点个数。
- 空间复杂度：$O(n)$ 。最坏情况下，二叉树退化成一条链，递归需要 $O(n)$ 的栈空间。

---
### 解法2 DFS自底向上
法一的思路是**维护 $B$ 的祖先节点中的最小值和最大值**，我们还可以站在 $A$ 的视角，维护 $A$ 的子孙节点中的最小值 $mn$ 和最大值 $mx$ 。换句话说，**最小值和最大值不再作为入参，而是作为返回值**，意思是以 $A$ 为根的子树中的最小值和最大值。

递归到节点 $A$ 时，初始化 $mn$ 和 $mx$ 为 $A.val$ ，然后递归左右子树，拿到左右子树的最小值和最大值，去更新 $mn$ 和 $mx$ ，然后计算
$$\max(|\textit{mn}-A.\textit{val}|,|\textit{mx}-A.\textit{val}|)$$
并更新答案的最大值。由于 $\textit{mn}\le A.\textit{val} \le \textit{mx}$ ，上式可化简为
$$\max(A.\textit{val}-\textit{mn},\textit{mx}-A.\textit{val})$$
```cpp
class Solution {
    int ans = 0; 
    pair<int, int> dfs(TreeNode *node) {
        if (node == nullptr)
            return {INT_MAX, INT_MIN}; // 保证空节点不影响 mn 和 mx
        int mn = node->val, mx = mn;
        auto [l_mn, l_mx] = dfs(node->left);
        auto [r_mn, r_mx] = dfs(node->right);
        mn = min(mn, min(l_mn, r_mn));
        mx = max(mx, max(l_mx, r_mx));
        ans = max(ans, max(node->val - mn, mx - node->val));
        return {mn, mx};
    } 
public:
    int maxAncestorDiff(TreeNode *root) {
        dfs(root);
        return ans;
    }
};
```
复杂度：
- 时间复杂度：$O(n)$ ，其中 $n$ 为二叉树的节点个数。
- 空间复杂度：$O(n)$ 。最坏情况下，二叉树退化成一条链，递归需要 $O(n)$ 的栈空间。