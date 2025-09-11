

Given a binary tree, return the *zigzag level order* traversal of its nodes' values. (ie, from left to right, then right to left for the next level and alternate between).

For example: Given binary tree `[3,9,20,null,null,15,7]`,
```clike
    3
   / \
  9  20
    /  \
   15   7
```
return its zigzag level order traversal as:
```clike
[
  [3],
  [20,9],
  [15,7]
]
```
题意：给定一个二叉树，返回其节点值的锯齿形层次遍历。即先从左往右，再从右往左进行下一层遍历，以此类推，层与层之间交替进行。

---
### 解法 标志变量
使用BFS+标志变量 `flag` ，很简单的题：
```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
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
            if (!flag) reverse(temp.begin(), temp.end()); //在flag为false时逆序
            flag = !flag;
            ans.push_back(temp);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了74.42% 的用户
内存消耗：11.2 MB, 在所有 C++ 提交中击败了57.79% 的用户
```
