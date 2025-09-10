> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

定义一个函数 `f(s)`，统计 `s`  中 **（按字典序比较）最小字母的出现频次** ，其中 `s` 是一个非空字符串。

例如，若 `s = "dcce"`，那么 `f(s) = 2`，因为字典序最小字母是 `"c"`，它出现了 2 次。

现在，给你两个字符串数组待查表 `queries` 和词汇表 `words` 。对于每次查询 `queries[i]` ，需统计 `words` 中满足 `f(queries[i])` < `f(W)` 的 **词的数目** ，`W` 表示词汇表 `words` 中的每个词。

请你返回一个整数数组 `answer` 作为答案，其中每个 `answer[i]` 是第 `i` 次查询的结果。

**示例 1：**
```java
输入：queries = ["cbd"], words = ["zaaaz"]
输出：[1]
解释：查询 f("cbd") = 1，而 f("zaaaz") = 3 所以 f("cbd") < f("zaaaz")。
```
**示例 2：**
```java
输入：queries = ["bbb","cc"], words = ["a","aa","aaa","aaaa"]
输出：[1,2]
解释：第一个查询 f("bbb") < f("aaaa")，第二个查询 f("aaa") 和 f("aaaa") 都 > f("cc")。
```
**提示：**
- `1 <= queries.length <= 2000`
- `1 <= words.length <= 2000`
- `1 <= queries[i].length, words[i].length <= 10`
- `queries[i][j]`、`words[i][j]` 都由小写英文字母组成

---
### 解法 后缀和
题目定义了一个函数 $f(s)$ ，用于统计字符串 $s$ 中字典序最小的字母的出现频次。然后给定两个字符串数组 $\textit{queries}$ 和 $\textit{words}$ ，要求对于每个 $\textit{queries}[i]$ ，统计 $\textit{words}$ 中有多少个字符串 $\textit{word}$ 满足 $f(\textit{queries}[i]) < f(\textit{word})$ 。

实现函数 $f(s)$ 时，首先初始化一个字符变量 $\textit{ch}$ 为 `'z'` ，表示当前遇到的字典序最小的字母，然后再初始化一个整数 $\textit{cnt} = 0$ 表示该字母的出现次数。接下来依次遍历字符串 $s$ 中的每个字符 $c$ ：
- 如果 $c$ 的字典序小于 $ch$ ，则将 $\textit{ch}$ 更新为 $c$ ，并将 $\textit{cnt}$ 置为 $1$ 。
- 否则如果 $c = \textit{ch}$ ，则令 $\textit{cnt}$ 加 $1$ 。
- 最后，$\textit{cnt}$ 即为 $s$ 中字典序最小的字母的出现次数。

可提前将所有的 $words[i]$ 在 $f(s)$ 中计算一遍，由于 $queries[i]$ 和 $\textit{words}[i]$ 的长度都不超过 $10$ ，因此 $f(s)$ 的范围是 $[1,10]$ 。然后用一个长度固定的整数数组 $\textit{cnt}$ 来统计每种 $f(words[i])$ 的个数，$\textit{queries}[i]$ 对应的答案即为 $\sum_{u=f(\textit{queries}[i]) + 1}^{10} \textit{cnt}[u]$ 。

**为了加快答案的计算，可以倒序遍历 $\textit{cnt}$ ，将 $\textit{cnt}$ 更新为后缀和数组**。这样一来，$queries[i]$ 对应的答案即为 $\textit{cnt}[f(\textit{queries}[i]) + 1]$ 。
```cpp
class Solution {
private:
    int f(string& s) { 
        int cnt = 0;
        char c = 'z';
        for (char ch : s) {
            if (ch < c) {
                c = ch;
                cnt = 1;
            } else if (ch == c) ++cnt;
        }
        return cnt;
    }
public:
    vector<int> numSmallerByFrequency(vector<string>& queries, vector<string>& words) {
        int n = words.size();
        int cnt[12]{0};
        for (auto &s : words) ++cnt[f(s)]; 
        for (int i = 9; i >= 1; --i) cnt[i] += cnt[i + 1]; // 后缀和
        vector<int> ans;
        for (auto &s : queries)
            ans.push_back(cnt[f(s) + 1]);
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O((n + m)p)$ ，其中 $n$ 是 $\textit{queries}$ 的长度，$m$ 是 $\textit{words}$ 的长度，$p$ 是 $\textit{queries}$ 和 $\textit{words}$ 中的最长字符串的长度。
- 空间复杂度：$O(1)$ 。不统计返回值所占用的空间，只使用到了常数个变量。