Given a binary search tree, rearrange the tree in **in-order** so that the leftmost node in the tree is now the root of the tree, and every node has no left child and only 1 right child.

**Example 1:**

```swift
Input: [5,3,6,2,4,null,8,1,null,null,null,7,9]

       5
      / \
    3    6
   / \    \
  2   4    8
 /        / \ 
1        7   9

Output: [1,null,2,null,3,null,4,null,5,null,6,null,7,null,8,null,9]

 1
  \
   2
    \
     3
      \
       4
        \
         5
          \
           6
            \
             7
              \
               8
                \
                 9  
```
**Constraints:**
- The number of nodes in the given tree will be between `1` and `100` .
-  Each node will have a unique integer value from `0` to `1000` .

题意：给你一个树，按中序遍历重新排列树，使树中最左边的结点现在是树的根，并且每个结点没有左子结点，只有一个右子结点。

---
### 思路1 递归遍历
递归中序遍历，得到中序序列，然后建树：
```cpp
class Solution {
public:
    vector<int> inorder;
    void inorderTraversal(TreeNode* root) {
        if (root) {
            inorderTraversal(root->left);
            inorder.push_back(root->val);
            inorderTraversal(root->right);
        }
    }
    void rebuild(TreeNode*& root, int idx) {
        if (idx < inorder.size()) {
            root = new TreeNode(inorder[idx++]);
            rebuild(root->right, idx);
        }
    }
    TreeNode* increasingBST(TreeNode* root) {
        inorderTraversal(root);
        TreeNode *newRoot;
        rebuild(newRoot, 0);
        return newRoot;
    }
};
```
结果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了63.16% 的用户
内存消耗：7.2 MB, 在所有 C++ 提交中击败了31.83% 的用户
```
不进行收集，而是在递归遍历的时候直接建树，实际上是在用尾插法创建一个链表：
```cpp
class Solution {
public: 
    TreeNode *rear;
    void inorderTraversalAndReBuild(TreeNode* root) {
        if (root) {
            inorderTraversalAndReBuild(root->left);
            rear->right = new TreeNode(root->val);
            rear = rear->right;
            inorderTraversalAndReBuild(root->right);
        }
    } 
    TreeNode* increasingBST(TreeNode* root) {
        TreeNode dummy(0);
        rear = &dummy;
        inorderTraversalAndReBuild(root);
        return dummy.right;
    }
};
```

---
### 思路2 非递归遍历
使用非递归方法+尾插法，代码如下：
```cpp
class Solution {
public:  
    TreeNode* increasingBST(TreeNode* root) {
        if (root == nullptr) return root;
        TreeNode dummy(0), *rear = &dummy;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            rear->right = new TreeNode(root->val);
            rear = rear->right; //尾插法
            root = root->right;
        } 
        return dummy.right;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了53.39% 的用户
```
