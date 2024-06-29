

Given a sorted (increasing order) array with unique integer elements, write an algo­rithm to create a binary search tree with **minimal height**.

**Example:**

```clike
Given sorted array: [-10,-3,0,5,9],

One possible answer is: [0,-3,9,-10,null,5]，which represents the following tree: 

          0 
         / \ 
       -3   9 
       /   / 
     -10  5 
```
题意：给定一个有序整数数组，元素各不相同且按升序排列，编写算法创建一棵**高度最小**的**二叉搜索树**。

---
> 尽管本题和[310. Minimum Height Trees](https://leetcode-cn.com/problems/minimum-height-trees/)题目名称相似，但是难度可是大不相同!
> <b></b>
> 顺道一提，本题实际上就是**替罪羊树拍扁重建的过程**。

### 解法1 递归先序建树
高度最小的二叉搜索树，显而易见是一棵**平衡的二叉搜索树**，位于树根的左侧和右侧的节点数量大致相同。如果我们不断将元素插入二叉搜索树中，然后进行AVL调整……时间复杂度绝对爆炸，而且也没有充分利用升序数组这一条件。

正确的做法是，不断递归先序建立二叉搜索树：
- 易知根节点值为 `nums[mid]` ，建立根节点；
- 然后分别递归 `nums[0 : mid)` 和 `nums[mid + 1 : )` ，建立左右子树；
- 这样左右子树的节点数量大致相同，而且也符合二叉搜索树的有序性质。

```cpp
class Solution {
public:
    void preorderBuild(const vector<int>& nums, int left, int right, TreeNode *&root) {
        if (left <= right) {
            int mid = left + (right - left) / 2;
            root = new TreeNode(nums[mid]);
            preorderBuild(nums, left, mid - 1, root->left);
            preorderBuild(nums, mid + 1, right, root->right);
        }
    }
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        TreeNode *root = nullptr;
        if (nums.empty()) return root;
        int n = nums.size();
        preorderBuild(nums, 0, n - 1, root);
        return root;
    }
};
```
提交后效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了98.84% 的用户
内存消耗：25.4 MB, 在所有 C++ 提交中击败了24.19% 的用户
```
---
### 解法2 非递归先序建树
代码写得稀烂，用到了二级指针，建议看看就好：
```cpp
 */
class Solution {
private:
    struct node { int l, r; TreeNode **ptr; }; //指针的指针,修改指针
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        TreeNode *root = nullptr;
        if (nums.empty()) return root;
        node temp{0, static_cast<int>(nums.size() - 1), &root};
        queue<node> q; 
        while (temp.l <= temp.r || !q.empty()) {
            while (temp.l <= temp.r) {
                int mid = temp.l + (temp.r - temp.l) / 2;
                *temp.ptr = new TreeNode(nums[mid]);
                q.push(temp);
                temp = node{temp.l, mid - 1, &((*temp.ptr)->left)};
            }
            temp = q.front(); q.pop();
            int mid = temp.l + (temp.r - temp.l) / 2;
            temp = node{mid + 1, temp.r, &((*temp.ptr)->right)};
        }
        return root;
    }
};
```
执行结果还行，空间效率倒是大幅提高：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了84.16% 的用户
内存消耗：24 MB, 在所有 C++ 提交中击败了84.19% 的用户
```
