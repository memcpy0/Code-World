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
- [2681. Power of Heroes](https://leetcode.cn/problems/power-of-heroes/description/)，比本题简单。
- 与 2281 同样使用单调栈，获取贡献区间的还有很多题目，如[2818. 操作使得分最大](https://leetcode.cn/problems/apply-operations-to-maximize-score/)
### 解法 单调栈+贡献法
**提示 1-1** 枚举每位巫师，假设他是最弱的巫师，那么他能在哪些子数组中？

设当前枚举的巫师的能力值为 $v$ ，那么他对答案产生的贡献是 $v$ 乘上「在左右边界 $[L,R]$ 内的、所有包含 $v$ 的子数组的元素和」的**和**。
- 如何计算子数组的元素和？用前缀和来计算。
- 如何计算子数组的元素和的和？

**提示 1-2** 左右边界最远能到哪？具体地，这些子数组的左边界的最小值是多少，右边界的最大值是多少？

用单调栈来计算左右边界。不了解单调栈可以看一下[496. 下一个更大元素 I]。对于本题，我们需要求的是下一个更小元素。

**提示 1-3** 注意，本题是可能有重复元素的，这会对最终答案的计算产生什么影响？

设左右边界为 $[L,R]$ 。为了避免重复计算，我们可以考虑左侧**严格小于**当前元素的最近元素位置 $L-1$ ，以及右侧**小于等于**当前元素的最近元素位置 $R+1$ 。

以示例 1 中的数组 $[1,3,1,2]$ 为例，**如果左右两侧都是找严格小于**，那么第一个 $1$ 和第二个 $1$ 算出来的边界范围都是一样的（都是整个数组），这就重复统计了。为了避免这种情况，可以把某一侧改为小于等于，比如把右侧改成小于等于，那么第一个 $1$ 算出来的右边界不会触及或越过第二个 $1$ ，这样就能**避免重复统计同一个子数组**。

**提示 1-4** 如何计算子数组的元素和的和？
- 不妨将子数组的右端点固定，子数组左端点的范围是多少？
- 对于多个不同的右端点，其对应的左端点的范围是否均相同？

设子数组左端点为 $l$ ，右端点为 $r$ ，当前枚举的元素下标为 $i$ ，那么有 $L\le l\le i \le r\le R$ 。

设 $\textit{strength}$ 数组的前缀和为 $s$ ，其中 $s[i]=\sum\limits_{j=0}^{i-1} \textit{strength}[j]$ ，因此**子数组 $[l,r]$ 的元素和**可以表示为
$$s[r+1]−s[l]$$
在范围 $[L,R]$ 内的所有子数组的元素和的和可以表示为
$$\begin{aligned} &\sum_{r=i+1}^{R+1}\sum_{l=L}^{i} (s[r]-s[l]) \\ =&\left(\sum_{r=i+1}^{R+1}\sum_{l=L}^{i} s[r]\right)-\left(\sum_{r=i+1}^{R+1}\sum_{l=L}^{i} s[l]\right) \\ =&(i-L+1)\cdot \sum_{r=i+1}^{R+1}s[r] -(R-i+1)\cdot \sum_{l=L}^{i} s[l] \end{aligned}$$
因此我们还需要计算出前缀和 $s$ 的前缀和 $ss$ ，其中 $\textit{ss}[i]=\sum\limits_{j=0}^{i-1}s[j]$ ，上式即为
$$(i-L+1)\cdot (\textit{ss}[R+2]-\textit{ss}[i+1]) - (R-i+1)\cdot (\textit{ss}[i+1]-\textit{ss}[L])$$ 
再乘上 $v$ 即为当前巫师的贡献，累加所有贡献即为答案。
```cpp
class Solution {
public:
    int totalStrength(vector<int>& strength) {
        const int mod = 1e9 + 7;
        int n = strength.size();
        vector<int> left(n, -1); // left[i] 为左侧严格小于 strength[i] 的最近元素位置（不存在时为 -1）
        vector<int> right(n, n); // right[i] 为右侧小于等于 strength[i] 的最近元素位置（不存在时为 n）
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && strength[st.top()] >= strength[i]) {
                right[st.top()] = i;
                st.pop();
            }
            if (!st.empty()) left[i] = st.top();
            st.push(i);
        }
        long s = 0L; // 前缀和
        vector<int> ss(n + 2); // 前缀和的前缀和
        for (int i = 1; i <= n; ++i) {
            s += strength[i - 1];
            ss[i + 1] = (ss[i] + s) % mod; // 注意取模后，下面计算两个 ss 相减，结果可能为负
        }

        int ans = 0;
        for (int i = 0; i < n; ++i) { // [l,r]左闭右闭
            long l = left[i] + 1, r = right[i] - 1;
            long tot = ((i - l + 1) * (ss[r + 2] - ss[i + 1]) - (r - i + 1) * (ss[i + 1] - ss[l])) % mod;
            ans = (ans + strength[i] * tot) % mod; // 累加贡献
        }
        return (ans + mod) % mod; // 防止算出负数
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ 。
- 空间复杂度：$O(n)$ 。
 