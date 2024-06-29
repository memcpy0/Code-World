> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

有 `n` 个气球，编号为`0` 到 `n - 1`，每个气球上都标有一个数字，这些数字存在数组 `nums` 中。

现在要求你戳破所有的气球。戳破第 `i` 个气球，你可以获得 `nums[i - 1] * nums[i] * nums[i + 1]` 枚硬币。 这里的 `i - 1` 和 `i + 1` 代表和 `i` 相邻的两个气球的序号。如果 `i - 1`或 `i + 1` 超出了数组的边界，那么就当它是一个数字为 `1` 的气球。

求所能获得硬币的最大数量。

**示例 1：**
```java
输入：nums = [3,1,5,8]
输出：167
解释：
nums = [3,1,5,8] --> [3,5,8] --> [3,8] --> [8] --> []
coins =  3*1*5    +   3*5*8   +  1*3*8  + 1*8*1 = 167
```
**示例 2：**
```java
输入：nums = [1,5]
输出：10
```
**提示：**
-   `n == nums.length`
-   `1 <= n <= 300`
-   `0 <= nums[i] <= 100`

---
### 解法1 递归+分治+记忆化搜索
对于气球，如果超出边界，那么就是左右各认为是 $1$ ，不妨直接在左右边界添加 $1$ ，形成一个连续的数组 $ref$ 。设 **$dfs(i,j)$ 为戳破 $(i, j)$ 区间内所有气球获得的最大硬币数**，原数组长度为 $n$ ，本题答案即为 $dfs(0, n+1)$ 。

现在，我们可以改变问题——**在一排气球中，戳破气球 $i, j$ 之间的所有气球（不包括 $i, j$ ），使得最终只剩下两个气球 $i, j$ ，最后能得到多少分**？

对于全开区间 $(i, j)$ 的气球要全部戳破，不失一般性地考虑如何转移：
- 递归边界或者基准情形：$0\le i \le n +1,\ i \le j \le i+1$ ，这种情况下开区间 $dfs(i,j)$ 中没有气球可以戳。
- 其他情形： 要求出戳破气球 $i$ 和气球 $j$ 之间的最多硬币数，正向思考「第一个被戳破的气球是哪个」只能导向回溯解法，效率低。
   不难发现，最后会剩下两个气球 $i, j$ ，而 $(i,j)$ 中最后一个被戳破的气球假设为 $k$ ，则戳破 $k$ 获得的硬币数为 $nums[i] \times nums[k] \times nums[j]$ 。此时 $dfs(i,j)$ 可被分成两个规模更小、形式相似的子问题 $dfs(i, k),\ dfs(k, j)$ ，**分而治之**、分别递归求解。$$dfs(i,j) = nums[i] \times nums[k] \times nums[j] + dfs(i, k) +dfs(k,j)$$ ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304211532005.png)
	受到上面的启发，我们可以反向思考，想一下**气球 $i, j$ 之间最后一个被戳破的气球可能是哪个**？其实气球 $i, j$ 之间的所有气球都可能是最后被戳破的那一个！因此，对 $(i,j)$ 间的所有下标 $k$ 分别进行分治，比较得到的所有答案，选最大硬币数即可。

当然，这种写法必然存在大量重复的递归调用。而这里的递归函数是无副作用的，相同的入参必然会返回相同的结果，我们可以用记忆化搜索来优化。
```cpp
class Solution {
public:
    int maxCoins(vector<int> nums) {
        vector<int> ref;
        ref.push_back(1);
        for (int i : nums) ref.push_back(i);
        ref.push_back(1); // 返回戳破(i,j)区间内所有气球得到的最大硬币数
        int dp[310][310]; memset(dp, 0, sizeof(dp));
        function<int(int,int)> dfs = [&](int i, int j) -> int { 
            if (i + 1 == j) return 0; // 空区间
            if (dp[i][j]) return dp[i][j];
            for (int k = i + 1; k < j; ++k) { // 最后戳破气球k得到的最大分数
                dp[i][j] = max(dp[i][j], ref[k] * ref[i] * ref[j] +
                    dfs(i, k) + dfs(k, j));
            }
            return dp[i][j];
        };
        return dfs(0, ref.size() - 1);
    }
};
```
---
### 解法2 动态规划
上述代码稍微修改，改为递推形式，就是动态规划：
```cpp
class Solution {
public:
    int maxCoins(vector<int> nums) {
        vector<int> ref;
        ref.push_back(1);
        for (int i : nums) ref.push_back(i);
        ref.push_back(1); // 返回戳破(i,j)区间内所有气球得到的最大硬币数
        int dp[310][310]; memset(dp, 0, sizeof(dp)); 
        int m = ref.size();
        for (int len = 1; len <= m; ++len) {
            for (int i = 0; i < m; ++i) {
                int j = min(i + len, m - 1); 
                for (int k = i + 1; k < j; ++k) { // 戳破气球k得到的最大分数
                    dp[i][j] = max(dp[i][j], ref[k] * ref[i] * ref[j] +
                        dp[i][k] + dp[k][j]);
                }
            }
        }
        return dp[0][m - 1]; 
    }
};
```