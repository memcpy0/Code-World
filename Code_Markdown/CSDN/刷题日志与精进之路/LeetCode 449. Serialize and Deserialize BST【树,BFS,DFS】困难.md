> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

序列化是将数据结构或对象转换为一系列位的过程，以便它可以存储在文件或内存缓冲区中，或通过网络连接链路传输，以便稍后在同一个或另一个计算机环境中重建。

设计一个算法来序列化和反序列化 **二叉搜索树** 。 对序列化/反序列化算法的工作方式没有限制。 您只需确保二叉搜索树可以序列化为字符串，并且可以将该字符串反序列化为最初的二叉搜索树。

**编码的字符串应尽可能紧凑。**

**示例 1：**
```js
输入：root = [2,1,3]
输出：[2,1,3]
```
**示例 2：**
```js
输入：root = []
输出：[]
```
**提示：**
- 树中节点数范围是 `[0, 10^4]`
- `0 <= Node.val <= 10^4`
- 题目数据 **保证** 输入的树是一棵二叉搜索树。

---
类似题目：
- [449. 序列化和反序列化二叉搜索树](https://leetcode.cn/problems/serialize-and-deserialize-bst/)
- [297. 二叉树的序列化与反序列化](https://leetcode.cn/problems/serialize-and-deserialize-binary-tree/) 困难
- [428. 序列化和反序列化 N 叉树](https://leetcode.cn/problems/serialize-and-deserialize-n-ary-tree/) 困难

二叉搜索树是一种特殊的二叉树，序列化和反序列化过程也可以直接使用「[297. 二叉树的序列化与反序列化](https://memcpy0.blog.csdn.net/article/details/132778608)」中的方法，即实现上我们可以忽略「BST」这一条件，使用BFS或DFS、存储空节点来进行序列号和反序列化。由于点的数量是 $1e4$ ，最坏情况下是当BST成链时，会有较大的空间浪费。

但二叉搜索树的特殊之处在于**其中序遍历是有序的**，可以利用这一点来优化时间和空间复杂度。

---
### 解法 二叉搜索树+后序遍历+栈
给定一棵二叉树的「先序遍历」和「中序遍历」可以恢复这颗二叉树。给定一棵二叉树的「后序遍历」和「中序遍历」也可以恢复这颗二叉树。而对于二叉搜索树，给定「先序遍历」或者「后序遍历」，对其经过排序即可得到「中序遍历」。因此，==仅对二叉搜索树做「先序遍历」或者「后序遍历」，即可达到序列化和反序列化的要求。此题解采用「后序遍历」的方法==。
- 序列化时，只需要对二叉搜索树进行后序遍历（注意跳过空节点），再将数组编码成字符串即可。
- 反序列化时，**需要先将字符串解码成后序遍历的数组**。在将后序遍历的数组恢复成二叉搜索树时，**不需要先排序得到中序遍历的数组**、再根据中序和后序遍历的数组来恢复二叉树，而可以**根据有序性直接由后序遍历的数组恢复二叉搜索树**——后序遍历得到的数组中，根结点的值位于数组末尾，左子树的节点均小于根节点的值，右子树的节点均大于根节点的值，可以根据这些性质设计递归函数恢复二叉搜索树。
```java
public class Codec {
    // Encodes a tree to a single string.
    public String serialize(TreeNode root) {
        return postOrder(root, new StringBuilder()).toString();
    }
    private StringBuilder postOrder(TreeNode root, StringBuilder sb) {
        if (root != null) { // 可以不用存储空指针
            sb = postOrder(root.left, sb);
            sb = postOrder(root.right, sb);
            sb.append(String.valueOf(root.val) + ",");
        }
        return sb;
    }

    // Decodes your encoded data to tree.
    public TreeNode deserialize(String data) {
        if (data.isEmpty()) return null;
        String[] arr = data.split(",");
        Deque<Integer> stack = new ArrayDeque<Integer>();
        int length = arr.length;
        for (int i = 0; i < length; ++i)
            stack.push(Integer.parseInt(arr[i]));
        return construct(Integer.MIN_VALUE, Integer.MAX_VALUE, stack);
    }
    // 使用栈和后序遍历重构二叉搜索树
    private TreeNode construct(int lower, int upper, Deque<Integer> stack) {
        // 当前元素超出范围，不能用作子树根节点的值
        if (stack.isEmpty() || stack.peek() < lower || stack.peek() > upper) return null;
        int val = stack.pop();
        TreeNode root = new TreeNode(val);
        root.right = construct(val, upper, stack);
        root.left = construct(lower, val, stack);
        return root;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是树的节点数。$serialize$ 需要 $O(n)$ 时间遍历每个点。$deserialize$ 需要 $O(n)$ 时间恢复每个点
- 空间复杂度：$O(n)$ ，其中 $n$ 是树的节点数。$serialize$ 需要 $O(n)$ 空间用数组保存每个点的值，递归的深度最深也为 $O(n)$ 。$deserialize$ 需要 $O(n)$ 空间用数组保存每个点的值，递归的深度最深也为 $O(n)$ 。