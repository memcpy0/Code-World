> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个有根节点 `root` 的二叉树，返回它 _最深的叶节点的最近公共祖先_ 。

回想一下：
- **叶节点** 是二叉树中没有子节点的节点
- 树的根节点的 **深度** 为 `0`，如果某一节点的深度为 `d`，那它的子节点的深度就是 `d+1`
- 如果我们假定 `A` 是一组节点 `S` 的 **最近公共祖先**，`S` 中的每个节点都在以 `A` 为根节点的子树中，且 `A` 的深度达到此条件下可能的最大值。

**示例 1：**

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/07/01/sketch1.png)

```js
输入：root = [3,5,1,6,2,0,8,null,null,7,4]
输出：[2,7,4]
解释：我们返回值为 2 的节点，在图中用黄色标记。
在图中用蓝色标记的是树的最深的节点。
注意，节点 6、0 和 8 也是叶节点，但是它们的深度是 2 ，而节点 7 和 4 的深度是 3 。
```
**示例 2：**
```js
输入：root = [1]
输出：[1]
解释：根节点是树中最深的节点，它是它本身的最近公共祖先。
```
**示例 3：**
```js
输入：root = [0,1,3,null,2]
输出：[2]
解释：树中最深的叶节点是 2 ，最近公共祖先是它自己。
```
**提示：**
- 树中的节点数将在 `[1, 1000]` 的范围内。
- `0 <= Node.val <= 1000`
- 每个节点的值都是 **独一无二** 的。

**注意：** 本题与力扣 865 重复：[https://leetcode-cn.com/problems/smallest-subtree-with-all-the-deepest-nodes/](https://leetcode-cn.com/problems/smallest-subtree-with-all-the-deepest-nodes/)

---

### 解法1 递归
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309072119109.png)
看上图（示例 1），这棵树的节点 $3,5,2$ 都是最深叶节点 $7,4$ 的公共祖先，但只有节点 $2$ 是最近的公共祖先。

如果我们要找的节点只在左子树中，那么最近公共祖先也必然只在左子树中。对于本题，**如果左子树的最大深度比右子树的大，那么最深叶结点就只在左子树中，所以最近公共祖先也只在左子树中**。反过来说，如果右子树的最大深度大于左子树，那么最深叶结点就只在右子树中，所以最近公共祖先也只在右子树中。

如果左右子树的最大深度一样呢？当前节点一定是最近公共祖先吗？不一定。比如节点 $1$ 的左右子树最深叶节点 $0,8$ 的深度都是 $2$ ，但该深度并不是**全局最大深度**，所以节点 $1$ 并不能是答案。

根据以上讨论，正确做法如下：
- 递归这棵二叉树，同时维护全局最大深度 $\textit{maxDepth}$ 。
- 在「**递**」的时候往下传 $depth$ ，用来表示**当前节点的深度**。
- 在「**归**」的时候往上传**当前子树最深叶节点的深度**。
- 设左子树最深叶节点的深度为 $\textit{leftMaxDepth}$ ，右子树最深叶节点的深度为  $\textit{rightMaxDepth}$ 。如果 $\textit{leftMaxDepth}=\textit{rightMaxDepth}=\textit{maxDepth}$ ，那么更新答案为当前节点。注意这并不代表我们找到了答案，如果后面发现了更深的叶节点，那么答案还会更新。

```cpp
class Solution {
public:
    TreeNode *lcaDeepestLeaves(TreeNode *root) {
        TreeNode *ans = nullptr;
        int max_depth = -1; // 全局最大深度
        function<int(TreeNode*, int)> dfs = [&](TreeNode *node, int depth) {
            if (node == nullptr) {
                max_depth = max(max_depth, depth); // 维护全局最大深度
                return depth;
            }
            int left_max_depth = dfs(node->left, depth + 1); // 获取左子树最深叶节点的深度
            int right_max_depth = dfs(node->right, depth + 1); // 获取右子树最深叶节点的深度
            if (left_max_depth == right_max_depth && left_max_depth == max_depth)
                ans = node;
            return max(left_max_depth, right_max_depth); // 当前子树最深叶节点的深度
        };
        dfs(root, 0);
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ 。每个节点都会恰好访问一次。
- 空间复杂度：$\mathcal{O}(n)$ 。最坏情况下，二叉树是一条链，递归需要 O(n)\mathcal{O}(n)O(n) 的栈空间。

---
### 解法2 自底向上
也可以不用全局变量，而是**把每棵子树都看成是一个「子问题」**，即对于每棵子树，我们需要知道：
- 这棵子树最深叶结点的深度。这里是指叶子在这棵子树内的深度，而不是在整棵二叉树的视角下的深度。相当于这棵子树的**高度**。
- 这棵子树的最深叶结点的最近公共祖先 $\textit{lca}$ 。

分类讨论：
- 设子树的根节点为 $node$，$node$ 的左子树的高度为 $\textit{leftHeight}$ ，$node$ 的右子树的高度为  $\textit{rightHeight}$ 。
- 如果 $leftHeight>rightHeight$ ，那么子树的高度为 $\textit{leftHeight} + 1$ ，$\textit{lca}$ 是左子树的 $\textit{lca}$ 。
- 如果 $\textit{leftHeight} < \textit{rightHeight}$ ，那么子树的高度为 $rightHeight+1$ ，$lca$  是右子树的 $lca$ 。
- 如果 $\textit{leftHeight} = \textit{rightHeight}$ ，那么子树的高度为 $\textit{leftHeight} + 1$ ，$lca$  就是 $node$ 。反证法：如果 $lca$  在左子树中，那么 $lca$  不是右子树的最深叶结点的祖先，这不对；如果 $lca$  在右子树中，那么 $lca$  不是左子树的最深叶结点的祖先，这也不对；如果 $lca$  在 $node$ 的上面，那就不符合「最近」的要求。所以 $lca$  只能是 $node$。
```cpp
class Solution {
    pair<int, TreeNode*> dfs(TreeNode *node) {
        if (node == nullptr)
            return {0, nullptr};
        auto [left_height, left_lca] = dfs(node->left);
        auto [right_height, right_lca] = dfs(node->right);
        if (left_height > right_height) // 左子树更高
            return {left_height + 1, left_lca};
        if (left_height < right_height) // 右子树更高
            return {right_height + 1, right_lca};
        return {left_height + 1, node}; // 一样高
    }

public:
    TreeNode *lcaDeepestLeaves(TreeNode *root) {
        return dfs(root).second;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ 。每个节点都会恰好访问一次。
- 空间复杂度：$\mathcal{O}(n)$ 。最坏情况下，二叉树是一条链，递归需要 $\mathcal{O}(n)$ 的栈空间。

更简洁的写法是：
```cpp
class Solution {
public:
    int depth[1010];
    TreeNode* lcaDeepestLeaves(TreeNode* root) {
        if (root == nullptr) return nullptr;
        TreeNode* left = root->left, *right = root->right;
        TreeNode* lcaLeft = lcaDeepestLeaves(root->left), *lcaRight = lcaDeepestLeaves(root->right);
        int dl = left ? depth[left->val] : 0, dr = right ? depth[right->val] : 0;
        depth[root->val] = max(dl, dr) + 1;
        if (dl > dr) return lcaLeft;
        if (dr > dl) return lcaRight;
        return root;
    }
};
```