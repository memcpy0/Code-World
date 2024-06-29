> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个含重复值的二叉搜索树（BST）的根节点 `root` ，找出并返回 BST 中的所有 [众数](https://baike.baidu.com/item/%E4%BC%97%E6%95%B0/44796)（即，出现频率最高的元素）。

如果树中有不止一个众数，可以按 **任意顺序** 返回。

假定 BST 满足如下定义：
- 结点左子树中所含节点的值 **小于等于** 当前节点的值
- 结点右子树中所含节点的值 **大于等于** 当前节点的值
- 左子树和右子树都是二叉搜索树

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/03/11/mode-tree.jpg)
```js
输入：root = [1,null,2,2]
输出：[2]
```
**示例 2：**
```js
输入：root = [0]
输出：[0]
```
**提示：**
- 树中节点的数目在范围 `[1, 10^4]` 内
- `-10^5 <= Node.val <= 10^5`

**进阶：** 你可以不使用额外的空间吗？（假设由递归产生的隐式调用栈的开销不被计算在内）

---
首先一定能想到一个最朴素的做法：因为这棵树的中序遍历是一个有序的序列，所以可以先获得这棵树的中序遍历，然后从扫描这个中序遍历序列，然后用一个哈希表来统计每个数字出现的个数，这样就可以找到出现次数最多的数字。但是这样做的空间复杂度显然不是 $O(1)$ 的，原因是哈希表和保存中序遍历序列的空间代价都是 $O(n)$ 。
### 解法1 $O(1)$ 空间遍历，但仍有递归调用开销
首先，我们考虑在寻找出现次数最多的数时，**不使用哈希表**。 这个优化是**基于二叉搜索树中序遍历的性质**：一棵二叉搜索树的中序遍历序列是一个非递减的有序序列。例如：
```
      1
    /   \
   0     2
  / \    /
-1   0  2
```
这样一颗二叉搜索树的中序遍历序列是 $\{ -1, 0, 0, 1, 2, 2 \}$ 。

可以发现==重复出现的数字一定是连续出现的==，例如这里的 $0$ 和 $2$ ，它们都重复出现了，并且所有的 $0$ 都集中在一个连续的段内，所有的 $2$ 也集中在一个连续的段内。**顺序扫描中序遍历序列**，用 $base$ 记录当前的数字，用 $\textit{count}$ 记录当前数字重复的次数，用 $maxCount$ 来维护已经扫描过的数当中**出现最多的那个数字的出现次数**，用 $answer$ 数组记录出现的众数。每次扫描到一个新的元素：
- 首先更新 $\textit{base}$ 和 $count$ ：
    - 如果该元素和 $base$ 相等，那么 $count$ 自增 $1$ ；
    - 否则将 $base$ 更新为当前数字，$\textit{count}$ 复位为 $1$ 。
- 然后更新 $maxCount$ ：
    - 如果 $count=maxCount$ ，那么说明当前的这个数字（$base$）出现的次数等于当前众数出现的次数，将 $base$ 加入 $answer$ 数组；
    - 如果 $count>maxCount$ ，那么说明当前的这个数字（$base$ 出现的次数大于当前众数出现的次数，因此，我们需要将 $maxCount$ 更新为 $\textit{count}$ ，清空 $answer$ 数组后将 $\textit{base}$ 加入 $answer$ 数组。

把这个过程写成一个 $update$ 函数。这样在寻找出现次数最多的数字时，就可以省去一个哈希表带来的空间消耗。然后，我们考虑不存储这个中序遍历序列。 如果在递归进行中序遍历的过程中，访问当了某个点的时候直接使用上面的 $\text{update}$ 函数，就可以省去中序遍历序列的空间，代码如下。
```cpp
class Solution {
public:
    vector<int> answer;
    int base, count, maxCount;
    void update(int x) {
        if (x == base) ++count;
        else { count = 1, base = x; }
        if (count == maxCount) answer.push_back(base);
        if (count > maxCount) { maxCount = count; answer = vector<int> { base }; }
    }
    void dfs(TreeNode* root) {
        if (!root) return;
        dfs(root->left);
        update(root->val);
        dfs(root->right);
    }
    vector<int> findMode(TreeNode* root) {
        dfs(root);
        return answer;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 。即遍历这棵树的复杂度。
- 空间复杂度：$O(n)$ 。即递归的栈空间的空间代价。

---
### 解法2 $Morris$ 中序遍历
接着上面的思路，用 $Morris$ 中序遍历的方法把中序遍历的空间复杂度优化到 $O(1)$ 。在中序遍历时，一定先遍历左子树，然后遍历当前节点，最后遍历右子树。在常规方法中，用递归回溯或者是栈来保证遍历完左子树可以再回到当前节点，但这需要付出额外的空间代价。==我们用一种巧妙地方法可以在 $O(1)$ 的空间下，遍历完左子树可以再回到当前节点==。

我们希望**当前的节点在遍历完当前点的前驱之后被遍历**，考虑修改它的前驱节点的 $\textit{right}$ 指针。**当前节点的前驱节点的 $right$ 指针可能本来就指向当前节点（前驱是当前节点的父节点）**，也可能是当前节点左子树最右下的节点。==如果是后者，我们希望遍历完这个前驱节点之后再回到当前节点==，可以将它的 $\textit{right}$ 指针指向当前节点。

Morris 中序遍历的一个重要步骤就是寻找当前节点的前驱节点，并且 $Morris$ 中序遍历寻找下一个点**始终是通过转移到 $right$ 指针指向的位置来完成的**。
- 如果当前节点没有左子树，则遍历这个点，然后跳转到当前节点的右子树。
- 如果当前节点有左子树，那么**它的前驱节点一定在左子树上**，我们可以在左子树上一直向右行走，**找到当前点的前驱节点**。
- 如果前驱节点没有右子树，就**将前驱节点的 $\textit{right}$ 指针指向当前节点**。这一步是为了在遍历完前驱节点后**能找到前驱节点的后继**，也就是当前节点。
- 如果前驱节点的右子树为当前节点，说明前驱节点已经被遍历过并被修改了 $\textit{right}$ 指针，这个时候我们**重新将前驱的右孩子设置为空**，遍历当前的点，然后跳转到当前节点的右子树。

因此我们可以得到这样的代码框架：
```cpp
TreeNode *cur = root, *pre = nullptr;
while (cur) {
    if (!cur->left) {
        // 遍历cur
        cur = cur->right;
        continue;
    }
    pre = cur->left;
    while (pre->right && pre->right != cur) pre = pre->right;
    if (!pre->right) {
        pre->right = cur;
        cur = cur->left;
    } else {
        pre->right = nullptr;
        // 遍历cur
        cur = cur->right;
    }
}
```
最后我们将 遍历 `cur` 替换成之前的 $\text{update}$ 函数即可。
```cpp
class Solution {
public:
    int base, count, maxCount;
    void update(int x) {
        if (x == base) ++count;
        else { count = 1, base = x; }
        if (count == maxCount) answer.push_back(base);
        if (count > maxCount) { maxCount = count; answer = vector<int> { base }; }
    }
    vector<int> findMode(TreeNode* root) {
        TreeNode *cur = root, *pre = nullptr;
        while (cur) {
            if (!cur->left) {
                update(cur->val);
                cur = cur->right;
                continue;
            }
            pre = cur->left;
            while (pre->right && pre->right != cur) {
                pre = pre->right;
            }
            if (!pre->right) {
                pre->right = cur;
                cur = cur->left;
            } else {
                pre->right = nullptr;
                update(cur->val);
                cur = cur->right;
            }
        }
        return answer;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 。每个点被访问的次数不会超过两次，故这里的时间复杂度是 $O(n)$ 。
- 空间复杂度：$O(1)$ 。使用临时空间的大小和输入规模无关。