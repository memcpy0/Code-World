> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an integer array `arr` and an integer `difference`, return the length of the longest subsequence in `arr` which is an arithmetic sequence such that the difference between adjacent elements in the subsequence equals `difference`.

A **subsequence** is a sequence that can be derived from `arr` by deleting some or no elements without changing the order of the remaining elements.

**Example 1:**
```java
Input: arr = [1,2,3,4], difference = 1
Output: 4
Explanation: The longest arithmetic subsequence is [1,2,3,4].
```
**Example 2:**
```java
Input: arr = [1,3,5,7], difference = 1
Output: 1
Explanation: The longest arithmetic subsequence is any single element.
```
**Example 3:**
```java
Input: arr = [1,5,7,8,5,3,4,2,1], difference = -2
Output: 4
Explanation: The longest arithmetic subsequence is [7,5,3,1].
```
**Constraints:**
-   `1 <= arr.length <= 10^5`
-   `-10^4 <= arr[i], difference <= 10^4`

题意：给定一个数组 `arr` 和一个公差 `difference` ，在数组 `arr` 中寻找公差为 `difference` 的最长等差子数列的长度。

---
相似题目是[1027. 最长等差数列](https://leetcode-cn.com/problems/longest-arithmetic-subsequence/)。不过本题与其说是「**最长等差子序列**」，不如说是「**最长定差子序列**」。
### 解法1 序列DP+哈希表+贪心
我们定义 $dp[i][j]$（$j$ 非 $0$ 即 $1$ ）为**考虑前 $i + 1$ 个数、其第 $i + 1$ 个数的选择情况为 $j$ 时，得到的最长定差子序列的长度**。从而，最终答案为 $\max(dp[n - 1][0], dp[n - 1][1])$ ，初始化条件则显然为 $dp[0][0] = 0, dp[0][1] = 1$ 。

不失一般性地考虑 $dp[i][j]$ 如何转移：
- $dp[i][0]$ ：明确了第 $i$ 个不选，那么此时最长长度为**前一个位置的结果**，即有 $$dp[i][0] = \max(dp[i - 1][0], dp[i - 1][1])$$
- $dp[i][1]$ ：明确了第 $i$ 个要选，此时分情况讨论：
	- $arr[i]$ **独立成为一个子序列**，此时有 $dp[i][1] = 1$ ；
	- $arr[i]$ **接在某个数的后面**，由于给定了差值 $difference$ ，可直接算得上一位的值为 $prev = arr[i] - difference$ ，此时要找到值为 $prev$ 、下标最大的位置（小于 $i$ ），然后从该位置转移过来，即有 $dp[i][1] = dp[hash[prev]][1] + 1$ 

容易证明，==如果存在多个位置的值为 $prev$ ，从中选择一个下标最大的位置 $j$（下标小于 $i$ ）进行转移，结果相对于最优位置 $k$（下标也小于 $i$ ；使 $arr[k] = prev$ 且使得 $dp[k][1]$ 值最大的位置）不会变差==。为什么？我们假设当前位置为 $i$ ，有多个位置的值为 $prev$ ，其中最优位置为 $k\ (k < i)$ ，其他位置假设为 $j\ (j< i)$ 。于是有两种情况：
-  对所有 $j$ 均有 $j \le k$ ，此时选择下标最大的位置 $k$ 即可；
- 存在 $j$ 满足 $j > k$ ，~~此时 $dp[k][1] > dp[j][1]$ ，即 $k$ 处 $prev$ 值形成的最长定差子序列的长度，大于 $j$ 处 $prev$ 值形成的最长定差子序列的长度~~……这可能吗？事实上，$j$ 处 $prev$ 值完全可以连接到 $k$ 处 $prev$ 值之前的最长定差子序列，从而有 $dp[j][1] \ge dp[k][1]$ 。因此，我们选择下标更大的位置 $j$ ，不会使结果变差。或者说，**下标最大的位置即是最优位置**，于是我们**贪心地选择下标最大的位置即可**。

综上所述，我们有：
$$dp[i][1] = \begin{cases}
1 \quad &hash[prev] = -1 \\
dp[hash[prev]][1] + 1 \quad  &hash[prev] \ne -1 \\
\end{cases}
$$

这一算法中，我们有 $2n$ 个状态需要被计算，每个状态转移的复杂度为 $O(1)$ ，整体时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ 。

C++具体实现中，用的是 $2$ 行 $n$ 列的 $dp$ 数组和 `unordered_set` ：
```cpp
//C++ version
class Solution {
public:
    int longestSubsequence(vector<int>& arr, int difference) {
        int n = arr.size();
        // dp[i][j]表示arr[0,i]中第i个数选择情况为j时的最长定差子序列长度
        vector<vector<int>> dp(2, vector<int>(n));
        dp[0][0] = 0, dp[1][0] = 1;
        unordered_map<int, int> hash;
        hash[arr[0]] = 0;
        for (int i = 1; i < n; ++i) {
            dp[0][i] = max(dp[0][i - 1], dp[1][i - 1]);
            dp[1][i] = 1;
            int prev = arr[i] - difference;
            if (hash.find(prev) != hash.end())
                dp[1][i] = dp[1][hash[prev]] + 1;
            hash[arr[i]] = i; 
        } 
        return max(dp[0][n - 1], dp[1][n - 1]);
    }
};
//执行用时：116 ms, 在所有 C++ 提交中击败了57.83% 的用户
//内存消耗：59.1 MB, 在所有 C++ 提交中击败了5.00% 的用户
```
Java具体实现中，用的是 $n$ 行 $2$ 列的 $dp$ 数组和整型数组：
```java
//Java version
class Solution {
    int N = 40010, M = N / 2;
    public int longestSubsequence(int[] arr, int difference) {
        int n = arr.length;
        // dp[i][j]表示arr[0,i]中第i个数选择情况为j时的最长定差子序列长度
        int[][] dp = new int[n][2];
        dp[0][1] = 1;
        int[] hash = new int[N]; // 整型数组
        Arrays.fill(hash, -1);
        hash[arr[0] + M] = 0;
        for (int i = 1; i < n; ++i) {
            dp[i][0] = Math.max(dp[i - 1][0], dp[i - 1][1]);
            dp[i][1] = 1;
            int prev = arr[i] - difference;
            if (hash[prev + M] != -1) 
                dp[i][1] = dp[hash[prev + M]][1] + 1; // 注意偏移为M
            hash[arr[i] + M] = i;
        }
        return Math.max(dp[n - 1][0], dp[n - 1][1]);
    }
}
//执行用时：18 ms, 在所有 Java 提交中击败了86.56% 的用户
//内存消耗：49.7 MB, 在所有 Java 提交中击败了37.18% 的用户
```
---
### 解法2 动态规划+哈希表（优化状态定义）
不难发现，多定义一维状态，是用来「**区分某个位置的值是否被选择**」，目的是**正确转移出第 $i$ 位被选择的情况**。

我们调整状态定义，$dp[i]$ 表示**考虑前 $i + 1$ 个数（第 $i + 1$ 个数必选）时，得到的最长定差子序列长度**。`Without loss of generality` ，考虑 $dp[i]$ 如何转移：
- **$arr[i]$ 独立成为一个子序列**，此时有 $dp[i] = 1$ ；
- **$arr[i]$ 接在某个数的后面**，由于给定了差值 $difference$ ，可直接算得上一位的值为 $prev = arr[i] - difference$ ，此时找到 $arr[j]$ 为 $prev$ 的最新位置（下标最大，同时满足 $j \lt i$ ）的转移结果，在此基础上加一即可，即有 $dp[i] = dp[hash[prev]] + 1$ 。

最后，我们返回最大的 $dp[i]$ 作为答案。

这一算法中，我们有 $n$ 个状态需要被计算，每个状态转移的复杂度为 $O(1)$ ，整体时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ 。
```java
//Java version
class Solution {
    int N = 40010, M = N / 2;
    public int longestSubsequence(int[] arr, int difference) {
        int n = arr.length;
        // dp[i]表示arr[0,i]中选择第i个数时的最长定差子序列长度
        int[] dp = new int[n];
        dp[0] = 1;
        int[] hash = new int[N]; // 整型数组
        Arrays.fill(hash, -1);
        hash[arr[0] + M] = 0;
        int ans = 1;
        for (int i = 1; i < n; ++i) {
            dp[i] = 1;
            int prev = arr[i] - difference;
            if (hash[prev + M] != -1) 
                dp[i] = dp[hash[prev + M]] + 1; // 注意偏移为M
            hash[arr[i] + M] = i;
            ans = Math.max(ans, dp[i]);
        }
        return ans;
    }
}
//执行用时：10 ms, 在所有 Java 提交中击败了86.81% 的用户
//内存消耗：55.1 MB, 在所有 Java 提交中击败了24.19% 的用户
```
事实上，只使用哈希表，我们就能完成这一点。只要**将 $hash$ 数组初始化为零**，并将 $hash$ 和 $dp$ 数组合二为一，用元素值即有：
$$dp[arr[i]] = dp[prev] + 1$$

此时，$dp[i]$ 表示**以值 $i$ 结尾的最长定差子序列的长度**。
```cpp
//C++ version
class Solution {
public:
    int longestSubsequence(vector<int>& arr, int difference) {
        unordered_map<int, int> dp;
        int ans = 0;
        for (const int &i : arr)
            ans = max(ans, dp[i] = dp[i - difference] + 1);
        return ans;
    }
};
//执行用时：108 ms, 在所有 C++ 提交中击败了77.89% 的用户
//内存消耗：55.2 MB, 在所有 C++ 提交中击败了59.31% 的用户
//C++ version
class Solution {
public:
    int longestSubsequence(vector<int>& arr, int difference) {
        const int N = 40010, M = N / 2;
        vector<int> dp(N);
        int ans = 0;
        for (const int &i : arr)
            ans = max(ans, dp[i + M] = dp[i + M - difference] + 1);
        return ans;
    }
};
//执行用时：76 ms, 在所有 C++ 提交中击败了91.61% 的用户
//内存消耗：54.9 MB, 在所有 C++ 提交中击败了75.39% 的用户
```
