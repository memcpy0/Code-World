
Given the array of integers `nums`, you will choose two different indices `i` and `j` of that array. Return the maximum value of `(nums[i] - 1) * (nums[j] - 1)` .

 

**Example 1:**

```swift
Input: nums = [3,4,5,2]
Output: 12 
Explanation: If you choose the indices i=1 and j=2 (indexed from 0), you will get the maximum value, that is, (nums[1]-1)*(nums[2]-1) = (4-1)*(5-1) = 3*4 = 12. 
```

**Example 2:**

```swift
Input: nums = [1,5,4,5]
Output: 16
Explanation: Choosing the indices i=1 and j=3 (indexed from 0), you will get the maximum value of (5-1)*(5-1) = 16.
```

**Example 3:**

```swift
Input: nums = [3,7]
Output: 12
```

 

**Constraints:**
- `2 <= nums.length <= 500`
 - `1 <= nums[i] <= 10^3`

题意：给定一个整数数组 `nums`，选择数组的两个不同下标 `i` 和 `j` ，使 `(nums[i] - 1) * (nums[j] - 1)` 取得最大值并返回。

---
### 解法1 暴力
```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int mv = 0, n = nums.size();
        for (int i = 0; i < n; ++i) 
            for (int j = i + 1; j < n; ++j) 
                mv = max(mv, (nums[i] - 1) * (nums[j] - 1));
        return mv;
    }
};
```
执行效率很低：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了6.69% 的用户
内存消耗：10.2 MB, 在所有 C++ 提交中击败了13.32% 的用户
```

---
### 解法2 排序
```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        sort(nums.begin(), nums.end(), greater<int>());
        return (nums[0] - 1) * (nums[1] - 1);
    }
};
```
执行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了94.49% 的用户
内存消耗：10 MB, 在所有 C++ 提交中击败了33.58% 的用户
```
---
### 解法3 线性比较
```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int max = 0, sec = 0, n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (nums[i] > max) sec = max, max = nums[i];
            else if (nums[i] > sec) sec = nums[i];
        }  
        return (max - 1) * (sec - 1);
    }
};
```
执行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了99.53% 的用户
内存消耗：10.2 MB, 在所有 C++ 提交中击败了12.91% 的用户
```
