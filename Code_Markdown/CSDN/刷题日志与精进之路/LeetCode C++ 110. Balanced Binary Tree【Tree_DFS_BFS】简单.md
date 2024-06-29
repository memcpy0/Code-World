Given a binary tree, determine if it is **height-balanced**.

For this problem, a height-balanced binary tree is defined as:

>   a binary tree in which the left and right subtrees of every node differ in height by no more than 1.

 
**Example 1:**
```cpp
Given the following tree [3,9,20,null,null,15,7]:

    3
   / \
  9  20
    /  \
   15   7

Return true.
```

**Example 2:**
```cpp
Given the following tree [1,2,2,3,3,null,null,4,4]:

       1
      / \
     2   2
    / \
   3   3
  / \
 4   4

Return false.
```

题意：判断给出的二叉树是否是一棵高度平衡二叉树。

---
思路1：DFS+回溯。这不就是AVL树平衡的判断嘛。以前学写AVL树的时候，不存储高度信息，而是在插入后自底向上回溯时，用 `getHeight(root), getFactor(root)` 递归求出高度和平衡因子来。如果平衡因子的值不是 `-1,0,1` ，就说明不平衡。

代码如下：
```cpp
class Solution {
public:
    int getHeight(TreeNode* root) {
        if (!root) return 0;
        int leftHeight = getHeight(root->left);
        int rightHeight = getHeight(root->right);
        return max(leftHeight, rightHeight) + 1;
    }
    int getFactor(TreeNode* root) {
        return getHeight(root->left) - getHeight(root->right);
    }

    bool isBalanced(TreeNode* root) {
        if (!root) return true;
        bool l = isBalanced(root->left);
        bool r = isBalanced(root->right);
        if (abs(getFactor(root)) >= 2) return false;
        else return l && r;
    }
};
```
由于**自底向上回溯时**，又需要**递归求出高度和平衡因子**，重复操作效率较低：
```cpp
执行用时：340 ms, 在所有 C++ 提交中击败了7.26% 的用户
内存消耗：14.9 MB, 在所有 C++ 提交中击败了48.15% 的用户
```

---
思路2：BFS+递归。用BFS遍历每个结点，分别判断其平衡性质。

代码2：
```cpp
class Solution {
public:
    int getHeight(TreeNode* root) {
        if (!root) return 0;
        else {
            int leftHeight = getHeight(root->left);
            int rightHeight = getHeight(root->right);
            return max(leftHeight, rightHeight) + 1;
        }
    }
    int getFactor(TreeNode* root) {
        return getHeight(root->left) - getHeight(root->right);
    }

    bool isBalanced(TreeNode* root) {
        if (!root) return true; 
        queue<TreeNode*> q; 
        q.push(root);
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop();
            if (abs(getFactor(t)) >= 2) return false; 
            if (t->left)  q.push(t->left);
            if (t->right) q.push(t->right);
        } 
        return true; 
    }
};
```
效率还行：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了99.07% 的用户
内存消耗：15.3 MB, 在所有 C++ 提交中击败了5.21% 的用户
```
---
思路3：将DFS和求高度结合起来。空子树的高度是 `0` ，存在子树时高度一定大于 `0` 。这里**按平常的方法计算子树的高度**：
- 如果递归回溯时发现左子树或者右子树的高度 `> 0` ，就说明它们是平衡的，此时检查左子树高度和右子树高度的差值的绝对值：
	- 如果超过 `1` ，说明以当前结点为根的子树不平衡，返回 `-1` ；
	- 否则返回高度的正常值，说明当前子树为平衡子树；
- 否则，左子树或者右子树中存在不平衡子树，直接返回 `-1` 。

代码：
```cpp
class Solution {
public:
    int height(TreeNode *root) {
        if (!root) return 0;
        int leftHeight = height(root->left), rightHeight = height(root->right);
        if (leftHeight >= 0 && rightHeight >= 0 && abs(leftHeight - rightHeight) <= 1)
            return max(leftHeight, rightHeight) + 1;
        return -1; 
    }

    bool isBalanced(TreeNode* root) {
        return height(root) >= 0;   //-1表示不平衡 
    }
};
```
效率如下，相较于思路1来说大大提高：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了92.35% 的用户
内存消耗：15.1 MB, 在所有 C++ 提交中击败了12.06% 的用户
```
