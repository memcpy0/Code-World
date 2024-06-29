> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a **positive** integer `n`, return _the smallest positive integer that is a multiple of **both**_ `2` _and_ `n`.

**Example 1:**
```java
Input: n = 5
Output: 10
Explanation: The smallest multiple of both 5 and 2 is 10.
```
**Example 2:**
```java
Input: n = 6
Output: 6
Explanation: The smallest multiple of both 6 and 2 is 6. Note that a number is a multiple of itself.
```
**Constraints:**
-   `1 <= n <= 150`

---
### 解法 数学
要求 $2$ 与 $n$ 的最小公倍数，显然，当 $n$ 为奇数时最小公倍数为 $2n$ ；当 $n$ 为偶数时最小公倍数为 $n$ 。
```cpp
// class Solution {
// public:
//     int smallestEvenMultiple(int n) {
//         return 2 / __gcd(2, n) * n;
//     }
// };
class Solution {
public:
    int smallestEvenMultiple(int n) {
        return (n & 1) ? 2 * n : n;
    }
};
```