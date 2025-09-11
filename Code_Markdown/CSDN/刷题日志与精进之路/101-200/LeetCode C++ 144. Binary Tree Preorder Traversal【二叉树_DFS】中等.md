Given a binary tree, return the preorder traversal of its nodes' values.

**Example:**
```cpp
Input: [1,null,2,3]
   1
    \
     2
    /
   3

Output: [1,2,3]
```
**Follow up:** Recursive solution is trivial, could you do it iteratively?

题意：进行二叉树的先序遍历，返回元素序列。

---
### 解法1 递归遍历
先序递归遍历，很简单的代码：
```cpp
class Solution {
public: 
    vector<int> ans;
    vector<int> preorderTraversal(TreeNode* root) { 
        if (root) {
        	ans.push_back(root->val);
            preorderTraversal(root->left);
            preorderTraversal(root->right);
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了49.37% 的用户
内存消耗：10.4 MB, 在所有 C++ 提交中击败了5.07% 的用户
```
---
### 解法2 非递归遍历
非递归先序遍历，教科书式的写法：
```cpp
class Solution {
public: 
    vector<int> preorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        vector<int> ans;    
        stack<TreeNode*> st;
        TreeNode *cur = root;
        while (cur || !st.empty()) {
            while (cur) {
                ans.push_back(cur->val);
                st.push(cur);
                cur = cur->left;
            }
            cur = st.top(); st.pop();
            cur = cur->right;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了49.37% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了26.30% 的用户
```

---
### 解法3 非递归遍历
模拟系统栈的调用。代码如下：
```cpp
class Solution {
public: 
    struct command {
        TreeNode *node;
        int instruction;
        command(int i = 0, TreeNode *t = nullptr) : instruction(i), node(t) { }
    };
    vector<int> preorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        vector<int> ans;    
        stack<command> st;
        st.push(command(1, root));
        while (!st.empty()) {
            const command t = st.top(); st.pop();
            if (t.instruction == 0) {
                ans.push_back(t.node->val);
            } else {
                if (t.node->right) st.push(command(1, t.node->right));
                if (t.node->left) st.push(command(1, t.node->left));
                st.push(command(0, t.node));
            }
        }
        return ans;
    }
};
```
提交结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了49.61% 的用户
```
---
### 解法4 简化版
简化思路3的写法，更简单的非递归先序遍历代码如下：
```cpp
class Solution {
public:  
    vector<int> preorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        vector<int> ans;  
        stack<TreeNode*> st;   
        st.push(root);
        while (!st.empty()) {
            const TreeNode *t = st.top(); st.pop();
            if (t->right) st.push(t->right);
            if (t->left) st.push(t->left);
            ans.push_back(t->val);
        }
        return ans;
    }
};
```
提交结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了26.30% 的用户
```
---
### 解法5 Morris遍历（20201027 Update）
$O(n)$ 时间、$O(1)$ 空间的一种解法，具体代码如下：
```cpp
class Solution {
public:  
    vector<int> preorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        TreeNode *cur = root, *mostRight;
        vector<int> ans;
        while (cur) {
            if (cur->left == nullptr) {
                ans.push_back(cur->val);
                cur = cur->right;
            } else {
                mostRight = cur->left;
                while (mostRight->right && mostRight->right != cur)
                    mostRight = mostRight->right;
                if (mostRight->right == nullptr) { //第一次访问该节点cur
                    ans.push_back(cur->val);
                    mostRight->right = cur;
                    cur = cur->left;
                } else { //第二次访问该节点
                    mostRight->right = nullptr;
                    cur = cur->right;
                }
            }
        }
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了44.50% 的用户
```
