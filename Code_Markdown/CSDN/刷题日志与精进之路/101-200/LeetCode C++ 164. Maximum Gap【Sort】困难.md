

Given an unsorted array, find the maximum difference between the successive elements in its sorted form.

Return 0 if the array contains less than 2 elements.

**Example 1:**

```swift
Input: [3,6,9,1]
Output: 3
Explanation: The sorted form of the array is [1,3,6,9], either
             (3,6) or (6,9) has the maximum difference 3.
```

**Example 2:**

```swift
Input: [10]
Output: 0
Explanation: The array contains less than 2 elements, therefore return 0.
```

**Note:**

- You may assume all elements in the array are non-negative integers and fit in the 32-bit signed integer range.
- Try to solve it in **linear time/space**.


题意：给定一个无序的数组，找出数组在排序之后，相邻元素之间最大的差值。如果数组元素个数小于 `2` ，则返回 `0` 。

---
### 解法1 `sort`
不强求 $O(n)$ 时间和空间，可以直接排序，然后计算最大差值。Python代码如下：
```cpp
class Solution:
    def maximumGap(self, nums: List[int]) -> int:
        nums.sort()
        maxDiff = 0
        for i in range(len(nums) - 1):
            maxDiff = max(nums[i + 1] - nums[i], maxDiff)
        return maxDiff
```
对应的执行效率如下：
```py
执行用时：48 ms, 在所有 Python3 提交中击败了73.27% 的用户
内存消耗：14 MB, 在所有 Python3 提交中击败了60.86% 的用户
```
---
### 解法2 桶排序
强制要求线性时空，可以使用桶排序：
```cpp

```
