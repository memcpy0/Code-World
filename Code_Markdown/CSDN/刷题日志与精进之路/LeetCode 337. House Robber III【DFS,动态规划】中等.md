> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为 `root` 。

除了 `root` 之外，每栋房子有且只有一个“父“房子与之相连。一番侦察之后，聪明的小偷意识到“这个地方的所有房屋的排列类似于一棵二叉树”。 如果 **两个直接相连的房子在同一天晚上被打劫** ，房屋将自动报警。

给定二叉树的 `root` 。返回 _**在不触动警报的情况下** ，小偷能够盗取的最高金额_ 。

**示例 1:**
![](https://assets.leetcode.com/uploads/2021/03/10/rob1-tree.jpg)
```js
输入: root = [3,2,3,null,3,null,1]
输出: 7 
解释: 小偷一晚能够盗取的最高金额 3 + 3 + 1 = 7
```
**示例 2:**
![](https://assets.leetcode.com/uploads/2021/03/10/rob2-tree.jpg)
```js
输入: root = [3,4,5,1,3,null,1]
输出: 9
解释: 小偷一晚能够盗取的最高金额 4 + 5 = 9
```
**提示：**
- 树的节点数在 `[1, 10^4]` 范围内
- `0 <= Node.val <= 10^4`

---
### 解法 DFS+记忆化
简化一下这个问题：一棵二叉树，树上的每个点都有对应的权值，每个点有两种状态（选中和不选中），问在**不能同时选中有父子关系的点**的情况下，能选中的点的最大权值和是多少。

我们可以用 $f(o)$ 表示选择 $o$ 节点的情况下，$o$ 节点的子树上被选择的节点的最大权值和；$g(o)$ 表示不选择 $o$ 节点的情况下，$o$ 节点的子树上被选择的节点的最大权值和；$l$ 和 $r$ 代表 $o$ 的左右孩子。
- 当 $o$ 被选中时，$o$ 的左右孩子都不能被选中，故 **$o$ 被选中情况下子树上被选中点的最大权值和**为 **$l$ 和 $r$ 不被选中的最大权值和相加**，即 $f(o)=g(l)+g(r)$ 。
- **当 $o$ 不被选中时，$o$ 的左右孩子可以被选中，也可以不被选中**。对于 $o$ 的某个具体的孩子 $x$ ，它对 $o$ 的贡献是 $x$ 被选中和不被选中情况下权值和的较大值。故 $g(o) = \max \{ f(l) , g(l)\}+\max\{ f(r) , g(r) \}$ 。

至此，我们可以用哈希表来存 $f$ 和 $g$ 的函数值，用深度优先搜索的办法后序遍历这棵二叉树，我们就可以得到每一个节点的 $f$ 和 $g$ 。根节点的 $f$ 和 $g$ 的最大值就是我们要找的答案。
```cpp
class Solution {
public:
    unordered_map <TreeNode*, int> f, g;
    void dfs(TreeNode* node) {
        if (!node) return;
        dfs(node->left);
        dfs(node->right);
        f[node] = node->val + g[node->left] + g[node->right];
        g[node] = max(f[node->left], g[node->left]) + max(f[node->right], g[node->right]);
    }
    int rob(TreeNode* root) {
        dfs(root);
        return max(f[root], g[root]);
    }
};
```
假设二叉树的节点个数为 $n$ 。我们可以看出，以上的算法对二叉树做了一次后序遍历，时间复杂度是 $O(n)$；由于递归会使用到栈空间，空间代价是 $O(n)$，哈希表的空间代价也是 $O(n)$，故空间复杂度也是 $O(n)$。

可以做一个小小的优化，我们发现无论是 $f(o)$ 还是 $g(o)$ ，他们最终的值只和 $f(l)$ 、$g(l)$ 、$f(r)$ 、$g(r)$ 有关，所以**对于每个节点，我们只关心它的孩子节点们的 $f$ 和 $g$ 是多少**。我们可以设计一个结构，表示某个节点的 $f$ 和 $g$ 值，在每次递归返回时，都把这个点对应的 $f$ 和 $g$ 返回给上一级调用，这样可以省去哈希表的空间。
```cpp
struct SubtreeStatus {
    int selected;
    int notSelected;
};
class Solution {
public:
    SubtreeStatus dfs(TreeNode* node) {
        if (!node) {
            return {0, 0};
        }
        auto l = dfs(node->left);
        auto r = dfs(node->right);
        int selected = node->val + l.notSelected + r.notSelected;
        int notSelected = max(l.selected, l.notSelected) + max(r.selected, r.notSelected);
        return {selected, notSelected};
    }
    int rob(TreeNode* root) {
        auto rootStatus = dfs(root);
        return max(rootStatus.selected, rootStatus.notSelected);
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 。上文中已分析。
- 空间复杂度：$O(n)$ 。虽然优化过的版本省去了哈希表的空间，但是栈空间的使用代价依旧是 $O(n)$，故空间复杂度不变。

```cpp
class Solution {
public: //尝试对以root为根节点的二叉树进行偷取,得到的最大金钱
    unordered_map<TreeNode*, int> sums;
    int rob(TreeNode* root) {
        if (root == nullptr) return 0; //没有可偷窃的地方
        if (sums[root]) return sums[root]; //对该节点为根的子树进行偷窃,得到的值已经确定
        int robVal = root->val; //偷窃当前根节点的金钱
        if (root->left) //存在左子树时
            robVal += rob(root->left->left) + rob(root->left->right); //只能偷窃子树的子树
        if (root->right) //存在右子树时
            robVal += rob(root->right->left) + rob(root->right->right); //只能偷窃子树的子树
        int notRob = rob(root->left) + rob(root->right); //不偷窃该节点
        sums[root] = max(robVal, notRob);
        return max(robVal, notRob);
    }
};
```