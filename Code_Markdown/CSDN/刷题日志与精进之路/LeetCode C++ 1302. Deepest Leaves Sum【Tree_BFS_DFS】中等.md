
Given a binary tree, return the sum of values of its deepest leaves.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201127011232397.png#pic_center)

```swift
Input: root = [1,2,3,4,5,null,6,7,null,null,null,null,8]
Output: 15
```

**Constraints:**
- The number of nodes in the tree is between `1` and `10^4`.
- The value of nodes is between `1` and `100`.


题意：给定一棵二叉树，返回层数最深的叶子节点的和。

---
### 解法1 BFS 
计算每一层中可能的叶子节点的和，最后一层即为所求结果：
```cpp
class Solution {
public:
    int deepestLeavesSum(TreeNode* root) {
        if (root == nullptr) return 0;
        int ans = 0;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size(), leavesSum = 0;
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop(); //其实不必额外判断是否是叶子节点; 最后一层无疑都是叶子节点
                if (!t->left && !t->right) { leavesSum += t->val; continue; } 
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
            ans = leavesSum;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了90.93% 的用户
内存消耗：38.8 MB, 在所有 C++ 提交中击败了14.18% 的用户
```
---
### 解法2 DFS
不断更新最大层数，同时记录最大层对应的叶子节点值之和：
```cpp
class Solution {
private:
    int sum = 0, maxLevel = 0;
    void dfs(TreeNode* root, int level) {
        if (root == nullptr) return;
        if (level > maxLevel) {
            maxLevel = level;
            sum = root->val;
        } else if (level == maxLevel) sum += root->val;
        dfs(root->left, level + 1);
        dfs(root->right, level + 1);
    } 
public:
    int deepestLeavesSum(TreeNode* root) { 
        dfs(root, 0);
        return sum;
    }
};
```
执行结果如下：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了90.93% 的用户
内存消耗：38.1 MB, 在所有 C++ 提交中击败了34.94% 的用户
```
