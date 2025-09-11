Given an array of integers and an integer $k$ , you need to find the total number of **continuous subarrays** whose sum equals to $k$ .

**Example 1:**
```cpp
Input:nums = [1,1,1], k = 2
Output: 2
```
**Constraints:**
- The length of the array is in range `[1, 20,000]` .
- The range of numbers in the array is `[-1000, 1000]` and the range of the integer `k` is `[-1e7, 1e7]` .

题意：找出元素和等于 $k$ 的连续子数组的个数。

---
思路1：搜索每个子数组，计算它们的和。

代码如下，超出时间限制：
```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int num = 0;
        for (int i = 0; i < nums.size(); ++i) {
            int sum = 0;
            for (int j = i; j < nums.size(); ++j) {
                sum += nums[j];
                if (sum == k) ++num;
            }
        }
        return num;
    }
};
```
---
思路2：为了降低时间复杂度到 $\text{O(N)}$ ，我们需要思考一下。我们的目的是求出和为 `k` 的连续子数组的个数。如何快速求出区间和——用前缀和 `sum(nums[i:j]) = sum(nums[:j]) - sum(nums[:i])` ！但是怎样与题意结合起来呢？我们平时使用前缀和，往往是为了求出连续区间 `[i, j]` 的和，假设其为 `k` ，于是有 `sum(nums[:j]) - sum(nums[i:j]) = sum(nums[:j]) - k = sum(nums[:i])` 这一公式成立。

我们遍历数组 `nums` ，计算从第一个元素到当前元素的和，然后使用**哈希表存储累计和 `sum` 的出现次数**。现在有 `sum(nums[:j])` 的值，同时存储了前面的所有累计和 `sum(nums[:i])` ，对于 `nums[0:j]` 而言，如果这个区间内存在和为 `sum(nums[0:j]) - k = sum(nums[:i])` 的子数组，就说明存在和为 `k` 的子数组，**从当前下标 `j` 往前有连续子数组的和为 `k`** 。和为 `k` 的子数组的个数需要加上 `sum(nums[:i])` 在哈希表中的出现次数。


代码：
```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int num = 0, sum = 0;
        unordered_map<int, int> dict;
        dict[0] = 1;
        for (int i = 0; i < nums.size(); ++i) { 
        	sum += nums[i];
        	num += dict[sum - k];
	        ++dict[sum];
        }
        return num;
    }
};
```
效率：
```cpp
执行用时：116 ms, 在所有 C++ 提交中击败了55.37% 的用户
内存消耗：26.5 MB, 在所有 C++ 提交中击败了20.82% 的用户
```
