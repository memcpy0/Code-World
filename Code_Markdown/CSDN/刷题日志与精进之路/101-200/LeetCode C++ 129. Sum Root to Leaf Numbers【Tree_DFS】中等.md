Given a binary tree containing digits from `0-9` only, each root-to-leaf path could represent a number.

An example is the root-to-leaf path `1->2->3` which represents the number `123` . Find the total sum of all root-to-leaf numbers.

**Note**: A leaf is a node with no children.

**Example:**

```swift
Input: [1,2,3]
    1
   / \
  2   3
Output: 25
Explanation:
The root-to-leaf path 1->2 represents the number 12.
The root-to-leaf path 1->3 represents the number 13.
Therefore, sum = 12 + 13 = 25.
```

**Example 2:**
```swift
Input: [4,9,0,5,1]
    4
   / \
  9   0
 / \
5   1
Output: 1026
Explanation:
The root-to-leaf path 4->9->5 represents the number 495.
The root-to-leaf path 4->9->1 represents the number 491.
The root-to-leaf path 4->0 represents the number 40.
Therefore, sum = 495 + 491 + 40 = 1026.
```
题意：给定一个二叉树，它的每个结点都存放一个 `0-9` 的数字，每条从根到叶子节点的路径都代表一个数字。例如，从根到叶子节点路径 `1->2->3` 代表数字 `123` 。计算从根到叶子节点生成的所有数字之和。

---
### 思路1 递归先序搜索
先序遍历，计算和即可。代码如下：
```cpp
class Solution {
public: 
    int getAllNumbers(TreeNode* root, int num) {
        if (!root) return 0;
        if (!root->left && !root->right) return num * 10 + root->val;
        int val = num * 10 + root->val;
        return getAllNumbers(root->left, val) + getAllNumbers(root->right, val);
    }
    int sumNumbers(TreeNode* root) {
        return getAllNumbers(root, 0);
    }
};
```
 效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：12.2 MB, 在所有 C++ 提交中击败了34.94% 的用户
```
**20201029 Update** 如果使用原函数的话，可以写成如下形式：
```cpp
class Solution {
public:
    int sum = 0, num = 0;
    int sumNumbers(TreeNode* root) {
        if (root == nullptr) return 0;
        num = num * 10 + root->val;
        if (root->left == nullptr && root->right == nullptr) sum += num;
        sumNumbers(root->left);
        sumNumbers(root->right);
        num /= 10; //回溯到上一层
        return sum;
    }
};
```
效率不是很好：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了35.13% 的用户
内存消耗：12.4 MB, 在所有 C++ 提交中击败了28.99% 的用户
```

---
### 思路2 非递归先序遍历
实际代码如下：
```cpp
class Solution {
public: 
    int sumNumbers(TreeNode* root) {
        stack<TreeNode*> st;
        int sum = 0;
        TreeNode *temp = root;
        while (temp || !st.empty()) {
            while (temp) {
                st.push(temp);
                if (temp->left) temp->left->val += temp->val * 10; //修改原树上的值
                temp = temp->left;
            }
            temp = st.top(); st.pop();
            if (!temp->left && !temp->right) sum += temp->val; //遇到叶子节点
            if (temp->right) temp->right->val += temp->val * 10; //修改原树上的值
            temp = temp->right;
        }
        return sum;
    }
};
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了34.87% 的用户
内存消耗：12.2 MB, 在所有 C++ 提交中击败了36.07% 的用户
```
