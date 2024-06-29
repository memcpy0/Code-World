 >本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array of positive integers `arr` (not necessarily distinct), return _the_ _lexicographically_ _largest permutation that is smaller than_ `arr`, that can be **made with exactly one swap**. If it cannot be done, then return the same array.

**Note** that a _swap_ exchanges the positions of two numbers `arr[i]` and `arr[j]`

**Example 1:**
```java
Input: arr = [3,2,1]
Output: [3,1,2]
Explanation: Swapping 2 and 1.
```
**Example 2:**
```java
Input: arr = [1,1,5]
Output: [1,1,5]
Explanation: This is already the smallest permutation.
```

**Example 3:**
```java
Input: arr = [1,9,4,6,7]
Output: [1,7,4,6,9]
Explanation: Swapping 9 and 7.
```
**Constraints:**
-   `1 <= arr.length <= 10^4`
-   `1 <= arr[i] <= 10^4`

题意：给你一个正整数数组 `arr`（可能存在重复的元素），返回可在 **一次交换**（交换两数字 `arr[i]` 和 `arr[j]` 的位置）后得到的、按字典序排列小于 `arr` 的最大排列。如果无法这么操作，就请返回原数组。

---
### 解法 贪心
记数组 $arr$ 的长度为 $n$ ，对于 $0≤i<j<n$ ，如果交换 $arr[i]$  和 $arr[j]$ 后得到的新数组按字典序排列比原数组小，显然有 $arr[i]>arr[j]$ 成立。因此，**符合题意要求的交换会使得数组 $arr$ 在下标 $i$ 处的元素变小**。那么，为了得到「按字典序排列小于原数组的最大新数组」，**尽可能地保持前面的元素不变**是最优的，即让 $i$ 最大化。
- 如何最大化 $i$ ？可**从后往前枚举 $i \in [0, n - 2]$** ，然后枚举 $j∈[i+1,n)$ ，**如果存在 $j$ 使 $arr[i]>arr[j]$ 成立，那么说明当前枚举的 $i$ 是最大化的**。
- 这里只需要判断 $arr[i]>arr[i+1]$ 是否成立即可（即存在一个下标 $j>i$ 使得 $arr[i] > arr[j]$ 成立），这样可知 $arr[i+1]$ 及以后的元素是非递减的，即 $arr[i+1]$ 是 $arr[i]$ 后面的最小元素。当我们枚举 $i$ 时，显然不存在 $k>i$ 使 $arr[k]>arr[k+1]$ 成立，因此 ${arr}[i]$ 后面的元素是符合非递减的。
- $i$ 最大化后，$j∈[i+1,n)$ 应该怎么选择？显然**在满足 $arr[j]<arr[i]$ 的条件下，取最大的 $arr[j]$ 是最优的**。但题目并没有元素不重复的要求，最大的 $arr[j]$ 可能有重复值，那么**选择其中下标最小的 $arr[j]$ 是最优的**，从而令前一个排列最大化。
- 由前面推导可知，区间 $[i+1,n)$ 的元素是非递减的，因此我们从大到小、从后往前枚举 $j$ ，直到 $arr[j]<arr[i]$ 且 $arr[j]≠arr[j−1]$ 成立，那么得到的 $j$ 就是符合要求的。交换 $arr[i]$ 和 $arr[j]$ 。
```cpp
class Solution {
public:
    vector<int> prevPermOpt1(vector<int>& arr) {
        int n = arr.size(), i;
        for (i = n - 2; i >= 0 && arr[i] <= arr[i + 1]; --i);
        if (i == -1) return arr;
        int j;
        for (j = n - 1; arr[j] >= arr[i] || arr[j] == arr[j - 1]; --j);
        swap(arr[i], arr[j]);
        return arr;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是数组 $arr$ 的长度。查找 $i$ 需要 $O(n)$ 的时间复杂度，查找 $j$ 需要 $O(n)$ 的时间复杂度。
- 空间复杂度：$O(1)$ 。返回值不计入空间复杂度。
 