//返回与给定的前序和后序遍历匹配的任何一棵二叉树
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
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