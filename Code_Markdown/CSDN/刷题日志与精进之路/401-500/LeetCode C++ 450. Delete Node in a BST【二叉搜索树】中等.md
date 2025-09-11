<div class="notranslate"><p>Given a root node reference of a BST and a key, delete the node with the given key in the BST. Return the root node reference (possibly updated) of the BST.</p>

<p>Basically, the deletion can be divided into two stages:</p>

<ol>
	<li>Search for a node to remove.</li>
	<li>If the node is found, delete the node.</li>
</ol>

<p><b>Follow up:</b>&nbsp;Can you solve it with time complexity <code>O(height of tree)</code>?</p>


<p><strong>Example 1:</strong></p>
<img style="width: 800px; height: 214px;" src="https://assets.leetcode.com/uploads/2020/09/04/del_node_1.jpg" alt="" width="49%"><img style="width: 350px; height: 255px;" src="https://assets.leetcode.com/uploads/2020/09/04/del_node_supp.jpg" alt="" width="18%">

```swift
Input: root = [5,3,6,2,4,null,7], key = 3
Output: [5,4,6,2,null,null,7]
Explanation: Given key to delete is 3. So we find the node with value 3 and delete it.
One valid answer is [5,4,6,2,null,null,7], shown in the above BST.
Please notice that another valid answer is [5,2,6,null,4,null,7] and it's also accepted.
```

<p><strong>Example 2:</strong></p>

```swift
Input: root = [5,3,6,2,4,null,7], key = 0
Output: [5,3,6,2,4,null,7]
Explanation: The tree does not contain a node with value = 0.
```

<p><strong>Example 3:</strong></p>

```swift
Input: root = [], key = 0
Output: []
```

 
<p><strong>Constraints:</strong></p> 
<ul>
	<li>The number of nodes in the tree is in the range <code>[0, 10<sup>4</sup>]</code>.</li>
	<li><code>-10<sup>5</sup> &lt;= Node.val &lt;= 10<sup>5</sup></code></li>
	<li>Each node has a <strong>unique</strong> value.</li>
	<li><code>root</code> is a valid binary search tree.</li>
	<li><code>-10<sup>5</sup> &lt;= key &lt;= 10<sup>5</sup></code></li>
</ul>
</div>


题意：给定一个二叉搜索树的根节点 `root` 和一个值 `key` ，删除二叉搜索树中的 `key` 对应的节点，并保证二叉搜索树的性质不变。返回二叉搜索树（有可能被更新）的根节点的引用。

---
### 思路
递归删除，注意分类讨论：
```cpp 
class Solution {
private:
    TreeNode *successor(TreeNode *root, TreeNode *parent) { //寻找后继节点
        TreeNode *temp = root;
        while (temp->left) { 
            parent = temp;
            temp = temp->left;
        } //parent节点发生了变化,后继节点不是要删除节点的直接孩子,需要保存后继节点的右子树
        if (temp != root) parent->left = temp->right; 
        return temp;
    }
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (root == nullptr) return root;
        if (key < root->val) root->left = deleteNode(root->left, key);
        else if (root->val < key) root->right = deleteNode(root->right, key);
        else {
            TreeNode *temp = root;
            if (temp->left == nullptr || temp->right == nullptr) { //如果是叶子节点或者只有一个孩子
                root = temp->left ? temp->left : temp->right; //如果存在孩子节点则保留
                if (temp->left) temp->left = nullptr;   //断开要删除节点和其他子树的连接
                if (temp->right) temp->right = nullptr; //断开要删除节点和其他子树的连接
            } else { //有两个孩子的内部节点
                root = successor(temp->right, temp);    //后继节点
                root->left = temp->left; 
                if (root == temp->right) temp->right = nullptr; //避免循环指向 
                else root->right = temp->right; 
                temp->left = temp->right = nullptr;     //断开要删除节点和其他子树的连接
            }
            delete temp; //删除节点
        }
        return root;
    }
};
```
效率如下：
```cpp
执行用时：68 ms, 在所有 C++ 提交中击败了43.30% 的用户
内存消耗：32.2 MB, 在所有 C++ 提交中击败了5.12% 的用户
```
