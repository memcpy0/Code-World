/*
** Problem: https://leetcode.cn/problems/find-majority-element-lcci/
** Article: https://memcpy0.blog.csdn.net/article/details/133637494
** Author: memcpy0
*/
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candidate = 0, count = 0;
        for (int num : nums) {
            if (count == 0) candidate = num;
            if (candidate == num) ++count;
            else --count;
        }
        count = 0;
        for (int num : nums) if (num == candidate) ++count;
        return count * 2 > nums.size() ? candidate : -1;
    }
};