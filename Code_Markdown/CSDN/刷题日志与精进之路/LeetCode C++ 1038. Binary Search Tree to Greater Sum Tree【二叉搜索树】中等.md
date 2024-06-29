Given the root of a binary **search** tree with distinct values, modify it so that every node has a new value equal to the sum of the values of the original tree that are greater than or equal to `node.val` .

As a reminder, a *binary search tree* is a tree that satisfies these constraints:
-    The left subtree of a node contains only nodes with keys **less than** the node's key.
 -   The right subtree of a node contains only nodes with keys **greater than** the node's key.
 -   Both the left and right subtrees must also be binary search trees.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200922011910668.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```cpp
Input: [4,1,6,0,2,5,7,null,null,null,3,null,null,null,8]
Output: [30,36,21,36,35,26,15,null,null,null,33,null,null,null,8]
```

**Constraints:**
-    The number of nodes in the tree is between `1` and `100` .
 -   Each node will have value between `0` and `100` .
 -   The given tree is a binary search tree.


题意：给出二叉搜索树的根节点，该二叉树的节点值各不相同，修改二叉树，使每个节点 `node` 的新值等于原树中大于或等于 `node.val` 的值之和。

---
思路1：我们需要以有序的形式从大到小遍历二叉搜索树，逐渐累加结点的值，赋给当前结点。下面的代码是递归中序遍历（左根右）的变形（右根左）：
```cpp
class Solution {
public:
    int sum = 0;
    TreeNode* bstToGst(TreeNode* root) {
        if (root) {
            bstToGst(root->right);
            sum += root->val;
            root->val = sum;
            bstToGst(root->left);
        }
        return root;
    }  
};
```
---
思路2：非递归中序遍历。代码如下：
```cpp
class Solution {
public: 
    TreeNode* bstToGst(TreeNode* root) {
        stack<TreeNode*> st;
        TreeNode *temp = root;
        int sum = 0;
        while (!st.empty() || temp) {
            while (temp) {
                st.push(temp);
                temp = temp->right;
            }
            temp = st.top(); st.pop();
            sum += temp->val;
            temp->val = sum;
            temp = temp->left;
        }
        return root;
    }
};
```
---
思路3：非递归中序遍历，使用 `stack` 模拟系统栈的函数调用：
```cpp
class Solution {
public: 
    struct command {
        int instruction; //为0时下次访问结点,为1时下次遍历该结点代表的子树(相当于递归函数调用访问子树)
        TreeNode *root;
        command(int i = 0, TreeNode *rt = nullptr) : instruction(i), root(rt) { }
    };
    TreeNode* bstToGst(TreeNode* root) {
        if (root == nullptr) return root;
        stack<command> st;
        st.push(command(1, root));
        int sum = 0;
        while (!st.empty()) {
            const command temp = st.top(); st.pop();
            if (temp.instruction == 0) { //访问结点
                sum += temp.root->val;
                temp.root->val = sum;
            } else { //访问结点代表的子树（右-根-左）
                TreeNode *t = temp.root;
                if (t->left) st.push(command(1, t->left));
                st.push(command(0, t));
                if (t->right) st.push(command(1, t->right));
            }
        }
        return root;
    }
};
```
