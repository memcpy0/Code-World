

从上到下按层打印二叉树，同一层的节点按从左到右的顺序打印，每一层打印到一行。

例如：给定二叉树 `[3,9,20,null,null,15,7]` ：

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
提示：`节点总数 <= 1000`

---
### 解法 BFS
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> ans;
        if (!root) return ans;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            ans.push_back(vector<int>());
            for (int i = 0; i < size; ++i) {
                TreeNode *temp = q.front(); q.pop();
                ans.back().push_back(temp->val);
                if (temp->left) q.push(temp->left);
                if (temp->right) q.push(temp->right);
            }
        }
        return ans;
    }
};
```
执行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了57.47% 的用户
内存消耗：12.5 MB, 在所有 C++ 提交中击败了48.66% 的用户
```
