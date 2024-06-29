Find the sum of all left leaves in a given binary tree.

**Example:**
```cpp
    3
   / \
  9  20
    /  \
   15   7

There are two left leaves in the binary tree, with values 9 and 15 respectively. Return 24.
```
题意：求出树中所有左叶子结点的和。

---
### 思路1
在原函数上递归，先序遍历。代码如下：
```cpp
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (root == nullptr) return 0;
        int ans = 0; //先序遍历
        if (root->left && !root->left->left && !root->left->right) ans += root->left->val;
        return ans + sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right);
    }
    //或者写成
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) return 0;
        if (root->left && !root->left->left && !root->left->right) 
            return root->left->val + sumOfLeftLeaves(root->right);
        return sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right);
    }
};
```
递归，中序遍历：
```cpp
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (root == nullptr) return 0;
        int lval = sumOfLeftLeaves(root->left);
        int ans = 0; //中序遍历
        if (root->left && !root->left->left && !root->left->right) ans += root->left->val;
        int rval = sumOfLeftLeaves(root->right);
        return ans + lval + rval;
    }
};
```
递归，后序遍历：
```cpp
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (root == nullptr) return 0;
        int lval = sumOfLeftLeaves(root->left), rval = sumOfLeftLeaves(root->right);
        int ans = 0; //后序遍历
        if (root->left && !root->left->left && !root->left->right) ans += root->left->val;
        return ans + lval + rval;
    }
};
```
---
### 思路2
使用标记，进行递归。代码如下：
```cpp
class Solution {
public:
    int dfs(TreeNode* root, int dir) { //加上左右节点标记的DFS
        if (root == NULL) return 0; //为NULL返回0
        if (root->left == NULL && root->right == NULL)  //为叶子节点
            return dir == -1 ? root->val : 0;           //左叶子节点就返回值
        return dfs(root->left, -1) + dfs(root->right, 1);
    }
    int sumOfLeftLeaves(TreeNode* root) {
        return dfs(root, 0); 
    }
};
```
---
### 思路3
BFS遍历。代码如下：
```cpp
class Solution {
public: 
    int sumOfLeftLeaves(TreeNode* root) { 
        if (!root) return 0;
        queue<TreeNode*> q;
        int sum = 0;
        q.push(root);
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop();
            if (t->left && !t->left->left && !t->left->right) 
                sum += t->left->val;
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
        }
        return sum;
    }
};
```
