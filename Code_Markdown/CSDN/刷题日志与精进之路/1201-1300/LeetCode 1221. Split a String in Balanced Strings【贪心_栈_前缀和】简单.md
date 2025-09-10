> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

*Balanced strings* are those who have equal quantity of `'L'` and `'R'` characters.

Given a balanced string `s` split it in the maximum amount of balanced strings.

Return the maximum amount of splitted balanced strings.

 

**Example 1:**

```swift
Input: s = "RLRRLLRLRL"
Output: 4
Explanation: s can be split into "RL", "RRLL", "RL", "RL", each substring contains same number of 'L' and 'R'.
```

**Example 2:**

```swift
Input: s = "RLLLLRRRLR"
Output: 3
Explanation: s can be split into "RL", "LLLRRR", "LR", each substring contains same number of 'L' and 'R'.
```

**Example 3:**

```swift
Input: s = "LLLLRRRR"
Output: 1
Explanation: s can be split into "LLLLRRRR".
```

**Example 4:**

```swift
Input: s = "RLRRRLLRLL"
Output: 2
Explanation: s can be split into "RL", "RRRLLRLL", since each substring contains an equal number of 'L' and 'R'
```

 

**Constraints:**
- `1 <= s.length <= 1000`
-  `s[i] = 'L' or 'R'`

题意：在一个**平衡字符串**中，`'L'` 和 `'R'` 字符的数量是相同的。给出一个平衡字符串 `s` ，将它分割成尽可能多的平衡字符串，返回分割的最大数量。

---
### 解法 贪心（归纳证明）/栈/前缀和
平衡字符串是那些有着相同数量 `'L'` 和 `'R'` 的字符串。因此，平衡字符串有相同数量的 `'L'` 和 `'R'` ；字符 `'L'` 和 `'R'` 数量不同的不是平衡字符串。然而常规检查一个字符串是否平衡，需要 $O(n)$ 遍历计数——如果转而使用前缀和数组记录前缀信息，可以 $O(1)$ 时间判定，只是需要 $O(n)$ 空间。更好的方法是，转换为数学判定，即**使用 `-1` 来代表 `'L'` 、使用 `1` 来代表 `'R'`**，此时一个子串是否平衡当且仅当「**整个 `LR` 子串的总和为 `0`** 」

这道题能让人想到**有效括号匹配**这类问题，因此做法也差不多：
```cpp
class Solution {
public:
    int balancedStringSplit(string s) {
        int LR = 0, splittedBalancedStrings = 0, n = s.size();
        for (int i = 0; i < n; ++i) {
            if (s[i] == 'L') ++LR;
            else --LR;
            if (!LR) ++splittedBalancedStrings;
        }
        return splittedBalancedStrings;
    }
};
```
提交后效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了40.72% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了9.30% 的用户
```
