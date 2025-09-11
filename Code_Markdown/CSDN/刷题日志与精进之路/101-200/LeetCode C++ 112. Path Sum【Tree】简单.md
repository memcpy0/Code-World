Given a binary tree and a sum, determine if the tree has a root-to-leaf path such that adding up all the values along the path equals the given sum.

**Note**: A leaf is a node with no children.

**Example**: Given the below binary tree and `sum = 22` ,
```cpp
      5
     / \
    4   8
   /   / \
  11  13  4
 /  \      \
7    2      1
```

return true, as there exist a root-to-leaf path `5->4->11->2` which sum is `22` .

题意：判断二叉树中是否有一条从根到叶子节点的路径，其值之和为 `target` 。

思路：递归即可。

代码：
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
    bool count(TreeNode *root, int sum, int target) {
        if (!root) return false;
        sum += root->val;
        if (root->left == NULL && root->right == NULL) {
            if (sum == target) return true;
            return false;
        }
        if (root->left && count(root->left, sum, target)) return true;
        if (root->right && count(root->right, sum, target)) return true;
        return false;
    }
    bool hasPathSum(TreeNode* root, int sum) {
        return count(root, 0, sum); 
    }
};
```
或者改写成如下形式，更简单：
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
    bool hasPathSum(TreeNode* root, int sum) {
        if (!root) return false;
        sum -= root->val;
        if (!root->left && !root->right) {
            if (!sum) return true;
            return false;
        }
        if (root->left && hasPathSum(root->left, sum)) return true;
        if (root->right && hasPathSum(root->right, sum)) return true;
        return false;
    }
};
```
效率：
```
执行用时：12 ms, 在所有 C++ 提交中击败了84.30% 的用户
内存消耗：17.6 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
> Update: 2020/8/26
> 改成下面的代码：
> ```cpp
> class Solution {
> public:
>    bool hasPathSum(TreeNode* root, int sum) {
>        if (root == nullptr) return false; 
>        if (root->left == nullptr && root->right == nullptr)
>            return root->val == sum; 
>        return hasPathSum(root->left, sum - root->val) || 
>        		  hasPathSum(root->right, sum - root->val);
>    }
> }; 
> ```
