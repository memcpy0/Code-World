> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给出一个单词数组 `words` ，其中每个单词都由小写英文字母组成。如果我们可以 **不改变其他字符的顺序** ，在 `wordA` 的任何地方添加 **恰好一个** 字母使其变成 `wordB` ，那么我们认为 `wordA` 是 `wordB` 的 **前身** 。
-   例如，`"abc"` 是 `"abac"` 的 **前身** ，而 `"cba"` 不是 `"bcad"` 的 **前身**

**词链**是单词 `[word_1, word_2, ..., word_k]` 组成的序列，`k >= 1`，其中 `word1` 是 `word2` 的前身，`word2` 是 `word3` 的前身，依此类推。一个单词通常是 `k == 1` 的 **单词链** 。从给定单词列表 `words` 中选择单词组成词链，返回 词链的 **最长可能长度** 。  

**示例 1：**
```java
输入：words = ["a","b","ba","bca","bda","bdca"]
输出：4
解释：最长单词链之一为 ["a","ba","bda","bdca"]
```
**示例 2:**
```java
输入：words = ["xbc","pcxbcf","xb","cxbc","pcxbc"]
输出：5
解释：所有的单词都可以放入单词链 ["xb", "xbc", "cxbc", "pcxbc", "pcxbcf"].
```
**示例 3:**
```java
输入：words = ["abcd","dbqca"]
输出：1
解释：字链["abcd"]是最长的字链之一。
["abcd"，"dbqca"]不是一个有效的单词链，因为字母的顺序被改变了。
```
**提示：**
-   `1 <= words.length <= 1000`
-   `1 <= words[i].length <= 16`
-   `words[i]` 仅由小写英文字母组成。

---
### 解法1 记忆化搜索+哈希表
对于字符串 $s$ 来说，假设它是词链的最后一个单词，那么去掉 $s$ 中的一个字母，设新字符串为 $t$ ，问题就变成计算以 $t$ 结尾的词链的最长长度。由于这是一个和原问题相似的子问题，因此可以用递归解决。

直接把字符串作为递归的参数，定义 $dfs(s)$ 表示以 $s$ 结尾的词链的最长长度。**由于字符串的长度不超过 $16$ ，暴力枚举去掉的字符**，设新字符串为 $t$ 且**在 $words$ 中**，则有
$$\textit{dfs}(s) = \max\{\textit{dfs}(t)\} + 1$$ 
为了快速判断字符串是否在 $words$ 中，需要将所有 $\textit{words}[i]$ 存入哈希表 $ws$ 中。

**由于 `"aba"` 和 `"aca"` 去掉中间字母都会变成 `"aa"` ，为避免重复计算，代码实现时可以用记忆化搜索**。进一步地，可以直接把计算结果存到 $ws$ 中。（Python 可以使用 `@cache` ）
```java
class Solution {
    private Map<String, Integer> ws = new HashMap<>();
    public int longestStrChain(String[] words) {
        for (var s : words) ws.put(s, 0); // 0表示未被计算
        int ans = 0;
        for (var s : ws.keySet())
            ans = Math.max(ans, dfs(s));
        return ans;
    }
    private int dfs(String s) { // dfs(s)表示以s为最后一个单词的链的最长长度
        int res = ws.get(s);
        if (res > 0) return res; // 之前计算过
        for (int i = 0; i < s.length(); ++i) { // 枚举去掉s[i] 
            var t = s.substring(0, i) + s.substring(i + 1);
            if (ws.containsKey(t)) // t在words中
                res = Math.max(res, dfs(t));
        }
        ws.put(s, res + 1); // 记忆化
        return res + 1;
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(nL^2)$ ，其中 $n$ 为 $words$ 的长度，$L$ 为字符串的最大长度，本题不超过 $16$ 。动态规划的时间复杂度 = 状态个数 $\times$ 单个状态的计算时间。这里状态个数为 $\mathcal{O}(n)$ ，单个状态的计算时间为 $\mathcal{O}(L^2)$，因此时间复杂度为 $\mathcal{O}(nL^2)$ 。
- 空间复杂度：$\mathcal{O}(nL)$ 。每个状态都需要 $\mathcal{O}(L)$ 的空间。

---
### 解法2 动态规划——1:1 翻译成递推
我们可以去掉递归中的「递」，只保留「归」的部分，即自底向上计算。对于本题，只需要把递归改成循环。==由于总是从短的字符串转移到长的字符串，所以要先把字符串按长度从小到大排序，然后从短的开始递推==。
```java
class Solution {
    private Map<String, Integer> ws = new HashMap<>();
    public int longestStrChain(String[] words) {
        Arrays.sort(words, (a, b) -> a.length() - b.length());
        int ans = 0;
        for (var s : words) {
            int res = 0;
            for (int i = 0; i < s.length(); ++i) { // 枚举去掉s[i]
                var t = s.substring(0, i) + s.substring(i + 1);
                res = Math.max(res, ws.getOrDefault(t, 0));
            }
            ws.put(s, res + 1);
            ans = Math.max(ans, res + 1);
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n\log n + nL^2)$ ，其中 $n$ 为 $words$ 的长度，$L$ 为字符串的最大长度，本题不超过 $16$ 。排序的时间复杂度为 $\mathcal{O}(n\log n)$（注意只比较长度）。动态规划的时间复杂度 = 状态个数 $\times$ 单个状态的计算时间，这里状态个数为 $\mathcal{O}(n)$ ，单个状态的计算时间为 $\mathcal{O}(L^2)$ 。
- 空间复杂度：$\mathcal{O}(nL)$ 。每个状态都需要 $\mathcal{O}(L)$ 的空间。