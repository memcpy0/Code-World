
Given the root of a binary tree, return *the inorder traversal of its nodes' values*.

**Follow up:** Recursive solution is trivial, could you do it iteratively?


**Example 1:**
<img src="https://img-blog.csdnimg.cn/20200922153604282.png" width="15%">
```cpp
Input: root = [1,null,2,3]
Output: [1,3,2]
```
**Example 2:**
```cpp
Input: root = []
Output: []
```
**Example 3:**
```cpp
Input: root = [1]
Output: [1]
```
**Example 4:**
<img src="https://img-blog.csdnimg.cn/20200922153614883.png" width="15%">
```cpp
Input: root = [1,2]
Output: [2,1]
```
**Example 5:**
<img src="https://img-blog.csdnimg.cn/20200922153620704.png" width="15%">
```cpp
Input: root = [1,null,2]
Output: [1,2]
```
**Constraints:**
-  The number of nodes in the tree is in the range `[0, 100]` .
 -  `-100 <= Node.val <= 100`


题意：进行二叉树的中序遍历，返回元素序列。

---
### 思路1
中序递归遍历，很简单的代码：
```cpp
class Solution {
public: 
    vector<int> ans;
    vector<int> inorderTraversal(TreeNode* root) { 
        if (root) {
            inorderTraversal(root->left);
            ans.push_back(root->val);
            inorderTraversal(root->right);
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：10.1 MB, 在所有 C++ 提交中击败了5.01% 的用户
```
---
### 思路2
非递归中序遍历，教科书式的写法：
```cpp
class Solution {
public: 
    vector<int> inorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        vector<int> ans;    
        stack<TreeNode*> st;
        TreeNode *cur = root;
        while (cur || !st.empty()) {
            while (cur) {
                st.push(cur);
                cur = cur->left;
            }
            cur = st.top(); st.pop();
            ans.push_back(cur->val);
            cur = cur->right;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.3 MB, 在所有 C++ 提交中击败了8.06% 的用户
```

---
### 思路3
模拟系统栈的调用。代码如下：
```cpp
class Solution {
public: 
    struct command {
        TreeNode *node;
        int instruction;
        command(int i = 0, TreeNode *t = nullptr) : instruction(i), node(t) { }
    };
    vector<int> inorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        vector<int> ans;    
        stack<command> st;
        st.push(command(1, root));
        while (!st.empty()) {
            command t = st.top(); st.pop();
            if (t.instruction == 0) {
                ans.push_back(t.node->val);
            } else {
                if (t.node->right) st.push(command(1, t.node->right));
                st.push(command(0, t.node));
                if (t.node->left) st.push(command(1, t.node->left));
            }
        }
        return ans;
    }
};
```
效率较低：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了46.11% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了6.20% 的用户
```
