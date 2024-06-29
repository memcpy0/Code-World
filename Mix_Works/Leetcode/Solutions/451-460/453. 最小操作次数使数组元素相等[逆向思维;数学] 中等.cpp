/*
** Problem: https://leetcode.cn/problems/minimum-moves-to-equal-array-elements/
** Article: https://memcpy0.blog.csdn.net/article/details/109482267
** Author: memcpy0
*/
// BEST: 逆向思维,让所有数都变为mn
class Solution {
public:
    int minMoves(vector<int>& nums) {
        int mn = *min_element(nums.begin(),nums.end()), ans = 0;
        for (const int &v : nums) ans += v - mn;
        return ans;
    }
};
// 数学
class Solution {
public:
    int minMoves(vector<int>& nums) {
        long long mn = *min_element(nums.begin(), nums.end()), sum = accumulate(nums.begin(), nums.end(), 0);
        return sum - mn * nums.size();
    }
};