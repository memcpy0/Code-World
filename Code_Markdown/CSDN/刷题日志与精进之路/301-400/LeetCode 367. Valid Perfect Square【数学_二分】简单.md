> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a **positive** integer `num`, write a function which returns True if `num` is a perfect square else False.

**Follow up:** **Do not** use any built-in library function such as `sqrt`.

 

**Example 1:**

```swift
Input: num = 16
Output: true
```

**Example 2:**

```swift
Input: num = 14
Output: false
```

**Constraints:** `1 <= num <= 2^31 - 1`

题意：给定一个正整数 `num`，编写一个函数，如果 `num` 是一个完全平方数，则返回 `True`，否则返回 `False`。

---
本题属于[69. Sqrt(x)](https://leetcode-cn.com/problems/sqrtx/)（[题解文章](https://memcpy0.blog.csdn.net/article/details/121135932)在此）的变形，在做这题之前，可以先做69题。
### 解法1 内置函数
```cpp
//C++ version
class Solution {
public:
    bool isPerfectSquare(int num) {
        int t = static_cast<int>(sqrt(num));
        return t * t == num;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了22.12% 的用户
```
---

### 解法2 数学
显然，对于一个完全平方数而言，可以写成「**从 $1$ 开始的连续奇数序列的和**」：
$$num = n^2 = 1 + 3 + 5 +\dots + (2n - 1)$$

因此另一种做法是，对 $num$ 不断使用奇数试减，如果最终能够减到 $0$ ，说明 $num$ 可以展开成 $1 + 3 +5+\dots + (2n -1)$ 的形式，即 $num$ 为完全平方数。这一做法的时间复杂度为 $O(\sqrt{n})$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isPerfectSquare(int num) {
        int x = 1;
        while (num >= x) {
            num -= x;
            x += 2;
        }
        return !num;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了51.30% 的用户
```

---
### 解法3 二分（多种写法）
对于平方运算来说，随着数字的增大，结果也越大，即具有**单调性**。又假设答案为 $ans$ ，那么以 $ans$ 为分割点的数轴上具有**二段性**：
- 小于 $ans$ 的一段中，$x$ 必然不满足 $x \times x\ge num$ ；
- 大于等于 $ans$ 的一段中，$x$ 必然满足 $x \times x\ge num$ 。

因此可以使用「二分」来找到分割点 $ans$ 。当然，二分有多种写法，不过它们的时间复杂度都为 $O(\log n)$ ，空间复杂度都为 $O(1)$ 。
```cpp
//C++ version
class Solution {
public:
    bool isPerfectSquare(int num) {
        int l = 1, r = num;
        while (l <= r) { // 找到自乘后等于num的数
            int mid = l + (r - l) / 2;
            long long t = (long long)mid * mid;
            if (t == num) return true;
            else if (t < num) l = mid + 1;
            else r = mid - 1;
        }
        return false;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了59.71% 的用户
//C++ version
class Solution {
public:
    bool isPerfectSquare(int num) {
        int l = 1, r = num;
        while (l < r) { // 找到第一个自乘后大于等于num的数
            int mid = l + (r - l) / 2;
            long long t = (long long)mid * mid;
            if (t >= num) r = mid;
            else l = mid + 1;
        }
        return (long long)l * l == num;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了36.04% 的用户
```
---
### 解法4 牛顿迭代法
实际上，计算 $\sqrt{n}$ 的正解是[**牛顿迭代法**](https://en.wikipedia.org/wiki/Integer_square_root#Using_only_integer_division)：
$$x_{k+1} = \dfrac{1}{2} \Big( x_k + \dfrac{n}{x_k} \Big), \quad k\ge 0,\ x_0 > 0.$$

不过本题中要计算的是 $\lfloor \sqrt{n} \rfloor$ ，即 $\lfloor \sqrt{n} \rfloor \times \lfloor \sqrt{n} \rfloor$ 是否等于 $n$ 。针对  $\lfloor \sqrt{n}\rfloor$ 的特殊情形，可以使用[**欧几里得除法**](https://en.wikipedia.org/wiki/Euclidean_division)（即数论中的[**带余除法**](https://memcpy0.blog.csdn.net/article/details/121180524)）的商来代替两个除法操作，这样就不用使用浮点数了：
$$x_{k+1} =\Big \lfloor  \dfrac{1}{2} \Big( x_k + \Big \lfloor   \dfrac{n}{x_k}\Big \rfloor   \Big) \Big \rfloor  , \quad k\ge 0,\ x_0 > 0.$$

变换一下：
$$\Big \lfloor  \dfrac{1}{2} \Big( x_k + \Big \lfloor   \dfrac{n}{x_k}\Big \rfloor   \Big) \Big \rfloor = \Big \lfloor  \dfrac{1}{2} \Big( x_k +    \dfrac{n}{x_k}   \Big) \Big \rfloor $$

```java
//Java version
class Solution {
    public boolean isPerfectSquare(int num) {
        if (num <= 1) return true;
        int xi = num / 2;
        while (xi > num / xi) xi = (xi + num / xi) / 2;
        return xi * xi == num;
    }
}
//执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
//内存消耗：35.2 MB, 在所有 Java 提交中击败了58.83% 的用户
```
