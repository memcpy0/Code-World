/*
** Problem: https://leetcode.cn/problems/minimum-moves-to-equal-array-elements-ii/
** Article: https://memcpy0.blog.csdn.net/article/details/109500987
** Author: memcpy0
*/
// O(nlog n),两种写法
class Solution {
public:
    int minMoves2(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), ans = 0, x = nums[n / 2];
        for (int i = 0; i < n; ++i) ans += abs(nums[i] - x);
        // int i = 0, j = nums.size() - 1, ans = 0;
        // while (i < j) ans += nums[j--] + nums[i++];
        return ans;
    }
};
// O(n)快速选择
class Solution {
public:
    int quickSelect(vector<int>& nums, int left, int right, int index) {
        int q = randomPartition(nums, left, right);
        if (q == index) {
            return nums[q];
        } else {
            return q < index ? quickSelect(nums, q + 1, right, index) : quickSelect(nums, left, q - 1, index);
        }
    }

    inline int randomPartition(vector<int>& nums, int left, int right) {
        int i = rand() % (right - left + 1) + left;
        swap(nums[i], nums[right]);
        return partition(nums, left, right);
    }

    inline int partition(vector<int>& nums, int left, int right) {
        int x = nums[right], i = left - 1;
        for (int j = left; j < right; ++j) {
            if (nums[j] <= x) {
                swap(nums[++i], nums[j]);
            }
        }
        swap(nums[i + 1], nums[right]);
        return i + 1;
    }

    int minMoves2(vector<int>& nums) {
        srand(time(0));
        int n = nums.size(), x = quickSelect(nums, 0, n - 1, n / 2), ret = 0;
        for (int i = 0; i < n; ++i) {
            ret += abs(nums[i] - x);
        }
        return ret;
    }
};