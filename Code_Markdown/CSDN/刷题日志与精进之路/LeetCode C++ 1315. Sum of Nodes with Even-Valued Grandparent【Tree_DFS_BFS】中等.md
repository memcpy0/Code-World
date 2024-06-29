
<p>Given a binary tree, return the sum of values of nodes with even-valued grandparent.&nbsp; (A <em>grandparent</em> of a node is the parent of its parent, if it exists.)</p>

<p>If there are no nodes with an even-valued grandparent, return&nbsp;<code>0</code>.</p>

 
<p><strong>Example 1:</strong></p>

<p><strong><img style="width: 350px; height: 214px;" src="https://assets.leetcode.com/uploads/2019/07/24/1473_ex1.png" alt=""></strong></p>

```css
Input: root = [6,7,8,2,7,1,3,9,null,1,4,null,null,null,5]
Output: 18
Explanation: The red nodes are the nodes with even-value grandparent while the blue nodes are the even-value grandparents. 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li>The number of nodes in the tree is between&nbsp;<code>1</code>&nbsp;and&nbsp;<code>10^4</code>.</li>
	<li>The value of nodes is between&nbsp;<code>1</code>&nbsp;and&nbsp;<code>100</code>.</li>
</ul>

题意：给出一棵二叉树，返回祖父节点的值为偶数的所有节点的值之和。如果不存在祖父节点值为偶数的节点，那么返回 `0` 。

---
### 解法 DFS
很简单的DFS题目。如果当前节点为空，则返回 `0` 。如果当前节点的祖父节点非空，则检查其值是否是偶数，是则加上当前节点的值。然后对左右子树递归调用，此时需要注意**父节点和祖父节点的变化**。
```cpp
class Solution {
private:
    int sumEven(TreeNode* root, TreeNode* grand, TreeNode* par) {
        if (root == nullptr) return 0;  
        return (grand ? ((grand->val & 1) ? 0 : root->val) : 0) + sumEven(root->left, par, root) + sumEven(root->right, par, root);
    }
public:
    int sumEvenGrandparent(TreeNode* root) {
        return sumEven(root, nullptr, nullptr);
    }
};
```
运行效率如下：
```cpp
执行用时：48 ms, 在所有 C++ 提交中击败了99.30% 的用户
内存消耗：37.1 MB, 在所有 C++ 提交中击败了99.82% 的用户
```
