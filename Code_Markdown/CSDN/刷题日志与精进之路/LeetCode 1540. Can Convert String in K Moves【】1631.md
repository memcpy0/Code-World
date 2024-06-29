> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个字符串 `s` 和 `t` ，你的目标是在 `k` 次操作以内把字符串 `s` 转变成 `t` 。

在第 `i` 次操作时（`1 <= i <= k`），你可以选择进行如下操作：

- 选择字符串 `s` 中满足 `1 <= j <= s.length` 且之前未被选过的任意下标 `j` （下标从 1 开始），并将此位置的字符切换 `i` 次。
- 不进行任何操作。

切换 1 个字符的意思是用字母表中该字母的下一个字母替换它（字母表环状接起来，所以 `'z'` 切换后会变成 `'a'`）。第 `i` 次操作意味着该字符应切换 `i` 次

请记住任意一个下标 `j` 最多只能被操作 1 次。

如果在不超过 `k` 次操作内可以把字符串 `s` 转变成 `t` ，那么请你返回 `true` ，否则请你返回 `false` 。

**示例 1：**

**输入：**s = "input", t = "ouput", k = 9
**输出：**true
**解释：**第 6 次操作时，我们将 'i' 切换 6 次得到 'o' 。第 7 次操作时，我们将 'n' 切换 7 次得到 'u' 。

**示例 2：**

**输入：**s = "abc", t = "bcd", k = 10
**输出：**false
**解释：**我们需要将每个字符切换 1 次才能得到 t 。我们可以在第 1 次操作时将 'a' 切换成 'b' ，但另外 2 个字母在剩余操作中无法再转变为 t 中对应字母。

**示例 3：**

**输入：**s = "aab", t = "bbb", k = 27
**输出：**true
**解释：**第 1 次操作时，我们将第一个 'a' 切换 1 次得到 'b' 。在第 27 次操作时，我们将第二个字母 'a' 切换 27 次得到 'b' 。

**提示：**

- `1 <= s.length, t.length <= 10^5`
- `0 <= k <= 10^9`
- `s` 和 `t` 只包含小写英文字母。

```cpp
class Solution {
public:
    bool canConvertString(string s, string t, int k) {
        if (s.size() != t.size()) return false;
        int n = s.size();
        unordered_map<int, int> chg;
        for (int i = 0; i < n; ++i) {
            if (s[i] < t[i]) {
                // cout << (t[i] - s[i]) << endl;
                ++chg[t[i] - s[i]];
            } else if (s[i] > t[i]) {
                ++chg['z' - s[i] + 1 + t[i] - 'a'];
                // cout << ('z' - s[i] + 1 + t[i] - 'a') << endl;
            }
        }
        for (auto it : chg)
            if (it.first > k || (it.second - 1) * 26 + it.first > k) return false;
        return true;
    }
};
```