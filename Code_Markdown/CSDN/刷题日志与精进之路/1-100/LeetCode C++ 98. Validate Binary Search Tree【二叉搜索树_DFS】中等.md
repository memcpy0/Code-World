Given a binary tree, determine if it is a valid binary search tree (BST). Assume a BST is defined as follows:
-    The left subtree of a node contains only nodes with keys **less than** the node's key.
-    The right subtree of a node contains only nodes with keys **greater than** the node's key.
-    Both the left and right subtrees must also be binary search trees.

 

**Example 1:**
```cpp
    2
   / \
  1   3

Input: [2,1,3]
Output: true
```
**Example 2:**
```cpp
    5
   / \
  1   4
     / \
    3   6

Input: [5,1,4,null,null,3,6]
Output: false
Explanation: The root node's value is 5 but its right child's value is 4.
```

题意：验证给出的二叉树是否是二叉搜索树。

---
思路1：递归中序遍历。如果这是一棵二叉搜索树，那么递归中序遍历时，前驱结点的值一定小于后继结点的值。至于如何保持前驱结点，应该在二叉树线索化的时候学习过（有时间也会写一下）。代码如下：
```cpp
class Solution {
public:
    TreeNode *pre;
    bool isValidBST(TreeNode* root) {
        if (root == nullptr) return true; 
        pre = nullptr;
        return inorder(root);
    }
    bool inorder(TreeNode *root) {
        if (root == nullptr) return true;
        if (inorder(root->left) == false) return false;
        if (pre && pre->val >= root->val) return false;
        pre = root;
        if (inorder(root->right) == false) return false;
        return true;
    }
};
```
或者直接使用原函数：
```cpp
class Solution {
public:
    TreeNode *pre = nullptr;
    bool isValidBST(TreeNode* root) {
        if (root == nullptr) return true;
        if (isValidBST(root->left) == false) return false;
        if (pre && pre->val >= root->val) return false;
        pre = root;
        if (isValidBST(root->right) == false) return false;
        return true;
    } 
};
```
递归函数的时间效率较低：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了36.41% 的用户
内存消耗：17.9 MB, 在所有 C++ 提交中击败了35.56% 的用户
```

---
思路2：非递归中序遍历。代码如下：
```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        if (root == nullptr) return true;
        TreeNode *pre = nullptr, *cur = root;
        stack<TreeNode*> st;
        while (cur || !st.empty()) {
            while (cur) {
                st.push(cur);
                cur = cur->left;
            }
            cur = st.top(); st.pop();
            if (pre && pre->val >= cur->val) return false;
            pre = cur;
            cur = cur->right;
        }
        return true;
    } 
};
```
时间效率更高：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了92.82% 的用户
内存消耗：18.1 MB, 在所有 C++ 提交中击败了16.36% 的用户
```
