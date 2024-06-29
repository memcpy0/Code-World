Given a binary tree, find its **maximum depth**.

The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.

Note: A leaf is a node with no children.

Example:
```clike
Given binary tree [3,9,20,null,null,15,7],

    3
   / \
  9  20
    /  \
   15   7
```

return its depth `= 3` .

题意：给出一棵二叉树，返回最大的深度，即从根结点到最深的叶子结点的最长路径的**结点数目**。

---
思路1：BFS。

代码：
```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        int treeMaxDepth = 0;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            ++treeMaxDepth;
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
        }
        return treeMaxDepth;
    }
};
```
以前写的BFS代码，比上面快一些：
```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        // 层次遍历，数最大的层数，规定空树层数/深度为0   
        using Pair = pair<TreeNode*, int>;
        int treeMaxDepth = 0;
        queue<Pair> q;
        if (root) q.emplace(root, 1);
        while (!q.empty()) {
            Pair p = q.front(); q.pop();
            auto curRoot = p.first;
            int curDepth = p.second;
            treeMaxDepth = max(curDepth, treeMaxDepth); 
            if (curRoot->left) q.emplace(curRoot->left, curDepth + 1);
            if (curRoot->right) q.emplace(curRoot->right, curDepth + 1);
        }
        return treeMaxDepth; 
    }
};
```
---

思路2：DFS，相当于求树的高度。

代码：
```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        // 转换为求树根结点的高度，规定空树高0
        if (!root) return 0;
        else {
            int leftHeight = maxDepth(root->left);
            int rightHeight = maxDepth(root->right);
            return max(leftHeight, rightHeight) + 1;
        } 
    }
};
```


