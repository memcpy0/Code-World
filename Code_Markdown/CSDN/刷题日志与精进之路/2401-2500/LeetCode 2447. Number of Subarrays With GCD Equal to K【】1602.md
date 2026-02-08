> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums` 和一个整数 `k` ，请你统计并返回 `nums` 的子数组中元素的最大公因数等于 `k` 的子数组数目。

**子数组** 是数组中一个连续的非空序列。

**数组的最大公因数** 是能整除数组中所有元素的最大整数。

**示例 1：**

**输入：**nums = [9,3,1,2,6,3], k = 3
**输出：**4
**解释：**nums 的子数组中，以 3 作为最大公因数的子数组如下：
- [9,**_3_**,1,2,6,3]
- [9,3,1,2,6,_**3**_]
- [**_9,3_**,1,2,6,3]
- [9,3,1,2,_**6,3**_]

**示例 2：**

**输入：**nums = [4], k = 7
**输出：**0
**解释：**不存在以 7 作为最大公因数的子数组。

**提示：**

- `1 <= nums.length <= 1000`
- `1 <= nums[i], k <= 10^9`

```cpp
class Solution {
public:
    int subarrayGCD(vector<int>& nums, int k) {
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i) {
            int g = nums[i];
            if (g < k) continue;
            else if (g == k) ++ans;
            for (int j = i + 1; j < n; ++j) {
                g = __gcd(g, nums[j]);
                if (g == k) ++ ans;
                else if (g < k) break;
            }
        }
        return ans;
    }
};
```