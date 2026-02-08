> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数 `finalSum` 。请你将它拆分成若干个 **互不相同** 的正偶数之和，且拆分出来的正偶数数目 **最多** 。
- 比方说，给你 `finalSum = 12` ，那么这些拆分是 **符合要求** 的（互不相同的正偶数且和为 `finalSum`）：`(2 + 10)` ，`(2 + 4 + 6)` 和 `(4 + 8)` 。它们中，`(2 + 4 + 6)` 包含最多数目的整数。注意 `finalSum` 不能拆分成 `(2 + 2 + 4 + 4)` ，因为拆分出来的整数必须互不相同。

请你返回一个整数数组，表示将整数拆分成 **最多** 数目的正偶数数组。如果没有办法将 `finalSum` 进行拆分，请你返回一个 **空** 数组。你可以按 **任意** 顺序返回这些整数。

**示例 1：**
```java
输入：finalSum = 12
输出：[2,4,6]
解释：以下是一些符合要求的拆分：(2 + 10)，(2 + 4 + 6) 和 (4 + 8) 。
(2 + 4 + 6) 为最多数目的整数，数目为 3 ，所以我们返回 [2,4,6] 。
[2,6,4] ，[6,2,4] 等等也都是可行的解。
```
**示例 2：**
```java
输入：finalSum = 7
输出：[]
解释：没有办法将 finalSum 进行拆分。
所以返回空数组。
```
**示例 3：**
```java
输入：finalSum = 28
输出：[6,8,2,12]
解释：以下是一些符合要求的拆分：(2 + 26)，(6 + 8 + 2 + 12) 和 (4 + 24) 。
(6 + 8 + 2 + 12) 有最多数目的整数，数目为 4 ，所以我们返回 [6,8,2,12] 。
[10,2,4,12] ，[6,2,4,16] 等等也都是可行的解。
```
**提示：**
- $1 \le finalSum \le 10^{10}$

---
### 解法1 贪心+DFS+回溯
```cpp
class Solution {
public:
    vector<long long> ans;
    bool dfs(long long sum, long long b) {
        if (sum == 0) return true;
        for (int i = b; i <= sum; i += 2) {
            ans.push_back(i);
            if (dfs(sum - i, i + 2)) return true;
            ans.pop_back();
        }
        return false;
    }
    vector<long long> maximumEvenSplit(long long finalSum) {
        if (finalSum & 1) return {};
        dfs(finalSum, 2);
        return ans;
    }
};
```
上面的写法不简洁。

---
### 解法2 贪心+循环
首先，如果 $finalSum$ 为奇数，那么无法拆分为若干偶数，返回空数组即可。

其次，由于希望拆分成尽可能多的偶数，我们应从最小的偶整数 $2$ 开始依次尝试拆分，直到剩余数值小于等于当前被拆分的最大偶整数为止。此时，==已经拆分成尽可能多的偶数，不可能拆分出更多的互不相同的偶数。如果此时拆分后剩余的 $finalSum$ 大于零，则将这个数值加到最大的偶整数上，从而保证所有的数互不相同==。
```cpp
class Solution {
public:
    vector<long long> maximumEvenSplit(long long finalSum) {
        vector<long long> ans;
        if (finalSum & 1) return ans;
        for (int i = 2; i <= finalSum; i += 2) {
            ans.push_back(i);
            finalSum -= i;
        }
        ans.back() += finalSum;
        return res;
    }
};
```
复杂度分析：
- 时间复杂度：$O(\sqrt{n})$ ，即为拆分后的整数个数，其中 $n = \textit{finalSum}$ 。具体而言，若一个数想拆成 $k$ 个互不相同的偶数，则该数必定大于等于 $\sum_{i = 1}^k 2 \times i = k\times(k + 1)$ ，因此对于整数 $n$ ，可以拆分成的互不相同偶数个数最多大约为 $O(\sqrt{n})$ 个。
- 空间复杂度：$O(1)$ ，输出数组不计入空间复杂度。