> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given the `root` of a binary tree, return _the length of the **diameter** of the tree_.

The **diameter** of a binary tree is the **length** of the longest path between any two nodes in a tree. This path may or may not pass through the `root`.

The **length** of a path between two nodes is represented by the number of edges between them.

**Example 1:**
![](https://assets.leetcode.com/uploads/2021/03/06/diamtree.jpg)
```java
Input: root = [1,2,3,4,5]
Output: 3
Explanation: 3 is the length of the path [4,2,1,3] or [5,2,1,3].
```
**Example 2:**
```java
Input: root = [1,2]
Output: 1
```
**Constraints:**
-   The number of nodes in the tree is in the range `[1, 104]`.
-   `-100 <= Node.val <= 100`

题意：给定一棵二叉树 `root` ，找出该二叉树的直径。树的直径是任意两个节点间最长路径的长度，最长路径可能经过也可能不经过根结点。

---
### 解法 深度优先搜索
首先，一条路径的长度为该路径经过的节点数减一，所以求直径（即求路径长度的最大值）等效于**求路径经过节点数的最大值减一**。而==**在二叉树中**，任意一条路径均可被看作由某个节点为起点、从其左儿子和右儿子向下遍历的路径拼接得到==。**如果不是二叉树，就有另外的方法来求解，比如树型DP或两次BFS（用图结构存储树）**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303121738503.png)
如图，可知路径 `[9, 4, 2, 5, 7, 8]` 可以被看作以 $2$ 为起点，从其左儿子向下遍历的路径 `[2, 4, 9]` 和从其右儿子向下遍历的路径 `[2, 5, 7, 8]` 拼接得到。~~这里无需将 $1$ 视作 $2$ 的子节点，因为 $2$ 经过 $1$ 的路径也是 $1$ 经过 $2$ 的路径，树型DP也不用，它不会访问已访问的节点。~~

假设该节点的左儿子向下遍历经过最多的节点数 $L$ （即以左儿子为根的子树的深度），和其右儿子向下遍历经过最多的节点数 $R$ （即以右儿子为根的子树的深度），那么「以该节点为起点的路径经过**节点数**的最大值」即为 $L+R+1$  ——==对节点 $x$ 和它的左右儿子三个节点来说，经过这三个节点的最长链的长度肯定为== $L-1+R-1+2 = L+R$ 。

记「以节点 $node$ 为起点的路径」经过节点数的最大值为 $d_{node}$ ，那么**二叉树的直径就是所有节点 $d_{node}$ 的最大值减一**——即所有节点各自经过的最长链中的最长者。

算法流程为：定义一个递归函数 `depth(node)` 计算 $d_{node}$ ，函数返回该节点为根的子树的深度。先递归调用左儿子和右儿子求得它们为根的子树的深度 $L$ 和 $R$  ，则该节点为根的子树的深度即为 $\max(L,R)+1$ ，该节点的 $d_{node}$ 值为 $L+R+1$ ，递归搜索每个节点并设一个全局变量 $ans$ 记录 $d_{node}$ 的最大值，最后返回 $ans-1$ 即为二叉树的直径。
```cpp
class Solution {
private:
    int ans = 0;
    int depth(TreeNode* root) {
        if (root == NULL) return 0;
        int dl = depth(root->left), dr = depth(root->right);
        ans = max(ans, dl + dr + 1); // 计算d_node即L+R+1 并更新ans
        return max(dl, dr) + 1; // 当前节点的深度
    }
public:
    int diameterOfBinaryTree(TreeNode* root) {
        /*
        // d[x]表示以x为根的子树中x能抵达的最远距离(深度-1)
        // s[i]是x的子节点,2个; dist(s[i], x)是x到子节点的距离
        for (int i = 1; i <= 2; ++i)
            d[x] = max(d[s[i]] + dist(s[i], x), d[x]);
        或者d[x] = max(d[lx], d[rx]) + 1
        // l[x]表示经过x的最长链的长度,显然树的直径diam=max(l[x]);
        // 要求x,只需遍历x的任意两个子节点u,v
        // l[x] = max(l[x], d[u] + dist(u, x) + d[v] + dist(v, x))
        // 设u<v,则max(d[u]+dist(u,x))已经被求出
        // l[x] = max(d[lx] + 1 + d[rx] + 1);
        */
        depth(root);
        return ans - 1;
    }
};
```
复杂度分析
- 时间复杂度：$O(N)$ ，$N$ 为二叉树的节点数，即遍历一棵二叉树的时间复杂度，每个结点只被访问一次。
- 空间复杂度：$O(Height)$ ，$Height$ 为二叉树的高度。由于==在递归过程中需要为每一层递归函数分配栈空间，所以需要额外的空间、且该空间取决于递归的深度==，而递归的深度显然为二叉树的高度，==且每次递归调用中又只用了常数个变量==。