> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一棵以 `root` 为根的 **二叉树** ，请你返回 **任意** 二叉搜索子树的最大键值和。

二叉搜索树的定义如下：
-   任意节点的左子树中的键值都 **小于** 此节点的键值。
-   任意节点的右子树中的键值都 **大于** 此节点的键值。
-   任意节点的左子树和右子树都是二叉搜索树。

**示例 1：**
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/03/07/sample_1_1709.png)

```java
输入：root = [1,4,3,2,4,2,5,null,null,null,null,null,null,4,6]
输出：20
解释：键值为 3 的子树是和最大的二叉搜索树。
```
**示例 2：**
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2020/03/07/sample_2_1709.png)
```java
输入：root = [4,3,null,1,2]
输出：2
解释：键值为 2 的单节点子树是和最大的二叉搜索树。
```
**示例 3：**
```java
输入：root = [-4,-2,-5]
输出：0
解释：所有节点键值都为负数，和最大的二叉搜索树为空。
```
**示例 4：**
```java
输入：root = [2,1,3]
输出：6
```
**示例 5：**
```java
输入：root = [5,4,8,3,null,6,3]
输出：7
```
**提示：**
-   每棵树有 `1` 到 `40000` 个节点。
-   每个节点的键值在 `[-4 * 10^4 , 4 * 10^4]` 之间。

---
### 解法 DFS+二叉搜索树判断
这题名为困难，实际上就是一道简单题。只能说3年前和现在的内卷程度天差地别……

当前节点为根的树是不是二叉搜索树和几个状态有关：
- 左子树是不是二叉搜索树
- 右子树是不是二叉搜索树
- 当前 `val` 是不是大于左子树最大 `val`
- 当前 `val` 是不是小于右子树最小 `val`

我们确定 `root` 节点为根的树是不是二叉搜索树，需要其左右子树处理时返回四个值
- 子树是不是二叉搜索树 `vec[0]`
- 子树的最小值 `vec[1]`
- 子树的最大值 `vec[2]`
- 子树的 `sum` 值 `vec[3]`

根据左右子节点返回值，构造当前节点的返回：
- 当左右子树的任一 `vec[0]` 为 `false` ，或者当前 `val <= 左子vec[2]` 或者 `val >= 右子vec[1]` 时返回 `{false, 0, 0, 0}`
- 如果判断当前树是搜索树，则返回 `{true, 左子v[1], 右子v[2], val + 左子v[3] + 右子v[3]}` 
- 另外注意 `null` 的处理，我这里返回了 `{true, INT_MAX, INT_MIN, 0}` 。

```cpp
class Solution {
public:
    int maxsum = 0;
    int maxSumBST(TreeNode* root) {
        dfs(root);
        return maxsum;
    }
    vector<int> dfs(TreeNode* root) {
        if (!root) return {true, INT_MAX, INT_MIN, 0};
        auto lArr = dfs(root->left);
        auto rArr = dfs(root->right);
        int sum = 0, curmax, curmin;
        if (!lArr[0] || !rArr[0] || root->val >= rArr[1] || root->val <= lArr[2]) 
            return {false, 0, 0, 0};
        curmin = root->left ? lArr[1] : root->val;
        curmax = root->right ? rArr[2] : root->val;
        sum += (root->val + lArr[3] + rArr[3]);
        maxsum = max(maxsum, sum);
        return {true, curmin, curmax, sum};
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，每个节点只需遍历一次，遍历一次的复杂为 $O(1)$
- 空间复杂度：$O(n)$ ，递归过程中占用的栈空间和返回的 `vector` 所需的空间，在二叉树退化成链的情况下为 $O(n)$ 。
