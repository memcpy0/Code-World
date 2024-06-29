> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


给你一个下标从 **0** 开始的字符串数组 `words` 以及一个二维整数数组 `queries` 。

每个查询 `queries[i] = [li, ri]` 会要求我们统计在 `words` 中下标在 `li` 到 `ri` 范围内（**包含** 这两个值）并且以元音开头和结尾的字符串的数目。

返回一个整数数组，其中数组的第 `i` 个元素对应第 `i` 个查询的答案。

**注意：** 元音字母是 `'a'`、`'e'`、`'i'`、`'o'` 和 `'u'` 。

**示例 1：**
```java
输入：words = ["aba","bcb","ece","aa","e"], queries = [[0,2],[1,4],[1,1]]
输出：[2,3,0]
解释：以元音开头和结尾的字符串是 "aba"、"ece"、"aa" 和 "e" 。
查询 [0,2] 结果为 2（字符串 "aba" 和 "ece"）。
查询 [1,4] 结果为 3（字符串 "ece"、"aa"、"e"）。
查询 [1,1] 结果为 0 。
返回结果 [2,3,0] 。
```
**示例 2：**
```java
输入：words = ["a","e","i"], queries = [[0,2],[0,1],[2,2]]
输出：[3,2,1]
解释：每个字符串都满足这一条件，所以返回 [3,2,1] 。
```
**提示：**
-   `1 <= words.length <= 10^5`
-   `1 <= words[i].length <= 40`
-   `words[i]` 仅由小写英文字母组成
-   `sum(words[i].length) <= 3 * 10^5`
-   `1 <= queries.length <= 10^5`
-   `0 <= queries[j][0] <= queries[j][1] < words.length`

---
### 解法 前缀和
为方便表述，以下将**以元音开头和结尾的字符串**称为「元音字符串」。这道题要求返回一系列查询的答案，每个查询为计算特定区间中的元音字符串数。可以使用前缀和实现区间查询。

用 $n$ 表示数组 $\textit{words}$ 的长度，创建长度为 $n + 1$ 的数组 $\textit{sum}$ ，其中 $sum[i]$ 表示数组 $\textit{words}$ 的前 $i$ 个字符串（即下标范围 $[0, i - 1]$ 的字符串）中的元音字符串数，$\textit{sum}[0] = 0$ 。

从左到右遍历数组 $\textit{words}$ ，对于下标 $0 \le i < n$ ，执行如下操作：
- 如果 $\textit{words}[i]$ 是元音字符串，则 $sum[i + 1] = sum[i] + 1$ ；
- 如果 $\textit{words}[i]$ 不是元音字符串，则 $sum[i+1] = sum[i]$ 。

最后，我们遍历每个查询 $(l,r)$ ，那么当前查询的答案就是 $sum[r+1] - sum[l]$ 。
```cpp
class Solution {
public:
    vector<int> vowelStrings(vector<string>& words, vector<vector<int>>& queries) {
       function<bool(char)> is_vowel = [&](char c) { 
           return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
       };
       vector<int> sum;
       sum.push_back(0);
       for (string word : words)
           sum.push_back(sum.back() + (is_vowel(word[0]) && is_vowel(word.back())));
       vector<int> ans;
       for (vector<int> &q : queries)
           ans.push_back(sum[q[1] + 1] - sum[q[0]]);
       return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n + m)$ 
- 空间复杂度：$O(n)$ 。$n$ 和 $m$ 分别是数组 $words$ 和 $queries$ 的长度。 
