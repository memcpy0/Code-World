Given a binary tree, return the tilt of the **whole tree**.

The tilt of a **tree node** is defined as the **absolute difference** between **the sum of all left subtree node values** and **the sum of all right subtree node values**. Null node has tilt `0` .

The tilt of the **whole tree** is defined as **the sum of all nodes' tilt**.

**Example:**
```c
Input: 
         1
       /   \
      2     3
Output: 1
Explanation: 
Tilt of node 2 : 0
Tilt of node 3 : 0
Tilt of node 1 : |2-3| = 1
Tilt of binary tree : 0 + 0 + 1 = 1
```
**Note:**
- The sum of node values in any subtree won't exceed the range of 32-bit integer.
-  All the tilt values won't exceed the range of 32-bit integer.


题意：给定一个二叉树，计算整个树的坡度。

---
### 思路 递归后序遍历
注意：一个二叉树的节点坡度定义为，该节点**左子树的结点值之和**和**右子树结点值之和**的**差**的**绝对值**。空结点的的坡度是 `0` 。题目要求出整个树的坡度，就是所有节点的坡度之和。递归后序遍历的代码如下：
```cpp
class Solution {
public:
    int findTilt(TreeNode* root) {
        int tilt = 0;
        postorder(root, tilt);
        return tilt;
    }
    int postorder(TreeNode *root, int &tilt) { //返回子树的值之和
        if (!root) return 0;
        if (!root->left && !root->right) return root->val; 
        int lsum = postorder(root->left, tilt);
        int rsum = postorder(root->right, tilt);
        tilt += abs(lsum - rsum);
        return root->val + lsum + rsum;
    }
};
```
效率较低：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了67.95% 的用户
内存消耗：23.2 MB, 在所有 C++ 提交中击败了30.93% 的用户
```

