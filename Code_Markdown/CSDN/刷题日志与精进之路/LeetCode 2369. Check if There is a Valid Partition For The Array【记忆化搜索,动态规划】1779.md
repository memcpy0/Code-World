> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的整数数组 `nums` ，你必须将数组划分为一个或多个 **连续** 子数组。

如果获得的这些子数组中每个都能满足下述条件 **之一** ，则可以称其为数组的一种 **有效** 划分：
1.  子数组 **恰** 由 `2` 个相等元素组成，例如，子数组 `[2,2]` 。
2.  子数组 **恰** 由 `3` 个相等元素组成，例如，子数组 `[4,4,4]` 。
3.  子数组 **恰** 由 `3` 个连续递增元素组成，并且相邻元素之间的差值为 `1` 。例如，子数组 `[3,4,5]` ，但是子数组 `[1,3,5]` 不符合要求。

如果数组 **至少** 存在一种有效划分，返回 `true` ，否则，返回 `false` 。

**示例 1：**
```java
输入：nums = [4,4,4,5,6]
输出：true
解释：数组可以划分成子数组 [4,4] 和 [4,5,6] 。
这是一种有效划分，所以返回 true 。
```
**示例 2：**
```java
输入：nums = [1,1,1,2]
输出：false
解释：该数组不存在有效划分。
```
**提示：**
-   `2 <= nums.length <= 10^5`
-   `1 <= nums[i] <= 10^6`

---
### 解法1 记忆化搜索
对于 `nums = [4,4,4,5,6]`  ，第一个子数组可以是 `[4,4]` 或 `[4,4,4]` 。去掉这段恢复的整数，例如去掉 `[4,4]` 后，剩下要解决的问题就是「判断 `nums = [4,5,6]` 是否存在有效划分」。==这是一个和原问题相似的子问题，所以可以用递归解决==。

根据上面的讨论，递归参数只需要一个 `i` ，**`dfs(i)` 表示数组 `nums[i:n-1]` 是否存在有效划分**。假设**当前划分出的这段有效子数组**（必须符合题意）的结束下标为 `j` ，那么以该子数组开头的 `nums[i:n-1]` 是否存在有效划分，就要看 `dfs(j + 1)` 的返回值。

考虑「能划分的第一段子数组（即**前两个/三个数**）」的不同结束下标 $j$ ，只要其中有一个 $dfs(j + 1)$ 为 $true$ ，则 $dfs(i)$ 也为 $true$ ，否则为 $false$ ，不存在有效划分。
- 递归边界：$dfs(n) = true, dfs(n - 1) = false$ 。
- 递归入口：$dfs(0)$ ，就是答案。
 
不难发现，整个递归中有大量重复递归调用（递归入参相同），==由于**递归函数没有副作用**，同样的入参无论计算多少次，算出来的结果都是一样的，因此可以用**记忆化搜索**来优化==。这里用DP数组进行记忆，为了区分已经访问过的位置，我们额外用一个 `vis` 数组进行标记：
```cpp
class Solution { 
public:
    bool validPartition(vector<int>& nums) { 
        int n = nums.size();
        vector<bool> dp(n + 1); 
        vector<bool> vis(n + 1);
        function<bool(int)> dfs = [&](int i) -> bool {  
            if (i >= n) return dp[n] = vis[i] = true;
            if (i == n - 1) { vis[i] = true; return dp[i] = false; }
            if (vis[i]) return dp[i];
            vis[i] = true; // 标记访问
            if (nums[i] == nums[i + 1]) dp[i] = dp[i] || dfs(i + 2);
            if (i + 2 < n && (nums[i] == nums[i + 1] && nums[i + 1] == nums[i + 2])) 
                dp[i] = dp[i] || dfs(i + 3); 
            if (i + 2 < n && (nums[i] + 1 == nums[i + 1] && nums[i + 1] + 1 == nums[i + 2]))
                dp[i] = dp[i] || dfs(i + 3); 
            return dp[i];
        }; 
        return dfs(0);
    }
};
```

---
### 解法2 动态规划
将上述代码改为递推形式即可。
```cpp
class Solution { 
public:
    bool validPartition(vector<int>& nums) {  
        int n = nums.size();
        vector<bool> dp(n + 1); 
        dp[n] = true, dp[n - 1] = false; 
        for (int i = n - 2; i >= 0; --i) {
            if (nums[i] == nums[i + 1]) dp[i] = dp[i] || dp[i + 2];
            if (i + 2 < n && nums[i] == nums[i + 1] && nums[i + 1] == nums[i + 2])
                dp[i] = dp[i] || dp[i + 3]; 
            if (i + 2 < n && nums[i] + 1 == nums[i + 1] && nums[i + 1] + 1 == nums[i + 2])
                dp[i] = dp[i] || dp[i + 3];
        }
        return dp[0];
    }
};
```
或者这样考虑：
- 如果 $\textit{nums}$ 的**最后两个数**相等，那么去掉这两个数，问题变成剩下 $n-2$ 个数能否有效划分。
- 如果 $\textit{nums}$ 的**最后三个数**相等，那么去掉这三个数，问题变成剩下 $n-3$ 个数能否有效划分。
- 如果 $\textit{nums}$ 的**最后三个数**是连续递增的，那么去掉这三个数，问题变成剩下 $n-3$ 个数能否有效划分。

我们要解决的问题都形如「 **$\textit{nums}$ 的前 $i$ 个数**能否有效划分」。就可以定义 $f[0]=true$ ，$f[i+1]$ 表示能否有效划分 $\textit{nums}[0]$ 到 $\textit{nums}[i]$ 。

根据有效划分的定义，有：
$$f[i+1]=∨ \begin{cases}
f[i−1] ∧ nums[i]=nums[i−1], & i>0 \\
f[i−2] ∧ nums[i]=nums[i−1]=nums[i−2], & i>1\\
f[i−2] ∧ nums[i]=nums[i−1]+1=nums[i−2]+2, & i>1
\end{cases}$$
​
 答案为 $f[n]$ 。

---
### 解法3 动态规划+滚动数组优化
在解法2的基础上，我们发现DP数组只需使用一部分空间，可以用**大小为 $4$ 的滚动数组**进行优化。需要注意的是，已使用过的某个 `dp[i]` 可能被置为了 `true` ，不能直接用 `dp[i]` 进行逻辑或。
```cpp
class Solution { 
public:
    bool validPartition(vector<int>& nums) {  
        int n = nums.size();
        vector<bool> dp(4); 
        dp[n % 4] = true, dp[(n % 4 + 3) % 4] = false; 
        for (int i = n - 2; i >= 0; --i) {
            int r = i % 4; 
            bool flag = false;
            if (nums[i] == nums[i + 1]) flag = flag || dp[(i + 2) % 4];
            if (i + 2 < n && nums[i] == nums[i + 1] && nums[i + 1] == nums[i + 2])
                flag = flag || dp[(i + 3) % 4]; 
            if (i + 2 < n && nums[i] + 1 == nums[i + 1] && nums[i + 1] + 1 == nums[i + 2])
                flag = flag || dp[(i + 3) % 4];
            dp[r] = flag;
        }
        return dp[0];
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$ 