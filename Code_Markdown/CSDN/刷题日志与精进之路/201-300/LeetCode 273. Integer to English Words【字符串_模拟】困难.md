> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Convert a non-negative integer <code>num</code> to its English words representation.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: num = 123
Output: "One Hundred Twenty Three"
```

<p><strong>Example 2:</strong></p>

```cpp
Input: num = 12345
Output: "Twelve Thousand Three Hundred Forty Five"
```

<p><strong>Example 3:</strong></p>

```cpp
Input: num = 1234567
Output: "One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven"
```
<p><strong>Example 4:</strong></p>

```cpp
Input: num = 1234567891
Output: "One Billion Two Hundred Thirty Four Million Five Hundred Sixty Seven Thousand Eight Hundred Ninety One"
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= num &lt;= 2<sup>31</sup> - 1</code></li>
</ul>

题意：将非负整数 <code>num</code> 转换为其对应的英文表示。

---
### 解法 模拟+字符串
要想解决本题，需要注意到数字被划分为一堆单词的「**模式**」——其实就是把数字按千（3位数）分组。说实在的，本题重点之一就是考察「**对数字英文单词的熟练程度**」。于是，我们可以编写一个帮助函数 `num2str` ，将小于 `1000` 的非负整数转换为对应的字符串表示，然后在其后接上 `Billion, Million, Thousand` ，即可表示出范围不超过 <code>2<sup>32</sup> - 1</code> 的任意非负整数。

 现在要解决 `num2str` 函数的实现，它要表示出 `[0, 999]` 范围内的所有整数。一个特例是零，我们当然可以在 `numberToWords` 方法中一开始就特判返回 `Zero` ，然而 `num` 数字中每三位划分为一组，也会形成零，而 `Zero Billion/Million/Thousand` 并不是一个合法的描述，因此也需要特判 `0`（跳过它进入下一组的表示），从而让 `num2str` 对范围在 `[1, 999]` 的数字进行转换。

考虑如何实现 `num2str` 。假设当前要转换的数字是 `x` ，对其大小分情况讨论：
- <code>x &ge; 100</code> ：首先需要表示为 `??? Hundred` 形式，然后考虑更小的位数；
- <code>x &ge; 20</code> ：此时要表示成 `???ty` 形式，然后考虑更小的数；
- <code>x &ge; 1</code> ：此时 `x` 小于20且大于等于1，可以直接描述为具体的单词。


实现完 `num2str` 后，剩下的只有考虑如何将 `num` 拆分成三位一组进行处理。整个算法中，时空复杂度均取决于最终构建的答案的长度，不过由于每三位一组进行处理、且三位一组转换成的英文描述有明确的长度上界，因此最终答案的长度「**与 `num` 的十进制长度成线性关系**」，即复杂度为 $O(\log n)$ 。
```cpp
//C++ version
vector<string> num2str_lo = {
    "Zero",
    "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten",
    "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"
};
vector<string> num2str_med = {
    "", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"
};
vector<string> num2str_hi = {
    "Billion", "Million", "Thousand", ""
};
int group[4] = {(int)1e9, (int)1e6, (int)1e3, 1};
class Solution {
private:
    string num2str(int x) {
        string ans;
        if (x >= 100) {
            ans += num2str_lo[x / 100] + " Hundred ";
            x %= 100;
        }
        if (x >= 20) {
            ans += num2str_med[x / 10] + " ";
            x %= 10;
        }
        if (x != 0) ans += num2str_lo[x] + " ";
        return ans;
    }
public:
    string numberToWords(int num) {
        if (!num) return num2str_lo[0]; // 特判0
        string ans;
        for (int i = 0; i < 4; ++i) {
            if (num < group[i]) continue; // 该数达不到对应位数(长度不够,对应的几位为0)
            ans += num2str(num / group[i]) + num2str_hi[i] + " ";
            num %= group[i];
        }
        while (ans.back() == ' ') ans.pop_back(); // 有时结尾可能有多余的空格
        return ans;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了77.35% 的用户
//内存消耗：6.1 MB, 在所有 C++ 提交中击败了81.92% 的用户
```

类似的题目： [12. 整数转罗马数字](https://leetcode-cn.com/problems/integer-to-roman/)和[13. 罗马数字转整数](https://leetcode-cn.com/problems/roman-to-integer/)。
