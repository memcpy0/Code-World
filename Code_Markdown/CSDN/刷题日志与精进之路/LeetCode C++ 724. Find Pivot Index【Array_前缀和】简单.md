Given an array of integers `nums` , write a method that returns the "pivot" index of this array.

We define the pivot index as the index where **the sum of all the numbers to the left** of the index is equal to **the sum of all the numbers to the right** of the index.

If no such index exists, we should return `-1` . If there are multiple pivot indexes, you should return the **left-most** pivot index.

 

**Example 1:**
```cpp
Input: nums = [1,7,3,6,5,6]
Output: 3
Explanation:
The sum of the numbers to the left of index 3 (nums[3] = 6) is equal to the sum of numbers to the right of index 3.
Also, 3 is the first index where this occurs.
```
**Example 2:**
```cpp
Input: nums = [1,2,3]
Output: -1
Explanation:
There is no index that satisfies the conditions in the problem statement.
```
**Constraints:**
-  The length of nums will be in the range `[0, 10000]` .
-  Each element nums[i] will be an integer in the range `[-1000, 1000]` .

题意：判断数组中是否存在枢纽元，即该索引的左侧所有元素相加的和等于右侧所有元素相加的和。

----
思路：前缀和。我们求出前缀和数组，左侧所有元素的和等于 `presum[i]` ，右侧所有元素的和等于 `presum[n] - presum[i + 1]` ，判断是否相等即可。

代码如下：
```cpp
class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        if (nums.empty()) return -1;
        if (nums.size() == 1) return 0;
        int n = nums.size(), presum[n + 1];
        presum[0] = 0;
        for (int i = 0; i < n; ++i)
            presum[i + 1] = presum[i] + nums[i];
        for (int i = 0; i < n; ++i) {
            int leftSum = presum[i], rightSum = presum[n] - presum[i + 1];
            if (leftSum == rightSum)
                return i;
        }
        return -1;
    }
};
```
这里可以优化一下，不使用前缀和数组，节省一点空间：
```cpp
class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        if (nums.empty()) return -1;
        if (nums.size() == 1) return 0;
        int n = nums.size(), presum = 0, total = 0;
        for (int i = 0; i < n; ++i)
            total += nums[i];

        for (int i = 0; i < n; ++i) {
            if (presum * 2 + nums[i] == total) //presum == total - presum - nums[i]
                return i;
            presum += nums[i];
        }
        return -1;
    }
};
```
效率：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了74.21% 的用户
内存消耗：31.1 MB, 在所有 C++ 提交中击败了50.29% 的用户
```
