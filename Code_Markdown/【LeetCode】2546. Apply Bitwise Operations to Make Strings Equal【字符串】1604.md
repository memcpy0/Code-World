> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

[2546. Apply Bitwise Operations to Make Strings Equal](https://leetcode.cn/problems/apply-bitwise-operations-to-make-strings-equal/)

给你两个下标从 **0** 开始的 **二元** 字符串 `s` 和 `target` ，两个字符串的长度均为 `n` 。你可以对 `s` 执行下述操作 **任意** 次：

- 选择两个 **不同** 的下标 `i` 和 `j` ，其中 `0 <= i, j < n` 。
- 同时，将 `s[i]` 替换为 (`s[i]` **OR** `s[j]`) ，`s[j]` 替换为 (`s[i]` **XOR** `s[j]`) 。

例如，如果 `s = "0110"` ，你可以选择 `i = 0` 和 `j = 2`，然后同时将 `s[0]` 替换为 (`s[0]` **OR** `s[2]` = `0` **OR** `1` = `1`)，并将 `s[2]` 替换为 (`s[0]` **XOR** `s[2]` = `0` **XOR** `1` = `1`)，最终得到 `s = "1110"` 。

如果可以使 `s` 等于 `target` ，返回 `true` ，否则，返回 `false` 。

**示例 1：**

**输入：**s = "1010", target = "0110"
**输出：**true
**解释：**可以执行下述操作：
- 选择 i = 2 和 j = 0 ，得到 s = "_**0**_0_**1**_0".
- 选择 i = 2 和 j = 1 ，得到 s = "0_**11**_0".
可以使 s 等于 target ，返回 true 。

**示例 2：**

**输入：**s = "11", target = "00"
**输出：**false
**解释：**执行任意次操作都无法使 s 等于 target 。

**提示：**

- `n == s.length == target.length`
- `2 <= n <= 10^5`
- `s` 和 `target` 仅由数字 `0` 和 `1` 组成

```cpp
class Solution {
public:
    bool makeStringsEqual(string s, string target) {
        int o = 0, z = 0, to = 0;
        for (int i = 0, n = s.size(); i < n; ++i) {
            if (s[i] == '1') ++to;
            if (s[i] == target[i]) continue;
            if (s[i] == '1') ++o;
            else ++z;
        }
        if (!z && !o || z && o) return true;
        if (z == 0 && o && to > o) return true;
        if (z && o == 0 && to) return true;
        return false;   
    }
};
// 1 --> 0
// 1 ^ 1 = 0 不会改变另外一个1，只要存在别的1即可


// 0 --> 1
// 0 | 1 = 1 不会改变另外一个1
// 0 ^ 1 = 1
```