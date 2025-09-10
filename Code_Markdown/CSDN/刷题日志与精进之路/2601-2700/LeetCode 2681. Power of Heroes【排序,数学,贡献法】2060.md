> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的整数数组 `nums` ，它表示英雄的能力值。如果我们选出一部分英雄，这组英雄的 **力量** 定义为：
- `i0` ，`i1` ，... `ik` 表示这组英雄在数组中的下标。那么这组英雄的力量为 `max(nums[i0],nums[i1] ... nums[ik])2 * min(nums[i0],nums[i1] ... nums[ik])` 。

请你返回所有可能的 **非空** 英雄组的 **力量** 之和。由于答案可能非常大，请你将结果对 `109 + 7` **取余。**

**示例 1：**
```java
输入：nums = [2,1,4]
输出：141
解释：
第 1 组：[2] 的力量为 22 * 2 = 8 。
第 2 组：[1] 的力量为 12 * 1 = 1 。
第 3 组：[4] 的力量为 42 * 4 = 64 。
第 4 组：[2,1] 的力量为 22 * 1 = 4 。
第 5 组：[2,4] 的力量为 42 * 2 = 32 。
第 6 组：[1,4] 的力量为 42 * 1 = 16 。
第​ ​​​​​​7 组：[2,1,4] 的力量为 42​​​​​​​ * 1 = 16 。
所有英雄组的力量之和为 8 + 1 + 64 + 4 + 32 + 16 + 16 = 141 。
```
**示例 2：**
```java
输入：nums = [1,1,1]
输出：7
解释：总共有 7 个英雄组，每一组的力量都是 1 。所以所有英雄组的力量之和为 7 。
```
**提示：**
- `1 <= nums.length <= 10^5`
- `1 <= nums[i] <= 10^9`

---
贡献法，与本题类似的但不同的：
- [907. 子数组的最小值之和](https://leetcode.cn/problems/sum-of-subarray-minimums/)
- [1508. 子数组和排序后的区间和](https://leetcode.cn/problems/range-sum-of-sorted-subarray-sums/)
- [1856. 子数组最小乘积的最大值](https://leetcode.cn/problems/maximum-subarray-min-product/)
- [2104. 子数组范围和](https://leetcode.cn/problems/sum-of-subarray-ranges/)
- [2281. 巫师的总力量和](https://leetcode.cn/problems/sum-of-total-strength-of-wizards/)，但比本题难。
- 与 2281 同样使用单调栈，获取贡献区间的还有很多题目，如[2818. 操作使得分最大](https://leetcode.cn/problems/apply-operations-to-maximize-score/)
### 解法 贡献法
**由于元素的顺序不影响答案，先排序**。

设有 $a,b,c,d,e$ 五个数，顺序从小到大。如果把 $d$ 当成最大值：
1. 如果只选 $d$ 单独一个数，那么力量为 $d^3$ 。
2. 选 $a$ 为最小值，由于中间的 $b$ 和 $c$ 可选可不选，一共有 $2^2$ 种方案，所以力量总和为 $d^2\cdot a\cdot 2^2$ 。
3. 选 $b$ 为最小值，由于中间的 $c$ 可选可不选，一共有 $2^1$ 种方案，所以力量总和为 $d^2\cdot b\cdot 2^1$ 。
4. 选 $c$ 为最小值，只有 $2^0=1$ 种方案，所以力量总和为 $d^2\cdot c\cdot 2^0$ 。

因此，当 $d$ 为最大值时，$d$ 及其左侧元素对答案的贡献为
$$d^3 + d^2\cdot (a\cdot 2^2 + b\cdot 2^1 + c\cdot 2^0)$$
令 $s=a\cdot 2^2 + b\cdot 2^1 + c\cdot 2^0$ ，上式为
$$d^3 + d^2\cdot s = d^2\cdot(d+s)$$
继续，把 $e$ 当成最大值，观察 $s$ 如何变化，也就是 $a,b,c,d$ 作为最小值的贡献：
$$\begin{aligned} &\ a\cdot 2^3 + b\cdot 2^2 + c\cdot 2^1 + d\cdot 2^0\\ =&\ 2\cdot(a\cdot 2^2 + b\cdot 2^1 + c\cdot 2^0) + d\cdot 2^0\\ =&\ 2\cdot s + d\\ \end{aligned}$$ 
这意味着，**我们不需要枚举最小值，只需要枚举最大值**，就可以把 $s$ 递推计算出来。
```cpp
class Solution {
public:
    int sumOfPower(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        const int mod = 1e9 + 7;
        int ans = 0, s = 0;
        for (long long x : nums) { // x作为最大值
            ans = (ans + x * x % mod * (x + s)) % mod; // 中间模1次防止溢出
            s = (s * 2 + x) % mod; // 递推计算下个s
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 为 $\textit{nums}$ 的长度。瓶颈在排序上。
- 空间复杂度：$O(1)$ 。忽略排序的栈空间，仅用到若干额外变量。

> 思考题：把「子序列」改成「子数组」，要怎么做？