> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个字符串 `s`，请你返回 **两个相同字符之间的最长子字符串的长度** ，计算长度时不含这两个字符。如果不存在这样的子字符串，返回 `-1` 。

**子字符串** 是字符串中的一个连续字符序列。

**示例 1：**
```cpp
输入：s = "aa"
输出：0
解释：最优的子字符串是两个 'a' 之间的空子字符串。
```
**示例 2：**
```cpp
输入：s = "abca"
输出：2
解释：最优的子字符串是 "bc" 。
```
**示例 3：**
```cpp
输入：s = "cbzxy"
输出：-1
解释：s 中不存在出现出现两次的字符，所以返回 -1 。
```
**示例 4：**
```cpp
输入：s = "cabbac"
输出：4
解释：最优的子字符串是 "abba" ，其他的非最优解包括 "bb" 和 "" 。
```
**提示：**
- `1 <= s.length <= 300`
- `s` 只含小写英文字母

---
### 解法 哈希表
求出两个相同字符之间的最长子字符串的长度：对于字符 $ch$，只需要求出 $ch$ 第一次出现在字符串中的索引位置 $first$ 和最后一次出现在字符串中的索引位置 $last$ ，则以 $ch$ 为相同字符之间的子字符串的最大长度一定为 $last−first−1$ ，依次求出所有可能的子字符串的长度的最大值即可。我们设数组 $\textit{firstIndex}$ 记录每个字符 $i$ 在字符串中第一次出现的索引，$maxLength$ 表示当前子字符串的最大长度。

初始化时 $firstIndex$ 中的每个元素都初始化为 $-1$ ，表示该字符还未出现。
- 当遍历到第 $i$ 个字符 $ch$ 时，如果当前数组中 $firstIndex[ch]=−1$ ，则记录该字符第一次出现的索引为 $i$ ，更新 $\textit{firstIndex}[\textit{ch}] = i$ ；
- 如果当前数组中 $firstIndex[ch]≥0$ 时，则表示字符 $ch$ 之前已经出现过，此时两个 $ch$ 之间的子字符串长度为 $i−firstIndex[ch]−1$ ，同时更新 $maxLength$ 。

返回最大的长度 $maxLength$ 即可。
```cpp
// cpp
class Solution {
public:
    int maxLengthBetweenEqualCharacters(string s) {
        int maxLength = -1;
        vector<int> firstIndex(26, -1);
        for (int i = 0; i < s.size(); ++i) {
            if (firstIndex[s[i] - 'a'] < 0) {
                firstIndex[s[i] - 'a'] = i;
            } else {
                maxLength = max(maxLength, i - firstIndex[s[i] - 'a'] - 1);
            }
        }
        return maxLength;
    }
};

// java
class Solution {
    public int maxLengthBetweenEqualCharacters(String s) {
        int maxLength = -1;
        int[] firstIndex = new int[26];
        Arrays.fill(firstIndex, -1);
        for (int i = 0; i < s.length(); ++i) {
            if (firstIndex[s.charAt(i) - 'a'] < 0) {
                firstIndex[s.charAt(i) - 'a'] = i;
            } else {
                maxLength = Math.max(maxLength, i - firstIndex[s.charAt(i) - 'a'] - 1);
            }
        }
        return maxLength;
    }
}

// python
class Solution:
    def maxLengthBetweenEqualCharacters(self, s: str) -> int:
        ans = -1
        firstIndex = {}
        for i, c in enumerate(s):
            if c not in firstIndex:
                firstIndex[c] = i
            else:
                ans = max(ans, i - firstIndex[c] - 1)
        return ans

// go
func maxLengthBetweenEqualCharacters(s string) int {
    ans := -1
    firstIndex := [26]int {}
    for i := range firstIndex {
        firstIndex[i] = -1
    }
    for i, c := range s {
        c -= 'a'
        if firstIndex[c] < 0 {
            firstIndex[c] = i
        } else {
            ans = max(ans, i - firstIndex[c] - 1)
        }
    }
    return ans
}

func max(a, b int) int {
    if b > a {
        return b
    }
    return a
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 表示字符串的长度。我们只需遍历一遍字符串即可。
- 空间复杂度：$O(|\Sigma|)$ ，其中 $\Sigma$ 是字符集，在本题中字符集为所有小写字母，$|\Sigma|=26$ 。
