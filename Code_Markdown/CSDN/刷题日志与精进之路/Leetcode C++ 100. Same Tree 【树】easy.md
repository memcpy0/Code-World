Given two binary trees, write a function to check if they are the same or not.

Two binary trees are considered the same if they are structurally identical and the nodes have the same value.

- Example 1:
	```
	Input:     1         1
	          / \       / \
	         2   3     2   3
	
	        [1,2,3],   [1,2,3]
	        
	Output: true
	```
- Example 2:
	```
	Input:     1         1
	          /           \
	         2             2
	
	        [1,2],     [1,null,2]
	
	Output: false
	```
- Example 3:
	```
	Input:     1         1
	          / \       / \
	         2   1     1   2
	
	        [1,2,1],   [1,1,2]
	
	Output: false
	```
题意：判断两棵二叉树是否相同，完全一致，不仅结构相同，对应结点的值也要一样。

方法一：递归求解(深度优先遍历)。递归模型很简单。
```
终止条件与返回值：
    当两棵树的当前节点都为 null 时返回 true 
    当其中一个为 null 另一个不为 null 时返回 false 
    当两个都不为空但是值不相等时，返回 false 
    当满足终止条件时进行返回，不满足时分别判断左子树和右子树是否相同 
```
时间复杂度：O(n)，n为树的节点个数 
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
方法二：从根开始，每次迭代将当前两棵树的结点从队列中弹出。然后，进行方法一中的判断：

    p 和 q 不是 None, 
    p.val 等于 q.val,

若以上均满足，则压入两棵树的左右子结点。 相当于对两棵树同时进行层次遍历。

执行用时 :0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗 :9.7 MB, 在所有 C++ 提交中击败了89.04%的用户
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

