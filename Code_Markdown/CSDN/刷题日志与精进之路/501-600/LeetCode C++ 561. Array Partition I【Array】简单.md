Given an array of `2n` integers, your task is to group these integers into `n` pairs of integer, say `(a1, b1), (a2, b2), ..., (an, bn)` which makes sum of `min(ai, bi)` for all `i` from `1` to `n` as large as possible.
 
 **Example 1:**
```
Input: [1,4,3,2]
Output: 4 
Explanation: n is 2, and the maximum sum of pairs is 4 = min(1, 2) + min(3, 4).
```
**Note:**
- `n` is a positive integer, which is in the range of `[1, 10000]`.
- All the integers in the array will be in the range of `[-10000, 10000]` . 

题意：这题给出一个数组，`2*n` 个元素，每两个元素为一对，求使得 `n` 对中每一对数的最小值的和最大为多少。

---
思路：贪心。先从小到大排序，然后取每一对元素的左侧元素(这一对数的最小值)，求和即可。代码如下：
```c
class Solution {
public:
    int arrayPairSum(vector<int>& nums) {
        int num = 0;
        sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size(); i += 2) num += nums[i];
        return num;
    }
};
```
效率：
```cpp
执行用时：156 ms, 在所有 C++ 提交中击败了87.82% 的用户
内存消耗：26.7 MB, 在所有 C++ 提交中击败了38.61% 的用户
```
