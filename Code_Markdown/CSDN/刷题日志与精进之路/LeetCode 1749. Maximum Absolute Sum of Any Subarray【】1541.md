> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums` 。一个子数组 `[numsl, numsl+1, ..., numsr-1, numsr]` 的 **和的绝对值** 为 `abs(numsl + numsl+1 + ... + numsr-1 + numsr)` 。

请你找出 `nums` 中 **和的绝对值** 最大的任意子数组（**可能为空**），并返回该 **最大值** 。

`abs(x)` 定义如下：

- 如果 `x` 是负整数，那么 `abs(x) = -x` 。
- 如果 `x` 是非负整数，那么 `abs(x) = x` 。

**示例 1：**

**输入：**nums = [1,-3,2,3,-4]
**输出：**5
**解释：**子数组 [2,3] 和的绝对值最大，为 abs(2+3) = abs(5) = 5 。

**示例 2：**

**输入：**nums = [2,-5,1,-4,3,-2]
**输出：**8
**解释：**子数组 [-5,1,-4] 和的绝对值最大，为 abs(-5+1-4) = abs(-8) = 8 。

**提示：**

- `1 <= nums.length <= 10^5`
- `-10^4 <= nums[i] <= 10^4`

```cpp
class Solution {
public:
    int maxAbsoluteSum(vector<int>& nums) {
        // 最大子数组
        int maxArr = 0, curMax = 0;
        for (int i : nums) {
            curMax = max(curMax + i, i);
            maxArr = max(maxArr, curMax);
        }
        // 最小子数组
        int minArr = 0, curMin = 0;
        for (int i : nums) {
            curMin = min(curMin + i, i);
            minArr = min(minArr, curMin);
        }
        return max(maxArr, abs(minArr));
    }
};
```