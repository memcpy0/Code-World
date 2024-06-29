/*
** Problem: https://leetcode.cn/problems/house-robber/
** Article: https://memcpy0.blog.csdn.net/article/details/109324628
** Author: memcpy0
*/
class Solution {
public:
    int rob(vector<int> &nums) {
        int f0 = 0, f1 = 0;
        for (int x : nums) {
            int new_f = max(f1, f0 + x);
            f0 = f1;
            f1 = new_f;
        }
        return f1;
    }
};