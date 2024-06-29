Given a binary tree, design an algorithm which creates a linked list of all the nodes at each depth (e.g., if you have a tree with depth `D`, you'll have `D` linked lists). Return a array containing all the linked lists.

 

**Example:**

```swift
Input: [1,2,3,4,5,null,7,8]

        1
       /  \ 
      2    3
     / \    \ 
    4   5    7
   /
  8

Output: [[1],[2,3],[4,5,7],[8]]
```

题意：给定一棵二叉树，对每一深度上的所有节点分别创建链表，返回一个包含所有深度的链表的数组。


---
### 解法 BFS
```cpp
class Solution {
public:
    vector<ListNode*> listOfDepth(TreeNode* tree) {
        if (tree == nullptr) return {};
        vector<ListNode*> ans;
        queue<TreeNode*> q;
        q.push(tree);
        while (!q.empty()) {
            int size = q.size();
            ListNode dummy(0), *rear = &dummy; //虚拟头结点+尾插法
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                rear->next = new ListNode(t->val);
                rear = rear->next;
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
            ans.push_back(dummy.next);
        }
        return ans;
    }
};
```
执行结果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了68.22% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了29.59% 的用户
```
