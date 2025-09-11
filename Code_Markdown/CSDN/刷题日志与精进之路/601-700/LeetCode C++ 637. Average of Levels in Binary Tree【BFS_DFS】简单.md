Given a non-empty binary tree, return the average value of the nodes on each level in the form of an array.

**Example 1:**
```cpp
Input:
    3
   / \
  9  20
    /  \
   15   7
Output: [3, 14.5, 11]
Explanation:
The average value of nodes on level 0 is 3,  on level 1 is 14.5, and on level 2 is 11. Hence return [3, 14.5, 11].
```
**Note:** The range of node's value is in the range of 32-bit signed integer.

题意：给定一个非空二叉树，返回一个由每层节点平均值组成的数组。

---
思路：BFS。代码如下。不过不知道为什么，几乎一样的代码，Java运行时间是3ms左右，但是C++总是20ms左右。
```cpp
class Solution {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> ans;
        if (root == nullptr) return ans;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            long long aver = 0;
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                aver += t->val;
                if (t->left != nullptr) q.push(t->left);
                if (t->right != nullptr) q.push(t->right);
            }
            ans.emplace_back(1.0 * aver / size);
        }
        return ans;
    }
};
```
执行效率：
```css
执行用时：28 ms, 在所有 C++ 提交中击败了64.40% 的用户
内存消耗：22.7 MB, 在所有 C++ 提交中击败了45.62% 的用户
```
