> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串 `s` 和一个单词字典 `dictionary` 。你需要将 `s` 分割成若干个 **互不重叠** 的子字符串，每个子字符串都在 `dictionary` 中出现过。`s` 中可能会有一些 **额外的字符** 不在任何子字符串中。

请你采取最优策略分割 `s` ，使剩下的字符 **最少** 。

**示例 1：**
```java
输入：s = "leetscode", dictionary = ["leet","code","leetcode"]
输出：1
解释：将 s 分成两个子字符串：下标从 0 到 3 的 "leet" 和下标从 5 到 8 的 "code" 。只有 1 个字符没有使用（下标为 4），所以我们返回 1 。
```
**示例 2：**
```java
输入：s = "sayhelloworld", dictionary = ["hello","world"]
输出：3
解释：将 s 分成两个子字符串：下标从 3 到 7 的 "hello" 和下标从 8 到 12 的 "world" 。下标为 0 ，1 和 2 的字符没有使用，所以我们返回 3 。
```
**提示：**
- `1 <= s.length <= 50`
- `1 <= dictionary.length <= 50`
- `1 <= dictionary[i].length <= 50`
- `dictionary[i]` 和 `s` 只包含小写英文字母。
- `dictionary` 中的单词互不相同。

---
我们有一个字符串 $s$ 和一个 $dictionary$ 。目标是将 $s$ 分解为一个或多个不重叠的子字符串，每个子字符串都出现在 $dictionary$ 中，并**最大限度地减少剩余的额外字符数**。

我们可以使用动态规划来解决这个问题。有两种常见的方法：自下而上（迭代）和自上而下（带记忆的递归）。这两种方法都旨在找到最少额外字符数（从字符串末尾遍历到开头）。

初始化：初始化一个 $dp$ 数组，在字符串 $s$ 的每个位置存储最少额外字符数。先要将 $dp$ 中所有元素初始化为一个最大值（例如，$-1$ 或一个大数字），表示它们还没有被计算出来。
### 解法1 记忆化搜索——自上而下方法
设 $n$ 为 $s$ 的长度，我们可以**定义一个递归函数** $dfs(i)$ ，该函数从字符串 $s[i]$ 开始计算最少额外字符数。并使用数组 $dp$ 来存储中间结果，以避免重复计算；将 $dp$ 中的所有元素初始化为一个特殊值（例如 $-1$ ），表示它们还没有计算出来。

在递归函数中，**如果 $dp[i]$ 已经算出则直接返回**；**如果尚未计算**（等于 $-1$ ），则按如下方式计算：
- 初始化 $dp[i]$ ，值为 $1+dfs(s, i+1)$ ，表示在当前字符处断开字符串，并将当前字符 $s[i]$ 作为额外字符，在下一位置找到的最少额外字符数中添加这个额外字符。即直接跳过当前字符 $s[i]$ ，让问题变为 $s$ 的后面 $n - i - 1$ 个字符的子问题。
- 检查 $dictioinary$ 中的每个单词，是否与「从当前位置 $i$ 开始的子串」匹配。如果找到匹配，则将 $dp[i]$ 更新为其当前值和 $1+dfs(s, i+w.size())$ 之间的最小值，其中 $w.size()$ 是匹配单词的长度。

最后返回 $dp[0]$ ，它表示从位置 $0$ 开始的最小额外字符数。
```cpp
class Solution {
public:
    int n;
    vector<int> dp;
    int dfs(string &s, vector<string>& d, int i) {
        if (i >= n) return 0;
        if (dp[i] != -1) return dp[i]; // 已经计算了
        dp[i] = 1 + dfs(s, d, i + 1);
        for (string& w : d) 
            if (i + w.size() <= n && s.compare(i, w.size(), w) == 0)
                dp[i] = min(dp[i], dfs(s, d, i + w.size()));
        return dp[i];
    }
    int minExtraChar(string s, vector<string>& dictionary) {
        n = s.size();
        dp.resize(n, -1);
        return dfs(s, dictionary, 0);
    }
};
```

---
### 解法2 动态规划——自下而上（迭代）
开始从末尾（从右到左）遍历字符串 $s$ 。
1. 对于每个位置 $i$ ，使用值 $1+dp[i+1]$ 初始化 $dp[i]$ 。这表示**在当前字符处断开字符串，并将当前字符 $s[i]$ 作为额外字符，在下一位置找到的最少额外字符数中添加这个额外字符** *adding one extra character to the minimum extra characters found in the next position* 。
2. 然后检查 $dictionary$ 中的每个单词，是否与「从当前位置 $i$ 开始的子串」匹配，即不把 $s[i]$ 作为额外字符去除。如果找到匹配，将 $dp[i]$ 更新为其当前值与 $dp[i+w.size()]$ 两者间最小值，其中 $w.size()$ 是匹配单词的长度。
3. 对字符串 $s$ 中的所有位置继续此过程，**$dp[0]$ 的最终值将表示剩余的最小额外字符数**。

```cpp
class Solution {
public:
    int minExtraChar(string s, vector<string>& dictionary) {
        int n = s.size();
        vector<int> dp(n + 1);
        // dp[i]表示从s[i:n)中分割剩下的最少字符数
        for (int i = n - 1; i >= 0; --i) {
            dp[i] = dp[i + 1] + 1;
            for (string& w : dictionary) 
                if (i + w.size() <= n && s.compare(i, w.size(), w) == 0)
                    dp[i] = min(dp[i], dp[i + w.size()]);
        }
        return dp[0];
    }
};
```
复杂度分析：
- 时间复杂度：$O(nmL)$ ，其中 $n$ 为 $s$ 的长度，$m$ 为 $dictionary$ 的长度，$L$ 为 $\textit{dictionary}$ 所有字符串的长度之和。动态规划的时间复杂度 = 状态个数 $\times$ 单个状态的计算时间。本题中状态个数等于 $O(n)$ ，单个状态的计算时间为 $O(mL)$ 。
- 空间复杂度：$O(n)$ 。