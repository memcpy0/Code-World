给定一棵二叉搜索树，请找出其中第 `k` 的节点。

**示例 1:**

```swift
输入: root = [3,1,4,null,2], k = 1
   3
  / \
 1   4
  \
   2
输出: 4
```
**示例 2:**
```swift
输入: root = [5,3,6,2,4,null,null,1], k = 3
       5
      / \
     3   6
    / \
   2   4
  /
 1
输出: 4 
```
**限制：** `1 ≤ k ≤ 二叉搜索树元素个数`

---
### 解法一 递归中序遍历
要找到第 $k$ 大的结点，只需要依据右根左的遍历顺序，递归中序遍历整棵二叉树即可。时间复杂度为 $O(n)$ 。
```cpp
class Solution {
public:
    int kth = 0, ans;
    int kthLargest(TreeNode* root, int k) {
        inorder(root, k);
        return ans;
    }
    void inorder(TreeNode* root, int k) {
        if (root) {
            inorder(root->right, k);
            ++kth;
            if (kth == k) ans = root->val;
            inorder(root->left, k);
        } 
    }
};
```
效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了96.71% 的用户
内存消耗：23.6 MB, 在所有 C++ 提交中击败了64.23% 的用户
```

---
### 解法二 迭代中序遍历
如果我们找到了第 $k$ 大的结点之后及时退出，似乎能节省一些时间。使用非递归遍历可以做到这一点：
```cpp
class Solution {
public: 
    int kthLargest(TreeNode* root, int k) {
        stack<TreeNode*> st;
        int ans;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->right;
            }
            root = st.top(); st.pop();
            if (--k == 0) { ans = root->val; break; }
            root = root->left;
        }
        return ans;
    }  
};
```
效率如下，反而慢了一点？？？
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了87.14% 的用户
内存消耗：23.6 MB, 在所有 C++ 提交中击败了51.97% 的用户
```
