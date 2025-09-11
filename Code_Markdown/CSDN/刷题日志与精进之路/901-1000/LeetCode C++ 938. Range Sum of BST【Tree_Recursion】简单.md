Given the `root` node of a binary search tree, return the sum of values of all nodes with value between `L` and `R` (inclusive).

The binary search tree is guaranteed to have unique values.

 

**Example 1:**

```clike
Input: root = [10,5,15,3,7,null,18], L = 7, R = 15
Output: 32
```

**Example 2:**

```clike
Input: root = [10,5,15,3,7,13,18,1,null,6], L = 6, R = 10
Output: 23
```

 

**Note:**
- The number of nodes in the tree is at most `10000`.
- The final answer is guaranteed to be less than `2^31` .

题意：给定二叉搜索树的根结点 `root`，返回 `L` 和 `R`（含）之间的所有结点的值的和。注意，二叉搜索树保证具有唯一的值。
 
---
### 解法1 递归遍历
$O(n)$ 时间的递归方法：
```cpp
//先序遍历
class Solution {
private:
    int l, r, ans = 0;
    void preorder(TreeNode* root) {
        if (root) {
            if (root->val >=  l && root->val <= r) ans += root->val;
            preorder(root->left);
            preorder(root->right);
        }
    }
public:
    int rangeSumBST(TreeNode* root, int L, int R) {
        if (root == nullptr) return 0;
        l = L, r = R; 
        preorder(root);
        return ans;
    }
};
//中序遍历
class Solution {
private:
    int l, r, ans = 0;
    void inorder(TreeNode* root) {
        if (root) {
            inorder(root->left);
            if (root->val >=  l && root->val <= r) ans += root->val;
            inorder(root->right);
        }
    }
public:
    int rangeSumBST(TreeNode* root, int L, int R) {
        if (root == nullptr) return 0;
        l = L, r = R;
        inorder(root);
        return ans;
    }
};
//后序遍历
class Solution {
private:
    int l, r, ans = 0;
    void postorder(TreeNode* root) {
        if (root) {
            postorder(root->left);
            postorder(root->right);
            if (root->val >=  l && root->val <= r) ans += root->val;
        }
    }
public:
    int rangeSumBST(TreeNode* root, int L, int R) {
        if (root == nullptr) return 0;
        l = L, r = R; 
        postorder(root);
        return ans;
    }
};
```

---
### 解法2 非递归遍历
$O(n)$ 时间的非递归解法如下：
```cpp
//先序遍历
class Solution { 
public:
    int rangeSumBST(TreeNode* root, int L, int R) {
        if (root == nullptr) return 0;
        int sum = 0;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                if (root->val >= L && root->val <= R) sum += root->val;
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            root = root->right;
        }
        return sum;
    }
};
//中序遍历
class Solution { 
public:
    int rangeSumBST(TreeNode* root, int L, int R) {
        if (root == nullptr) return 0;
        int sum = 0;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            if (root->val >= L && root->val <= R) sum += root->val;
            root = root->right;
        }
        return sum;
    }
};
//后序遍历
class Solution { 
public:
    int rangeSumBST(TreeNode* root, int L, int R) {
        if (root == nullptr) return 0;
        int sum = 0;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            TreeNode *temp = st.top();
            root = temp->right;
            if (root == nullptr) {
                if (temp->val >= L && temp->val <= R) sum += temp->val;
                st.pop();
                while (!st.empty() && st.top()->right == temp) {
                    temp = st.top(); st.pop();
                    if (temp->val >= L && temp->val <= R) sum += temp->val;
                }
            }
        }
        return sum;
    }
};
```
---
### 解法3 分解区间
有点像线段树划分区间，代码如下：
```cpp
class Solution {  
public:
    int rangeSumBST(TreeNode* root, int L, int R) { 
        if (root == nullptr) return 0; 
        if (root->val < L) return rangeSumBST(root->right, L, R); //当前节点小于L，往右子节点寻找
        else if (root->val > R) return rangeSumBST(root->left, L, R); //当前节点大于R，往左子节点寻找
        return root->val + rangeSumBST(root->left, L, R) + rangeSumBST(root->right, L, R);
    }
};
```
提交后效率如下：
```cpp
执行用时：208 ms, 在所有 C++ 提交中击败了97.49% 的用户
内存消耗：63.7 MB, 在所有 C++ 提交中击败了12.04% 的用户
```
