Given a binary tree, find its minimum depth.

The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.

**Note:** A leaf is a node with no children.

**Example:**
Given binary tree `[3,9,20,null,null,15,7]` ,
```cpp
    3
   / \
  9  20
    /  \
   15   7
```
return its minimum depth = `2` .

题意：找到二叉树的最小深度——从根结点到最近叶子结点的最短路径上的结点数量。

---
思路1：对二叉树首先想到的就是DFS，考虑二叉树的五种形态：
- 空树时直接返回 `0` ；
- 只有根结点时，根结点就是叶子结点，返回 `1` ；
- 存在左结点或者右结点时，根结点不是叶子结点，需要返回 `2` —— 也就是根结点+左结点/右结点的高度；
- 左右结点都存在时，需要返回它们之中更小的高度 `+1` (根结点)。


代码如下：
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        int lsum = minDepth(root->left), rsum = minDepth(root->right);
        return (lsum && rsum) ? min(lsum, rsum) + 1 : 1 + lsum + rsum;
        //前者是左右子树都存在时,后者是左右子树存在其一或者都不存在的情况
    }
};
```
---
思路2：BFS也行。不断计算层数，遇到叶子结点时直接返回高度。

代码如下：
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        int height = 0;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            ++height;
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
                if (!t->left && !t->right) return height;
            }
        }
        return height;
    }
};
```
效率：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了53.28% 的用户
内存消耗：17.3 MB, 在所有 C++ 提交中击败了77.61% 的用户
```
