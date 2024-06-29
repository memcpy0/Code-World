

输入一棵二叉树和一个整数，打印出二叉树中节点值的和为输入整数的所有路径。从树的根节点开始往下一直到叶节点所经过的节点形成一条路径。

示例：给定如下二叉树，以及目标和 `sum = 22` ，

```go
      5
     / \
    4   8
   /   / \
  11  13  4
 /  \    / \
7    2  5   1
```

返回:

```go
[
   [5,4,11,2],
   [5,8,4,5]
]
```
提示：`节点总数 <= 10000`

---
### 解法 DFS
```cpp
class Solution {
private:
    vector<vector<int>> ans;
    vector<int> temp;
public:
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        if (root == nullptr) return {};
        temp.push_back(root->val);
        if (root->val == sum && !root->left && !root->right) ans.push_back(temp);
        else {
            pathSum(root->left, sum - root->val);
            pathSum(root->right, sum - root->val);
        }
        temp.pop_back();
        return ans;
    }
};
```
效率之低，难以想象：
```cpp
执行用时：440 ms, 在所有 C++ 提交中击败了6.95% 的用户
内存消耗：136.2 MB, 在所有 C++ 提交中击败了5.02% 的用户
```
突然想到，因为是直接在原始函数上进行递归，每次都要返回 `ans` ，可能导致大量的时空开销。因此下述代码使用一个独立的递归函数：
```cpp
class Solution {
private:
    vector<vector<int>> ans;
    vector<int> temp;
    void dfs(TreeNode *root, int sum)
    {
        if(!root) return;
        temp.push_back(root->val);
        sum -= root->val;
        if(sum == 0 && !root->left && !root->right) ans.push_back(temp);
        dfs(root -> left, sum);  //左
        dfs(root -> right, sum); //右
        temp.pop_back(); //函数退出之前先弹出当前节点
    }
public:
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        dfs(root, sum);
        return ans;
    }
}; 
```
瞬间好起来了：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了86.13% 的用户
内存消耗：19.8 MB, 在所有 C++ 提交中击败了49.46% 的用户
```
