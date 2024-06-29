给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。

百度百科中最近公共祖先的定义为：“对于有根树 `T` 的两个结点 `p` 、`q` ，最近公共祖先表示为一个结点 `x` ，满足 `x` 是 `p`、`q` 的祖先且 `x` 的深度尽可能大（一个节点也可以是它自己的祖先）。”

例如，给定如下二叉树:  `root = [3,5,1,6,2,0,8,null,null,7,4]`
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200709170227163.png)

示例 1:
```cpp
输入: root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
输出: 3
解释: 节点 5 和节点 1 的最近公共祖先是节点 3。
```

示例 2:
```cpp
输入: root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4
输出: 5
解释: 节点 5 和节点 4 的最近公共祖先是节点 5。因为根据定义最近公共祖先节点可以为节点本身。
```
说明:
-    所有节点的值都是唯一的。
-    `p、q` 为不同节点且均存在于给定的二叉树中。

题意：求出两个给定结点在二叉树上的最近公共祖先。

思路1：设 `root` 为二叉树的根节点，**所有节点的值都是唯一的**，`p,q` 是给出的两个实际存在节点，它们的最近公共祖先有多种情况：
- 1.递归边界：`root == NULL || root == p || root == q` ，返回 `root` ；
- 2.`p,q` 分别处在左右子树两边，则当前的根节点就是最近公共祖先，返回 `root` ；
- 3.`p,q` 都在左子树，则返回**从左子树找到的最近公共祖先**；
- 4.`p,q` 都在右子树，则返回**从右子树找到的最近公共祖先**。

代码1：
```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
	//如果根节点为空,则返回空
	if (!root) return nullptr; 
	//如果根节点是p或者q,则返回root
	if (root == p || root == q)
	    return root;
	//根节点非空且非p非q,去左右两边的子树找
	TreeNode *left = lowestCommonAncestor(root->left, p, q);
	TreeNode *right = lowestCommonAncestor(root->right, p, q);
	//如果p,q一个在左子树一个在右子树,则根节点就是最近公共祖先
	if (left && right)
	    return root;
	else if (left) //只在左子树中找到,则返回找到的left
	    return left;
	else if (right) //只在右子树中找到,则返回找到的right
	    return right;
	return nullptr;
}
```

思路2：先序遍历根节点到 `p` 和 `q` 的两个路径，注意剪枝——如果找到了到`p,q` 的路径就及时返回；如果搜索完左右子树后，路径最后一个结点不是要找的结点就进行删除。然后**两个路径逐个对比，最后一个相同的点即为最近公共祖先**。

代码2：
```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) { 
	vector<TreeNode*> path1, path2;
	getPath(root, p, path1);
	getPath(root, q, path2); 
	int n = min(path1.size(), path2.size());
	for (int i = n - 1; i >= 0; --i)
		if (path1[i] == path2[i])
			return path1[i];
	return nullptr;
}
void getPath(TreeNode *root, TreeNode *node, vector<TreeNode*> &path) {
	if (!root) return;
	path.push_back(root);
	if (root == node) return; //得到路径后及时返回
	if (path.back() != node) getPath(root->left, node, path);
	if (path.back() != node) getPath(root->right, node, path);
	if (path.back() != node) path.pop_back();
}
```
