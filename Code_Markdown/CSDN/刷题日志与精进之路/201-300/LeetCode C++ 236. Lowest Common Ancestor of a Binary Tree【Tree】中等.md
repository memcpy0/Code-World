Given a binary tree, find the lowest common ancestor (LCA) of two given nodes in the tree.

According to the definition of LCA on Wikipedia: “The lowest common ancestor is defined between two nodes p and q as the lowest node in T that has both p and q as descendants (where we allow **a node to be a descendant of itself**).”

Given the following binary tree:  root = `[3,5,1,6,2,0,8,null,null,7,4]`
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200709170142847.png)
 Example 1:

```cpp
Input: root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
Output: 3
Explanation: The LCA of nodes 5 and 1 is 3.
```

Example 2:
```cpp
Input: root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4
Output: 5
Explanation: The LCA of nodes 5 and 4 is 5, since a node can be a descendant of itself according to the LCA definition.
```
Note:
-   All of the nodes' values will be unique.
-    `p` and `q` are different and both values will exist in the binary tree.

思路：做过的题目。

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
代码2：找到通往两个结点的路径，然后从后往前对比，找到最后一个相同的结点。
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
