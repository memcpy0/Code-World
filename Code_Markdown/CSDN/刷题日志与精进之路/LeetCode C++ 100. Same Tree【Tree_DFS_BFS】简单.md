Given two binary trees, write a function to check if they are the same or not.

Two binary trees are considered the same if they are structurally identical and the nodes have the same value.

**Example 1:**
```cpp
Input:     1         1
          / \       / \
         2   3     2   3

        [1,2,3],   [1,2,3]

Output: true
```

**Example 2:**
```cpp
Input:     1         1
          /           \
         2             2

        [1,2],     [1,null,2]

Output: false
```
**Example 3:**
```cpp
Input:     1         1
          / \       / \
         2   1     1   2

        [1,2,1],   [1,1,2]

Output: false
```

 题意：给定两个二叉树，编写一个函数来检验它们是否相同。如果两个树**在结构上相同**，并且节点**具有相同的值**，则认为它们是相同的。

---
思路1：DFS。很简单的递归函数。

代码如下：
```cpp
class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (p == nullptr && q) return false;
        if (q == nullptr && p) return false;
        if (p == nullptr && q == nullptr) return true;
        if (p->val == q->val) 
            return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
        else return false; 
    }
};
```

---
思路2：BFS，用一个队列同时存储两棵树上对应的结点。**这里需要存储空结点 `nullptr` ，原因是要保持两棵树之间结构的对应**。然后每次弹出两棵树的对应结点，如果都为空，就跳过；否则如果存在且值相等，就继续同时存储两棵树上面相同位置的指针；如果一个存在而另一个不存在，或者值不同，就直接返回 `false` 。

代码如下：
```cpp
class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        queue<TreeNode*> qu;
        qu.push(p);
        qu.push(q);
        while (!qu.empty()) { 
            p = qu.front(); qu.pop();
            q = qu.front(); qu.pop();
            if (!p && !q) continue; // 都为空, 则跳过后面的比较
            if (!p || !q) return false;
            if (p->val != q->val) return false;
            qu.push(p->left); // 同时存储两棵树上面相同位置的指针
            qu.push(q->left);
            qu.push(p->right);
            qu.push(q->right);
        }
        return true; 
    }
};
```
