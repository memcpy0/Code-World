<p>We are given the head node <code>root</code>&nbsp;of a binary tree, where additionally every node's value is either a 0 or a 1.</p>

<p>Return the same tree where every subtree (of the given tree) not containing a 1 has been removed.</p>

<p>(Recall that the subtree of a node X is X, plus every node that is a descendant of X.)</p>

<strong>Example 1:</strong>
 
```css
Input: [1,null,0,0,1]
Output: [1,null,0,null,1]

Explanation: 
Only the red nodes satisfy the property "every subtree not containing a 1".
The diagram on the right represents the answer.
```

<img style="width:450px" src="https://s3-lc-upload.s3.amazonaws.com/uploads/2018/04/06/1028_2.png" alt="">


<strong>Example 2:</strong>
 

```css
Input: [1,0,1,0,0,0,1]
Output: [1,null,1,null,1]
```

 
<img style="width:450px" src="https://s3-lc-upload.s3.amazonaws.com/uploads/2018/04/06/1028_1.png" alt="">
 
<strong>Example 3:</strong>
 

```css
Input: [1,1,0,1,1,0,1,0]
Output: [1,1,0,1,1,null,1]
```

<img style="width:450px" src="https://s3-lc-upload.s3.amazonaws.com/uploads/2018/04/05/1028.png" alt="">
 

<p><strong>Note: </strong></p>

<ul>
	<li>The binary tree&nbsp;will&nbsp;have&nbsp;at&nbsp;most <code>200 nodes</code>.</li>
	<li>The value of each node will only be <code>0</code> or <code>1</code>.</li>
</ul>

题意：给定二叉树根结点 `root` ，此外树的每个结点的值要么是 `0` ，要么是 `1` 。返回移除了**所有不包含 `1` 的子树**的原二叉树。

---
### 解法 递归
对左右子树分别递归，如果返回结果是 `true` ，则说明对应子树中存在 `1` ；否则移除对应子树。此后，如果当前节点为叶子节点，且值为 `0` ，则返回 `false` ；否则返回 `true` 。具体代码如下：
```cpp
class Solution {
private:
    bool pruneZeros(TreeNode* root) { //return true, have ones; return false, all zeros
        if (root == nullptr) return true; 
        if (pruneZeros(root->left) == false) root->left = nullptr;
        if (pruneZeros(root->right) == false) root->right = nullptr;
        if (!root->left && !root->right && root->val == 0) return false;
        return true;
    }
public:
    TreeNode* pruneTree(TreeNode* root) {
        return pruneZeros(root) ? root : nullptr;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：9.2 MB, 在所有 C++ 提交中击败了96.51% 的用户
```
利用 `pruneTree` 的语义，简化一下代码：
```cpp
class Solution { 
public:
    TreeNode* pruneTree(TreeNode* root) {
        if (root == nullptr) return root;
        root->left = pruneTree(root->left);
        root->right = pruneTree(root->right);
        if (!root->left && !root->right && root->val == 0) return nullptr;
        return root;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：9.1 MB, 在所有 C++ 提交中击败了98.20% 的用户
```
