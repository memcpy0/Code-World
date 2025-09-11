A binary tree is *univalued* if every node in the tree has the same value. Return `true` if and only if the given tree is univalued.

 

**Example 1:**

```swift
Input: [1,1,1,1,1,null,1]
Output: true
```

**Example 2:**

```swift
Input: [2,2,2,5,2]
Output: false
```
**Note:**
- The number of nodes in the given tree will be in the range `[1, 100]` .
- Each node's value will be an integer in the range` [0, 99]` .


题意：如果二叉树每个节点都具有相同的值，那么该二叉树就是单值二叉树。判断给定的二叉树是否是单值二叉树。

---
### 解法1 BFS
题目很简单，代码如下：
```cpp
class Solution {
public: 
    bool isUnivalTree(TreeNode* root) { 
        int val = root->val;
        queue<TreeNode*> q; 
        //queue<TreeNode*, list<TreeNode*>> q; //empty queue with vector as underlying container
        q.push(root);
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop(); 
            if (val != t->val) return false;
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
        } 
        return true;
    }
};
```
效率如下，还行：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了70.40% 的用户
内存消耗：10.2 MB, 在所有 C++ 提交中击败了5.10% 的用户
```

---
### 解法2 非递归DFS
```cpp
//简单版本的先序遍历
class Solution {
public: 
    bool isUnivalTree(TreeNode* root) {
        int val = root->val;
        vector<TreeNode*> st;
        st.push_back(root);
        while (!st.empty()) {
            TreeNode *t = st.back(); st.pop_back();
            if (t->val != val) return false;
            if (t->right) st.push_back(t->right);
            if (t->left) st.push_back(t->left);
        }
        return true;
    }
};
```
提交后效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：10 MB, 在所有 C++ 提交中击败了24.80% 的用户
```
另外的代码如下：
```cpp
//简单版本的后序遍历
class Solution {
public: 
    bool isUnivalTree(TreeNode* root) { 
        vector<TreeNode*> st;
        vector<int> rev;
        st.push_back(root);
        while (!st.empty()) {
            TreeNode *t = st.back(); st.pop_back();
            rev.push_back(t->val); 
            if (t->left) st.push_back(t->left);
            if (t->right) st.push_back(t->right);
        }
        for (int i = rev.size() - 1; i > 0; --i)
            if (rev[i] != rev[i - 1])
                return false;
        return true;
    }
};
```
效率就低很多了：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了21.93% 的用户
内存消耗：10.1 MB, 在所有 C++ 提交中击败了7.55% 的用户
```
之后同样的教科书式写法：
```cpp
//非递归先序遍历
class Solution {
public: 
    bool isUnivalTree(TreeNode* root) { 
        int val = root->val;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                if (root->val != val) return false;
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            root = root->right;
        }
        return true;
    }
};
//非递归中序遍历
class Solution {
public: 
    bool isUnivalTree(TreeNode* root) { 
        int val = root->val;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            if (root->val != val) return false;
            root = root->right;
        }
        return true;
    }
};
//非递归后序遍历
class Solution {
public: 
    bool isUnivalTree(TreeNode* root) { 
        int val = root->val;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            } 
            TreeNode *temp = st.top();
            root = temp->right;
            if (root == nullptr) { //root为空
                if (temp->val != val) return false; //访问temp
                st.pop(); //弹出temp
                while (!st.empty() && st.top()->right == temp) {
                    temp = st.top(); st.pop();
                    if (temp->val != val) return false; 
                }
            }
        }
        return true;
    }
};
```
---
### 解法3 递归遍历
```cpp
//先序遍历
class Solution {
private:
    int val;
    bool preorder(TreeNode* root) {
        if (!root) return true;
        if (root->val != val) return false;
        return preorder(root->left) && preorder(root->right);
    }
public: 
    bool isUnivalTree(TreeNode* root) { 
        val = root->val;
        return preorder(root);
    }
};
//中序遍历
class Solution {
private:
    int val;
    bool inorder(TreeNode* root) {
        if (!root) return true;
        if (inorder(root->left) == false) return false;
        if (root->val != val) return false;
        return inorder(root->right);
    }
public: 
    bool isUnivalTree(TreeNode* root) { 
        val = root->val;
        return inorder(root);
    }
};
//后序遍历
class Solution {
private:
    int val;
    bool postorder(TreeNode* root) {
        if (!root) return true;
        if (postorder(root->left) == false) return false;
        if (postorder(root->right) == false) return false;
        return root->val == val;
    }
public: 
    bool isUnivalTree(TreeNode* root) { 
        val = root->val;
        return postorder(root);
    }
};
```
其中递归后序遍历的效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了70.40% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了5.10% 的用户
```
---
### 解法4 Morris遍历
Morris先序遍历：
```cpp
class Solution { 
public: 
    bool isUnivalTree(TreeNode* root) { 
        if (!root) return true;
        TreeNode *cur = root, *mostRight = nullptr;
        int val = root->val;
        bool flag = true; //是否为单值二叉树
        while (cur) {
            if (cur->left == nullptr) { //没有左子树则直接访问cur
                if (cur->val != val) flag = false; 
                cur = cur->right; //访问右子树
            } else { //存在左子树时
                mostRight = cur->left; //寻找左子树最右节点(大部分节点都有独一无二的左子树最右节点,除了没有左子树的节点以外)
                while (mostRight->right && mostRight->right != cur) 
                    mostRight = mostRight->right;
                if (mostRight->right == nullptr) { //此时为第一次到达cur
                    if (cur->val != val) flag = false; //访问cur
                    mostRight->right = cur; //指向已经访问过的节点作为标记; 同时用作第二次到达cur后返回的路径
                    cur = cur->left;        //访问左子树
                } else { 
                    mostRight->right = nullptr; //恢复原样
                    cur = cur->right;       //访问右子树
                }
            }
        }
        return flag;
    }
};
```
执行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了70.40% 的用户
内存消耗：10.1 MB, 在所有 C++ 提交中击败了12.84% 的用户
```
Morris中序遍历：
```cpp
class Solution { 
public: 
    bool isUnivalTree(TreeNode* root) { 
        if (!root) return true;
        TreeNode *cur = root, *mostRight = nullptr;
        int val = root->val;
        bool flag = true; //是否为单值二叉树
        while (cur) {
            if (cur->left == nullptr) { //没有左子树则直接访问cur
                if (cur->val != val) flag = false; 
                cur = cur->right; //访问右子树
            } else { //存在左子树时
                mostRight = cur->left; //寻找左子树最右节点(大部分节点都有独一无二的左子树最右节点,除了没有左子树的节点以外)
                while (mostRight->right && mostRight->right != cur) 
                    mostRight = mostRight->right;
                if (mostRight->right == nullptr) { //此时为第一次访问cur
                    mostRight->right = cur; //指向cur作为标记; 同时用作第二次到达cur后返回的路径
                    cur = cur->left;        //访问左子树
                } else { 
                    if (cur->val != val) flag = false; //第二次到达cur时访问
                    mostRight->right = nullptr; //恢复原样
                    cur = cur->right;       //访问右子树
                }
            }
        }
        return flag;
    }
};
```
奇特的是，这次运行起来效率蛮高：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：10.2 MB, 在所有 C++ 提交中击败了5.10% 的用户
```
Morris后序遍历就不写了，这里不太适合使用。
