Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand. (i.e., `[0,1,2,4,5,6,7]` might become  `[4,5,6,7,0,1,2]`).

Find the **minimum** element. The array may contain duplicates.

**Example 1:**

```haskell
Input: [1,3,5]
Output: 1
```

**Example 2:**

```haskell
Input: [2,2,2,0,1]
Output: 0
```

**Note:**
- This is a follow up problem to `Find Minimum in Rotated Sorted Array` .
 - Would allow duplicates affect the run-time complexity? How and why?

题意：请找出旋转数组中最小的元素。注意数组中可能存在重复的元素。

---
### 解法1 顺序查找
直接使用 `STL` ：
```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        return *min_element(nums.begin(), nums.end());
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了93.04% 的用户
内存消耗：12.3 MB, 在所有 C++ 提交中击败了21.03% 的用户
```
