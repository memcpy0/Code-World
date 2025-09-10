> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


给你一个下标从 **0** 开始的整数数组 `nums` 。

- `nums` 的 **最小** 得分是满足 `0 <= i < j < nums.length` 的 `|nums[i] - nums[j]|` 的最小值。
- `nums`的 **最大** 得分是满足 `0 <= i < j < nums.length` 的 `|nums[i] - nums[j]|` 的最大值。
- `nums` 的分数是 **最大** 得分与 **最小** 得分的和。

我们的目标是最小化 `nums` 的分数。你 **最多** 可以修改 `nums` 中 **2** 个元素的值。

请你返回修改 `nums` 中 **至多两个** 元素的值后，可以得到的 **最小分数** 。

`|x|` 表示 `x` 的绝对值。

**示例 1：**

**输入：**nums = [1,4,3]
**输出：**0
**解释：**将 nums[1] 和 nums[2] 的值改为 1 ，nums 变为 [1,1,1] 。`|nums[i] - nums[j]|` 的值永远为 0 ，所以我们返回 0 + 0 = 0 。

**示例 2：**

**输入：**nums = [1,4,7,8,5]
**输出：**3
**解释：**
将 nums[0] 和 nums[1] 的值变为 6 ，nums 变为 [6,6,7,8,5] 。
最小得分是 i = 0 且 j = 1 时得到的 |`nums[i] - nums[j]`| = |6 - 6| = 0 。
最大得分是 i = 3 且 j = 4 时得到的 |`nums[i] - nums[j]`| = |8 - 5| = 3 。
最大得分与最小得分之和为 3 。这是最优答案。

**提示：**

- `3 <= nums.length <= 10^5`
- `1 <= nums[i] <= 10^9`

```cpp
class Solution {
public:
    int minimizeSum(vector<int>& nums) {
        if (nums.size() <= 3) return 0;
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int a1 = nums.back() - nums[2];
        int a2 = nums[n - 3] - nums[0];
        int a3 = nums[n - 2] - nums[1];
        return min(a1, min(a2, a3)); 
    }
};
```
 