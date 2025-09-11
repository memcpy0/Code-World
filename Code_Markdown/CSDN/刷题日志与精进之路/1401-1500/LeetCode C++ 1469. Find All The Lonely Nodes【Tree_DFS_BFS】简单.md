<p>In a binary tree, a <strong>lonely</strong> node is a node that is the only child of its parent node. The root of the tree is not lonely because it does not have a parent node.</p>

<p>Given the <code>root</code> of a binary tree, return <em>an array containing the values of all lonely nodes</em> in the tree. Return the list <strong>in any order</strong>.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 203px; height: 202px;" src="https://assets.leetcode.com/uploads/2020/06/03/e1.png" alt="">

```haskell
Input: root = [1,2,3,null,4]
Output: [4]
Explanation: Light blue node is the only lonely node.
Node 1 is the root and is not lonely.
Nodes 2 and 3 have the same parent and are not lonely.
```

 
 
<p><strong>Example 2:</strong></p>
<img style="width: 442px; height: 282px;" src="https://assets.leetcode.com/uploads/2020/06/03/e2.png" alt="">
 

```haskell
Input: root = [7,1,4,6,null,5,3,null,null,null,null,null,2]
Output: [6,2]
Explanation: Light blue nodes are lonely nodes.
Please remember that order doesn't matter, [2,6] is also an acceptable answer.
```

 
<p><strong>Example 3:</strong></p>
<strong><img style="width: 363px; height: 202px;" src="https://assets.leetcode.com/uploads/2020/06/03/tree.png" alt=""> </strong>

```haskell 
Input: root = [11,99,88,77,null,null,66,55,null,null,44,33,null,null,22]
Output: [77,55,33,66,44,22]
Explanation: Nodes 99 and 88 share the same parent. Node 11 is the root.
All other nodes are lonely.
```
 
<p><strong>Example 4:</strong></p>
 

```haskell
Input: root = [197]
Output: []
```

<p><strong>Example 5:</strong></p>

```haskell
Input: root = [31,null,78,null,28]
Output: [78,28] 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li>The number of nodes in the&nbsp;<code>tree</code>&nbsp;is in the range&nbsp;<code>[1, 1000].</code></li>
	<li>Each node's value is between&nbsp;<code>[1, 10^6]</code>.</li>
</ul>

题意：给定一棵二叉树的根节点 `root` ，返回树中 **所有的独生节点的值所构成的数组** 。数组的顺序 **不限** 。

---
### 解法 DFS
使用DFS，当前节点根据传递下来的信息（这一节点是否是独生节点），决定节点值是否加入结果；然后判断当前节点的左右子节点是否是独生节点，递归调用并传递信息：
```cpp
class Solution {
private:
    vector<int> ans;
    void dfs(TreeNode* root, bool isLone) {
        if (root == nullptr) return;
        if (isLone) ans.push_back(root->val); //是独生节点
        bool flag = (root->left && !root->right) || (!root->left && root->right);
        dfs(root->left, flag);
        dfs(root->right, flag);
    }
public:
    vector<int> getLonelyNodes(TreeNode* root) {
        dfs(root, false);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了81.15% 的用户
内存消耗：20.4 MB, 在所有 C++ 提交中击败了30.58% 的用户
```
