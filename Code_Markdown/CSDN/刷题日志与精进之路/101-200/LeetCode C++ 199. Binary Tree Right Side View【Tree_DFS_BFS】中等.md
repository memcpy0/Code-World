Given a binary tree, imagine yourself standing on the right side of it, return the values of the nodes you can see ordered from top to bottom.

**Example:**
```cpp
Input: [1,2,3,null,5,null,4]
Output: [1, 3, 4]
Explanation:

   1            <---
 /   \
2     3         <---
 \     \
  5     4       <---
```

题意：给定一棵二叉树，想象站在它的右侧，按照从顶部到底部的顺序，返回从右侧能看到的节点值组成的列表。

---
### 思路1 BFS
广度优先搜索进行层次遍历，记录每层最后一个元素即可。代码如下：
```cpp 
class Solution {
public: 
    vector<int> rightSideView(TreeNode* root) {
        if (!root) return vector<int>(); 
        vector<int> vi;
        queue<TreeNode*> q;
        if (root) q.push(root);
        while (!q.empty()) { 
            int size = q.size(); 
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                if (i == size - 1) vi.push_back(t->val); //记录每层最后一个结点
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            } 
        }
        return vi;  
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了81.63% 的用户
内存消耗：11.6 MB, 在所有 C++ 提交中击败了46.80% 的用户
```
从右往左遍历的代码如下：
```cpp
class Solution {
public: 
    vector<int> rightSideView(TreeNode* root) {
        if (!root) return vector<int>(); 
        vector<int> vi;
        queue<TreeNode*> q;
        if (root) q.push(root);
        while (!q.empty()) { 
            int size = q.size(); 
            vi.push_back(q.front()->val); //记录每层最后一个结点
            while (size--) {
                TreeNode *t = q.front(); q.pop(); 
                if (t->right) q.push(t->right);
                if (t->left) q.push(t->left);
            } 
        }
        return vi;  
    }
};
```

---
### 思路2 DFS
按照根-右-左的顺序访问，**可以保证每层都是最先访问最右边的结点**。而普通的先序遍历正好相反，每层最先访问的是最左边的节点。
```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> ans;
    void dfs(TreeNode* root, int depth) {
        if (root) { 
            //根-右-左保证每层首先访问的都是最右边的结点
            //如果当前节点所在深度==ans.size(),说明该深度下当前节点是第一个被访问的节点,将当前节点加入res中。
            if (depth == ans.size()) ans.push_back(root->val);
            if (root->right) dfs(root->right, depth + 1);
            if (root->left) dfs(root->left, depth + 1);
        }
    }
    vector<int> rightSideView(TreeNode* root) {
        if (!root) return vector<int>();  
        dfs(root, 0);
        return ans;  
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：11.5 MB, 在所有 C++ 提交中击败了87.62% 的用户
```
