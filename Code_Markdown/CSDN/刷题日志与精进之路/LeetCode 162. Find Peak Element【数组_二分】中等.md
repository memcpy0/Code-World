> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>A peak element is an element that is strictly greater than its neighbors.</p>

<p>Given an integer array <code>nums</code>, find a peak element, and return its index. If the array contains multiple peaks, return the index to <strong>any of the peaks</strong>.</p>

<p>You may imagine that <code>nums[-1] = nums[n] = -∞</code>.</p>

<p>You must write an algorithm that runs in <code>O(log n)</code> time.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: nums = [1,2,3,1]
Output: 2
Explanation: 3 is a peak element and your function should return the index number 2.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: nums = [1,2,1,3,5,6,4]
Output: 5
Explanation: Your function can return either index number 1 where the peak element is 2, or index number 5 where the peak element is 6.
```
<p><strong>Constraints:</strong></p>
<ul>
	<li><code>1 &lt;= nums.length &lt;= 1000</code></li>
	<li><code>-2<sup>31</sup> &lt;= nums[i] &lt;= 2<sup>31</sup> - 1</code></li>
	<li><code>nums[i] != nums[i + 1]</code> for all valid <code>i</code>.</li>
</ul>

题意：峰值元素是指其值严格大于左右相邻值的元素。给你一个元素互不相同的整数数组 <code>nums</code>，找到峰值元素并返回其索引。数组可能包含多个峰值，在这种情况下，返回 <strong>任何一个峰值</strong> 所在位置即可。假设 <code>nums[-1] = nums[n] = -∞</code> 。必须实现时间复杂度为 <code>O(log n)</code><em> </em>的算法来解决此问题。

---
本题是[852. Peak Index in a Mountain Array](https://leetcode-cn.com/problems/peak-index-in-a-mountain-array/submissions/)（对应题解见[852. Peak Index in a Mountain Array【二分/三分/数组】简单](https://memcpy0.blog.csdn.net/article/details/109695803)）的加强版。
### 解法1 顺序遍历
只需要找到一个下标 `i` ，其对应的元素值 `arr[i - 1] < arr[i] && arr[i] > arr[i + 1]` 即可，为了可以顺序遍历整个数组 。实际代码如下，算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        if (nums.size() == 1) return 0;
        for (int i = 0, n = nums.size(); i < n; ++i) {
            if (i > 0 && i < n - 1) {
                if (nums[i] > nums[i - 1] && nums[i] > nums[i + 1]) return i;
            } else if (i == 0) {
                if (nums[i] > nums[i + 1]) return 0;
            } else {
                if (nums[i] > nums[i - 1]) return n - 1;
            }
        }
        return -1;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了82.94% 的用户
//内存消耗：8.6 MB, 在所有 C++ 提交中击败了71.82% 的用户
```
不过这样写就太过繁琐了，由于 <code>nums[-1] = nums[n] = -∞</code> ，所以我们一直往右遍历，**遇到的第一个大于 `nums[i + 1]` 的 `nums[i]` 就是一个峰值元素**——`nums[i]` 左侧都是前一个元素小于后一个元素（即单调递增），`nums[i]` 右侧则呈现递减趋势，于是 `nums[i]` 为此处山峰段的峰值元素。
```cpp
//C++ version
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int i = 0, n = nums.size();
        while (i < n - 1 && nums[i] < nums[i + 1]) ++i;
        return i;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：8.5 MB, 在所有 C++ 提交中击败了92.29% 的用户
```

---
### 解法2 二分
将数组看做由多个山峰段组成，当前下标位置假设为 `i` ，则有：
- 如果 `nums[i] > nums[i + 1]` ，说明 `i` 处于某个山峰段的右侧，`i` 及其左侧一定存在峰值元素
- 如果 `nums[i] <= nums[i + 1]` ，说明 `i` 处于某个山峰段的左侧，`i` 的右侧一定存在峰值元素

实际代码如下。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ 。一个可能的疑惑是， `nums[mid] > nums[mid + 1]` 不用担心数组越界吗？想一下就会明白，这种情况不可能发生，假设只有一个峰值元素：
- 如果峰值元素在数组中间，此时不会越界；
- 如果峰值元素在数组开头，此时不会越界；
- 如果峰值元素在数组末尾，则由于 `l + (r - l) / 2` 总是取较小的 `mid` 值，所以总是存在 `mid + 1` ，且一直是 `nums[mid] <= nums[mid + 1]` ，于是 `l` 也不断右移直至抵达数组末尾
```cpp
//C++ version
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int l = 0, r = nums.size() - 1;
        while (l < r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] > nums[mid + 1]) r = mid; // mid落在一个山峰的右侧,mid可能是答案
            else l = mid + 1; // nums[mid]<nums[mid+1](不会取等),mid落在一个山峰的左侧且不可能是答案
        }
        return l;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：8.4 MB, 在所有 C++ 提交中击败了99.16% 的用户
```
或者写成如下形式。仍然假设只有一个峰值元素、且峰值元素在数组开头，则由于 `l + (r - l + 1) / 2` 总是取较大的 `mid` 值，所以总是存在 `mid - 1` ，并且一直是 `nums[mid] <= nums[mid - 1]` ，于是 `r` 也不断左移直至抵达数组开头：
```cpp
//C++ version
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int l = 0, r = nums.size() - 1;
        while (l < r) {
            int mid = l + (r - l + 1) / 2;
            if (nums[mid] > nums[mid - 1]) l = mid; // mid落在一个山峰的左侧,mid可能是答案
            else r = mid - 1; // nums[mid]<nums[mid-1](不会取等),mid落在一个山峰的右侧且不可能是答案
        }
        return l;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了82.94% 的用户
//内存消耗：8.6 MB, 在所有 C++ 提交中击败了48.50% 的用户
```
