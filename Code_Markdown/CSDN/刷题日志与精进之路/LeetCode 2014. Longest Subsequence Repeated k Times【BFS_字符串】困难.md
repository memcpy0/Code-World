> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>You are given a string <code>s</code> of length <code>n</code>, and an integer <code>k</code>. You are tasked to find the <strong>longest subsequence repeated</strong> <code>k</code> times in string <code>s</code>.</p>

<p>A <strong>subsequence</strong> is a string that can be derived from another string by deleting some or no characters without changing the order of the remaining characters.</p>

<p>A subsequence <code>seq</code> is <strong>repeated</strong> <code>k</code> times in the string <code>s</code> if <code>seq * k</code> is a subsequence of <code>s</code>, where <code>seq * k</code> represents a string constructed by concatenating <code>seq</code> <code>k</code> times.</p>

<ul>
	<li>For example, <code>"bba"</code> is repeated <code>2</code> times in the string <code>"bababcba"</code>, because the string <code>"bbabba"</code>, constructed by concatenating <code>"bba"</code> <code>2</code> times, is a subsequence of the string <code>"<strong><u>b</u></strong>a<strong><u>bab</u></strong>c<strong><u>ba</u></strong>"</code>.</li>
</ul>

<p>Return <em>the <strong>longest subsequence repeated</strong> </em><code>k</code><em> times in string </em><code>s</code><em>. If multiple such subsequences are found, return the <strong>lexicographically largest</strong> one. If there is no such subsequence, return an <strong>empty</strong> string</em>.</p>


<p><strong>Example 1:</strong></p>
<img style="width: 457px; height: 99px;" src="https://assets.leetcode.com/uploads/2021/08/30/longest-subsequence-repeat-k-times.png" alt="example 1">
 

```cpp
Input: s = "letsleetcode", k = 2
Output: "let"
Explanation: There are two longest subsequences repeated 2 times: "let" and "ete".
"let" is the lexicographically largest one.
```

<p><strong>Example 2:</strong></p>

```cpp
Input: s = "bb", k = 2
Output: "b"
Explanation: The longest subsequence repeated 2 times is "b".
```

<p><strong>Example 3:</strong></p>

```cpp
Input: s = "ab", k = 2
Output: ""
Explanation: There is no subsequence repeated 2 times. Empty string is returned.
```
<p><strong>Example 4:</strong></p>

```cpp
Input: s = "bbabbabbbbabaababab", k = 3
Output: "bbbb"
Explanation: The longest subsequence "bbbb" is repeated 3 times in "bbabbabbbbabaababab".
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == s.length</code></li>
	<li><code>2 &lt;= n, k &lt;= 2000</code></li>
	<li><code>2 &lt;= n &lt; k * 8</code></li>
	<li><code>s</code> consists of lowercase English letters.</li>
</ul>

题意：给你一个长度为 <code>n</code> 的字符串 <code>s</code> ，和一个整数 <code>k</code> 。请你找出字符串 <code>s</code> 中 <strong>重复</strong> <code>k</code> 次的 <strong>最长子序列</strong> 。l
<p>如果&nbsp;<code>seq * k</code> 是 <code>s</code> 的一个子序列，其中 <code>seq * k</code> 表示一个由 <code>seq</code> 串联 <code>k</code>&nbsp;次构造的字符串，那么就称 <code>seq</code><strong> </strong>是字符串 <code>s</code> 中一个 <strong>重复 <code>k</code> 次</strong> 的子序列。</p>


<p>返回字符串 <code>s</code> 中 <strong>重复 k 次的最长子序列</strong>&nbsp; 。如果存在多个满足的子序列，则返回 <strong>字典序最大</strong> 的那个。如果不存在这样的子序列，返回一个 <strong>空</strong> 字符串。</p>


---
### 解法 BFS+剪枝
初一看，可能是懵的，这是个什么题啊？不过看到最后 `2 <= n, k <= 2000, 2 <= n < k * 8` ，就能有点想法了。根据「鸽笼/抽屉原理」，如果有 `8` 个出现次数达到 `k` 次的字母，就达到了 `8 * k` ，超过了字符串 `s` 的长度 `n`  。

因此，想象长度为 `m` 的模式串彼此连接、重复 `k` 次，仍然是 `s` 的子序列，就很容易发现「**模式串长度不超过 `7`**」这一隐藏条件。由于小写字母只有 `26` 个，我们枚举长度 `[1, 7]` 的模式串——或者说由长度为 `1` 的模式串逐步BFS，每次对当前模式串添加一个新字母得到新模式串 `t` ，如果在字符串 `s` 中找不到 `k` 个及以上的 `t` 时，就不要尝试在 `t` 后面添加新字母了（**剪枝策略**），否则将 `t` 入队。如此不断操作，直到队列为空。

一个小小的疑问是，为什么明明是**重复 `k` 次的最长子序列**，却可以重复更多次呢？很简单，仔细看题意——如果 `seq * k` 是 `s` 的子序列，且 `seq * k` 表示由 `seq` 串联 `k` 次构造的字符串，则 `seq` 就是 `s` 中一个**重复 `k` 次的子序列**。显而易见，`seq` 如果在 `s` 中重复了 `k` 次以上，那么 `seq * k` 也必然是 `s` 的子序列。

实际代码如下所示。整个算法的理论时间复杂度为 $O(24^7 * n * m)$ ，不过由于剪枝的使用，实际效率高得多。
```cpp
//C++ version
class Solution {
private:
    int checkCount(const string &sub, const string &s) {
        int n = sub.size(), m = s.size(), i = 0, j = 0, cnt = 0;
        while (j < m) {
            if (sub[i] == s[j]) {
                ++i;
                if (i == n) ++cnt, i = 0;
            }
            ++j;
        }
        return cnt;
    }
public:
    string longestSubsequenceRepeatedK(string s, int k) {
        int n = s.size();
        if (n < k) return ""; //n个字符不足以让一个子序列重复k次
        queue<string> q; q.push("");
        string ans;
        while (!q.empty()) {
            string p = q.front(); q.pop();
            ans = p; //p是更长的、字典序更大的，重复k次的子序列
            for (char c = 'a'; c <= 'z'; ++c) {
                //从a加到z,保证队列中的数据一定按照字典序、且长度非递减排列
                string t = p + c;
                if (checkCount(t, s) >= k) q.push(t);
            }
        }
        return ans;
    }
};
//执行用时：584 ms, 在所有 C++ 提交中击败了40.63% 的用户
//内存消耗：8 MB, 在所有 C++ 提交中击败了70.76% 的用户
```
