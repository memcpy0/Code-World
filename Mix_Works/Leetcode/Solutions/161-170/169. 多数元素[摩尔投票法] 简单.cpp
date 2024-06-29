/*
** Problem: https://leetcode.cn/problems/majority-element/
** Article: https://memcpy0.blog.csdn.net/article/details/133637387
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