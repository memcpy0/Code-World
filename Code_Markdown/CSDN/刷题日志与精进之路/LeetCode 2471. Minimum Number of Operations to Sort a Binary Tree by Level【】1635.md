> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


给你一个 **值互不相同** 的二叉树的根节点 `root` 。

在一步操作中，你可以选择 **同一层** 上任意两个节点，交换这两个节点的值。

返回每一层按 **严格递增顺序** 排序所需的最少操作数目。

节点的 **层数** 是该节点和根节点之间的路径的边数。

**示例 1 ：**

![](https://assets.leetcode.com/uploads/2022/09/18/image-20220918174006-2.png)

**输入：**root = [1,4,3,7,6,8,5,null,null,null,null,9,null,10]
**输出：**3
**解释：**
- 交换 4 和 3 。第 2 层变为 [3,4] 。
- 交换 7 和 5 。第 3 层变为 [5,6,8,7] 。
- 交换 8 和 7 。第 3 层变为 [5,6,7,8] 。
共计用了 3 步操作，所以返回 3 。
可以证明 3 是需要的最少操作数目。

**示例 2 ：**

![](https://assets.leetcode.com/uploads/2022/09/18/image-20220918174026-3.png)

**输入：**root = [1,3,2,7,6,5,4]
**输出：**3
**解释：**
- 交换 3 和 2 。第 2 层变为 [2,3] 。 
- 交换 7 和 4 。第 3 层变为 [4,6,5,7] 。 
- 交换 6 和 5 。第 3 层变为 [4,5,6,7] 。
共计用了 3 步操作，所以返回 3 。 
可以证明 3 是需要的最少操作数目。

**示例 3 ：**

![](https://assets.leetcode.com/uploads/2022/09/18/image-20220918174052-4.png)

**输入：**root = [1,2,3,4,5,6]
**输出：**0
**解释：**每一层已经按递增顺序排序，所以返回 0 。

**提示：**

- 树中节点的数目在范围 `[1, 10^5]` 。
- `1 <= Node.val <= 10^5`
- 树中的所有值 **互不相同** 。

```cpp
class Solution {
public:
    int minimumOperations(TreeNode* root) {
        if (root == nullptr) return 0;
        queue<TreeNode*> q;
        q.push(root);
        int arr[100010];
        memset(arr, 0, sizeof(arr));
        int ans = 0;
        while (!q.empty()) {
            int size = q.size();
            vector<int> tmp, stmp;
            for (int i = 0; i < size; ++i) {
                TreeNode* u = q.front(); q.pop();
                tmp.push_back(u->val);
                if (u->left) q.push(u->left);
                if (u->right) q.push(u->right);
            }
            stmp = tmp;
            sort(stmp.begin(), stmp.end());
            for (int i = 0; i < size; ++i) arr[stmp[i]] = tmp[i];
            for (int i = 0; i < size; ++i) {
                while (arr[stmp[i]] != stmp[i]) {
                    swap(arr[stmp[i]], arr[arr[stmp[i]]]);
                    ++ans;
                }
            }
        }
        return ans;
    }
};
```