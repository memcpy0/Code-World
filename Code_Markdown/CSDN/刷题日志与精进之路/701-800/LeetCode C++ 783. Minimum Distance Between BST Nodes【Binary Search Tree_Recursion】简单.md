Given a Binary Search Tree (BST) with the root node root, return the **minimum** difference between the values of any two different nodes in the tree.

**Example :**

```swift
Input: root = [4,2,6,1,3,null,null]
Output: 1
Explanation:
Note that root is a TreeNode object, not an array.

The given tree [4,2,6,1,3,null,null] is represented by the following diagram:

          4
        /   \
      2      6
     / \    
    1   3  

while the minimum difference in this tree is 1, it occurs between node 1 and node 2, also between node 3 and node 2.
```

**Note:**
- The size of the BST will be between `2` and `100` .
 - The BST is always valid, each node's value is an integer, and each node's value is different. 

题意：给定一个二叉搜索树的根节点 `root` ，返回树中任意两节点值之差的最小值。

---
### 解法1 递归中序遍历
由于要求任意两节点值之差的最小值，所以递归中序遍历，从小到大遍历所有值，计算两两之差，进行比较得到最小的差值：
```cpp
class Solution {
public:
    int ans = INT_MAX;
    TreeNode *pre = nullptr;
    void inorder(TreeNode* root) {
        if (root) {
            inorder(root->left);
            if (pre) ans = min(ans, root->val - pre->val);
            pre = root;
            inorder(root->right);
        }
    }
    int minDiffInBST(TreeNode* root) {
        inorder(root);
        return ans;   
    }
};
```
效果如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了29.59% 的用户
内存消耗：9.9 MB, 在所有 C++ 提交中击败了5.02% 的用户
```

---
### 解法2 迭代中序遍历
```cpp
class Solution {
public:
    int minDiffInBST(TreeNode* root) {
        TreeNode *pre = nullptr;
        int ans = INT_MAX;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            if (pre) ans = min(ans, root->val - pre->val);
            pre = root;
            root = root->right;
        }
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：9.9 MB, 在所有 C++ 提交中击败了5.02% 的用户
```
