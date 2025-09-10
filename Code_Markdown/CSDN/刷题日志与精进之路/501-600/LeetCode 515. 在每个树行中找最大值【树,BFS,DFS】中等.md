> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一棵二叉树的根节点 `root` ，请找出该二叉树中每一层的最大值。

**示例1：**

![](https://assets.leetcode.com/uploads/2020/08/21/largest_e1.jpg)
```js
输入: root = [1,3,2,5,3,null,9]
输出: [1,3,9]
```
**示例2：**
```js
输入: root = [1,2,3]
输出: [1,3]
```
**提示：**
- 二叉树的节点个数的范围是 `[0,10^4]`
- `-2^31 <= Node.val <= 2^31 - 1`

---
### 解法1 深度优先搜索
我们用树的「先序遍历」来进行「深度优先搜索」处理，并用 $\textit{curHeight}$ 来标记遍历到的当前节点的高度。当遍历到 $\textit{curHeight}$ 高度的节点就判断是否更新该层节点的最大值。
```cpp
class Solution {
public:
    void dfs(vector<int>& res, TreeNode* root, int curHeight) {
        if (curHeight == res.size()) {
            res.push_back(root->val);
        } else {
            res[curHeight] = max(res[curHeight], root->val);
        }
        if (root->left) dfs(res, root->left, curHeight + 1);
        if (root->right) dfs(res, root->right, curHeight + 1);
    }
    vector<int> largestValues(TreeNode* root) {
        if (!root) {
            return {};
        }
        vector<int> res;
        dfs(res, root, 0);
        return res;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为二叉树节点个数。二叉树的遍历中每个节点会被访问一次且只会被访问一次。
- 空间复杂度：$O(\textit{height})$ 。其中 $\textit{height}$ 表示二叉树的高度。递归函数需要栈空间，而栈空间取决于递归的深度，因此空间复杂度等价于二叉树的高度。

---
### 解法2 广度优先搜索
也可以用「广度优先搜索」的方法来解决这道题目。「广度优先搜索」中的队列里存放的是「当前层的所有节点」。每次拓展下一层的时候，不同于「广度优先搜索」的每次只从队列里拿出一个节点，==我们把当前队列中的全部节点拿出来进行拓展，这样能保证每次拓展完时队列里存放的是下一层的所有节点==，即我们是一层一层地进行拓展，然后每一层我们用 $\textit{maxVal}$ 来标记该层节点的最大值。当该层全部节点都处理完后，$\textit{maxVal}$ 就是该层全部节点中的最大值。

```cpp
class Solution {
public:
    vector<int> largestValues(TreeNode* root) {
        if (root == nullptr) return {};
        vector<int> ans;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size(), maxval = INT_MIN;
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                if (t->val > maxval) maxval = t->val;
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
            ans.push_back(maxval);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为二叉树节点个数，每一个节点仅会进出队列一次。
- 空间复杂度：$O(n)$ ，存储二叉树节点的空间开销。