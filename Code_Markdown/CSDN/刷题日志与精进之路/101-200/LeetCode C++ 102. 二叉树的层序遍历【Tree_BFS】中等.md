

给你一个二叉树，请你返回其按 **层序遍历** 得到的节点值。 （即逐层地，从左到右访问所有节点）。

**示例**：二叉树：`[3,9,20,null,null,15,7]`，
```swift
    3
   / \
  9  20
    /  \
   15   7
```
返回其层次遍历结果：
```swift
[
  [3],
  [9,20],
  [15,7]
]
```

---
### 解法 BFS
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (root == nullptr) return {};
        vector<vector<int>> ans;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            ans.push_back(vector<int>());
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                ans.back().push_back(t->val);
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
        }
        return ans;
    }
};
```
提交后的执行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了46.70% 的用户
内存消耗：11.6 MB, 在所有 C++ 提交中击败了37.69% 的用户
```
