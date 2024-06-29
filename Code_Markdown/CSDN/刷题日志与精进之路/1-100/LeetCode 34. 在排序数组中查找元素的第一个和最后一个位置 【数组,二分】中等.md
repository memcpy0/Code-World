> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091122225.png)

Given an array of integers nums sorted **in ascending order**, find the starting and ending position of a given target value.

Your algorithm's runtime complexity must be in the order of `O(log n)` .

If the target is not found in the array, return `[-1, -1]` .

**Example 1:**
```cpp
Input: nums = [5,7,7,8,8,10], target = 8
Output: [3,4]
```
**Example 2:**
```cpp
Input: nums = [5,7,7,8,8,10], target = 6
Output: [-1,-1]
```
**Constraints:**
-  `0 <= nums.length <= 10^5`
 -  `-10^9 <= nums[i] <= 10^9`
 -   `nums` is a non decreasing array.
 -   `-10^9 <= target <= 10^9`

题意：在排序数组中确定数字 `target` 出现的左右边界。

---
直观的思路肯定是从前往后遍历一遍。用两个变量记录第一次和最后一次遇见 $target$ 的下标，但这个方法的时间复杂度为 $O(n)$ ，没有利用到数组升序排列的条件。

由于数组已经排序，因此整个数组是单调递增的，我们可以利用二分法来加速查找的过程。
### 解法1 STL
用STL的二分查找函数。代码如下：
```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) { 
        int left = lower_bound(nums.begin(), nums.end(), target) - nums.begin();
        int right = upper_bound(nums.begin(), nums.end(), target) - nums.begin() - 1;
        if (left > right) return {-1, -1};
        return vector<int>{left, right};
    }
};
```
---
### 解法2 手写二分上下界函数
考虑 $target$ 开始和结束位置，其实我们要找的就是数组中「第一个等于 $target$ 的位置」（记为 $leftIdx$）和「第一个大于 $target$ 的位置减一」（记为 $\textit{rightIdx}$）。

二分查找中，寻找 $leftIdx$ 即为在数组中寻找第一个大于等于 $target$ 的下标，寻找 $\textit{rightIdx}$ 即为在数组中寻找第一个大于 $target$ 的下标，然后将下标减一。

最后，因为 $target$ 可能不存在数组中，因此我们需要重新校验得到的两个下标 $leftIdx$ 和 $rightIdx$ ，看是否符合条件，如果符合条件就返回 $[leftIdx,rightIdx]$ ，不符合就返回 $[-1,-1]$ 。
```cpp
class Solution {
private: 
    //找到第一个>=v的元素的位置
    int lowerBound(vector<int>& A, int x, int y, int v) {
        while (x < y) {
            int m = x + (y - x) / 2;
            if (A[m] >= v) y = m;
            else x = m + 1;
        }
        return x;
    }
    //找到第一个>v的元素的位置
    int upperBound(vector<int>& A, int x, int y, int v) {
        while (x < y) {
            int m = x + (y - x) / 2;
            if (A[m] > v) y = m;
            else x = m + 1;
        }
        return x;
    }
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.empty()) return {-1, -1}; //特判空
        int n = nums.size();
        int first = lowerBound(nums, 0, n, target); //第一个<=
        if (first >= n || nums[first] != target) return vector<int>{-1, -1}; //特判不存在
        int last = upperBound(nums, 0, n, target); //第一个< 
        return vector<int>{first, last - 1}; 
    }
};
```
复杂度分析：
- 时间复杂度：$O(\log n)$ ，其中 $n$ 为数组的长度。二分查找的时间复杂度为 $O(\log n)$ ，一共会执行两次，因此总时间复杂度为 $O(\log n)$ 。
- 空间复杂度：$O(1)$ 。只需要常数空间存放若干变量。