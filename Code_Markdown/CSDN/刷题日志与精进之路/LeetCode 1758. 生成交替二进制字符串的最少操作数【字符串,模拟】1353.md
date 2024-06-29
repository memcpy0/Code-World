> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个仅由字符 `'0'` 和 `'1'` 组成的字符串 `s` 。一步操作中，你可以将任一 `'0'` 变成 `'1'` ，或者将 `'1'` 变成 `'0'` 。

**交替字符串** 定义为：如果字符串中不存在相邻两个字符相等的情况，那么该字符串就是交替字符串。例如，字符串 `"010"` 是交替字符串，而字符串 `"0100"` 不是。

返回使 `s` 变成 **交替字符串** 所需的 **最少** 操作数。

**示例 1：**
```java
输入：s = "0100"
输出：1
解释：如果将最后一个字符变为 '1' ，s 就变成 "0101" ，即符合交替字符串定义。
```
**示例 2：**
```java
输入：s = "10"
输出：0
解释：s 已经是交替字符串。
```
**示例 3：**
```java
输入：s = "1111"
输出：2
解释：需要 2 步操作得到 "0101" 或 "1010" 。
```
**提示：**
- `1 <= s.length <= 10^4`
- `s[i]` 是 `'0'` 或 `'1'`

---
### 解法 模拟
根据题意，经过多次操作，$s$ 可能会变成两种不同的交替二进制字符串，即：
- 开头为 $0$ ，后续交替的字符串；
- 开头为 $1$ ，后续交替的字符串。

注意到，**变成这两种不同的交替二进制字符串所需要的最少操作数加起来等于 $s$ 的长度**，我们只需要计算出变为其中一种字符串的最少操作数，就可以推出另一个最少操作数，然后取最小值即可。
```cpp
// cpp
class Solution {
public:
    int minOperations(string s) {
        int cnt = 0;
        for (int i = 0; i < s.size(); ++i) {
            char c = s[i];
            if (c != ('0' + i % 2)) ++cnt;
        }
        return min(cnt, (int)s.size() - cnt);
    }
};

// java
class Solution {
    public int minOperations(String s) {
        int cnt = 0;
        for (int i = 0; i < s.length(); ++i) {
            char c = s.charAt(i);
            if (c != (char)('0' + i % 2)) ++cnt;
        }
        return Math.min(cnt, s.length() - cnt);
    }
}

// python
class Solution:
    def minOperations(self, s: str) -> int:
        cnt = sum(int(c) != i % 2 for i, c in enumerate(s)) # 010101...
        return min(cnt, len(s) - cnt)

// go
func minOperations(s string) int {
    cnt := 0
    for i, c := range s {
        if i % 2 != int(c - '0') {
            cnt++
        }
    }
    return min(cnt, len(s) - cnt)
}
func min(a, b int) int {
    if a > b {
        return b
    }
    return a
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为输入 $s$ 的长度，仅需遍历一遍字符串。
- 空间复杂度：$O(1)$ ，只需要常数额外空间。