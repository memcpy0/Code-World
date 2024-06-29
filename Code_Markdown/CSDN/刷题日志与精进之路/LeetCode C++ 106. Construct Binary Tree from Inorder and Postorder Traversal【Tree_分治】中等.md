Given inorder and postorder traversal of a tree, construct the binary tree.

**Note:** You may assume that duplicates do not exist in the tree.

For example, given
```c
inorder = [9,3,15,20,7]
postorder = [9,15,7,20,3]
```
Return the following binary tree:
```c
    3
   / \
  9  20
    /  \
   15   7
```


题意：根据一棵树的中序遍历与后序遍历构造二叉树。可以假设树中没有重复的元素。

----
### 思路 
简单的分治题目，递归解决。代码如下：
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
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        return rebuild(0, inorder.size() - 1, 0, postorder.size() - 1, inorder, postorder);
    }
    TreeNode* rebuild(int leftin, int rightin, int leftpost, int rightpost, vector<int>& inorder, vector<int>& postorder) {
        if (leftin > rightin || leftpost > rightpost) return nullptr;
        TreeNode *root = new TreeNode(postorder[rightpost]);
        int rootin = leftin;
        while (rootin <= rightin && inorder[rootin] != postorder[rightpost]) ++rootin;
        int leftsize = rootin - leftin;
        root->left = rebuild(leftin,  rootin - 1, leftpost, leftpost + leftsize - 1, inorder, postorder);
        root->right = rebuild(rootin + 1, rightin, leftpost + leftsize, rightpost - 1, inorder, postorder);  
        return root;      
    }
};
```
效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了26.98% 的用户
内存消耗：16.9 MB, 在所有 C++ 提交中击败了85.71% 的用户
```
