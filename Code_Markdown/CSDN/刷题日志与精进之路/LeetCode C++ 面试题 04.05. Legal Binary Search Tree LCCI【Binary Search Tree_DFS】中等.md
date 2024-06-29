

Implement a function to check if a binary tree is a binary search tree.

**Example 1:**

```swift
Input:
    2
   / \
  1   3
Output: true
```

**Example 2:**

```swift
Input:
    5
   / \
  1   4
     / \
    3   6
Output: false
Explanation: Input: [5,1,4,null,null,3,6].
     the value of root node is 5, but its right child has value 4.
```

题意：实现一个函数，检查一棵二叉树是否为二叉搜索树。

---
### 解法1 迭代中序遍历
```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        if (root == nullptr) return true;
        TreeNode *prev = nullptr;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            if (prev && prev->val >= root->val) return false; //注意:直接前驱的值必须<直接后继的值
            prev = root;
            root = root->right;
        }
        return true;
    }
};
```
执行效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了64.43% 的用户
内存消耗：21.7 MB, 在所有 C++ 提交中击败了9.92% 的用户
```
---
### 解法2 递归中序遍历
```cpp
class Solution { 
public:
    TreeNode *prev = nullptr;
    bool isValidBST(TreeNode* root) {
        if (root == nullptr) return true; 
        if (!isValidBST(root->left)) return false;
        if (prev && prev->val >= root->val) return false;
        prev = root;
        if (!isValidBST(root->right)) return false;
        return true;
    }
};
```
执行效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了89.01% 的用户
内存消耗：21.4 MB, 在所有 C++ 提交中击败了39.61% 的用户
```
