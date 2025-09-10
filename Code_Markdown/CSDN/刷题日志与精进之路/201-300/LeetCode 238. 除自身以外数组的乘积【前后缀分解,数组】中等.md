> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums`，返回 _数组 `answer` ，其中 `answer[i]` 等于 `nums` 中除 `nums[i]` 之外其余各元素的乘积_ 。

题目数据 **保证** 数组 `nums`之中任意元素的全部前缀元素和后缀的乘积都在  **32 位** 整数范围内。

请 **不要使用除法，**且在 `O(_n_)` 时间复杂度内完成此题。

**示例 1:**
```js
输入: nums = [1,2,3,4]
输出: [24,12,8,6]`
```
**示例 2:**
```js
输入: nums = [-1,1,0,-3,3]
输出: [0,0,9,0,0]
```
**提示：**
- `2 <= nums.length <= 10^5`
- `-30 <= nums[i] <= 30`
- **保证** 数组 `nums`之中任意元素的全部前缀元素和后缀的乘积都在  **32 位** 整数范围内

**进阶：** 你可以在 `O(1)` 的额外空间复杂度内完成这个题目吗？（ 出于对空间复杂度分析的目的，输出数组 **不被视为** 额外空间。）
 
---
我们不考虑「全部乘起来、然后除以对应位置的数」这种做法，因为客观条件限制了这一做法的可行性——数组中可能有零元素。最暴力的方法是使用双重循环，求出每个位置的前缀和后缀的乘积，$\text{O(N}^2\text{)}$ 时间复杂度，$O(1)$ 空间复杂度。结果不出所料地超出时间限制。
```cpp
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int> ans(nums.size(), 1);
        for (int i = 0; i < nums.size(); ++i)  
            for (int j = 0; j < nums.size(); ++j) 
                if (j != i) ans[i] *= nums[j];  
        return ans;
    }
};
```
前后缀分解（右边的数字为难度分）
- [238. 除自身以外数组的乘积](https://leetcode.cn/problems/product-of-array-except-self/) **和本题几乎一样**
- [剑指Offer66. 构建乘积数组](https://leetcode.cn/problems/gou-jian-cheng-ji-shu-zu-lcof/) 和本题几乎一样
- [2256. 最小平均差](https://leetcode.cn/problems/minimum-average-difference/) 1395
- [2483. 商店的最少代价](https://leetcode.cn/problems/minimum-penalty-for-a-shop/) 1495
- [2420. 找到所有好下标](https://leetcode.cn/problems/find-all-good-indices/) 1695
- [2167. 移除所有载有违禁货物车厢所需的最少时间](https://leetcode.cn/problems/minimum-time-to-remove-all-cars-containing-illegal-goods/) 2219
- [2484. 统计回文子序列数目](https://leetcode.cn/problems/count-palindromic-subsequences/) 2223
- [2565. 最少得分子序列](https://leetcode.cn/problems/subsequence-with-the-minimum-score/) 2432
- [2552. 统计上升四元组](https://leetcode.cn/problems/count-increasing-quadruplets/) 2433
- [42. 接雨水](https://leetcode.cn/problems/trapping-rain-water/)
### 解法 前后缀分解
我们学过「**前缀和**」，依此类推，可以得到「**前缀积**」和「**后缀积**」的概念。回过头再看题目，发现题意分明是要我们把 `nums[i]` 的前缀积和后缀积求出来并相乘，这样就能达到 $O(n)$ 的时间复杂度。

下面是容易理解的一版代码：
```cpp
//C++ version
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size(), prefix[n], suffix[n];
        prefix[0] = 1, suffix[n - 1] = 1;
        for (int i = 1; i < n; ++i)
            prefix[i] = prefix[i - 1] * nums[i - 1];
        for (int i = n - 2; i >= 0; --i) 
            suffix[i] = suffix[i + 1] * nums[i + 1];
        vector<int> ans(n);
        ans[0] = suffix[0], ans[n - 1] = prefix[n - 1];
        for (int i = 1; i < n - 1; ++i) 
            ans[i] = prefix[i] * suffix[i];
        return ans;
    }
};
//执行用时：24 ms, 在所有 C++ 提交中击败了52.55% 的用户
//内存消耗：23.9 MB, 在所有 C++ 提交中击败了40.32% 的用户
```
通过使用临时变量，可以**在排除输出数组的情况下，只使用常量的空间**。
```cpp
//C++ version
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(n); 
        for (int i = 0, l = 1; i < n; l *= nums[i++]) ans[i] = l;
        for (int i = n - 1, r = 1; i >= 0; r *= nums[i--]) ans[i] *= r;
        return ans;
    }
};
//执行用时：20 ms, 在所有 C++ 提交中击败了76.92% 的用户
//内存消耗：23.3 MB, 在所有 C++ 提交中击败了91.12% 的用户
```


