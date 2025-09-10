> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a **binary tree** in which each node contains an integer value.

Find the number of paths that sum to a given value.

The path does not need to start or end at the root or a leaf, but it must go **downwards** (traveling only from parent nodes to child nodes).

The tree has no more than `1,000` nodes and the values are in the range `-1,000,000` to `1,000,000`.

**Example:**
```cpp
root = [10,5,-3,3,2,null,11,3,-2,null,1], sum = 8
      10
     /  \
    5   -3
   / \    \
  3   2   11
 / \   \
3  -2   1

Return 3. The paths that sum to 8 are:
1.  5 -> 3
2.  5 -> 2 -> 1
3. -3 -> 11
```
题意：找出二叉树中所有的路径——其上的结点值之和为 `sum` 。返回这些路径的数量。

---
### 解法1 树的遍历/双重递归
要注意的是，**此题的路径不必以根结点为起点，也不必以叶结点为终点**。所以一个朴素的做法是，搜索「以每个节点为根的（向下的）所有路径」，并累加统计总和为 `num` 的路径。为此**需要双重递归**——使用 `pathSum` 先序递归遍历所有的结点，其中分别以每个结点为起点，用 `findPath` 递归向下寻找和为 `sum` 的路径。
<img src="https://img-blog.csdnimg.cn/20200827190125730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="66%">
另外，对每个结点遍历时，**「找到从它出发的一条路径满足题意」并不是返回的基准点，还可以继续往下递归**。如下所示，找到 `1 -> -2` 满足条件时, 不能立即返回，这样会忽视 `1 -> -2 -> 1 -> -1` 这条路径：
```cpp
sum = -1 
       1 
     /   \
   -2    -3
   / \   /
  1   3 -2  
 / 
-1
```
具体代码如下。算法整体的时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
private:
    //在以node为根结点的二叉树中,寻找包含node、和为num的路径个数
    int findPath(TreeNode* node, int num) {
        if (node == nullptr) return 0;
        int res = 0;
        if (node->val == num) ++res;
        //如果node的值可能是负数,则加上后续的路径值的和仍然可能等于num
        res += findPath(node->left, num - node->val);
        res += findPath(node->right, num - node->val);
        return res;
    }
public: 
    //在以root为根结点的二叉树中,寻找和为sum的路径,返回这样的路径个数
    int pathSum(TreeNode* root, int sum) { 
        if (root == nullptr) return 0;
        int res = findPath(root, sum); //寻找包含root并且和为sum的路径
        //处理不包含root结点且和为sum的路径数目
        res += pathSum(root->left, sum);    
        res += pathSum(root->right, sum);
        return res;
    }
};
//执行用时：36 ms, 在所有 C++ 提交中击败了24.73% 的用户
//内存消耗：15.3 MB, 在所有 C++ 提交中击败了57.92% 的用户
```

---
### 解法2 树的遍历+前缀和+哈希表+回溯
在解法一中，我们统计以每个节点为根的（向下直到叶节点）所有（符合条件的）路径，即统计以每个节点为「路径开头」的所有合法路径。此处优化的切入点是「路径只能往下」。

不妨考虑一维普通数组的情形，要统计其中有多少元素和为 `sum` 的子数组。使用暴力解法，时间复杂度为 $O(n^2)$ 。改用前缀和与哈希表，则可在 $O(n)$ 时间内解决此题。

运用这一思想，我们对本题转换一下，统计以每个节点为「路径结尾」的（向上直到根节点）所有合法路径，将问题彻底变化为一维问题：**求解从原始起点（根节点）到当前节点 `j` 的路径中，有多少节点 `i` 满足 `sum[i...j] = targetSum`** 。由于从根节点到当前节点的路径唯一，这本质上就是一个「树上一维前缀和」问题。

注意，由于树的遍历会同时搜索左右子树，因此我们在搜索完当前节点的左右子树后，应**回溯**将此节点（到根节点）的路径和从哈希表中删除，防止统计时错误计数。
```cpp
//C++ version
class Solution {
private:
    unordered_map<int, int> rec;
    int ans = 0, target = 0; //返回结果
    void dfs(TreeNode* root, int sum) {
        ans += rec[sum - target]; //可能不存在sum-target,于是ans+=0
        ++rec[sum];
        if (root->left) dfs(root->left, sum + root->left->val);
        if (root->right) dfs(root->right, sum + root->right->val);
        --rec[sum]; //注意回溯
    }
public:
    int pathSum(TreeNode* root, int sum) { 
        if (root == nullptr) return 0;
        target = sum;
        rec[0] = 1;
        dfs(root, root->val);
        return ans;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了98.59% 的用户
//内存消耗：19.8 MB, 在所有 C++ 提交中击败了11.29% 的用户
```
