

The data structure `TreeNode` is used for binary tree, but it can also used to represent a single linked list (where left is null, and right is the next node in the list). Implement a method to convert a binary search tree (implemented with `TreeNode`) into a single linked list. The values should be kept in order and the operation should be performed in place (that is, on the original data structure).

Return the head node of the linked list after converting.

**Note:** This problem is slightly different from the original one in the book.

 

**Example:**

```swift
Input:  [4,2,5,1,3,null,6,0]
Output:  [0,null,1,null,2,null,3,null,4,null,5,null,6]
```

**Note:** The number of nodes will not exceed `100000`.

题意：给定一棵二叉搜索树，将其转变为一个单向链表，要求依然符合二叉搜索树的性质，也就是在原始的二叉搜索树上直接修改。

---
### 解法1 递归
递归中序遍历和线索二叉树的写法，每次将当前节点的左指针置空，并将直接前驱节点的右指针指向当前节点。`pre` 始终指向当前节点的直接前驱结点，在遍历时不断移动。
```cpp
class Solution {
private:
    TreeNode *listHead = nullptr, *pre = nullptr;
    void inorder(TreeNode* root) {
        if (!root) return;
        inorder(root->left);
        root->left = nullptr; 
        if (listHead == nullptr) listHead = root;
        if (pre == nullptr) pre = root;
        else { 
             // pre->left = nullptr; //写这一句会导致堆栈溢出
            pre->right = root;
            pre = root;
        }
        inorder(root->right);
    }
public:
    TreeNode* convertBiNode(TreeNode* root) {
        if (!root) return root;
        inorder(root); 
        return listHead;
    }
};
```
写 `pre->left = nullptr` 会导致内存超出，其原因在于：可能会出现最后一个节点 `final` 的左指针没有清空，而且 `final` 的直接前驱节点 `prev` 恰好是 `final` 的左孩子节点，`prev->left = nullptr, prev->right = final;` ，同时 `final->left = prev` 。这样就存在两个节点相互引用，而LeetCode检验结果时会不断循环。

---
### 解法2 迭代
```cpp
class Solution {  
public:
    TreeNode* convertBiNode(TreeNode* root) {
        if (!root) return root;
        TreeNode *listHead = nullptr, *pre = nullptr; 
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            TreeNode *temp = st.top(); st.pop();
            temp->left = nullptr;
            if (listHead == nullptr) listHead = temp;
            if (pre == nullptr) pre = temp;
            else {
                // pre->left = nullptr;
                pre->right = temp;
                pre = temp;
            }
            root = temp->right;
        }
        return listHead;
    }
};
```
效率如下：
```cpp
执行用时：96 ms, 在所有 C++ 提交中击败了98.15% 的用户
内存消耗：32.3 MB, 在所有 C++ 提交中击败了8.67% 的用户
```
