请完成一个函数，输入一个二叉树，该函数输出它的镜像。

例如输入：
```swift
     4
   /   \
  2     7
 / \   / \
1   3 6   9
```
镜像输出：
```swift
     4
   /   \
  7     2
 / \   / \
9   6 3   1
```
**示例 1：**

```swift
输入：root = [4,2,7,1,3,6,9]
输出：[4,7,2,9,6,3,1]
```
**限制：** `0 <= 节点个数 <= 1000`

---
### 解法1 递归遍历
```cpp
class Solution {
public:
    TreeNode* mirrorTree(TreeNode* root) {
        if (root) {
            swap(root->left, root->right);
            mirrorTree(root->left);
            mirrorTree(root->right);
        }
        return root;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：9.4 MB, 在所有 C++ 提交中击败了7.27% 的用户
```
---
### 解法2 非递归遍历
```cpp
class Solution {
public:
    TreeNode* mirrorTree(TreeNode* root) {
        if (!root) return root;
        vector<TreeNode*> st; 
        TreeNode *temp = root;
        while (temp || !st.empty()) {
            while (temp) {
                swap(temp->left, temp->right);
                st.push_back(temp);
                temp = temp->left;
            }
            temp = st.back(); st.pop_back();
            temp = temp->right;
        }
        return root;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：9.4 MB, 在所有 C++ 提交中击败了10.70% 的用户
```
