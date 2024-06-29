Given the root node of a binary search tree `BST` and a value. You need to find the node in the `BST` that the node's value equals the given value. Return the subtree rooted with that node. If such node doesn't exist, you should return `NULL` .

For example, 

```cpp
Given the tree:
        4
       / \
      2   7
     / \
    1   3

And the value to search: 2
```

You should return this subtree:

```cpp
  2     
 / \   
1   3
```

In the example above, if we want to search the value `5` , since there is no node with value `5` , we should return `NULL` .

Note that an empty tree is represented by `NULL` , therefore you would see the expected output (serialized tree format) as `[]` , not `null` .

题意：给定二叉搜索树的根节点和一个值。 需要在BST中找到节点值等于给定值的节点，返回以该节点为根的子树。 如果节点不存在，则返回 `NULL` 。

---
### 思路1：递归实现 
```cpp
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        if (root == nullptr || root->val == val) return root;
        return val < root->val ? searchBST(root->left, val) : searchBST(root->right, val); 
    }
};
```
效率如下：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了95.52% 的用户
内存消耗：34 MB, 在所有 C++ 提交中击败了58.19% 的用户
```
---
### 思路2：迭代实现
```cpp
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        while (true) { 
            if (root == nullptr || root->val == val) return root;
            root = val < root->val ? root->left : root->right;
        } 
    }
};
```
效率如下：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了55.36% 的用户
内存消耗：34 MB, 在所有 C++ 提交中击败了54.38% 的用户
```
