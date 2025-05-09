/*
** Problem: https://leetcode.cn/problems/house-robber-iii/
** Article: https://memcpy0.blog.csdn.net/article/details/109324656
** Author: memcpy0
*/
// BEST: 树型DP
struct SubtreeStatus {
    int selected;
    int notSelected;
};
class Solution {
public:
    SubtreeStatus dfs(TreeNode* node) {
        if (!node) {
            return {0, 0};
        }
        auto l = dfs(node->left);
        auto r = dfs(node->right);
        int selected = node->val + l.notSelected + r.notSelected;
        int notSelected = max(l.selected, l.notSelected) + max(r.selected, r.notSelected);
        return {selected, notSelected};
    }
    int rob(TreeNode* root) {
        auto rootStatus = dfs(root);
        return max(rootStatus.selected, rootStatus.notSelected);
    }
};