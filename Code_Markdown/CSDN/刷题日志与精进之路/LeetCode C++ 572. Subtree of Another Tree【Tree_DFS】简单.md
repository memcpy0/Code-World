Given two **non-empty** binary trees `s` and `t` , check whether tree `t` has exactly the same structure and node values with a subtree of `s` . A **subtree** of `s` is a tree consists of a node in s and all of this node's descendants. The tree `s` could also be considered as a subtree of itself.

**Example 1:**
Given tree `s` :
```c
     3
    / \
   4   5
  / \
 1   2
```
Given tree t:
```c
   4 
  / \
 1   2
```

Return `true` , because `t` has the same structure and node values with a subtree of `s` .

**Example 2:**
Given tree `s` :
```c
     3
    / \
   4   5
  / \
 1   2
    /
   0
```
Given tree `t` :
```c
   4
  / \
 1   2
```
Return `false` .

题意：判断 `t` 是不是 `s` 的某个子树。要求结构相同，对应结点的值也相同。

---
### 思路：双重DFS
 相当于回顾了 `LeetCode 100. 相同的树` 这一题。我们先序DFS遍历 `s` 的所有结点，对以当前结点为根的每个子树，都和 `t` 进行**树是否相同**的判断。具体代码如下：
```cpp
class Solution {
public:
    bool isSubtree(TreeNode* s, TreeNode* t) {
        if (s == nullptr) return false; //s遍历完了,t给定为非空树,所以返回false
        return isSameTree(s, t) || isSubtree(s->left, t) || isSubtree(s->right, t); //短路运算符,有一个为真,返回真
    }
    //LeetCode 100题 相同的树的那个函数
    bool isSameTree(TreeNode *s, TreeNode *t) {
        if (!s && !t) return true;  //两子树相同
        if (!s || !t) return false; //有一个不为空
        if (s->val != t->val) return false;
        return isSameTree(s->left, t->left) && isSameTree(s->right, t->right);
    }
};
```
效率如下：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了71.61% 的用户
内存消耗：28.5 MB, 在所有 C++ 提交中击败了65.21% 的用户
```
