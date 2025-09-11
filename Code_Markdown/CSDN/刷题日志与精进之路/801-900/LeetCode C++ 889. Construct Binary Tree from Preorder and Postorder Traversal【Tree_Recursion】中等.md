<p>Return any binary tree that matches the given preorder and postorder traversals.</p>

<p>Values in the traversals&nbsp;<code>pre</code> and <code>post</code>&nbsp;are distinct&nbsp;positive integers.</p>
 
<div>
<p><strong>Example 1:</strong></p>

```clike
Input: pre = [1,2,4,5,3,6,7], post = [4,5,2,6,7,3,1]
Output: [1,2,3,4,5,6,7]
```
 
<p><strong><span>Note:</span></strong></p>

<ul>
	<li><code>1 &lt;= pre.length == post.length &lt;= 30</code></li>
	<li><code>pre[]</code> and <code>post[]</code>&nbsp;are both permutations of <code>1, 2, ..., pre.length</code>.</li>
	<li>It is guaranteed an answer exists. If there exists multiple answers, you can return any of them.</li>
</ul> 

题意：返回与给定的前序和后序遍历匹配的任何二叉树。<code>pre</code>&nbsp;和&nbsp;<code>post</code>&nbsp;遍历中的值是不同的正整数。</p>

---
### 解法 递归
我们早就学过，二叉树的先序和后序遍历序列无法唯一确定二叉树的形态。不过这句话其实不太正确，必须加上限定条件——**如果二叉树中每个节点的度数要么为 $0$ （叶子节点）要么为 $2$ （内部节点），此时先序和后序遍历序列能够唯一地确定一棵二叉树**；如果存在内部节点度数为 $1$ ，此时无法从先序和后序序列中确定其子节点是左子树还是右子树，就无法唯一确定一棵二叉树。对于存在左右子树判断的，下述代码中统一处理为左子树。因此，代码中是通过**寻找当前子树的左子树节点**，区分开当前节点的左右子树：
```cpp
class Solution {
private:
    TreeNode* dfs(vector<int>& pre, int pl, int pr, vector<int>& post, int tl, int tr) {
        if (pl > pr || tl > tr) return nullptr;
        TreeNode *root = new TreeNode(pre[pl]); //以先序序列pl处的值为根节点
        if (pl == pr) return root; //当前根节点没有子树,度数为0
        int index = tl;
        while (index <= tr && post[index] != pre[pl + 1]) ++index; //寻找当前根节点的左子树节点
        int leftsize = index - tl + 1; //当前根节点的左子树大小
        root->left = dfs(pre, pl + 1, pl + leftsize, post, tl, index);
        root->right = dfs(pre, pl + leftsize + 1, pr, post, index + 1, tr - 1);
        return root;
    } //当前根节点如果只有一个子树节点时,二叉树的形态无法确定,不过这里统一处理为左子树
public:
    TreeNode* constructFromPrePost(vector<int>& pre, vector<int>& post) {
        return dfs(pre, 0, pre.size() - 1, post, 0, post.size() - 1);
    }
};  
```
运行效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了84.14% 的用户
内存消耗：24.5 MB, 在所有 C++ 提交中击败了96.52% 的用户
```
