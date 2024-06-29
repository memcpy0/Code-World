#include <bits/stdc++.h>
using namespace std;
/*
输入一棵二叉树的根节点，求该树的深度。从根节点到叶节点依次经过的节点（含根、叶节点）形成树的一条路径，最长路径的长度为树的深度。
例如：
给定二叉树 [3,9,20,null,null,15,7]，

    3
   / \
  9  20
    /  \
   15   7

返回它的最大深度 3 。 

功能测试：输入普通的二叉树；二叉树中所有结点都没有左/右子树
特殊输入测试：只有一个二叉树结点，二叉树头结点为NULL指针
边界测试：无 

--------------------------------------------------------------------------
输入一棵二叉树的根节点，判断该树是不是平衡二叉树。如果某二叉树中任意节点的左右子树的深度相差不超过1，那么它就是一棵平衡二叉树。

示例 1:
给定二叉树 [3,9,20,null,null,15,7]
    3
   / \
  9  20
    /  \
   15   7
返回 true 。

示例 2:
给定二叉树 [1,2,2,3,3,null,null,4,4]
       1
      / \
     2   2
    / \
   3   3
  / \
 4   4
返回 false 。

限制：
    1 <= 树的结点个数 <= 10000
    
功能测试：输入平衡二叉树；不平衡的二叉树；二叉树中所有结点都没有左/右子树
特殊输入测试：只有一个二叉树结点，二叉树头结点为NULL指针
边界测试：无 
*/ 
//重复遍历多次 
bool isBalanced(BinaryTreeNode *pRoot) {
	if (pRoot == nullptr) return true;
	int left = treeDepth(pRoot->left);
	int right = TreeDepth(pRoot->right);
	int diff = left - right;
	if (diff > 1 || diff < -1) 
		return false;
	return isBalanced(pRoot->left) && isBalanced(pRoot->right);
}
//只遍历了一次：用后序遍历的方法,遍历二叉树的每个结点，在遍历到一个结点之前，我们就已经遍历了它的左右子树
//那么只要在遍历每个结点的时候记录它的深度（某一结点的深度等于它到叶子结点的路径长度）。我们就可以一边遍历
//一边判断每个结点是不是平衡的
bool isBalanced(BinaryTreeNode *pRoot, int *pDepth) {
	if (pRoot == nullptr) {
		*pDepth = 0; 
		return true; 
	}
	int left, right;
	if (isBalanced(pRoot->left, &left) && isBalanced(pRoot->right, &right)) {
		int diff = left - right;
		if (diff <= 1 && diff >= -1) {
			*pDepth = 1 + max(left, right);
			return true;
		}
	}
	return false;
}
//用后序遍历的方式遍历一棵二叉树，在遍历某结点的左右子结点之后，我们可以根据它的左右子结点的深度判断它是否是平衡的
//并得到当前结点的深度
//当最后遍历到树的根结点时，就判断了整棵二叉树是否平衡二叉树。 
bool isBalanced(BinaryTreeNode *pRoot) {
	int depth = 0;
	return isBalanced(pRoot, &depth);
} 
 
int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

