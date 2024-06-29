Given two binary search trees `root1` and `root2` . Return a list containing *all the integers* from both trees sorted in **ascending** order.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201002162246437.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```swift
Input: root1 = [2,1,4], root2 = [1,0,3]
Output: [0,1,1,2,3,4]
```

**Example 2:**

```swift
Input: root1 = [0,-10,10], root2 = [5,1,7,0,2]
Output: [-10,0,0,1,2,5,7,10]
```

**Example 3:**

```swift
Input: root1 = [], root2 = [5,1,7,0,2]
Output: [0,1,2,5,7]
```

**Example 4:**

```swift
Input: root1 = [0,-10,10], root2 = []
Output: [-10,0,10]
```

**Example 5:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201002162256452.png#pic_center)

```swift
Input: root1 = [1,null,8], root2 = [8,1]
Output: [1,1,8,8]
```
**Constraints:**
-  Each tree has at most `5000` nodes.
- Each node's value is between `[-10^5, 10^5]` .


题意：返回一个列表，包含两棵树中的所有整数，并按照升序排列。

---
### 思路1
全部收集在一个数组中，然后排序的做法是 $O(n\log n)$ 的。这里对两棵树分别中序遍历得到序列，然后归并即可，$O(n)$ 时间和空间：
```cpp 
class Solution {
public:
    vector<int> v1, v2;
    vector<int> getAllElements(TreeNode* root1, TreeNode* root2) {
        inorder(root1, v1); //中序遍历
        inorder(root2, v2); //中序遍历
        vector<int> ans;
        int i = 0, j = 0, size1 = v1.size(), size2 = v2.size(); 
        while (i < size1 && j < size2) { //归并
            if (v1[i] < v2[j]) ans.push_back(v1[i++]);
            else ans.push_back(v2[j++]);
        }
        while (i < size1) ans.push_back(v1[i++]);
        while (j < size2) ans.push_back(v2[j++]);
        return ans;
    }
    void inorder(TreeNode *root, vector<int> &vi) {
        if (root) {
            inorder(root->left, vi);
            vi.push_back(root->val);
            inorder(root->right, vi);
        }
    }
};
```
效率如下：
```cpp
执行用时：244 ms, 在所有 C++ 提交中击败了94.47% 的用户
内存消耗：84.7 MB, 在所有 C++ 提交中击败了41.27% 的用户
```
---
### 思路2
利用非递归中序遍历的思想，和 `173. Binary Search Tree Iterator` 的做法。我们可以想出一种方法，仅仅使用 $O(\log h)$ 的空间和 $O(n)$ 的时间就可以完成这一题：
```cpp 
class Solution {
private:
    void inorderLeftTraverse(TreeNode *root, stack<TreeNode*> &st) { //访问并保存二叉搜索树的左子链
        while (root) {
            st.push(root);
            root = root->left;
        }
    }
    void helper(stack<TreeNode*> &st, vector<int> &ans) { 
        TreeNode *t = st.top(); st.pop(); //访问这一结点,并弹出
        ans.push_back(t->val); 
        inorderLeftTraverse(t->right, st); //访问该结点的右子树的左子链
    }
public: 
    vector<int> getAllElements(TreeNode* root1, TreeNode* root2) {
        stack<TreeNode*> st1, st2;
        inorderLeftTraverse(root1, st1); //保存左子链
        inorderLeftTraverse(root2, st2); //保存左子链
        vector<int> ans;
        while (!st1.empty() && !st2.empty()) { 
            if (st1.top()->val < st2.top()->val) helper(st1, ans); 
            else helper(st2, ans); 
        }
        while (!st1.empty()) helper(st1, ans); 
        while (!st2.empty()) helper(st2, ans); 
        return ans;
    }
};
```
提交后效率如下：
```cpp
执行用时：248 ms, 在所有 C++ 提交中击败了91.58% 的用户
内存消耗：82.8 MB, 在所有 C++ 提交中击败了77.23% 的用户
```
