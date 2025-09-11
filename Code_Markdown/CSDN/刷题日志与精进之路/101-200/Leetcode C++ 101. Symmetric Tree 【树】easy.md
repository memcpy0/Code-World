Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).

For example, this binary tree [1,2,2,3,4,4,3] is symmetric:
```
    1
   / \
  2   2
 / \ / \
3  4 4  3
```
But the following [1,2,2,null,3,null,3] is not:
```
    1
   / \
  2   2
   \   \
   3    3
```
Note:
		Bonus points if you could solve it both recursively and iteratively.
 
 
 题意：判断一棵树是否是对称的。
 思路：和100题. Same Tree是相同的，改一下代码就可以了。

 方法一：
 ```cpp
/*
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
    bool isSymmetric(TreeNode* root) {
        return isMirror(root, root);
        // stack<TreeNode*> st;
        // vector<int> vi;
        // TreeNode *p = root;
        // while (!st.empty() || p) {
        //     while (p) {
        //         st.push(p);
        //         p = p->left;
        //     }
        //     p = st.top(); 
        //     if (!p->left && p->right) vi.push_back(-1);
        //     vi.push_back(p->val);
        //     if (p->left && !p->right) vi.push_back(-1); 
        //     st.pop();
        //     p = p->right;
        // }
        // for (int left = 0, right = vi.size() - 1; left <= right; left++, right--) {
        //     if (vi[left] != vi[right]) return false;
        // }
        // return true;
    }
    bool isMirror(TreeNode *r1, TreeNode *r2) {
        if (r1 == nullptr && r2 == nullptr) return true;
        if (r1 == nullptr || r2 == nullptr) return false;
        else {
            if (r1->val != r2->val) return false;
            else return isMirror(r1->left, r2->right) && isMirror(r1->right, r2->left);
        }
    }
};
```
 方法二：迭代(BFS/层次遍历)。
 ```cpp
         TreeNode *p = root, *q = root;
        queue<TreeNode*> qLeft, qRight;
        qLeft.push(p);
        qRight.push(q);
        while (!qLeft.empty() || p || !qRight.empty() || q) { 
            p = qLeft.front(); qLeft.pop();
            q = qRight.front(); qRight.pop();
            if (!p && !q) continue;
            if (!p || !q) return false;
            if (p->val != q->val) return false;
            qLeft.push(p->left);
            qLeft.push(p->right);
            qRight.push(q->right);
            qRight.push(q->left);
        }
        return true;
  ```


