/*
** Problem: https://leetcode.cn/problems/check-if-a-number-is-majority-element-in-a-sorted-array
** Article: https://memcpy0.blog.csdn.net/article/details/133654070
** Author: memcpy0
*/
// class Solution {
// public:
//     bool isMajorityElement(vector<int>& nums, int target) {
//         return (upper_bound(nums.begin(), nums.end(), target) - lower_bound(nums.begin(), nums.end(), target)) * 2 > nums.size();
//     }
// };
// BEST: 一次二分查找
class Solution {
public:
    bool isMajorityElement(vector<int>& nums, int target) {
        int left = lower_bound(nums.begin(), nums.end(), target) - nums.begin();
        int right = left + nums.size() / 2;
        return right >= nums.size() ? false : nums[right] == target;
    }
};