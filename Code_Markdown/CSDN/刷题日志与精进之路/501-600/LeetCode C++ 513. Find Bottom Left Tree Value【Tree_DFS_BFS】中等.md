

Given a binary tree, find the leftmost value in the last row of the tree.

**Example 1:**

```swift
Input:

    2
   / \
  1   3

Output:
1
```

**Example 2:**

```swift
Input:

        1
       / \
      2   3
     /   / \
    4   5   6
       /
      7

Output:
7
```

**Note:** You may assume the tree (i.e., the given root node) is not **NULL**.


题意：给定一个二叉树，在树的最后一行找到最左边的值。

---
### 解法 BFS
找到最后一层的第一个节点：
```cpp
class Solution {
public:
    int findBottomLeftValue(TreeNode* root) { 
        queue<TreeNode*> q;
        q.push(root);
        TreeNode *ans = nullptr;
        while (!q.empty()) {
            int size = q.size(); 
            bool isfirst = false;
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                if (!isfirst) { ans = t; isfirst = true; }
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
        }
        return ans->val;
    }
};
```
执行效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了89.23% 的用户
内存消耗：21.8 MB, 在所有 C++ 提交中击败了23.13% 的用户
```
或者在进队的时候，先进队右子树，再进队左子树，这样最后出队的就是最后一行的最左边节点：
```cpp
class Solution {
public:
    int findBottomLeftValue(TreeNode* root) { 
        queue<TreeNode*> q;
        q.push(root); 
        while (!q.empty()) {   
            root = q.front(); q.pop();  
            if (root->right) q.push(root->right);
            if (root->left) q.push(root->left); 
        }
        return root->val;
    }
};
```
执行结果如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了89.23% 的用户
内存消耗：21.7 MB, 在所有 C++ 提交中击败了28.29% 的用户
```
