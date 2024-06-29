统计一个数字在**排序数组**中出现的次数。

示例 1:
```cpp
输入: nums = [5,7,7,8,8,10], target = 8
输出: 2
```
示例 2:
```cpp
输入: nums = [5,7,7,8,8,10], target = 6
输出: 0
```
**限制：** `0 <= 数组长度 <= 50000`

 
---
思路1：使用哈希表计数，完全没有利用到题目的条件。代码如下：
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        unordered_map<int, int> mp;
        for (const auto i : nums) ++mp[i];
        return mp[target];
    }
};
```
效率：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了89.16% 的用户
内存消耗：13.4 MB, 在所有 C++ 提交中击败了13.27% 的用户
```

---
思路2：顺序扫描，然后计数。代码如下：
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int cnt = 0;
        for (const int i : nums) if (i == target) ++cnt;
        return cnt;
    }
};
```
效率如下，因为有条件判断，可能出现分支预测失败：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了64.38% 的用户
内存消耗：13.2 MB, 在所有 C++ 提交中击败了60.30% 的用户
```
---
思路3：二分找到最左边的值，然后顺序扫描到最右边的值。用 `lower_bound` 函数。这里不实现，复杂度和思路2一样。

---
思路4：二分找到最左边的值的位置，然后二分找到最右边的值的位置，就可以求出出现次数。使用STL的代码如下：
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = lower_bound(nums.begin(), nums.end(), target) - nums.begin();
        int right = upper_bound(nums.begin(), nums.end(), target) - nums.begin();
        return left >= right ? 0 : right - left;
    }
};
```
手写二分如下：
```cpp
class Solution {
public:
    int lower(const vector<int> &nums, int target) {
        int lo = 0, hi = nums.size();
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (nums[mid] >= target)
                hi = mid;
            else 
                lo = mid + 1;
        }
        return lo;
    }
    int upper(const vector<int> &nums, int target) {
        int lo = 0, hi = nums.size();
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (nums[mid] > target)
                hi = mid;
            else 
                lo = mid + 1;
        }
        return lo;
    }
    int search(vector<int>& nums, int target) {
        int left = lower(nums, target), right = upper(nums, target);
        return right - left;
    }
};
```
