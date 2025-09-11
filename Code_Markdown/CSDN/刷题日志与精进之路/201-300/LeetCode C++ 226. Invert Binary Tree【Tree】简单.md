Invert a binary tree.

**Example:**
Input:
```cpp
     4
   /   \
  2     7
 / \   / \
1   3 6   9
```
Output:
```cpp
     4
   /   \
  7     2
 / \   / \
9   6 3   1
```

**Trivia:**
This problem was inspired by this original tweet by Max Howell:
> Google: 90% of our engineers use the software you wrote (Homebrew), but you can’t invert a binary tree on a whiteboard so f*** off.

题意：翻转一棵二叉树。

---
思路1：先序遍历。代码如下：
```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return root;
        swap(root->left, root->right);
        root->left = invertTree(root->left);
        root->right = invertTree(root->right);
        return root;
    }
};
```
效率：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了60.36% 的用户
内存消耗：9.3 MB, 在所有 C++ 提交中击败了52.45% 的用户
```

---
思路2：中序遍历。代码如下：
```cpp
class Solution {
public: 
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return root;
        invertTree(root->left); //递归找到左结点
        swap(root->left, root->right);
        invertTree(root->left); //此时左右结点已经交换
        return root;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了60.36% 的用户
内存消耗：9.2 MB, 在所有 C++ 提交中击败了61.82% 的用户
```

---
思路3：后序遍历。代码如下：
```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return root;
        root->left = invertTree(root->left);
        root->right = invertTree(root->right);
        swap(root->left, root->right);
        return root;
    }
};
```
效率：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了60.36% 的用户
内存消耗：9.3 MB, 在所有 C++ 提交中击败了41.42% 的用户
```

---
思路4：层序遍历。代码如下：
```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (root == nullptr) return root;
        queue<TreeNode*> q;
        q.push(root); 
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop();
            swap(t->left, t->right);
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
        }
        return root;
    } 
};
```
效率：
```py
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：9.4 MB, 在所有 C++ 提交中击败了15.61% 的用户
```
