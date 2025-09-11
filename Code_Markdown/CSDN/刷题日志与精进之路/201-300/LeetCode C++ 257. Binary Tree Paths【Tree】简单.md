Given a binary tree, return all root-to-leaf paths.

**Note:** A leaf is a node with no children.

**Example:**
```cpp
Input:
   1
 /   \
2     3
 \
  5

Output: ["1->2->5", "1->3"]

Explanation: All root-to-leaf paths are: 1->2->5, 1->3
```
题意：给定一个二叉树，返回所有从根节点到叶子节点的路径的字符串表示。

---
思路：就是一个递归的题目。明确递归函数 `binaryTreePaths` 的语义是：返回以 `root` 为根结点的到叶子结点的路径字符串。然后按照二叉树的递归形态进行讨论即可。

代码：
```cpp
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> res;
        if (root == nullptr) return res;
        if (root->left == nullptr && root->right == nullptr) {
            res.push_back(to_string(root->val));
            return res;
        } 
        //获得以root为根结点的二叉树中,所有从根结点到每个叶子结点的字符串
        vector<string> leftStr = binaryTreePaths(root->left);
        for (int i = 0; i < leftStr.size(); ++i) 
            res.push_back(to_string(root->val) + "->" + leftStr[i]);
        vector<string> rightStr = binaryTreePaths(root->right);
        for (int i = 0; i < rightStr.size(); ++i) 
            res.push_back(to_string(root->val) + "->" + rightStr[i]);
        return res;
    }
};
```
效率：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：13.6 MB, 在所有 C++ 提交中击败了74.43% 的用户
```
