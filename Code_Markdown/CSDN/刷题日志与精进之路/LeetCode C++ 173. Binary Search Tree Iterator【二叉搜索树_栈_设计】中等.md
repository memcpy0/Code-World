Implement an iterator over a binary search tree (BST). Your iterator will be initialized with the root node of a BST. Calling `next()` will return **the next smallest number** in the BST.

**Example:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200922165631155.png)
```cpp
BSTIterator iterator = new BSTIterator(root);
iterator.next();    // return 3
iterator.next();    // return 7
iterator.hasNext(); // return true
iterator.next();    // return 9
iterator.hasNext(); // return true
iterator.next();    // return 15
iterator.hasNext(); // return true
iterator.next();    // return 20
iterator.hasNext(); // return false
```
**Note:**
- `next()` and `hasNext()` should run in average `O(1)` time and uses `O(h)` memory, where `h` is the height of the tree.
 - You may assume that `next()` call will always be valid, that is, there will be at least a next smallest number in the BST when `next()` is called.


题意：设计一个二叉搜索树迭代器，使用二叉搜索树的根结点初始化迭代器。假定调用 `next()` 时总是有效的，它会返回二叉搜索树的下一个最小的数。

---
### 思路1
二叉搜索树的中序遍历序列是单调递增的，我们先进行递归中序遍历，得到二叉搜索树的中序序列。然后返回下一个数即可。初始化使用 $O(n)$ 时间和空间，`next(), hasNext()` 使用 $O(1)$ 时间和空间。代码如下：
```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class BSTIterator {
private:
    vector<int> inorders;
    int idx = 0;
    void inorder(TreeNode *root) {
        if (root) {
            inorder(root->left);
            inorders.push_back(root->val);
            inorder(root->right);
        }
    }
public:
    BSTIterator(TreeNode* root) {
        inorder(root);
    }
    /** @return the next smallest number */
    int next() {
        return inorders[idx++];
   	}
    /** @return whether we have a next smallest number */
    bool hasNext() {
        return idx < inorders.size() ? true : false;
    }
};
/**
 * Your BSTIterator object will be instantiated and called as such:
 * BSTIterator* obj = new BSTIterator(root);
 * int param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */
```
效率如下：
```cpp
执行用时：116 ms, 在所有 C++ 提交中击败了11.68% 的用户
内存消耗：25.9 MB, 在所有 C++ 提交中击败了13.39% 的用户
```
---
思路2：关键是构建一个 $O(h)$ 的**最小存储栈**。我们不进行事先的遍历，而是**利用二叉树的迭代中序遍历**，保存左子链，从而只使用 $O(h)$ 的内存。注意：这里的 `next(), hasNext()` 的均摊复杂度为 $O(1)$ 。代码如下：
```cpp
class BSTIterator {
private:
    vector<TreeNode*> st;
public:
    BSTIterator(TreeNode* root) { //O(logN)
        while (root) {
            st.push_back(root);
            root = root->left;
        }
    }
    
    /** @return the next smallest number */
    int next() { 
        TreeNode *t = st.back(); st.pop_back(); //中序遍历访问结点
        int val = t->val;
        t = t->right;
        while (t) {
            st.push_back(t);
            t = t->left;
        }
        return val;
    }
    
    /** @return whether we have a next smallest number */
    bool hasNext() {
        return !st.empty();
    }
};
```
效率如下：
```cpp
执行用时：92 ms, 在所有 C++ 提交中击败了68.20% 的用户
内存消耗：25.4 MB, 在所有 C++ 提交中击败了98.51% 的用户
```
