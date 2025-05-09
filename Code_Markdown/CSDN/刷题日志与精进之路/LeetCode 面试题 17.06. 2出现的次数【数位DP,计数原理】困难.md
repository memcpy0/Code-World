> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。
 
编写一个方法，计算从 `0` 到 `n` (含 `n`) 中数字 `2` 出现的次数。

**示例:**
```java
输入: 25
输出: 9
解释: (2, 12, 20, 21, 22, 23, 24, 25)(注意 22 应该算作两次)
```
提示：
-   `n <= 10^9`

---
### 解法1 数位DP


---
### 解法2 计数原理
这一题其实是简化版，是一道「数位DP」模板题[P1980 [NOIP2013 普及组] 计数问题](https://www.luogu.com.cn/problem/P1980)的简化，不过那道NOIP题效率最高的解法也还是计数模拟，见[这篇文章](https://memcpy0.blog.csdn.net/article/details/119744908)。做过那道题后再做这道题，会轻松很多。而且还有道完全相似的题目：[233. Number of Digit One](https://leetcode.cn/problems/number-of-digit-one)。

回到本题，是时候重温计数原理了：
- 加法原理：简单来说就是**分类**。做一件事情，完成它有**不重不漏**的 $n$ 大类解法，第一大类有 $m_1$ 种方法，第二大类有 $m_2$ 种方法，……，第 $n$ 大类有 $m_n$ 种方法，那么完成这件事共有 $m_1+m_2+……+m_n$ 种方法。
- 乘法原理：简单来说就是**分步**。做第一步有 $m_1$ 种不同的方法，做第二步有 $m_2$ 种不同的方法，……，做第 $n$ 步有 $m_n$ 种不同的方法。那么完成这件事共有  $m_1×m_2×m_3×…×m_n$ 种不同的方法。

本题计算 $[0, n]$ 的所有整数中 $2$ 出现的个数，主要运用**加法分类原理**，即分别计算所有整数中出现在个位、十位、百位……的 $2$ 的个数，然后加总得到结果。现在的问题是，**如何统计 $2$ 在第 $i$ 位出现的次数**？

假设一个五位的数 $n = abcde$ ，我们需要**统计第 $3$ 位中 $2$ 出现的次数**，即计算满足 $\_\ \_\ 2\ \_\ \_$ 形式且 $0 \le \_\ \_\ 2\ \_\ \_ \le abcde$ 的整数 $x$ 有多少个，我们将这一条件简称为「限定」。下面主要对五位数 $n$ 的中间数 **$c$ 的大小**（和 **$x$ 前一部分的大小**）进行分类讨论：
- 当 $n$ 中间位 $c \gt 2$ 时：
	- $x$ 前两位数 $< ab$ 时，即范围为 $[0, ab)$ ，必然满足「限定」，因此 **$x$ 的后一部分可以任意取值**，范围为 $[0, 99]$ 。根据乘法原理，此时 $x$ 的第 $3$ 位中 $2$ 出现的次数加上 $ab * 100$ ；
	- $x$ 前两位数 $= ab$ 时，由于 $c > 2$ ，也必然满足「限定」，因此后一部分可以任意取值，范围为 $[0, 99]$ ，此时 $x$ 的第 $3$ 位中 $2$ 出现的次数加上 $100$ ；
	- $c$ 前一部分 $> ab$ 时，必然不满足「限定」。
- 当 $n$ 中间位 $c = 2$ 时：
	- $x$ 前两位数 $< ab$ 时，即范围为 $[0, ab)$ ，必然满足「限定」，因此 **$x$ 的后一部分可以任意取值**，范围为 $[0, 99]$ 。根据乘法原理，此时 $x$ 的第 $3$ 位中 $2$ 出现的次数加上 $ab * 100$ ；
	- $x$ 前两位数 $= ab$ 时，由于 $c = 2$ ，只有后一部分取值在范围 $[0, de]$ 时才满足「限定」，此时 $x$ 的第 $3$ 位中 $2$ 出现的次数加上 $de + 1$ ；
	- $x$ 前两位数 $> ab$ 时，必然不满足「限定」。
- 当 $n$ 中间位 $c < 2$ 时:
	- $x$ 前两位数 $< ab$ 时，即范围为 $[0, ab)$ ，必然满足「限定」，因此 **$x$ 的后一部分可以任意取值**，范围为 $[0, 99]$ 。根据乘法原理，此时 $x$ 的第 $3$ 位中 $2$ 出现的次数为 $ab * 100$ ；
	- $x$ 前两位数 $= ab$ 时，由于 $c<2$ ，无论后一部分如何取值，都不能满足「限定」，使中间位为 $2$ ；
	- $x$ 前两位数 $> ab$ 时，必然不满足「限定」。

总结来看，主要是对 **$x$ 前两位数的大小**进行分类讨论，简要归纳如下：
- $x$ 前两位数 $< ab$ 时，无论如何都满足「限定」，因此后一部分可以任意取值，此时 $x$ 的第 $3$ 位中 $2$ 出现的次数加上 $ab * 100$ ；
- **$x$ 前两位数 $= ab$ 时，是否满足「限定」取决于 $c$ 的大小**：
	- $c > 2$ 时，必然满足「限定」，因此后一部分可以任意取值，此时 $x$的第 $3$ 位中 $2$ 出现的次数加上 $100$ ；
	- $c = 2$ 时，只有后一部分取值在范围 $[0, de]$ 时才满足「限定」，此时 $x$ 的第 $3$ 位中 $2$ 出现的次数加上 $de + 1$ ；
	- $c <2$ 时，无论后一部分如何取值，都不能满足「限定」；
- $x$ 前两位数 $> ab$ 时，必然不满足「限定」。
- 因此，在 $[0, n]$ 的范围中，满足第三位为 $2$ 的整数 $x$ 有 $100ab + 100$ 个（$c>2$ 时）、或 $100ab + de + 1$ 个（$c=2$ 时）、或 $100ab$ 个（$c<2$ 时）。

下面将给出的数 $n$ 看做 $abc$ 三部分组成的数，数 $b$ 只有一位，$a,c$ 分别代表 $b$ 前后的数字。最后实现的代码如下：
```cpp
class Solution {
public:
    int numberOf2sInRange(int n) {
        unsigned r = 1;
        int ans = 0;
        while (r <= n) {
            int a = n / r / 10, b = n / r % 10, c = n % r;
            if (b > 2) ans += a * r + 1 * r;
            else if (b == 2) ans += a * r + c + 1;
            else if (b < 2) ans += a * r;
            r *= 10;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度为 $O(\lg n)$ 
- 空间复杂度为 $O(1)$ 