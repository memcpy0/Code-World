> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091342336.png)
罗马数字包含以下七种字符： `I`， `V`， `X`， `L`，`C`，`D` 和 `M`。
```
字符          数值
I             1
V             5
X             10
L             50
C             100
D             500
M             1000
```
例如， 罗马数字 2 写做 `II` ，即为两个并列的 1。12 写做 `XII` ，即为 `X` + `II` 。 27 写做  `XXVII`, 即为 `XX` + `V` + `II` 。

通常情况下，**罗马数字中小的数字在大的数字的右边**。但也存在特例，例如 4 不写做 `IIII`，而是 `IV`。数字 1 在数字 5 的左边，所表示的数等于大数 5 减小数 1 得到的数值 4 。同样地，数字 9 表示为 `IX`。这个特殊的规则只适用于以下六种情况：
- `I` 可以放在 `V` (5) 和 `X` (10) 的左边，来表示 4 和 9。
- `X` 可以放在 `L` (50) 和 `C` (100) 的左边，来表示 40 和 90。 
- `C` 可以放在 `D` (500) 和 `M` (1000) 的左边，来表示 400 和 900。

给你一个整数，将其转为罗马数字。

**示例 1:**
```java
输入: num = 3
输出: "III"
```
**示例 2:**
```java
输入: num = 4
输出: "IV"
```
**示例 3:**
```java
输入: num = 9
输出: "IX"
```
**示例 4:**
```java
输入: num = 58
输出: "LVIII"
解释: L = 50, V = 5, III = 3.
```
**示例 5:**
```java
输入: num = 1994
输出: "MCMXCIV"
解释: M = 1000, CM = 900, XC = 90, IV = 4.
```
**提示：**
- `1 <= num <= 3999`

---
### 解法 贪心+哈希表+数学
这一题其实不是很难，关键在于要理解罗马数字的组成规则。罗马数字由 $7$ 个不同的单字母组成，每个字母对应一个数值，**减法法则**又给出了额外的 $6$ 个**复合符号**。因此总共有 $13$ 个符号，每个符号由 $1 \sim 2$ 个字母组成。想到这一点，问题起码解决了20%。
$$\begin{cases}
I \to 1\\
IV \to 4\\
V \to 5\\
IX \to 9\\ 
X \to 10 \\  
XL  \to 40\\
L \to 50\\
XC  \to 90\\
C \to 100\\
CD \to 400 \\
D \to 500\\
CM \to 900\\
M \to 1000
\end{cases}$$
**罗马数字的唯一表示法**——不过更关键的是，如何表示罗马数字？比如说 $80$ 的罗马数字表示，随手就能列出来下面几种：
```cpp
L + X + X + X = 50 + 10 + 10 + 10 = 80
L + X + X + IX + I = 50 + 10 + 10 + 9 + 1 = 80
XL + XL = 40 + 40 = 80
```
我们注意到题目中有这么一句话：罗马数字中小的数字在大的数字的右边 *Roman numerals are usually written largest to smallest from left to right* 。罗马数字总是由大到小书写。因此，用于确定罗马数字的规则是：**对于罗马数字从左到右的每一位，选择尽可能大的符号值**。于是 $80$ 的罗马数字表示是 `LXXX` 。明白了这一规则后，问题就简单了。

下面模拟这一过程。首先建立一个数值为键、符号对为值的列表 `rec` ，按照数值从大到小排序，然后遍历每一键值对 `[val, symbols]` ，如果当前的 `num >= val` ，就从 `num` 中不断减去 `val` ，同时在字符串末尾添加 `symbols` ，直到 `num < val` 。接着遍历下一个键值对。直到 `num` 为 `0` 就跳出循环。
```cpp
class Solution {
public:
    string intToRoman(int num) { 
        int w[13] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
        string roman[13] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
        string ans;
        for (int i = 0; i < 13; ++i) {
            while (num >= w[i]) {
                ans += roman[i];
                num -= w[i];
            }
            if (num == 0) break;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了89.40% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了49.82% 的用户
```

复杂度分析

时间复杂度：O(1)O(1)O(1)。由于 valueSymbols\textit{valueSymbols}valueSymbols 长度是固定的，且这 131313 字符中的每个字符的出现次数均不会超过 333，因此循环次数有一个确定的上限。对于本题给出的数据范围，循环次数不会超过 151515 次。

空间复杂度：O(1)O(1)O(1)。

---
方法二：硬编码数字
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307022107902.png)
回顾前言中列出的这 131313 个符号，可以发现：

千位数字只能由 M\texttt{M}M 表示；
百位数字只能由 C\texttt{C}C，CD\texttt{CD}CD，D\texttt{D}D 和 CM\texttt{CM}CM 表示；
十位数字只能由 X\texttt{X}X，XL\texttt{XL}XL，L\texttt{L}L 和 XC\texttt{XC}XC 表示；
个位数字只能由 I\texttt{I}I，IV\texttt{IV}IV，V\texttt{V}V 和 IX\texttt{IX}IX 表示。
这恰好把这 131313 个符号分为四组，且组与组之间没有公共的符号。因此，整数 num\textit{num}num 的十进制表示中的每一个数字都是可以单独处理的。

进一步地，我们可以计算出每个数字在每个位上的表示形式，整理成一张硬编码表。如下图所示，其中 000 对应的是空字符串。

利用模运算和除法运算，我们可以得到 num\textit{num}num 每个位上的数字：

thousands_digit = num / 1000
hundreds_digit = (num % 1000) / 100
tens_digit = (num % 100) / 10
ones_digit = num % 10
最后，根据 num\textit{num}num 每个位上的数字，在硬编码表中查找对应的罗马字符，并将结果拼接在一起，即为 num\textit{num}num 对应的罗马数字。

```cpp
const string thousands[] = {"", "M", "MM", "MMM"};
const string hundreds[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
const string tens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
const string ones[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
class Solution {
public:
    string intToRoman(int num) {
        return thousands[num / 1000] + hundreds[num % 1000 / 100] + tens[num % 100 / 10] + ones[num % 10];
    }
};
```