You are given the `root` of a binary tree where each node has a value `0` or `1`.  Each root-to-leaf path represents a binary number starting with the most significant bit.  For example, if the path is `0 -> 1 -> 1 -> 0 -> 1`, then this could represent `01101` in binary, which is `13`.

For all leaves in the tree, consider the numbers represented by the path from the root to that leaf.

*Return the sum of these numbers*. The answer is **guaranteed** to fit in a **32-bits** integer.

 

**Example 1:**

```swift
Input: root = [1,0,1,0,1,0,1]
Output: 22
Explanation: (100) + (101) + (110) + (111) = 4 + 5 + 6 + 7 = 22
```

**Example 2:**

```swift
Input: root = [0]
Output: 0
```

**Example 3:**

```swift
Input: root = [1]
Output: 1
```

**Example 4:**

```swift
Input: root = [1,1]
Output: 3
```

 

**Constraints:**
- The number of nodes in the tree is in the range `[1, 1000]` .
- Node.val is `0` or `1` .

题意：给出一棵二叉树，其上每个结点的值都是 `0` 或 `1` 。每一条**从根到叶的路径**都代表一个从最高有效位开始的二进制数。对树上的每一片叶子，都要找出**从根到该叶子的路径**所表示的数字，并返回这些数字之和。

---
### 解法 递归
很简单的题目，就是需要注意空节点和叶子节点的处理，避免重复计算数值：
```cpp
class Solution {
private:
    long long ans = 0;
    void getAllPaths(TreeNode* root, int number) {
        if (!root) return;
        else if (root->left == nullptr && root->right == nullptr) //在叶子结点产生新的值
            ans += (number * 2 + root->val);
        else {
            getAllPaths(root->left, number * 2 + root->val);
            getAllPaths(root->right, number * 2 + root->val);
        }
    }
public:
    int sumRootToLeaf(TreeNode* root) {
        if (root == nullptr) return 0;
        getAllPaths(root, 0);
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了53.87% 的用户
内存消耗：16.7 MB, 在所有 C++ 提交中击败了16.61% 的用户
```
