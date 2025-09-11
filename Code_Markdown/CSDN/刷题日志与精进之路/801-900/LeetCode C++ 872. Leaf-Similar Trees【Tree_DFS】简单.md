

Consider all the leaves of a binary tree, from left to right order, the values of those leaves form a **leaf value sequence.**
<img src="https://img-blog.csdnimg.cn/20201114154253216.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" witdh="60%">

For example, in the given tree above, the leaf value sequence is `(6, 7, 4, 9, 8)` .

Two binary trees are considered *leaf-similar* if their leaf value sequence is the same.

Return `true` if and only if the two given trees with head nodes `root1` and `root2` are leaf-similar.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201114155147204.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```swift
Input: root1 = [3,5,1,6,2,9,8,null,null,7,4], root2 = [3,5,1,6,7,4,2,null,null,null,null,null,null,9,8]
Output: true
```

**Example 2:**

```swift
Input: root1 = [1], root2 = [1]
Output: true
```

**Example 3:**

```swift
Input: root1 = [1], root2 = [2]
Output: false
```

**Example 4:**

```swift
Input: root1 = [1,2], root2 = [2,2]
Output: true
```

**Example 5:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201114155152408.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```swift
Input: root1 = [1,2,3], root2 = [1,3,2]
Output: false
```

 

**Constraints:**
- The number of nodes in each tree will be in the range `[1, 200]` .
- Both of the given trees will have values in the range `[0, 200]` .


题意：考虑一棵二叉树上所有的叶子，这些叶子的值按从左到右的顺序排列形成一个叶值序列 。如果有两棵二叉树的叶值序列是相同的，就认为它们是**叶相似**的。如果给定的两个头结点 `root1` 和 `root2` 代表的二叉树是**叶相似**的，则返回 `true`；否则返回 `false` 。

 
---
### 解法1 递归先序遍历
注意到本题不能使用BFS，因为树的叶子节点不一定在同一层上。下面的代码使用递归先序遍历，收集叶子节点的值序列，然后比较：
```cpp
class Solution {
private:
    void dfs(TreeNode* root, vector<int>& v) { 
        if (!root) return;
        if (!root->left && !root->right) { v.push_back(root->val); return; }
        dfs(root->left, v);
        dfs(root->right, v);
    }
public:
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> v1, v2;
        dfs(root1, v1); dfs(root2, v2);
        return v1 == v2;
    }
};
```
执行结果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了83.26% 的用户
内存消耗：13.8 MB, 在所有 C++ 提交中击败了5.09% 的用户
```
---
### 解法2 非递归先序遍历
使用栈，按照根-右-左的顺序进栈：
```cpp
class Solution {
private:
    void dfs(TreeNode* root, vector<int>& v) {
        stack<TreeNode*> st;
        st.push(root);
        while (!st.empty()) {
            TreeNode* tmp = st.top(); st.pop();
            if (!tmp->left && !tmp->right) v.push_back(tmp->val);
            if (tmp->right) st.push(tmp->right);
            if (tmp->left) st.push(tmp->left);
        }
    } 
public:
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> v1, v2;
        dfs(root1, v1); dfs(root2, v2); 
        return v1 == v2;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了27.53% 的用户
内存消耗：12.2 MB, 在所有 C++ 提交中击败了98.72% 的用户
```
