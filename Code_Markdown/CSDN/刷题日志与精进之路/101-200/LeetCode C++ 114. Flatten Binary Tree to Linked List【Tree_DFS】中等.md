Given a binary tree, **flatten** it to a linked list **in-place**.

For example, given the following tree:
```cpp
    1
   / \
  2   5
 / \   \
3   4   6
```

The flattened tree should look like:

```cpp
1
 \
  2
   \
    3
     \
      4
       \
        5
         \
          6
```


题意：给出一棵二叉树，要求**原地**将二叉树 `flatten` 成为一个单链表，此时，二叉树的结点只有右指针有效，它们指向**先序遍历序列**的直接后继。  

---
思路1：一开始想了一段时间，如果不要求原地的话，可以先序遍历得到序列，然后建立一个二叉树结点的单链表。代码就不写了。

----
思路2：考虑二叉树的五种基本形态：
- 空树和只有根结点，就直接返回；
- 如果左子树为空，就直接 `flatten(root->right)` ，对右子树操作；
- 如果右子树为空，就需要 `flatten(root->left)` ，然后 `root->right = root->left` 将右子树的指针指向原左子树；
- 如果都不为空，需要 `flatten(root->left); flatten(root->right);` 。然后先保存右子树的指针为 `temp` ；之后 `root->right = root->left` ，将右子树的指针指向原左子树；接着，一直沿着原来的左子树或者说是现在的右子树的右指针向下，直到 `root->right == nullptr` ，此时将 `root->right = temp` ，将原来的右子树接在这一点。

下面的代码是这一过程的简单翻译：
```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    void flatten(TreeNode* root) { 
        if (!root) return;
        if (!root->left && !root->right) return;
        if (!root->left) { 
            flatten(root->right);
            return;
        }
        if (!root->right) { 
            flatten(root->left);
            root->right = root->left;
            root->left = nullptr;
            return;
        }
        if (root->left && root->right) { 
            flatten(root->left);
            flatten(root->right);
            TreeNode *temp = root->right; 
            root->right = root->left; 
            root->left = nullptr;
            while (root->right != nullptr) {
                root = root->right;
            } 
            root->right = temp;
        } 
    }
};
```
后来发现，**对于后面的三种情况，可以统一起来操作**，简化的代码如下：
```cpp
class Solution {
public:
    void flatten(TreeNode* root) {
        if (!root || (!root->left && !root->right)) return;
        flatten(root->left);
        flatten(root->right);
        TreeNode *temp = root->right;
        root->right = root->left;
        root->left = nullptr;
        while (root->right) root = root->right;
        root->right = temp; 
    }
};
```
