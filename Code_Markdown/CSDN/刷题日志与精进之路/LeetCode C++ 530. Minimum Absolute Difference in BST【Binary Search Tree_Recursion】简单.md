

Given a binary search tree with **non-negative** values, find the minimum **absolute** difference between values of any two nodes.

**Example:**

```swift
Input:

   1
    \
     3
    /
   2

Output:
1

Explanation:
The minimum absolute difference is 1, which is the difference between 2 and 1 (or between 2 and 3).
```

 

**Note:** There are at least two nodes in this BST.


题意：给出一棵所有节点为非负值的二叉搜索树，计算树中任意两节点的差的绝对值的最小值。

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
    int getMinimumDifference(TreeNode* root) {
        inorder(root);
        return ans;   
    }
};
```
效果如下：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了20.08% 的用户
内存消耗：25 MB, 在所有 C++ 提交中击败了19.79% 的用户
```
或者写成如下形式：
```cpp
class Solution {
public:
    int pre = -1, ans = INT_MAX;
    void inorder(TreeNode* root) {
        if (root) {
            inorder(root->left);
            if (~pre) ans = min(ans, abs(root->val - pre));
            pre = root->val; 
            inorder(root->right);
        }
    }
    int getMinimumDifference(TreeNode* root) {
        inorder(root);
        return ans;
    }
}; 
```

---
### 解法2 迭代中序遍历
```cpp
class Solution {
public:
    int getMinimumDifference(TreeNode* root) {
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
执行用时：32 ms, 在所有 C++ 提交中击败了59.46% 的用户
内存消耗：25.1 MB, 在所有 C++ 提交中击败了8.76% 的用户
```
