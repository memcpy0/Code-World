> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an integer array `arr`, partition the array into (contiguous) subarrays of length **at most** `k`. After partitioning, each subarray has their values changed to become the maximum value of that subarray.

Return _the largest sum of the given array after partitioning. Test cases are generated so that the answer fits in a **32-bit** integer._

**Example 1:**
```java
Input: arr = [1,15,7,9,2,5,10], k = 3
Output: 84
Explanation: arr becomes [15,15,15,9,10,10,10]
```
**Example 2:**
```java
Input: arr = [1,4,1,5,7,3,6,1,9,9,3], k = 4
Output: 83
```
**Example 3:**
```java
Input: arr = [1], k = 1
Output: 1
```
**Constraints:**
-   `1 <= arr.length <= 500`
-   `0 <= arr[i] <= 10^9`
-   `1 <= k <= arr.length`

题意：把数组 `arr` 分成若干个「最长不超过 $k$ 的子数组」，对各个子数组，将其中所有元素改为该子数组中的最大值，返回将数组分隔变换后能够得到的元素最大和。答案是一个 32 位整数。

---
### 解法1 递归+记忆化搜索
对于 `arr = [1,15,7,9,2,5,10]` ，`k = 3` ，最后一段子数组可以是 `[10], [5,10], [2,5,10]`（长度不能超过 $k = 3$ ）。根据题意，将子数组内的元素变为子数组的最大值，例如 `[2,5,10]` 变为 `[10,10,10]` ，其元素和为 `[30]` 。

去掉这段子数组，例如去掉 `[2,5,10]` ，剩下要解决的问题是 `[1,15,7,9]` 在分隔变换后能得到的元素最大和。==这是一个和原问题相似的子问题，所以可以用递归解决==。

根据上面的讨论，递归参数只需要一个 `i` ，**`dfs(i)` 表示把 `arr[0]` 到 `arr[i]` 这段分隔变换后得到的元素最大和**。**枚举某一段的子数组的开始下标 `j`** ，那么从 `arr[j]` 到 `arr[i]` 的这段子数组的所有值都要变为这段子数组的最大值，对应的元素和就是 $$(i - j + 1) \times \max^{i}_{p=j} arr[p]$$当然，子数组的长度不能超过 $k$ ，即 $j \ge \max(i-k+1,0)$（注意 $j$ 不能是负数）。

把「该段子数组 `arr[i:j]` 的元素和」加上「 $dfs(j - 1)$ 这个子问题的返回值」作为一个结果，再取这些结果中的最大值，就是 $dfs(i)$ ，即：
$$dfs(i) = \max_{j=\max(i-k+1,0) }^{i} \Bigg\{ dfs(j - 1) + (i - j + 1) \times \max_{p=j}^{i} arr[p] \Bigg\}$$
- 递归边界：$dfs(-1) = 0$ ，一个元素都没有，和为 $0$ 。
- 递归入口：$dfs(n - 1)$ ，就是答案。

代码实现时，可从 $i$ 开始倒着枚举 $j$ ，一边枚举一边计算子数组的最大值，从而 $O(1)$ 地计算出元素和。
```cpp
// 会超时的递归代码
class Solution {
public:
    int maxSumAfterPartitioning(vector<int> &arr, int k) {
        function<int(int)> dfs = [&](int i) -> int {
            // i=-1 时不会进入循环
            int res = 0;
            for (int j = i, mx = 0; j > i - k && j >= 0; --j) {
                mx = max(mx, arr[j]); // 一边枚举 j，一边计算子数组的最大值
                res = max(res, dfs(j - 1) + (i - j + 1) * mx);
            }
            return res;
        };
        return dfs(arr.size() - 1);
    }
}; 
```
复杂度分析：
- 时间复杂度：$O(k^n)$ ，$n$ 为 $arr$ 长度，近似地是在一棵高度为 $n$ 的 $k$ 叉树上递归。
- 空间复杂度：$O(n)$ ，递归需要 $O(n)$ 的栈空间。

一个示例是，「先分隔出 `arr[n - 1]` 、再分隔出 `arr[n - 2]` 」和「分隔出 `arr[n-1:n-2]` 」，都会递归到 $dfs(n - 3)$ 。因此可知，整个递归中有大量重复递归调用（递归入参相同），==由于**递归函数没有副作用**，同样的入参无论计算多少次，算出来的结果都是一样的，因此可以用**记忆化搜索**来优化==。
```cpp
class Solution {
public:
    int maxSumAfterPartitioning(vector<int> &arr, int k) {
		int n = arr.size(), dp[n];
		memset(dp, -1, sizeof(dp)); // -1表示没被计算过
		function<int(int)> dfs = [&](int i) -> int {
			if (i < 0) return 0;
			int &res = dp[i]; // 下面会直接修改dp[i]
			if (res != -1) return res;
			for (int j = i, mx = 0; j > i - k && j >= 0; --j) {
				mx = max(mx, arr[j]); // 一边枚举j,一边计算子数组的最大值
				res = max(res, dfs(j - 1) + (i - j + 1) * mx);
			}
			return res;
		};
		return dfs(n - 1);
	}
};
```
复杂度分析：
- 时间复杂度：$O(nk)$ ，其中 $n$ 为 $arr$ 的长度。**动态规划的时间复杂度=状态个数x单个状态的计算时间**。这里状态个数为 $O(n)$ ，单个状态计算时间为 $O(k)$ ，因此复杂度为 $O(nk)$ 。
- 空间复杂度：$O(n)$ 。

---
### 解法2 动态规划
我们可以去掉递归中的「递」，只保留「归」的部分，即自底向上计算。做法：
- $\textit{dfs}$ 改成 $dp$ 数组；
- 递归改成循环（每个参数都对应一层循环）；
- 递归边界改成 $dp$ 数组的初始值。

具体来说，$dp[i]$ 的含义和 $dfs(i)$  的含义是一致的，都**表示把 $\textit{arr}[0]$ 到 $\textit{arr}[i]$ 这段做分隔变换后能得到的元素最大和**。相应的递推式（状态转移方程）也和 $\textit{dfs}$ 的一致：
$$dp[i] = \max_{j=\max(i-k+1,0)}^{i}\left\{dp[j-1] + (i-j+1)\cdot \max_{p=j}^{i}\textit{arr}[p]\right\}$$

但这种定义方式没有状态能表示递归边界，即 $i=-1$ 的情况。不过我们可以将DP数组右移，即现在的 $dp[i]$等于之前的 $dp[i - 1]$ 。
- 初始值 $dp[0] = 0$  ——翻译自 $\textit{dfs}(-1)=0$ 
- 答案为 $dp[n]$ ——翻译自 $dfs(n - 1)$ 。

```cpp
class Solution {
public:
    int maxSumAfterPartitioning(vector<int>& arr, int k) {
        int n = arr.size(), dp[n + 1];
        dp[0] = 0;
        for (int i = 0; i < n; ++i) {
            dp[i + 1] = 0;
            for (int j = i, mx = 0; j >= i - k + 1 && j >= 0; --j) {
                mx = max(mx, arr[j]); // 一边枚举j,一边计算子数组的最大值
                dp[i + 1] = max(dp[i + 1], dp[j] + mx * (i - j + 1));
            }
        }
        return dp[n];
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal O(nk)$ 
- 空间复杂度：$\mathcal{O}(n)$ 

---
### 解法3 滚动数组空间优化
假设 $k$ 非常小，例如 $k=3$ ，如何优化额外空间呢？观察上面的状态转移方程，在计算 $dp[i + 1]$ 及其之后的状态时，不会用到下标 $\le i-k$ 的状态。

那么可以用一个长为 $k$ 的数组滚动计算。例如 $k=3$ 时，$dp[4]$ 填到 $dp[1]$ 中，$dp[5]$ 填到 $dp[2]$ 中，依此类推。状态转移方程改为
$$dp[(i+1)\bmod k] = \max_{j=\max(i-k+1,0)}^{i}\left\{dp[j\bmod k] + (i-j+1)\cdot \max_{p=j}^{i}\textit{arr}[p]\right\}$$
- 初始值 $dp[0]=0$ 。
- 答案为 $dp[n\bmod k]$ 。

```cpp
class Solution {
public:
    int maxSumAfterPartitioning(vector<int> &arr, int k) {
        int n = arr.size(), dp[k];
        dp[0] = 0;
        for (int i = 0; i < n; ++i) {
            int res = 0;
            for (int j = i, mx = 0; j > i - k && j >= 0; --j) {
                mx = max(mx, arr[j]); // 一边枚举 j，一边计算子数组的最大值
                // 注意在循环结束前，f[(i+1)%k] 是需要用到的，不能提前覆盖
                res = max(res, dp[j % k] + (i - j + 1) * mx);
            }
            dp[(i + 1) % k] = res;
        }
        return dp[n % k];
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(nk)$ 。
- 空间复杂度：$\mathcal{O}(k)$ 。
 
