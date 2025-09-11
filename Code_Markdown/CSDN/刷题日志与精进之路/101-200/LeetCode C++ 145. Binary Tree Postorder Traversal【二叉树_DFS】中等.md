Given the `root` of a binary tree, return *the postorder traversal of its nodes' values*.

**Follow up:** Recursive solution is trivial, could you do it iteratively?

**Example 1:**
<img src="https://img-blog.csdnimg.cn/20200922161622900.png" width="20%">
```cpp
Input: root = [1,null,2,3]
Output: [3,2,1]
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
<img src="https://img-blog.csdnimg.cn/20200922161633890.png" width="20%">
```cpp
Input: root = [1,2]
Output: [2,1]
```

**Example 5:**
<img src="https://img-blog.csdnimg.cn/20200922161645751.png" width="20%">
```cpp
Input: root = [1,null,2]
Output: [2,1]
```

**Constraints:**
-  The number of the nodes in the tree is in the range `[0, 100]` .
 -   `-100 <= Node.val <= 100`


题意：进行二叉树的后序遍历，返回元素序列。

---
### 思路1
后序递归遍历，很简单的代码：
```cpp
class Solution {
public:
    vector<int> ans; 
    vector<int> postorderTraversal(TreeNode* root) {
        if (root) {
            postorderTraversal(root->left);
            postorderTraversal(root->right);
        	ans.push_back(root->val);
        }
        return ans;
    }
}; 
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了48.38% 的用户
内存消耗：10 MB, 在所有 C++ 提交中击败了5.01% 的用户
```
---
### 思路2
非递归后序遍历，教科书式的写法：
```cpp
class Solution {
public: 
    vector<int> postorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        vector<int> ans;    
        stack<TreeNode*> st;
        TreeNode *cur = root;
        while (cur || !st.empty()) {
            while (cur) { 
                st.push(cur);
                cur = cur->left;
            }
            TreeNode *temp = st.top();  //先回到父结点一层
            cur = temp->right;          //看看右子树
            if (cur == nullptr) {       //右子树为空
                ans.push_back(temp->val); st.pop(); //访问这一结点
                while (!st.empty() && temp == st.top()->right) { //从右子树返回
                    temp = st.top(); st.pop(); 
                    ans.push_back(temp->val);
                }
            } //右子树非空,访问右子树
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了48.38% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了24.33% 的用户
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
    vector<int> postorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        vector<int> ans;    
        stack<command> st;
        st.push(command(1, root));
        while (!st.empty()) {
            const command t = st.top(); st.pop();
            if (t.instruction == 0) {
                ans.push_back(t.node->val);
            } else {
                st.push(command(0, t.node));
                if (t.node->right) st.push(command(1, t.node->right));
                if (t.node->left) st.push(command(1, t.node->left));
            }
        }
        return ans;
    }
};
```
提交结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了27.68% 的用户
```
---
### 思路4
使用双栈法进行后序遍历：
```cpp
class Solution {
public:  
    vector<int> postorderTraversal(TreeNode* root) { 
        if (root == nullptr) return vector<int>();
        vector<int> ans;     //输出栈  
        stack<TreeNode*> st; //输入栈
        st.push(root);
        while (!st.empty()) { //根右左
            const TreeNode *t = st.top(); st.pop(); 
            if (t->left) st.push(t->left);
            if (t->right) st.push(t->right); 
            ans.push_back(t->val);
        } //翻转就是左右根
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```
提交结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.3 MB, 在所有 C++ 提交中击败了62.02% 的用户
```
