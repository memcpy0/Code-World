Given an array consisting of `n` integers, find the contiguous subarray of given length `k` that has the maximum average value. And you need to output the maximum average value.

**Example 1:** 
```swift
Input: [1,12,-5,-6,50,3], k = 4
Output: 12.75
Explanation: Maximum average is (12-5-6+50)/4 = 51/4 = 12.75
```
 
**Note:**
- `1 <= k <= n <= 30,000` .
- Elements of the given array will be in the range `[-10,000, 10,000]` .
 
 题意：给定 `n` 个整数，找出**平均数最大**且**长度为 `k`** 的连续子数组，并输出该最大平均数。


----
### 解法 滑动窗口
维持一个固定长度为 `k` 的滑动窗口，不断更新窗口的起始和终止端，同时更新最大窗口和。得到最大窗口和后，除以 `k` 即等于最大平均数。
```cpp
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        if (nums.size() == 1) return nums[0];
        int cursum = 0, maxsum = 0, n = nums.size(); //最大窗口和不超过int范围
        for (int i = 0; i < n; ++i) {
            cursum += nums[i];
            if (i + 1 > k) {
                cursum -= nums[i - k];
                maxsum = max(maxsum, cursum); 
            } else if (i + 1 == k) maxsum = cursum;
        }
        return static_cast<double>(maxsum) / k; 
    }
};
```
提交后效率如下：
```cpp
执行用时：260 ms, 在所有 C++ 提交中击败了93.96% 的用户
内存消耗：70.3 MB, 在所有 C++ 提交中击败了22.01% 的用户
```
