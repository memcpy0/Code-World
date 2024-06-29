> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a **sorted** (in ascending order) integer array `nums` of `n` elements and a `target` value, write a function to search `target` in nums. If `target` exists, then return its index, otherwise return `-1` .


**Example 1:**

```swift
Input: nums = [-1,0,3,5,9,12], target = 9
Output: 4
Explanation: 9 exists in nums and its index is 4
```

**Example 2:**

```swift
Input: nums = [-1,0,3,5,9,12], target = 2
Output: -1
Explanation: 2 does not exist in nums so return -1
```

 

**Note:**
- You may assume that all elements in `nums` are unique.
 -  `n` will be in the range `[1, 10000]` .
-  The value of each element in `nums` will be in the range `[-9999, 9999]` .

 
题意：给定一个 `n` 个元素的升序整型数组 `nums` 和一个目标值 `target` ，写一个函数搜索 `nums` 中的 `target` ，如果目标值存在返回下标，否则返回 `-1` 。

---
### 解法1：使用STL
顺序查找的方法就不说了，这里使用STL的 `lower_bound` 函数，代码如下：
```cpp
//C++ version
class Solution {
public:
    int search(vector<int>& nums, int target) {
        auto it = lower_bound(nums.begin(), nums.end(), target);
        if (it == nums.end()) return -1;
        return *it == target ? it - nums.begin() : -1;
    }
};
//执行用时：76 ms, 在所有 C++ 提交中击败了93.84% 的用户
//内存消耗：25.2 MB, 在所有 C++ 提交中击败了10.49% 的用户
```

---
### 解法2 手写二分
二分题目的关键性在于，把握题目的「单调性」，想清楚二分的是哪个边界，而不是局限在用什么模板、大于或小于上面。
```cpp
//C++ version
//最普通的二分查找
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int lo = 0, hi = nums.size() - 1;
        while (lo <= hi) {
            int mid = lo + ((hi - lo) >> 1);
            if (nums[mid] == target) return mid;
            else if (nums[mid] > target) hi = mid - 1;
            else lo = mid + 1;
        }
        return -1;
    }
};
//执行用时：88 ms, 在所有 C++ 提交中击败了62.16% 的用户
//内存消耗：25.1 MB, 在所有 C++ 提交中击败了55.62% 的用户
```
修改一下范围，以左闭右开区间 `[lo, hi)` 进行搜索：
```cpp
//C++ version
//最普通的二分查找
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int lo = 0, hi = nums.size();
        while (lo < hi) {
            int mid = lo + ((hi - lo) >> 1);
            if (nums[mid] == target) return mid;
            else if (nums[mid] > target) hi = mid;
            else lo = mid + 1;
        }
        return -1;
    }
};
//执行用时：72 ms, 在所有 C++ 提交中击败了95.90% 的用户
//内存消耗：25.2 MB, 在所有 C++ 提交中击败了20.34% 的用户
```
此外，还可以二分升序数组的下界，即查找第一个大于等于 `target` 的元素的位置：
```cpp
//C++ version
//二分下界
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int lo = 0, hi = nums.size();
        while (lo < hi) {
            int mid = lo + hi >> 1;
            if (nums[mid] >= target) hi = mid; 
            else lo = mid + 1;  
        }
        return (lo == nums.size() || nums[lo] != target) ? -1 : lo;
    }
}; 
//执行用时：32 ms, 在所有 C++ 提交中击败了72.07% 的用户
//内存消耗：26.9 MB, 在所有 C++ 提交中击败了33.23% 的用户
```
由于不用返回要插入的位置，二分的解的范围可以缩小到 `[0, nums.size() - 1]` ，也就不用判断 `lo` 是否等于 `nums.size()`（即插入到数组尾部）：
```cpp
//C++ version
//二分下界
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int lo = 0, hi = nums.size() - 1;
        while (lo < hi) {
            int mid = lo + hi >> 1;
            if (nums[mid] >= target) hi = mid; 
            else lo = mid + 1;  
        }
        return nums[lo] != target ? -1 : lo;
    }
};
//执行用时：40 ms, 在所有 C++ 提交中击败了29.53% 的用户
//内存消耗：27 MB, 在所有 C++ 提交中击败了5.82% 的用户
```
还可以二分升序数组的上界，即查找最后一个小于等于 `target` 的元素的位置：
```cpp
//C++ version
//二分上界
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int lo = 0, hi = nums.size() - 1; //题解范围到[0,n),但是插入范围为[0,n],插入时特判结果下标为0的位置
        while (lo < hi) {
            int mid = lo + hi + 1 >> 1; //注意+1
            if (nums[mid] <= target) lo = mid; //找到最后一个<=target的元素位置
            else hi = mid - 1;
        }
        return nums[lo] == target ? lo : -1;
    }
};
//执行用时：32 ms, 在所有 C++ 提交中击败了72.07% 的用户
//内存消耗：26.8 MB, 在所有 C++ 提交中击败了84.80% 的用户
```
二分的写法多种多样，在升序数组中还可以二分查找第一个大于 `target` 的元素的位置、最后一个小于 `target` 的元素的位置。
