给定一个二叉搜索树, 找到该树中两个指定节点的最近公共祖先。

百度百科中最近公共祖先的定义为：“对于有根树 T 的两个结点 p、q，最近公共祖先表示为一个结点 `x`，满足 `x` 是 `p、q` 的祖先且 `x` 的深度尽可能大（一个节点也可以是它自己的祖先）。”

例如，给定如下二叉搜索树:  `root = [6,2,8,0,4,7,9,null,null,3,5]`

 
示例 1:
```cpp
输入: root = [6,2,8,0,4,7,9,null,null,3,5], p = 2, q = 8
输出: 6 
解释: 节点 2 和节点 8 的最近公共祖先是 6。
```

示例 2:
```cpp
输入: root = [6,2,8,0,4,7,9,null,null,3,5], p = 2, q = 4
输出: 2
解释: 节点 2 和节点 4 的最近公共祖先是 2, 因为根据定义最近公共祖先节点可以为节点本身。
```

 

说明:
-    所有节点的值都是唯一的。
-    `p、q` 为不同节点且均存在于给定的二叉搜索树中。

 
题意：给出一棵二叉搜索树，以及两个结点 `p,q` ，寻找它们的最近公共祖先 `x` ——满足 `x` 是 `p,q` 的祖先且 `x` 的深度尽可能大。

思路：如果是普通的LCA还有点麻烦，但是二叉搜索树的性质让这道题目简单多了：
- 如果是空树则返回 `nullptr` ；
- `p,q` 的值分别为 `val1,val2` ；
- 如果 `p,q` 被根节点分割开来，即一个在左子树，另一个在右子树；或者一个在根节点，另一个在左/右子树（题目中给出的示例），此时 `lca` 就是 `root` ；
- 如果 `p,q` 的值都严格小于 `root->val` ，则到左子树上寻找；
- 反之，到右子树上寻找。

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
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root) return nullptr;
        int val1 = p->val, val2 = q->val;
        if (val1 > val2) swap(val1, val2);
        if (val1 <= root->val && val2 >= root->val) return root;
        else if (val1 < root->val && val2 < root->val) return lowestCommonAncestor(root->left, p, q);
        else return lowestCommonAncestor(root->right, p, q);
    }
};
```
