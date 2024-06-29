> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给出二叉树的根节点 `root`，树上每个节点都有一个不同的值。

如果节点值在 `to_delete` 中出现，我们就把该节点从树上删去，最后得到一个森林（一些不相交的树构成的集合）。

返回森林中的每棵树。你可以按任意顺序组织答案。

**示例 1：**
**![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/07/05/screen-shot-2019-07-01-at-53836-pm.png)**
```java
输入：root = [1,2,3,4,5,6,7], to_delete = [3,5]
输出：[[1,2,null,4],[6],[7]]
```
**示例 2：**
```java
输入：root = [1,2,4,null,3], to_delete = [3]
输出：[[1,2,4]]
```
**提示：**
-   树中的节点数最大为 `1000`。
-   每个节点都有一个介于 `1` 到 `1000` 之间的值，且各不相同。
-   `to_delete.length <= 1000`
-   `to_delete` 包含一些从 `1` 到 `1000`、各不相同的值。

---
思考以下问题：
- 如何快速判断节点值是否在 $\textit{toDelete}$ 中？
- 如果当前节点被删除，但是左儿子（右儿子）没被删除，意味着什么？

其实很简单：
- 把 $toDelete$ 全部丢到一个哈希表 $s$ 或位图 $bst$ 中，这样可以 $O(1)$ 判断节点值是否在 $\textit{toDelete}$ 中。
- **如果当前节点被删除，但左儿子（右儿子）没被删除，意味着左儿子（右儿子）是一棵树的根节点，加入答案**。
### 解法1 后序遍历
写一个 DFS（后序遍历）：
- 更新左儿子（右儿子）为**递归**左儿子（右儿子）的返回值。
- 如果当前节点被删除，那么就检查左儿子（右儿子）是否被删除，**如果没被删除，就加入答案**。
- 如果当前节点被删除，返回空节点，否则返回当前节点。
- 最后，如果根节点没被删除，把根节点加入答案。

```cpp
class Solution {
private:
    vector<TreeNode*> ans;
    bitset<1010> bst;
    TreeNode* dfs(TreeNode* root) {
        if (root == nullptr) return nullptr;
        root->left = dfs(root->left);
        root->right = dfs(root->right);
        if (!bst[root->val]) return root;
        if (root->left) ans.push_back(root->left);
        if (root->right) ans.push_back(root->right);
        return nullptr;
    }
public:
    vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
        for (int del : to_delete) bst[del] = 1;
        root = dfs(root);
        if (root) ans.push_back(root);
        return ans;
    }
};
```
---
### 解法2 先序遍历
上述解法是先递去，并在归来后解决问题。这里使用先序遍历，在「递」的过程中获取答案，因此可将 $root$ 的处理合并到递归函数中。

做法是：设置一个额外的 `bool` 参数 $isTree$ 。对一个子树的根结点来说，如果它在要删除的结点之列， 递归左右儿子时 $isTree$ 就为 $true$ ，如果**它的左右儿子（存在且）不在删除之列，那么就是新树的根**。
```cpp
class Solution { 
public:
    vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
        vector<TreeNode*> ans;
        bitset<1010> bst;
        for (int del : to_delete) bst[del] = 1;
        function<TreeNode*(TreeNode*, bool)> dfs = [&](TreeNode* root, bool isTree) -> TreeNode* {
            if (root == nullptr) return nullptr; 
            bool flag = bst[root->val];
            if (isTree && !flag) ans.push_back(root);
            root->left = dfs(root->left, flag);
            root->right = dfs(root->right, flag);
            return flag ? nullptr : root;
        };
        dfs(root, true); 
        return ans;
    }
};
``` 
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$