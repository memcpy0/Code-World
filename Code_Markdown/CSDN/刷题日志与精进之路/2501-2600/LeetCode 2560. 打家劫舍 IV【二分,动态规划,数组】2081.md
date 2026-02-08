> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

沿街有一排连续的房屋。每间房屋内都藏有一定的现金。现在有一位小偷计划从这些房屋中窃取现金。

由于相邻的房屋装有相互连通的防盗系统，所以小偷 **不会窃取相邻的房屋** 。

小偷的 **窃取能力** 定义为他在窃取过程中能从单间房屋中窃取的 **最大金额** 。

给你一个整数数组 `nums` 表示每间房屋存放的现金金额。形式上，从左起第 `i` 间房屋中放有 `nums[i]` 美元。

另给你一个整数 `k` ，表示窃贼将会窃取的 **最少** 房屋数。小偷总能窃取至少 `k` 间房屋。

返回小偷的 **最小** 窃取能力。

**示例 1：**
```js
输入：nums = [2,3,5,9], k = 2
输出：5
解释：
小偷窃取至少 2 间房屋，共有 3 种方式：
- 窃取下标 0 和 2 处的房屋，窃取能力为 max(nums[0], nums[2]) = 5 。
- 窃取下标 0 和 3 处的房屋，窃取能力为 max(nums[0], nums[3]) = 9 。
- 窃取下标 1 和 3 处的房屋，窃取能力为 max(nums[1], nums[3]) = 9 。
因此，返回 min(5, 9, 9) = 5 。
```
**示例 2：**
```js
输入：nums = [2,7,9,3,1], k = 2
输出：2
解释：共有 7 种窃取方式。窃取能力最小的情况所对应的方式是窃取下标 0 和 4 处的房屋。返回 max(nums[0], nums[4]) = 2 。
```
**提示：**
- `1 <= nums.length <= 10^5`
- `1 <= nums[i] <= 10^9`
- `1 <= k <= (nums.length + 1)/2`

---
题意（版本 A）
说，有个小偷公司，给小偷定的 KPI 是偷至少 kkk 间房子，要求偷的房子不能相邻。

张三作为其中的一个小偷，他不想偷太多，否则一不小心就「数额巨大」，这可太刑了。所以张三计划，在他偷过的房子中，偷走的最大金额要尽量地小。

这个最小值是多少呢？

题意（版本 B）
给定数组 nums\textit{nums}nums，从中选择一个长度至少为 kkk 的子序列 AAA，要求 AAA 中没有任何元素在 nums\textit{nums}nums 中是相邻的。

最小化 max⁡(A)\max(A)max(A)。

方法一：二分+DP
有关二分的三种写法，请看【基础算法精讲 04】。本文采用开区间写法。

看到「最大化最小值」或者「最小化最大值」就要想到二分答案，这是一个固定的套路。

对于本题，「偷走的最大金额」越小，能偷的房子就越少，反之越多。例如 nums=[1,4,2,3]\textit{nums}=[1,4,2,3]nums=[1,4,2,3]，在最大金额为 222 时，nums\textit{nums}nums 中只有 111 和 222 是可以偷的；在最大金额为 444 时，nums\textit{nums}nums 中 1,2,3,41,2,3,41,2,3,4 都可以偷。

一般地，二分的值越小，越不能/能满足要求；二分的值越大，越能/不能满足要求。有单调性的保证，就可以二分答案了。

把二分中点 mid\textit{mid}mid 记作 mx\textit{mx}mx，仿照 198. 打家劫舍，定义 f[i]f[i]f[i] 表示从 nums[0]\textit{nums}[0]nums[0] 到 nums[i]\textit{nums}[i]nums[i] 中偷金额不超过 mx\textit{mx}mx 的房屋，最多能偷多少间房屋。如果 f[n−1]≥kf[n-1]\ge kf[n−1]≥k 则表示答案至多为 mx\textit{mx}mx，否则表示答案必须超过 mx\textit{mx}mx。

用「选或不选」来分类讨论：

不选 nums[i]\textit{nums}[i]nums[i]：f[i]=f[i−1]f[i] = f[i-1]f[i]=f[i−1]；
选 nums[i]\textit{nums}[i]nums[i]，前提是 nums[i]≤mx\textit{nums}[i] \le \textit{mx}nums[i]≤mx：f[i]=f[i−2]+1f[i] = f[i-2]+1f[i]=f[i−2]+1。
这两取最大值，即

f[i]=max⁡(f[i−1],f[i−2]+1)f[i] = \max(f[i-1], f[i-2] + 1)
f[i]=max(f[i−1],f[i−2]+1)
代码实现时，可以用两个变量滚动计算。具体请看【基础算法精讲 17】。

答疑
问：有没有可能，二分出来的答案，不在 nums\textit{nums}nums 中？

答：不可能。二分出来的答案，一定在 nums\textit{nums}nums 中。证明如下：

设答案为 ans\textit{ans}ans，那么当最大金额为 ans\textit{ans}ans 时，可以偷至少 kkk 间房子。如果 ans\textit{ans}ans 不在 nums\textit{nums}nums 中，那么当最大金额为 ans−1\textit{ans}-1ans−1 时，也可以偷至少 kkk 间房子。这与二分算法相矛盾：根据视频中讲的红蓝染色法，循环结束时，ans\textit{ans}ans 和 ans−1\textit{ans}-1ans−1 的颜色必然是不同的，即 ans\textit{ans}ans 可以满足题目要求，而 ans−1\textit{ans}-1ans−1 不满足题目要求。所以，二分出来的答案，一定在 nums\textit{nums}nums 中。

Python3
Java
C++
Go
JavaScript
class Solution:
    def minCapability(self, nums: List[int], k: int) -> int:
        # solve(mx) 返回最大金额为 mx 时，最多可以偷多少间房子
        def solve(mx: int) -> int:
            f0 = f1 = 0
            for x in nums:
                if x > mx:
                    f0 = f1
                else:
                    f0, f1 = f1, max(f1, f0 + 1)
            return f1
        return bisect_left(range(max(nums)), k, key=solve)
复杂度分析
时间复杂度：O(nlog⁡U)\mathcal{O}(n\log U)O(nlogU)，其中 nnn 为 nums\textit{nums}nums 的长度，U=max(nums)U=max(\textit{nums})U=max(nums)。
空间复杂度：O(1)\mathcal{O}(1)O(1)。仅用到若干额外变量。
方法二：二分+贪心
也可以用贪心做。

考虑到只需要计算个数，在从左到右遍历的情况下只要当前房子可以偷，就立刻偷。

例如 nums=[1,2,3,4], mx=3\textit{nums}=[1,2,3,4],\ mx=3nums=[1,2,3,4], mx=3，如果不偷 nums[0]=1\textit{nums}[0]=1nums[0]=1 去偷 nums[1]=2\textit{nums}[1]=2nums[1]=2，那么只能偷一间房子。而如果偷 nums[0]=1\textit{nums}[0]=1nums[0]=1 和 nums[2]=3\textit{nums}[2]=3nums[2]=3，就可以偷两间房子。

严格证明如下：

根据递推式 f[i]=max⁡(f[i−1],f[i−2]+1)f[i] = \max(f[i-1], f[i-2] + 1)f[i]=max(f[i−1],f[i−2]+1) 可知，

f[i]≥f[i−1]f[i] \ge f[i-1]
f[i]≥f[i−1]
所以 fff 数组是递增的。

此外，f[i]−f[i−1]f[i]-f[i-1]f[i]−f[i−1] 不会超过 111，毕竟我们统计的是「个数」，从 f[i−1]f[i-1]f[i−1] 到 f[i]f[i]f[i] 最多增加 111。

因此

f[i−2]+1≥f[i−1]f[i-2]+1\ge f[i-1]
f[i−2]+1≥f[i−1]
必然成立。也就是说，如果 nums[i]≤mx\textit{nums}[i]\le mxnums[i]≤mx，则

f[i]=f[i−2]+1f[i] = f[i-2] + 1
f[i]=f[i−2]+1
上式表明，在从左到右遍历 nums\textit{nums}nums 时，能偷就偷。如果 nums[i]≤mx\textit{nums}[i]\le mxnums[i]≤mx，我们可以偷 nums[i]\textit{nums}[i]nums[i]，并跳过 nums[i+1]\textit{nums}[i+1]nums[i+1]。
```cpp
class Solution {
public:
    int minCapability(vector<int>& nums, int k) {
        int l = 0, r = *max_element(nums.begin(), nums.end()) + 1;
        while (l < r) {
            int m = (l + r) / 2;
            int f0 = 0, f1 = 0; // 求最大金额为m时最多可偷多少间房子
            for (int num : nums) {
                // if (num > m) f0 = f1; // 超出金额,不偷
                int t = f1;
                if (num <= m) f1 = max(f1, f0 + 1); // 没超出金额,偷或不偷
                f0 = t; // 无论是否超出,f0都要存储f1之前的值
            }
            if (f1 >= k) r = m; // 可以偷k间以上的房子
            else l = m + 1; // 必须提高偷的金额,这样需要偷更多的房子
        }
        return l;
    }
};
```
复杂度分析
时间复杂度：O(nlog⁡U)\mathcal{O}(n\log U)O(nlogU)，其中 nnn 为 nums\textit{nums}nums 的长度，U=max(nums)U=max(\textit{nums})U=max(nums)。
空间复杂度：O(1)\mathcal{O}(1)O(1)。仅用到若干额外变量。
 