> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的数组 `nums` 和一个整数 `m` 。请你判断能否执行一系列操作，将数组拆分成 `n` 个 **非空** 数组。

在每一步操作中，你可以选择一个 **长度至少为 2** 的现有数组（之前步骤的结果） 并将其拆分成 **2** 个子数组，而得到的 **每个** 子数组，**至少** 需要满足以下条件之一：
- 子数组的长度为 1 ，或者
- 子数组元素之和 **大于或等于**  `m` 。

如果你可以将给定数组拆分成 `n` 个满足要求的数组，返回 `true` ；否则，返回 `false` 。

**注意：** 子数组是数组中的一个连续非空元素序列。

**示例 1：**
```java
输入：nums = [2, 2, 1], m = 4
输出：true
解释：
第 1 步，将数组 nums 拆分成 [2, 2] 和 [1] 。
第 2 步，将数组 [2, 2] 拆分成 [2] 和 [2] 。
因此，答案为 true 。
```
**示例 2：**
```java
输入：nums = [2, 1, 3], m = 5 
输出：false
解释：
存在两种不同的拆分方法：
第 1 种，将数组 nums 拆分成 [2, 1] 和 [3] 。
第 2 种，将数组 nums 拆分成 [2] 和 [1, 3] 。
然而，这两种方法都不满足题意。因此，答案为 false 。
```
**示例 3：**
```java
输入：nums = [2, 3, 3, 2, 3], m = 6
输出：true
解释：
第 1 步，将数组 nums 拆分成 [2, 3, 3, 2] 和 [3] 。
第 2 步，将数组 [2, 3, 3, 2] 拆分成 [2, 3, 3] 和 [2] 。
第 3 步，将数组 [2, 3, 3] 拆分成 [2] 和 [3, 3] 。
第 4 步，将数组 [3, 3] 拆分成 [3] 和 [3] 。
因此，答案为 true 。 
```
**提示：**
- `1 <= n == nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= m <= 200`

---
### 解法1 记忆化DFS/区间DP+前缀和
为了方便求出子数组的和，我们使用前缀和。

对于数组拆分，很自然地想到DFS，但如果每次都在数组两侧拆分，则复杂度可能到 $O(2^{100})$ ，为此必须使用记忆化+DFS。我个人的写法如下所示，令 $dfs(l, r)$ 表示区间 $[l,r]$ 可否拆分：
- 递归边界：**区间长度 $\le 2$ ，一定可拆分**；**在区间长度 $> 2$ 且区间和 $\le m$ 时，此时无论如何都无法继续拆分下去**。
- 递归过程：只有**区间和大于** $m$ 且**可以拆分出子区间 $[l + 1, r]$ 或 $[l, r - 1]$**（对应区间长度为 $1$ 或区间和 $\ge m$ ），且满足**子区间可拆分**——即 $dfs(l + 1, r) = true$ 或 $dfs(l, r - 1)= true$ ，此时区间 $[l, r]$ 可以拆分。
```cpp
class Solution {
private:
    int m;
    int sum[110];
    int dp[110][110];
    int dfs(int l, int r) {
        if (dp[l][r] != -1) return dp[l][r]; // 已经有答案
        if (l + 1 >= r) return dp[l][r] = 1; // 拆分前进行判断,可以拆分
        if (sum[r + 1] - sum[l] <= m) return dp[l][r] = 0; // 拆分前进行判断,不可拆分
        int left = 0, right = 0;
        if (l + 1 == r || sum[r + 1] - sum[l + 1] >= m) // 看是否可以拆分出[l+1,r]这个子数组
            left = dfs(l + 1, r); // 看[l+1,r]子数组是否可继续拆分
        if (l == r - 1 || sum[r] - sum[l] >= m) // 看是否可以拆分出[l,r-1]这个子数组
            right = dfs(l, r - 1); // 看[l,r-1]子数组是否可继续拆分
        return dp[l][r] = left || right; 
    }
public:
    bool canSplitArray(vector<int>& nums, int m) {
        this->m = m;
        memset(sum, 0, sizeof(sum));
        memset(dp, -1, sizeof(dp));
        int n = nums.size();
        for (int i = 0; i < n; ++i) sum[i + 1] = sum[i] + nums[i];
        return dfs(0, n - 1);
    }
};
```
还可使用区间DP：
```cpp
class Solution {
public:
    bool canSplitArray(vector<int>& nums, int m) {
        int sum[110];
        bool dp[110][110];
        memset(sum, 0, sizeof(sum));
        memset(dp, false, sizeof(dp));
        int n = nums.size();
        for (int i = 0; i < n; ++i) sum[i + 1] = sum[i] + nums[i];
        // dp[i][j]表示区间[i,j]能否拆分
        for (int i = n - 1; i >= 0; --i) {
            dp[i][i] = true;
            if (i + 1 < n) dp[i][i + 1] = true;
            // 区间[i,j-1], [i+1,j]是否可拆
            for (int j = i + 2; j < n; ++j) {
                // [i,j]能否拆分为[i+1,j]或[i,j-1],看拆出子数组的和是否>=m,且拆出子数组是否可继续拆分
                if (sum[j] - sum[i] >= m && dp[i][j - 1]
                    || sum[j + 1] - sum[i + 1] >= m && dp[i + 1][j])
                    dp[i][j] = true;        
            }
        }
        return dp[0][n - 1];
    }
};
```
---
### 解法2 脑筋急转弯（最优解法）
要善于将题目转换成另外一种解法，对题目的理解、数学逻辑要求较高。
- 先特判 $n \le 2$ 的情况，这是满足要求的。
- 对于 $n\ge 3$ 的情况，**无论按照何种方式分割，一定会在某个时刻，分割出一个长为 $2$ 的子数组**。
    - **如果 $\textit{nums}$ 中任何长为 $2$ 的子数组的元素和都小于 $m$ ，那么无法满足要求**。
    - 否则，可以用这个子数组作为「核心」，像剥洋葱一样，一个一个地去掉 $\textit{nums}$ 的首尾元素，最后得到这个子数组。由于子数组的元素和 $\ge m$ ，所以每次分割出一个元素时，剩余的子数组的元素和也必然是 $\ge m$ 的，满足要求。

于是本题可转换成：求数组中是否存在2个相邻元素之和 $\ge m$ 。相信题目如果这么问，100%的人都能做出来。
```cpp
class Solution {
public:
    bool canSplitArray(vector<int>& nums, int m) {
        int n = nums.size();
        for (int i = 1; i < n; ++i)
            if (nums[i - 1] + nums[i] >= m) return true;
        return n <= 2;
    }
};
```