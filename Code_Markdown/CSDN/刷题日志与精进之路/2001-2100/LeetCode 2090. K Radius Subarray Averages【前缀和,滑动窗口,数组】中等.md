> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的数组 `nums` ，数组中有 `n` 个整数，另给你一个整数 `k` 。

**半径为 k 的子数组平均值** 是指：`nums` 中一个以下标 `i` 为 **中心** 且 **半径** 为 `k` 的子数组中所有元素的平均值，即下标在 `i - k` 和 `i + k` 范围（**含** `i - k` 和 `i + k`）内所有元素的平均值。如果在下标 `i` 前或后不足 `k` 个元素，那么 **半径为 k 的子数组平均值** 是 `-1` 。

构建并返回一个长度为 `n` 的数组 `avgs` ，其中 `avgs[i]` 是以下标 `i` 为中心的子数组的 **半径为 k 的子数组平均值** 。

`x` 个元素的 **平均值** 是 `x` 个元素相加之和除以 `x` ，此时使用截断式 **整数除法** ，即需要去掉结果的小数部分。
- 例如，四个元素 `2`、`3`、`1` 和 `5` 的平均值是 `(2 + 3 + 1 + 5) / 4 = 11 / 4 = 2.75`，截断后得到 `2` 。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/11/07/eg1.png)

```java
输入：nums = [7,4,3,9,1,8,5,2,6], k = 3
输出：[-1,-1,-1,5,4,4,-1,-1,-1]
解释：
- avg[0]、avg[1] 和 avg[2] 是 -1 ，因为在这几个下标前的元素数量都不足 k 个。
- 中心为下标 3 且半径为 3 的子数组的元素总和是：7 + 4 + 3 + 9 + 1 + 8 + 5 = 37 。
  使用截断式 整数除法，avg[3] = 37 / 7 = 5 。
- 中心为下标 4 的子数组，avg[4] = (4 + 3 + 9 + 1 + 8 + 5 + 2) / 7 = 4 。
- 中心为下标 5 的子数组，avg[5] = (3 + 9 + 1 + 8 + 5 + 2 + 6) / 7 = 4 。
- avg[6]、avg[7] 和 avg[8] 是 -1 ，因为在这几个下标后的元素数量都不足 k 个。
```
**示例 2：**
```java
输入：nums = [100000], k = 0
输出：[100000]
解释：
- 中心为下标 0 且半径 0 的子数组的元素总和是：100000 。
  avg[0] = 100000 / 1 = 100000 。
```
**示例 3：**
```java
输入：nums = [8], k = 100000
输出：[-1]
解释：
- avg[0] 是 -1 ，因为在下标 0 前后的元素数量均不足 k 。
```
**提示：**
- `n == nums.length`
- `1 <= n <= 10^5`
- `0 <= nums[i], k <= 10^5`

---
### 解法1 前缀和
根据题目描述，只有当中心位置 $i \in [k, n-k-1]$ 时，整个长度为 $2k+1$ 的子区间才会完整地落在数组 $nums$ 内部。当 $i<k$ 或者 $i≥n−k$ 时，对应的平均值为 $-1$ 。

因此如果 $k \geq n-k-1$ 即 $2k+1≥n$ ，答案数组中所有的元素均为 $-1$ 。否则首先计算出数组 $nums$ 的前缀和 $sum$ ，然后对 $i \in [k,\ n - k - 1]$ 中的所有位置，利用前缀和求其 $[i -k , i  +1]$ 的元素和、并除以 $2k+1$ ：$$\dfrac{sum[i + k + 1] - sum[i - k]}{2k-1}$$
注意，前缀和数组要用 `long long` ，不然会溢出。
```java
class Solution {
    public int[] getAverages(int[] nums, int k) {
        if (k == 0) return nums;
        int n = nums.length, m = 2 * k + 1;
        int[] ans = new int[n];
        Arrays.fill(ans, -1);
        if (m > n) return ans;
        long[] sum = new long[n + 1];
        for (int i = 0; i < n; ++i) sum[i + 1] = sum[i] + nums[i];
        for (int i = k; i + k < n; ++i) ans[i] = (int)((sum[i + k + 1] - sum[i - k]) / m);
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(n)$

---
### 解法2 滑动窗口
不难发现，上述做法需要 $O(n)$ 的空间来存储前缀和，但实际上可以进一步优化，且只需遍历一次。

首先，先求出前 $2k+1$ 个元素的和，放在答案数组的 $ans[k]$ 中。由于：
$$\left\{ \begin{aligned} & \textit{ans}[i - 1] && = \textit{nums}[i - k - 1] + \textit{nums}[i - k] + \cdots + \textit{nums}[i + k - 1] \\ & \textit{ans}[i] && = \textit{nums}[i - k] + \cdots + \textit{nums}[i + k - 1] + \textit{nums}[i + k] \end{aligned} \right.$$
​因此随后只需要通过递推式：
$$\textit{ans}[i] = \textit{ans}[i - 1] + \textit{nums}[i + k] - \textit{nums}[i - k - 1]$$

即可得到所有中心位置 $i \in [k, n-k-1]$ 且长度为 $2k+1$ 的子数组的和。最后将每一个和除以 $2k+1$ 即可得到平均数。
```java
class Solution {
    public int[] getAverages(int[] nums, int k) {
        if (k == 0) return nums;
        int n = nums.length, m = 2 * k + 1;
        int[] ans = new int[n];
        Arrays.fill(ans, -1);
        if (m > n) return ans;
        long sum = 0;
        for (int i = 0; i < m; ++i) sum += nums[i];
        for (int i = k; i + k < n; ++i) {
            if (i != k) sum += nums[i + k] - nums[i - k - 1];
            ans[i] = (int)(sum / m);
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$ ，不计算答案数组的情况下，只使用了若干辅助变量