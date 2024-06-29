> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a **0-indexed** integer array `nums`, determine whether there exist **two** subarrays of length `2` with **equal** sum. Note that the two subarrays must begin at **different** indices.

Return `true` _if these subarrays exist, and_ `false` _otherwise._

A **subarray** is a contiguous non-empty sequence of elements within an array.

**Example 1:**
```java
Input: nums = [4,2,4]
Output: true
Explanation: The subarrays with elements [4,2] and [2,4] have the same sum of 6.
```
**Example 2:**
```java
Input: nums = [1,2,3,4,5]
Output: false
Explanation: No two subarrays of size 2 have the same sum.
```
**Example 3:**
```java
Input: nums = [0,0,0]
Output: true
Explanation: The subarrays [nums[0],nums[1]] and [nums[1],nums[2]] have the same sum of 0. 
Note that even though the subarrays have the same content, the two subarrays are considered different because they are in different positions in the original array.
```
**Constraints:**
-   `2 <= nums.length <= 1000`
-   `-10^9 <= nums[i] <= 10^9`

---
### 解法 哈希集合
我们只需要对数组进行一次遍历。当遍历到第 $i$ 个元素时，计算 $nums[i]+nums[i+1]$ 的值，并判断该值是否已经出现过即可。判断的操作可以使用一个哈希表，记录已经出现过的值。
```java
class Solution {
    public boolean findSubarrays(int[] nums) {
        HashSet<Long> rec = new HashSet<>();
        for (int i = 0; i < nums.length - 1; ++i) {
            Long sum = (long)nums[i] + nums[i + 1];
            if (rec.contains(sum)) return true;
            rec.add(sum);
        }
        return false;
    }
}
```
 复杂度分析：
 - 时间复杂度：$O(n)$
 - 空间复杂度：$O(n)$
