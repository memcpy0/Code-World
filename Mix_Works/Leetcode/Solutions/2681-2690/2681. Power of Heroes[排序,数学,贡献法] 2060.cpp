/*
** Problem: https://leetcode.cn/problems/power-of-heroes/
** Article: https://memcpy0.blog.csdn.net/article/details/132395565
** Author: memcpy0
*/
class Solution {
public:
    int sumOfPower(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        const int mod = 1e9 + 7;
        int ans = 0, s = 0;
        for (long long x : nums) { // x作为最大值
            ans = (ans + x * x % mod * (x + s)) % mod; // 中间模1次防止溢出
            s = (s * 2 + x) % mod; // 递推计算下个s
        }
        return ans;
    }
};