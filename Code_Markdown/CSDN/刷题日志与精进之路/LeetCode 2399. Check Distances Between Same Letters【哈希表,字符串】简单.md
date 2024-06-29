> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a **0-indexed** string `s` consisting of only lowercase English letters, where each letter in `s` appears **exactly** **twice**. You are also given a **0-indexed** integer array `distance` of length `26`.

Each letter in the alphabet is numbered from `0` to `25` (i.e. `'a' -> 0`, `'b' -> 1`, `'c' -> 2`, ... , `'z' -> 25`).

In a **well-spaced** string, the number of letters between the two occurrences of the `ith` letter is `distance[i]`. If the `ith` letter does not appear in `s`, then `distance[i]` can be **ignored**.

Return `true` _if_ `s` _is a **well-spaced** string, otherwise return_ `false`.

**Example 1:**
```java
Input: s = "abaccb", distance = [1,3,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
Output: true
Explanation:
- 'a' appears at indices 0 and 2 so it satisfies distance[0] = 1.
- 'b' appears at indices 1 and 5 so it satisfies distance[1] = 3.
- 'c' appears at indices 3 and 4 so it satisfies distance[2] = 0.
Note that distance[3] = 5, but since 'd' does not appear in s, it can be ignored.
Return true because s is a well-spaced string.
```
**Example 2:**
```java
Input: s = "aa", distance = [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
Output: false
Explanation:
- 'a' appears at indices 0 and 1 so there are zero letters between them.
Because distance[0] = 1, s is not a well-spaced string.
```
**Constraints:**
-   `2 <= s.length <= 52`
-   `s` consists only of lowercase English letters.
-   Each letter appears in `s` exactly twice.
-   `distance.length == 26`
-   `0 <= distance[i] <= 50`

题意：字符串 $s$ 中每个小写字母都出现了两次，看相同的两个小写字母之间的字符数是否等于 `distance[i]` ，`i` 为该小写字母在字母表中出现的顺序。

---
### 解法 哈希表
只要能想办法把初始状态与遍历过一次的状态区分开来，就很简单：
```cpp
class Solution {
public:
    bool checkDistances(string s, vector<int>& distance) {
        int d[26]; memset(d, -1, sizeof(d));
        for (int i = 0, n = s.size(); i < n; ++i) {
            int v = s[i] - 'a';
            if (d[v] == -1) d[v] = i;
            else if (i - d[v] - 1 != distance[v]) return false;
        }
        return true;
    }
};
```