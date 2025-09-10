> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

> 相关公司：亚马逊、彭博、谷歌
 
Given an integer array `nums`, return _the most frequent even element_.

If there is a tie, return the **smallest** one. If there is no such element, return `-1`.

**Example 1:**
```java
Input: nums = [0,1,2,2,4,4,1]
Output: 2
Explanation:
The even elements are 0, 2, and 4. Of these, 2 and 4 appear the most.
We return the smallest one, which is 2.
```
**Example 2:**
```java
Input: nums = [4,4,4,9,2,4]
Output: 4
Explanation: 4 is the even element appears the most.
```
**Example 3:**
```java
Input: nums = [29,47,21,41,13,37,25,7]
Output: -1
Explanation: There is no even element.
```
**Constraints:**
-   `1 <= nums.length <= 2000`
-   `0 <= nums[i] <= 10^5`

题意：给你一个整数数组 `nums` ，返回出现最频繁的偶数元素。如果存在多个满足条件的元素，只需要返回 **最小** 的一个。如果不存在这样的元素，返回 `-1` 。

---
### 解法 哈希表计数+一次遍历
遍历数组 $nums$ ，并且使用哈希表 $count$ 记录偶数元素的出现次数。然后统计最大频率跟最大频率的最小偶数：
```java
class Solution {
    public int mostFrequentEven(int[] nums) {
        var cnt = new HashMap<Integer, Integer>();
        int mx = -1, ans = -1;
        for (int i : nums) {
            if (i % 2 == 0) {
                int t = cnt.getOrDefault(i, 0) + 1;
                if (t > mx) {
                    mx = t;
                    ans = i;
                } else if (t == mx) ans = Math.min(ans, i);
                cnt.put(i,  t);
            }
        }
        return ans;
    }
}
```
