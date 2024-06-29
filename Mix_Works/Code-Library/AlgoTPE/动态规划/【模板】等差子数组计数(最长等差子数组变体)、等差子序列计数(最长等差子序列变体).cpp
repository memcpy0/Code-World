//等差子数组计数(最长等差子数组的变体)
/*
** Problem: https://leetcode-cn.com/problems/arithmetic-slices
** Article: https://memcpy0.blog.csdn.net/article/details/119641437
*/
class Solution { //双指针+贪心计数
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        if (nums.size() <= 2) return 0;
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n - 2; ) {
            int j = i, d = nums[i + 1] - nums[i];
            while (j + 1 < n && nums[j + 1] - nums[j] == d) ++j;
            int len = j - i + 1, a0 = 1, an = len - 3 + 1;
            //符合条件(长度>=3且<=len)的等差子数组的数量
            int cnt = (a0 + an) * an / 2;
            ans += cnt;
            i = j;
        } 
        return ans;
    }
};
class Solution { //动态规划计数
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        if (nums.size() <= 2) return 0;
        int n = nums.size(), ans = 0;
        vector<int> dp(n); //dp[i]为以nums[i]结尾的等差数组的数目
        for (int i = 2; i < n; ++i) {
            if (nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2]) {
                dp[i] = dp[i - 1] + 1;
                ans += dp[i];
            } 
        } 
        return ans;
    }
};
class Solution { //动态规划最优
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        if (nums.size() <= 2) return 0;
        int n = nums.size(), ans = 0, dp = 0;
        for (int i = 2; i < n; ++i) {
            if (nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2]) {
                ++dp; //dp[i]=dp[i-1]+1;
                ans += dp;
            } else dp = 0;
        } 
        return ans;
    }
};
//等差子序列计数(最长等差子序列的变体)