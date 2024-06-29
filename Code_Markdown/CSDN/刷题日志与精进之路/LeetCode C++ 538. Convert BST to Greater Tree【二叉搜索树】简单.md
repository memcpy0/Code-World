Given a Binary Search Tree (BST), convert it to a Greater Tree such that every key of the original BST is changed to the original key plus sum of all keys greater than the original key in BST.

**Example:**
```cpp
Input: The root of a Binary Search Tree like this:
              5
            /   \
           2     13

Output: The root of a Greater Tree like this:
             18
            /   \
          20     13
```

题意：给定一个二叉搜索树，把它转换成为累加树，使得每个节点的值是原来的节点值加上所有大于它的节点值之和。

---
思路：中序遍历的变形。代码如下：
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
    int sum = 0;
    TreeNode* convertBST(TreeNode* root) {
        if (root == nullptr) return root;
        convertBST(root->right);
        sum += root->val;
        root->val = sum;
        convertBST(root->left); 
        return root;
    } 
};
```
效率如下：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了92.32% 的用户
内存消耗：32.4 MB, 在所有 C++ 提交中击败了98.04% 的用户
```
 
