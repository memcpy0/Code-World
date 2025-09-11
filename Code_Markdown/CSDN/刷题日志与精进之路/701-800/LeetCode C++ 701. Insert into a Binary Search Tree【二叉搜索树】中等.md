<div class="content__1Y2H"><div class="notranslate"><p>Given the root node of a binary search tree (BST) and a value to be inserted into the tree,&nbsp;insert the value into the BST. Return the root node of the BST after the insertion. It is guaranteed that the new value <b>does not exist</b> in the original BST.</p>

<p>Note that there may exist&nbsp;multiple valid ways for the&nbsp;insertion, as long as the tree remains a BST after insertion. You can return any of them.</p>

<p>For example,&nbsp;</p>

```
Given the tree:
        4
       / \
      2   7
     / \
    1   3
And the value to insert: 5
```

<p>You can return this binary search tree:</p>

```
         4
       /   \
      2     7
     / \   /
    1   3 5
```

<p>This tree is also valid:</p>

```
		 5	
       /   \
      2     7
     / \   
    1   3
         \
          4
```

<p><strong>Constraints:</strong></p> 
<ul>
	<li>The number of nodes in the given tree will be between <code>0</code> and <code>10^4</code>.</li>
	<li>Each node will have a unique integer value from <code>0</code>&nbsp;to -<code>10^8</code>, inclusive.</li>
	<li><code>-10^8 &lt;= val &lt;= 10^8</code></li>
	<li>It's guaranteed that <code>val</code> does not exist in the original BST.</li>
</ul>
</div></div>

题意：给定二叉搜索树的根节点和要插入树中的值，将值插入二叉搜索树，返回插入后二叉搜索树的根节点。 这里保证原始二叉搜索树中不存在新值。

题目提示我们，**可能存在多种有效的插入方式**，只要树在插入后仍保持为二叉搜索树即可。

---
### 思路1：递归插入
```cpp
class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (root == nullptr) return new TreeNode(val);
        if (val < root->val) root->left = insertIntoBST(root->left, val);
        else root->right = insertIntoBST(root->right, val);
        return root;
    }
};
```
效率如下，感觉好低啊：
```cpp
执行用时：176 ms, 在所有 C++ 提交中击败了5.65% 的用户
内存消耗：56 MB, 在所有 C++ 提交中击败了5.08% 的用户
```
---
### 思路2：迭代插入
先找到要插入的地方及其父结点，然后比较值，插入为左结点或右结点：
```cpp
class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (root == nullptr) return new TreeNode(val);
        TreeNode *temp = root, *father = root;
        while (temp) {
            father = temp;
            temp = val < temp->val ? temp->left : temp->right;
        }
        if (val < father->val) father->left = new TreeNode(val);
        else father->right = new TreeNode(val);
        return root;
    }
};
```
效率也很低：
```cpp
执行用时：168 ms, 在所有 C++ 提交中击败了5.65% 的用户
内存消耗：55.8 MB, 在所有 C++ 提交中击败了5.08% 的用户
```
用Java提交一下：
```java
class Solution {
    public TreeNode insertIntoBST(TreeNode root, int val) {
        if (root == null) return new TreeNode(val);
        TreeNode temp = root, father = root;
        while (temp != null) {
            father = temp;
            temp = val < temp.val ? temp.left : temp.right;
        }
        if (val < father.val) father.left = new TreeNode(val);
        else father.right = new TreeNode(val);
        return root;
    }
}
```
这是赤裸裸的作弊吧？？？
```java
执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
内存消耗：39.4 MB, 在所有 Java 提交中击败了56.71% 的用户
```
**2020/9/30 Update**：Python代码如下：
```py
# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def insertIntoBST(self, root: TreeNode, val: int) -> TreeNode:
        if not root: 
            return TreeNode(val)
        if val < root.val:
            root.left = self.insertIntoBST(root.left, val)
        else:
            root.right = self.insertIntoBST(root.right, val)
        return root
```
