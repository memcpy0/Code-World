

Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand. (i.e.,  `[0,1,2,4,5,6,7]` might become  `[4,5,6,7,0,1,2]` ).

Find the **minimum** element. You may assume **no duplicate** exists in the array.

**Example 1:**

```swift
Input: [3,4,5,1,2] 
Output: 1
```

**Example 2:**

```swift
Input: [4,5,6,7,0,1,2]
Output: 0
```

题意：假设按照升序排序的数组在预先未知的某个点上进行了旋转。请找出其中最小的元素，本题假设数组中不存在重复元素。

---
### 解法1 顺序查找
```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        int minv = INT_MAX;
        for (const int &v : nums) minv = min(minv, v);
        return minv;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了91.23% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了5.07% 的用户
```
---
### 解法2 二分
旋转的升序数组，其特征在于：**任选一个位置，这一位置两边必有一边是有序区，另一边是旋转的升序数组**。如果这个位置落入未向右旋转的有序区，则左边一定有序；如果这个位置落入了向右旋转的有序区，则右边一定有序。因此通过判断有序区的存在，然后不断二分查找，就可以得到最小值。
```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        if (nums.size() == 1) return nums[0];
        int lo = 0, hi = nums.size() - 1;
        while (lo < hi) { //[lo,hi]会先排除未向右旋转的原数组后部区域
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] < nums[hi]) hi = mid; //右侧是有序区且一定大于mid,不是最小值; mid可能是最小值
            else lo = mid + 1;                  //左侧是有序区且一定小于mid; mid肯定不是最小值
        } //然后[lo,hi]搜索到有序区中,在有序区中需要向左查找最小值 
        return nums[lo];
    }
};
```
这样看来，这道题是一道二分模板题目：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了52.68% 的用户
内存消耗：10.2 MB, 在所有 C++ 提交中击败了5.01% 的用户
```

