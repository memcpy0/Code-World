

Write an algorithm to find the "next" node (i.e., in-order successor) of a given node in a binary search tree.

Return `null` if there's no "next" node for the given node.

**Example 1:**
```swift
Input: root = [2,1,3], p = 1

  2
 / \
1   3

Output: 2
```

**Example 2:**

```swift
Input: root = [5,3,6,2,4,null,null,1], p = 6

      5
     / \
    3   6
   / \
  2   4
 /   
1

Output: null
```

题意：找出二叉搜索树中指定节点的中序直接后继结点。如果指定节点没有对应的直接后继，则返回 `null` 。

---
### 解法1 迭代中序遍历
迭代中序遍历，首先找到 `p` ，然后取出 `p` 的直接后继结点。此处为了同时得到当前结点 `cur` 和其对应的直接后继 `succ` ，使用**右根左**的遍历顺序：
```cpp
class Solution {
public:
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        TreeNode *succ = nullptr;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->right;
            }
            root = st.top(); st.pop();
            if (root == p) return succ; //当前结点==p,后继结点为succ
            succ = root;
            root = root->left;
        }
        return succ;
    }
};
```
执行效率如下：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了77.48% 的用户
内存消耗：22.9 MB, 在所有 C++ 提交中击败了20.82% 的用户
```
---
### 解法2 递归中序遍历
使用**右根左**的遍历顺序：
```cpp
class Solution { 
private:
    TreeNode *succ = nullptr, *ans = nullptr;
    void dfs(TreeNode* root, TreeNode* p) {
        if (root == nullptr) return;
        dfs(root->right, p);
        if (root == p) ans = succ;
        succ = root;
        dfs(root->left, p);
    }
public: 
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        if (root == nullptr) return nullptr; 
        dfs(root, p);
        return ans;
    }
};
```
效率如下，$O(n)$ 时间：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了77.48% 的用户
内存消耗：22.9 MB, 在所有 C++ 提交中击败了25.60% 的用户
```
---
### 解法3 利用二叉搜索树特性
```cpp
class Solution { 
public: 
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        if (root == nullptr) return nullptr; 
        //当前节点值<=目标节点值,则目标节点的后继必然存在于右子树中
        if (p->val >= root->val) return inorderSuccessor(root->right, p);
        //否则结果可能是当前节点,或者在当前节点的左子树中
        TreeNode *succ = inorderSuccessor(root->left, p);
        return succ ? succ : root; //如果左子树中存在后继,则返回;否则返回当前节点
    }
};
```
执行效率如下：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了53.91% 的用户
内存消耗：23 MB, 在所有 C++ 提交中击败了19.96% 的用户
```
