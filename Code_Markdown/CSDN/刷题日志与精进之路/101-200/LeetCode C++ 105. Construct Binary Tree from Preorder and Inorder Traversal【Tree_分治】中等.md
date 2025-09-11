Given preorder and inorder traversal of a tree, construct the binary tree.

**Note:** You may assume that duplicates do not exist in the tree.

For example, given

```c
preorder = [3,9,20,15,7]
inorder = [9,3,15,20,7]
```

Return the following binary tree:
```cpp
    3
   / \
  9  20
    /  \
   15   7
```

题意：根据一棵树的前序遍历与中序遍历构造二叉树。树中没有重复的元素。
 
 ---
 ### 思路 
 采取分治方法，递归解决这一问题。很简单的题目：
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
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        return rebuild(0, preorder.size() - 1, 0, inorder.size() - 1, preorder, inorder);
    }
    TreeNode* rebuild(int leftpre, int rightpre, int leftin, int rightin, vector<int> &preorder, vector<int> &inorder) {
        if (leftpre > rightpre || leftin > rightin) return nullptr;
        TreeNode *root = new TreeNode(preorder[leftpre]);
        int rootin = leftin;
        while (rootin <= rightin && inorder[rootin] != preorder[leftpre]) ++rootin;
        int leftsize = rootin - leftin;
        root->left = rebuild(leftpre + 1, leftpre + leftsize, leftin, rootin - 1, preorder, inorder);
        root->right = rebuild(leftpre + leftsize + 1, rightpre, rootin + 1, rightin, preorder, inorder);
        return root;
    }
};
```
 

