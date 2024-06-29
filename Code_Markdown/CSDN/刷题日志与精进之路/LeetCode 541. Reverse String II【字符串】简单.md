> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


Given a string and an integer `k` , you need to reverse the first `k`  characters for every `2k` characters counting from the start of the string. If there are less than `k` characters left, reverse all of them. If there are less than `2k` but greater than or equal to `k` characters, then reverse the first `k` characters and left the other as original.

**Example:**

```swift
Input: s = "abcdefg", k = 2
Output: "bacdfeg"
```

**Restrictions:**
- The string consists of lower English letters only.
- Length of the given string and `k` will in the range `[1, 10000]` .


题意：给定一个字符串 `s` 和一个整数 `k` ，你需要对从字符串开头算起的每隔 `2k` 个字符的前 `k` 个字符进行反转。如果剩下的字符少于 `k` 个则全部反转；如果剩余的字符小于 `2k` 但是大于等于 `k` 个，则反转前 `k` 个字符，其余字符不变。

---
### 解法 字符串
看清楚题目后就简单了：
```cpp
class Solution {
public:
    string reverseStr(string s, int k) {
        for (int i = 0, n = s.size(); i < n; i += 2 * k) {
            auto send = (i + k > n) ? s.end() : (s.begin() + i + k);
            reverse(s.begin() + i, send);
        }   
        return s;
    }
};
```
时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ 。运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了85.41% 的用户
内存消耗：7.1 MB, 在所有 C++ 提交中击败了60.35% 的用户
```



