## 404. Sum of Left Leaves 【easy】
Find the sum of all left leaves in a given binary tree.

Example:
```
    3
   / \
  9  20
    /  \
   15   7
```
There are **two left leaves** in the binary tree, with values 9 and 15 respectively. Return 24.

题意：求出二叉树的所有左叶子节点的值之和。

答案：很简单的一道题。
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
    // 加上左右节点标记的DFS
    int dfs(TreeNode* root, int dir) {
        if (root == NULL) return 0; // 为NULL返回0
        if (root->left == NULL && root->right == NULL)  // 为叶子节点
            return dir == -1 ? root->val : 0;            // 左叶子节点就返回值
        return dfs(root->left, -1) + dfs(root->right, 1);
    }
    int sumOfLeftLeaves(TreeNode* root) {
        return dfs(root, 0);
        // 二叉树遍历
        // if (!root) return 0; 
        // if (root->left) { // 如果这个节点的左节点存在
        //     if (!root->left->left && !root->left->right) // 且为叶子
        //         return root->left->val + sumOfLeftLeaves(root->right); // 就加上这个左叶子节点和右子树的结果
        //     // 不为叶子
        //     return sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right); // 左右子树的和
        // }  // 节点的左节点不存在
        // return sumOfLeftLeaves(root->right); 
        
        // 二叉树遍历简化
        // if (!root) return 0;
        // if (root->left && !root->left->left && !root->left->right) 
        //     return root->left->val + sumOfLeftLeaves(root->right);
        // return sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right);
        
        // BFS 
        // if (!root) return 0;
        // queue<TreeNode*> q;
        // int sum = 0;
        // q.push(root);
        // while (!q.empty()) {
        //     TreeNode *t = q.front(); q.pop();
        //     if (t->left && !t->left->left && !t->left->right) 
        //         sum += t->left->val;
        //     if (t->left) q.push(t->left);
        //     if (t->right) q.push(t->right);
        // }
        // return sum;
    }
};
```

