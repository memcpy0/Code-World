
Given an array, rotate the array to the right by k steps, where k is non-negative.

**Follow up:**
- Try to come up as many solutions as you can, there are at least `3` different ways to solve this problem.
- Could you do it in-place with `O(1)` extra space?

 

**Example 1:**

```swift
Input: nums = [1,2,3,4,5,6,7], k = 3
Output: [5,6,7,1,2,3,4]
Explanation:
rotate 1 steps to the right: [7,1,2,3,4,5,6]
rotate 2 steps to the right: [6,7,1,2,3,4,5]
rotate 3 steps to the right: [5,6,7,1,2,3,4]
```

**Example 2:**

```swift
Input: nums = [-1,-100,3,99], k = 2
Output: [3,99,-1,-100]
Explanation: 
rotate 1 steps to the right: [99,-1,-100,3]
rotate 2 steps to the right: [3,99,-1,-100]
```

 

**Constraints:**
-  <code>1 <= nums.length <= 2 * 10<sup>4</sup></code>
 -  <code>-2<sup>31</sup> <= nums[i] <= 2<sup>31</sup> - 1</code>
  -  <code>0 <= k <= 10<sup>5</sup></code>




题意：旋转数组，将数组中的元素向右移动 $k$ 个位置，其中 $k$ 非负数。

---
### 解法1 原地工作
设数组 `nums = ab` ，`a` 部分向右移动 `k` 个位置，`b` 部分大小为 `k` ，会被移动到数组前端，得到 `new_nums = ba` 。为了实现这一过程，我们有什么方法呢？

一个性质是：对 `nums` 整体翻转得到的数组 `rev(nums) = rev(b) + rev(a)` 。因此为了得到 `new_nums = ba` ，可以对 `nums` 先进行部分翻转得到 `temp_nums = rev(a) + rev(b)` ，然后 `rev(temp_nums) = rev(rev(b)) + rev(rev(a)) = ba = new_nums` 。具体代码如下：
```cpp
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        if (nums.size() <= 1) return;
        int t = nums.size() - k % nums.size();
        reverse(nums.begin(), nums.begin() + t);
        reverse(nums.begin() + t, nums.end());
        reverse(nums.begin(), nums.end());
    }
};
```
实际效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了86.62% 的用户
内存消耗：10 MB, 在所有 C++ 提交中击败了9.21% 的用户
```
