> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的整数数组，每次操作将会使 `n - 1` 个元素增加 `1` 。返回让数组所有元素相等的最小操作次数。

**示例 1：**
```js
输入：nums = [1,2,3]
输出：3
解释：
只需要3次操作（注意每次操作会增加两个元素的值）：
[1,2,3]  =>  [2,3,3]  =>  [3,4,3]  =>  [4,4,4]
```
**示例 2：**
```js
输入：nums = [1,1,1]
输出：0
```
**提示：**
- `n == nums.length`
- `1 <= nums.length <= 10^5`
- `-10^9 <= nums[i] <= 10^9`
- 答案保证符合 **32-bit** 整数

---
题目集合：
- [453. 最小操作次数使数组元素相等](https://leetcode.cn/problems/minimum-moves-to-equal-array-elements/)
- [456. 最小操作次数使数组元素相等 II](https://leetcode.cn/problems/minimum-moves-to-equal-array-elements-ii/)
- [2448. 使数组相等的最小开销](https://leetcode.cn/problems/minimum-cost-to-make-array-equal/)
### 解法1 逆向思维
**对一个非空整数数组中的 $n - 1$ 个数加一，等价于对剩下的那个数减一**。于是我们每次对数组中的一个数（往往是当前最大的数）进行减一。因为只能做减法，所以数组最后剩下的不会被减一的数必然是最小值，此时所有的数都等于最小值。因此，**每个元素减去最小值的差**之和，就是需要的最少操作次数。
```cpp
class Solution {
public:
    int minMoves(vector<int>& nums) {
        int mn = *min_element(nums.begin(),nums.end()), ans = 0;
        for (const int &v : nums) ans += v - mn;
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为数组中的元素数量。我们需要一次遍历求出最小值，一次遍历计算操作次数。
- 空间复杂度：$O(1)$ 。

---
### 解法2 数学
设最后数组中所有数都变为 $x$（$x$ 就是最后一次操作之前整个数组的**最大值**；因为操作只会让值变大），于是整个数组的和为 $x \times n$ 。
- 初始整个数组的和等于 $\sum(nums)$ 
- 我们对数组的每次操作，就相当于让数组的和 $+= (n - 1)$ 。

设最少有 $m$ 次操作，可知最终 $\sum(nums) + m \times (n - 1) = x \times n$ 。如何求出 $m$ 呢？

我们需要再设初始数组最小的元素为 $minVal$ ，将 $minVal$ 不断加一得到 $x$ 所执行的操作次数就是 $m$ ，即 $m = x - minVal$ ，即 $x = minVal + m$ 。于是有：$$\sum(nums) + m \times (n - 1) = minVal \times n + m \times n$$ 即 $\sum(nums) - minVal \times n = m$ 。

具体代码如下，需要注意**溢出问题**：
```cpp
class Solution {
public:
    int minMoves(vector<int>& nums) {
        long long mn = *min_element(nums.begin(), nums.end()), sum = accumulate(nums.begin(), nums.end(), 0);
        return sum - mn * nums.size();
    }
};
```
