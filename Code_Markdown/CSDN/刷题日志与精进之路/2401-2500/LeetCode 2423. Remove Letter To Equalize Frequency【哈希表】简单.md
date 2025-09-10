> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的字符串 `word` ，字符串只包含小写英文字母。你需要选择 **一个** 下标并 **删除** 下标处的字符，使得 `word` 中剩余每个字母出现 **频率** 相同。

如果删除一个字母后，`word` 中剩余所有字母的出现频率都相同，那么返回 `true` ，否则返回 `false` 。

**注意：**
- 字母 `x` 的 **频率** 是这个字母在字符串中出现的次数。
- 你 **必须** 恰好删除一个字母，不能一个字母都不删除。

**示例 1：**
```java
输入：word = "abcc"
输出：true
解释：选择下标 3 并删除该字母，word 变成 "abc" 且每个字母出现频率都为 1 。
```
**示例 2：**
```java
输入：word = "aazz"
输出：false
解释：我们必须删除一个字母，所以要么 "a" 的频率变为 1 且 "z" 的频率为 2 ，要么两个字母频率反过来。所以不可能让剩余所有字母出现频率相同。
```
**提示：**
-   `2 <= word.length <= 100`
-   `word` 只包含小写英文字母。

---
### 解法1 暴力枚举删除的字符
枚举 $word[i]$ ，将其去掉后再**统计剩余字符的出现次数**。用数组或者哈希表统计都行。如果剩余字符的出现次数都相同，则返回 `true` 。反之，如果无论去掉哪个 $word[i]$ ，都无法让剩余字符的出现次数都相同，则返回 `false` 。
```cpp
class Solution {
    bool isSame(unordered_map<char, int> &cnt) {
        int c0 = cnt.begin()->second;
        for (auto &[_, c] : cnt) 
            if (c != c0) return false;
        return true;
    }
public:
    bool equalFrequency(string word) { 
        for (int i = 0, n = word.size(); i < n; ++i) { // 枚举删除的字符
            unordered_map<char, int> cnt;
            for (int j = 0; j < n; ++j)
                if (j != i) ++cnt[word[j]]; // 统计出现次数
            if (isSame(cnt)) // 出现次数都一样
                return true;
        } 
        return false;
    }
}; 
```
复杂度分析：
- 时间复杂度：$O(n^2)$ ，其中 n 为 $word$ 长度。
- 空间复杂度：$O(|\Sigma|)$，其中 $|\Sigma|$ 为字符集的大小，本题字符均为小写字母，所以 $|\Sigma| =26$ 。

---
### 解法2 分类讨论
如果有至少三种不同的出现次数，无论去掉哪个字符，剩下的出现次数仍然至少有两种。所以**只需要讨论出现次数至多两种的情况**：
1. 如果出现次数只有一种：
	1. **如果只有一种字符**，例如 $\texttt{"aaaaa"}$ ，那么无论删除哪个都是满足要求的。
	2. **如果每个字符恰好出现一次**，例如 $\texttt{"abcde"}$ ，那么无论删除哪个都是满足要求的。代码实现时，可以合并到下面的「较少的出现次数恰好是 $1$ 」的情况中。
	3. **如果每个字符出现不止一次**，例如 $\texttt{"aabbcc"}$ ，虽然出现次数均为 $2$ ，但题目要求恰好去掉一个字符，所以无法满足要求。
2. **如果出现次数有两种，那么必须变成一种**：
	1. 考虑**去掉出现次数较少的字符**：**它的出现次数必须恰好是 $1$ ，且只有这一种字符出现一次**。例如 $\texttt{"abbbccc"}$ ，去掉只出现一次的字符，就能满足要求。但如果它的出现次数大于 $1$ ，例如 $\texttt{"aabbbccc"}$ ，就无法满足要求。
	2. 考虑**去掉出现次数较多的字符**：**它的出现次数必须比出现次数较少的恰好多 $1$，且只有这一种字符出现一次**。例如 $\texttt{"aabbccc"}$ 去掉 $\texttt{c}$ 是可以满足要求的。其它情况如 $\texttt{"abccc"}$ 或 $\texttt{"abccdd"}$ 都是无法满足要求的。

```cpp
class Solution { 
public:
    bool equalFrequency(string word) { 
        unordered_map<char, int> rec;
        for (char c : word) ++rec[c];
        vector<int> cnt;
        for (auto &[_, c] : rec) cnt.push_back(c);
        sort(cnt.begin(), cnt.end()); // 出现次数从小到大排序
        // 只有1种字符 or 去掉次数最少的 or 出掉次数最多的
        return cnt.size() == 1 || 
            cnt[0] == 1 && equal(cnt.begin() + 2, cnt.end(), cnt.begin() + 1) ||
            cnt.back() == cnt[cnt.size() - 2] + 1 && equal(cnt.begin() + 1, cnt.end() - 1, cnt.begin());
    }
}; 
```
复杂度分析：
- 时间复杂度：$O(n+|\Sigma|\log |\Sigma|)$ ，其中 n 为 $word$ 长度，$|\Sigma|$ 为字符集的大小，本题字符均为小写字母，所以 $|\Sigma| =26$ 。
- 空间复杂度：$O(|\Sigma|)$ 。

是可以优化到 $O(n)$ 复杂度的，但对本题来说没有太多意义：
```cpp
class Solution {
public:
    bool equalFrequency(string word) {
        int cnt[26]{};
        for(char c : word) ++cnt[c-'a'];
        int maxCnt = 0, minCnt = word.size();
        int allCntCnt = 0; // 出现的字符种数
        for(int i = 0; i < 26; ++i) {
            if (cnt[i]) {
                ++allCntCnt;
                maxCnt=max(cnt[i], maxCnt);
                minCnt=min(cnt[i], minCnt);
            }
        }
        // (出现次数只有一种): 所有字符出现次数为1或只有一种字符的情况
        if (allCntCnt == 1 || maxCnt == 1) return true;
        int maxCntCnt = 0; // 出现次数最多的字符种数
        for(int i = 0; i < 26; ++i)
            if (cnt[i] == maxCnt)
                ++maxCntCnt;
        // 出现次数最大的字符和出现次数最少的字符仅相差1次, 且只有一种出现次数最大的字符
        // 或者出现次数最少的字符次数为1次, 且其它字符出现次数均相同
        return (maxCnt - minCnt == 1 && maxCntCnt == 1) || 
            (minCnt == 1 && maxCntCnt == allCntCnt - 1);
    }
};
```