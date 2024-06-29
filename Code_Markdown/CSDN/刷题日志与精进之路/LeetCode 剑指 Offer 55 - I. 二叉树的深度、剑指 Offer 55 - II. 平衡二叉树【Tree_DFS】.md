
输入一棵二叉树的根节点，求该树的深度。从根节点到叶节点依次经过的节点（含根、叶节点）形成树的一条路径，最长路径的长度为树的深度。

例如：
```cpp
给定二叉树 [3,9,20,null,null,15,7]，

    3
   / \
  9  20
    /  \
   15   7

返回它的最大深度 3 。
```

 

提示： `节点总数 <= 10000`


思路：简单。代码如下：
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
    int maxDepth(TreeNode* root) {
        return !root ? 0 : max(maxDepth(root->left), maxDepth(root->right)) + 1;
    }
};
```

---
# 剑指 Offer 55 - II. 平衡二叉树

输入一棵二叉树的根节点，判断该树是不是平衡二叉树。如果某二叉树中任意节点的左右子树的深度相差不超过 `1` ，那么它就是一棵平衡二叉树。

 

**示例 1:**
```cpp
给定二叉树 [3,9,20,null,null,15,7]

    3
   / \
  9  20
    /  \
   15   7

返回 true 。
```
**示例 2:**
```cpp
给定二叉树 [1,2,2,3,3,null,null,4,4]

       1
      / \
     2   2
    / \
   3   3
  / \
 4   4

返回 false 。
```

 

限制：`1 <= 树的结点个数 <= 10000`

思路：这道题是求树的深度的扩展题目。如果使用先序或者后序遍历每个结点，然后对每个结点的左右子树调用 `maxDepth()` 函数，就会造成大量的重复访问。事实上，只访问一次就可以完成这道题目。

用后序遍历的方法遍历二叉树的每个结点。在遍历到一个结点之前，我们就已经遍历了它的左右子树。然后可以根据它的左右子结点的深度判断它是否是平衡的，并得到和记录当前结点的深度（它到叶子结点的路径长度）。当最后遍历到树的根结点时，就判断了整棵二叉树是否是平衡二叉树。 

代码如下：
```cpp
class Solution {
public:
    bool isBalanced(TreeNode* root, int &depth) {
        if (!root) { depth = 0; return true; }
        int left, right;
        if (isBalanced(root->left, left) && isBalanced(root->right, right)) {
            depth = max(left, right) + 1;
            return abs(left - right) <= 1;
        }
        return false;
    }
    bool isBalanced(TreeNode* root) {
        int depth = 0;
        return isBalanced(root, depth);
    }
};
```
