Given an array where elements are sorted in ascending order, convert it to a height balanced BST.

For this problem, a height-balanced binary tree is defined as a binary tree in which **the depth of the two subtrees** of every node never differ by more than `1` .

**Example:**
```cpp
Given the sorted array: [-10,-3,0,5,9],

One possible answer is: [0,-3,9,-10,null,5], which represents the following height balanced BST:

      0
     / \
   -3   9
   /   /
 -10  5
```

题意：将一个升序有序数组转换为一棵高度平衡二叉搜索树。

---
思路1：以前做过的题目，当时第一反应是将这些值插入到AVL树中。于是有了下面的代码：
```cpp
//左旋函数
void L(TreeNode *&root) {  
    TreeNode *temp = root;
    root = root->right;
    temp->right = root->left;
    root->left = temp;
}
//右旋函数
void R(TreeNode *&root) { 
    TreeNode *temp = root;
    root = root->left;
    temp->left = root->right;
    root->right = temp;
}
```
然后是自底向上回溯时，递归求高度以及平衡因子的函数：
```cpp
int getHeight(TreeNode *root) {
    if (!root) return 0;  
    return max(getHeight(root->left), getHeight(root->right)) + 1; 
}
int getFactor(TreeNode *root) {
    return getHeight(root->left) - getHeight(root->right);
}
```
然后是插入到AVL树的函数：
```cpp
void insert(TreeNode *&root, int x) {
    if (!root) root = new TreeNode(x);  
    else {
        if (x < root->val) insert(root->left, x);
        else if (x > root->val) insert(root->right, x);
        else return;

        int fac1 = getFactor(root);
        if (fac1 >= 2) { 						//L
            int fac2 = getFactor(root->left);
            if (fac2 == 1) R(root);				//LL
            else if (fac2 == -1) {				//LR
                L(root->left);
                R(root);
            }
        } else if (fac1 <= -2) {				//R
            int fac3 = getFactor(root->right);
            if (fac3 == 1) {					//RL
                R(root->right);
                L(root);
            }
            else if (fac3 == -1) L(root);		//RR
        }
    }
}
```
> Update: 更新了一下AVL树的插入函数，在不实际存储高度信息的前提下，提高了一点效率，虽然和题目没有太大关系：
> ```cpp
> void insert(TreeNode *&root, int x) {
>    if (!root) root = new TreeNode(x);  
>    else {
>        if (x < root->val) insert(root->left, x);
>        else if (x > root->val) insert(root->right, x);
>        else return;
>
>        int fac1 = getFactor(root);
>        if (fac1 >= 2) { 
>            if (x < root->left->val) R(root);
>            else {
>                L(root->left);
>                R(root);
>            }
>        } else if (fac1 <= -2) {
>            if (root->right->val < x) L(root);
>            else {
>                R(root->right);
>                L(root);
>            }
>        }
>    }
> }
> ```

最后是题目给出的函数：
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
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        TreeNode *root = NULL;
        for (int i = 0; i < nums.size(); ++i) 
            insert(root, nums[i]); 
        return root;
    }
};
```
结果是过了，emmm……虽然耗时很长：
```
7 个月前	通过	1536 ms	20.7 MB	C++
7 个月前	通过	1688 ms	20.6 MB	C++
```
不过现在运行发现超时了，加强了数据吧……

---
思路2：利用二叉搜索树的中序性质，递归左右子树创建平衡二叉搜索树。这是此题的正解。

代码：
```cpp 
class Solution {
public: 
    void InOrderCreateTree(TreeNode*& root, vector<int>& nums, int lo, int hi) {
        if (lo > hi) return ;
        int mid = (lo + hi + 1) / 2;
        root = new TreeNode(nums[mid]);
        InOrderCreateTree(root->left, nums, lo, mid - 1);
        InOrderCreateTree(root->right, nums, mid + 1, hi);
    }

    TreeNode* sortedArrayToBST(vector<int>& nums) {
        TreeNode *root = NULL;
        InOrderCreateTree(root, nums, 0, nums.size() - 1);
        return root;
    }
};
```
效率：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了90.29% 的用户
内存消耗：21.5 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
