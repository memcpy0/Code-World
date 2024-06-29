> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091353608.png)

You are given an integer array `nums` sorted in ascending order, and an integer `target` .

Suppose that `nums` is rotated at some pivot unknown to you beforehand (i.e., `[0,1,2,4,5,6,7]` might become `[4,5,6,7,0,1,2]` ).

*If `target` is found in the array return its index, otherwise, return `-1` .*

 

**Example 1:**

```swift
Input: nums = [4,5,6,7,0,1,2], target = 0
Output: 4
```

**Example 2:**

```swift
Input: nums = [4,5,6,7,0,1,2], target = 3
Output: -1
```

**Example 3:**

```swift
Input: nums = [1], target = 0
Output: -1
```

 

**Constraints:**
- `1 <= nums.length <= 5000`
- `-10^4 <= nums[i] <= 10^4`
- All values of `nums` are **unique**.
- `nums` is guranteed to be rotated at some pivot.
- `-10^4 <= target <= 10^4`

题意：给出一个升序排序的数组和一个整数，这一数组在未知的某个点上进行了旋转，如 `[1,2,3,4,5,6,7]` 可能变为 `[4,5,6,7,1,2,3]` 。然后需要在数组中搜索目标值，如果存在则返回它的位置。


进阶题目：[81. 搜索旋转排序数组 II](https://leetcode-cn.com/problems/search-in-rotated-sorted-array-ii/)
153. 寻找旋转排序数组中的最小值;

----
### 解法1 顺序搜索
完全没有利用到数组的特征，直接进行最简单的顺序搜索，$O(n)$ 的时间复杂度：
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        for (int i = 0; i < nums.size(); ++i) if (nums[i] == target) return i;
        return -1;
    }
};
```
效率如下，比较感人：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了17.24% 的用户
内存消耗：11.3 MB, 在所有 C++ 提交中击败了5.21% 的用户
```
---
### 解法2 找到旋转点
我们可以找到那个旋转点，不是进行数组的恢复（太浪费时间了），而是经过判断，对旋转点的一边进行二分搜索，得到结果。这一算法的整体时间复杂度是 $O(n)$ ，没有实质的改进。暂时不想写代码。

---
### 解法3 利用数组的特征
对于旋转数组，如果将其一分为二，则其中一定有一个是有序的，另一个可能有序也可能无序。对有序部分用二分查找，无序部分再一分为二……这样循环。具体代码如下：
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) { 
        int lo = 0, hi = nums.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] == target) return mid;
            else if (nums[mid] < nums[hi]) { //右半段是有序的
                if (nums[mid] < target && target <= nums[hi]) lo = mid + 1; //对右半段二分搜索
                else hi = mid - 1; //右半段有序,但不在右半段的范围内,因此对左半段搜索
            } else { //左半段有序
                if (target >= nums[lo] && target < nums[mid]) hi = mid - 1; //对左半段二分搜索
                else lo = mid + 1; //左半段有序,但不在左半段的范围内,因此对右半段搜索
            }
        }
        return -1;
    }
};
```
提交后效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了82.70% 的用户
内存消耗：11.2 MB, 在所有 C++ 提交中击败了5.21% 的用户
```
递归版代码如下：
```cpp
class Solution {
private:
    int binarySearchInRotatedSortedArray(const vector<int>& a, int l, int r, int t) {
        if (l > r) return -1;
        int mid = l + (r - l) / 2;
        if (a[mid] == t) return mid;
        else if (a[mid] < a[r]) { //右半段有序
            return (a[mid] < t && t <= a[r]) ?
                binarySearchInRotatedSortedArray(a, mid + 1, r, t) : 
                binarySearchInRotatedSortedArray(a, l, mid - 1, t);
        } else { //左半段有序
            return (t < a[mid] && t >= a[l]) ?
                binarySearchInRotatedSortedArray(a, l, mid - 1, t) :
                binarySearchInRotatedSortedArray(a, mid + 1, r, t);
        }
    }
public:
    int search(vector<int>& nums, int target) { 
        return binarySearchInRotatedSortedArray(nums, 0, nums.size() - 1, target);
    }
};
```
递归版的效率低一些：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了45.60% 的用户
内存消耗：11.4 MB, 在所有 C++ 提交中击败了5.21% 的用户
```



对于有序数组，可以使用二分查找的方法查找元素。

但是这道题中，数组本身不是有序的，进行旋转后只保证了数组的局部是有序的，这还能进行二分查找吗？答案是可以的。

可以发现的是，我们将数组从中间分开成左右两部分的时候，一定有一部分的数组是有序的。拿示例来看，我们从 6 这个位置分开以后数组变成了 [4, 5, 6] 和 [7, 0, 1, 2] 两个部分，其中左边 [4, 5, 6] 这个部分的数组是有序的，其他也是如此。

这启示我们可以在常规二分查找的时候查看当前 mid 为分割位置分割出来的两个部分 [l, mid] 和 [mid + 1, r] 哪个部分是有序的，并根据有序的那个部分确定我们该如何改变二分查找的上下界，因为我们能够根据有序的那部分判断出 target 在不在这个部分：

如果 [l, mid - 1] 是有序数组，且 target 的大小满足 [nums[l],nums[mid])[\textit{nums}[l],\textit{nums}[mid])[nums[l],nums[mid])，则我们应该将搜索范围缩小至 [l, mid - 1]，否则在 [mid + 1, r] 中寻找。
如果 [mid, r] 是有序数组，且 target 的大小满足 (nums[mid+1],nums[r]](\textit{nums}[mid+1],\textit{nums}[r]](nums[mid+1],nums[r]]，则我们应该将搜索范围缩小至 [mid + 1, r]，否则在 [l, mid - 1] 中寻找。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307041424544.png)

需要注意的是，二分的写法有很多种，所以在判断 target 大小与有序部分的关系的时候可能会出现细节上的差别。

C++
Java
Python3
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int n = (int)nums.size();
        if (!n) {
            return -1;
        }
        if (n == 1) {
            return nums[0] == target ? 0 : -1;
        }
        int l = 0, r = n - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (nums[mid] == target) return mid;
            if (nums[0] <= nums[mid]) {
                if (nums[0] <= target && target < nums[mid]) {
                    r = mid - 1;
                } else {
                    l = mid + 1;
                }
            } else {
                if (nums[mid] < target && target <= nums[n - 1]) {
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
        }
        return -1;
    }
};
复杂度分析

时间复杂度： O(log⁡n)O(\log n)O(logn)，其中 nnn 为 nums\textit{nums}nums 数组的大小。整个算法时间复杂度即为二分查找的时间复杂度 O(log⁡n)O(\log n)O(logn)。

空间复杂度： O(1)O(1)O(1) 。我们只需要常数级别的空间存放变量。

下面这种写法会出错：[4,5,6,7,8,1,2,3] 8， -> -1 而非预期的4
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int l = 0, r = nums.size() - 1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (nums[m] == target) return m;
            else if (nums[m] < target) {
                if (nums[m] < nums[r] && target <= nums[r]) l = m + 1;
                else r = m - 1;
            } else {
                if (nums[m] >= nums[l] && target >= nums[l]) r = m - 1;
                else l = m + 1;
            }
        }
        return -1;
    }
};