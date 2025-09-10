> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 **不包含** 任何零的整数数组 `nums` ，找出自身与对应的负数都在数组中存在的最大正整数 `k` 。

返回正整数 `k` ，如果不存在这样的整数，返回 `-1` 。

**示例 1：**
```java
输入：nums = [-1,2,-3,3]
输出：3
解释：3 是数组中唯一一个满足题目要求的 k 。
```
**示例 2：**
```java
输入：nums = [-1,10,6,7,-7,1]
输出：7
解释：数组中存在 1 和 7 对应的负数，7 的值更大。
```
**示例 3：**
```java
输入：nums = [-10,8,6,7,-2,-3]
输出：-1
解释：不存在满足题目要求的 k ，返回 -1 。
```
**提示：**
-   `1 <= nums.length <= 1000`
-   `-1000 <= nums[i] <= 1000`
-   `nums[i] != 0`

---
### 解法 哈希集合+一次遍历
用一个哈希表记录出现过的数字。一边遍历，一边看 $-\textit{nums}[i]$ 是否在哈希表中，如果在，就更新答案的最大值 。
```java
class Solution {
    public int findMaxK(int[] nums) {
        var rec = new HashSet<Integer>();
        int ans = -1;
        for (int i : nums) {
            if (rec.contains(-i)) ans = Math.max(ans, Math.abs(i));
            rec.add(i);
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$ 