Given a binary tree and a sum, find all root-to-leaf paths where each path's sum equals the given sum.

**Note**: A leaf is a node with no children.

**Example**: Given the below binary tree and `sum = 22` ,
```cpp
      5
     / \
    4   8
   /   / \
  11  13  4
 /  \    / \
7    2  5   1
```
Return:
```cpp
[
   [5,4,11,2],
   [5,8,4,5]
]
```
题意：找出二叉树中所有从根到叶子结点形成的、其值之和等于 `target` 的路径。

----
### 思路 DFS
很简单的题目。使用先序遍历，在叶子结点处添加路径的代码如下：
```cpp
class Solution {
public:
    vector<vector<int>> ans;    
    void getAllPath(TreeNode *root, int sum, vector<int> &t) {
        if (!root) return;
        t.push_back(root->val);
        if (!root->left && !root->right && sum == root->val) ans.push_back(t); 
        getAllPath(root->left, sum - root->val, t);
        getAllPath(root->right, sum - root->val, t);
        t.pop_back();
    }

    vector<vector<int>> pathSum(TreeNode* root, int sum) { 
        vector<int> temp;
        getAllPath(root, sum, temp);
        return ans;
    }
};
```
效率如下，还算不错：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了93.08% 的用户
内存消耗：17.1 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
