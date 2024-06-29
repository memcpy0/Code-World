请实现一个函数按照之字形顺序打印二叉树，即第一行按照从左到右的顺序打印，第二层按照从右到左的顺序打印，第三行再按照从左到右的顺序打印，其他行以此类推。

例如: 给定二叉树: `[3,9,20,null,null,15,7]` ,
```clike
    3
   / \
  9  20
    /  \
   15   7
```
返回其层次遍历结果：
```clike
[
  [3],
  [20,9],
  [15,7]
]
```
**提示**：`节点总数 <= 1000`

---
### 解法 BFS+标志变量
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (root == nullptr) return {};
        vector<vector<int>> ans;
        queue<TreeNode*> q;
        q.push(root);
        bool flag = true;
        while (!q.empty()) {
            int size = q.size();
            vector<int> temp;
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                temp.push_back(t->val);
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
            if (!flag) reverse(temp.begin(), temp.end());
            flag = !flag;
            ans.push_back(temp);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了55.74% 的用户
内存消耗：12.8 MB, 在所有 C++ 提交中击败了23.52% 的用户
```
