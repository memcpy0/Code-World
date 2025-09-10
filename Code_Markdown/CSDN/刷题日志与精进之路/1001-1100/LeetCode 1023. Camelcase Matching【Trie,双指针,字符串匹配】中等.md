> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

> 相关公司：JP Morgan、亚马逊、Facebook、
 
Given an array of strings `queries` and a string `pattern`, return a boolean array `answer` where `answer[i]` is `true` if `queries[i]` matches `pattern`, and `false` otherwise.

A query word `queries[i]` matches `pattern` if you can insert lowercase English letters pattern so that it equals the query. You may insert each character at any position and you may not insert any characters.

**Example 1:**
```java
Input: queries = ["FooBar","FooBarTest","FootBall","FrameBuffer","ForceFeedBack"], pattern = "FB"
Output: [true,false,true,true,false]
Explanation: "FooBar" can be generated like this "F" + "oo" + "B" + "ar".
"FootBall" can be generated like this "F" + "oot" + "B" + "all".
"FrameBuffer" can be generated like this "F" + "rame" + "B" + "uffer".
```
**Example 2:**
```java
Input: queries = ["FooBar","FooBarTest","FootBall","FrameBuffer","ForceFeedBack"], pattern = "FoBa"
Output: [true,false,true,false,false]
Explanation: "FooBar" can be generated like this "Fo" + "o" + "Ba" + "r".
"FootBall" can be generated like this "Fo" + "ot" + "Ba" + "ll".
```
**Example 3:**
```java
Input: queries = ["FooBar","FooBarTest","FootBall","FrameBuffer","ForceFeedBack"], pattern = "FoBaT"
Output: [false,true,false,false,false]
Explanation: "FooBarTest" can be generated like this "Fo" + "o" + "Ba" + "r" + "T" + "est".
```
**Constraints:**
-   `1 <= pattern.length, queries.length <= 100`
-   `1 <= queries[i].length <= 100`
-   `queries[i]` and `pattern` consist of English letters.

题意：如果我们可以将**小写字母**插入模式串 `pattern` 得到待查询项 `query`，那么待查询项与给定模式串匹配。（我们可以在任何位置插入每个字符，也可以插入 0 个字符。）

给定待查询列表 `queries`，和模式串 `pattern`，返回由布尔值组成的答案列表 `answer`。只有在待查项 `queries[i]` 与模式串 `pattern` 匹配时， `answer[i]` 才为 `true`，否则为 `false` 。

---
### 解法1 双指针 
**如果 `pattern` 是 `queries[i]` 的子序列，并且去掉 `pattern` 之后 `queries[i]` 的剩余部分都由小写字母构成**，那么 `queries[i]` 就与 `pattern` 匹配。

具体来说，我们维护一个下标 `j` ，用来遍历 `pattern` ，然后遍历 `queries[i]` 中的每个字符 `c` ：
- 如果 `j < pattern.length` ，并且 `pattern[j] = c` ，那么令 `j` 加1。
- 否则（即 **`pattern` 遍历结束或两个字符不匹配**），考虑 **`c` 是否是一个大写字母**。如果是，则匹配失败；如果不是，则该小写字母可以插入 `pattern` 来与 `queries[i]` 匹配，因此可继续遍历下一个字符。

**`queries[i]` 遍历结束后**，如果 `p < pattern.length` ，则表示 `pattern` 中还有字符未被匹配（显而易见的失败情况），`queries[i]` 与 `pattern` 匹配失败**。其余情况 `pattern` 匹配完毕，匹配成功——即 **`queries[i]` 和 `pattern` 都要能遍历到结尾，就可以匹配**。
```cpp
class Solution {
public:
    vector<bool> camelMatch(vector<string>& queries, string pattern) {
        int n = queries.size();
        vector<bool> ans(n, true);
        for (int i = 0; i < n; ++i) {
            int j = 0;
            for (auto c : queries[i]) {
                if (j < pattern.size() && c == pattern[j]) ++j;
                else if (isupper(c)) {
                    ans[i] = false;
                    break;
                }
            }
            if (j < pattern.size()) ans[i] = false; 
            // if (j < plen || k < queries[i].size()) ans[i] = false;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(nm)$ 。
- 空间复杂度：$O(1)$ 。我们忽略返回值的空间复杂度，过程中只使用了常数个变量。



 