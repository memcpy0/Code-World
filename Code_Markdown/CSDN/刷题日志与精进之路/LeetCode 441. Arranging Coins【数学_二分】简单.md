> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


You have a total of `n` coins that you want to form in a staircase shape, where every `k-th` row must have exactly `k` coins.

Given `n`, find the total number of **full staircase rows** that can be formed.

`n` is a non-negative integer and fits within the range of a 32-bit signed integer.

**Example 1:**

```clike
n = 5

The coins can form the following rows:
¤
¤ ¤
¤ ¤

Because the 3rd row is incomplete, we return 2.
```

**Example 2:**

```clike
n = 8

The coins can form the following rows:
¤
¤ ¤
¤ ¤ ¤
¤ ¤

Because the 4th row is incomplete, we return 3. 
```
**Constraints:** <code>1 <= n <= 2<sup>31</sup> - 1</code>


题意：总共有 `n` 枚硬币，需要将它们摆成一个阶梯形状，第 `k` 行必须正好有 `k` 枚硬币。找出可形成完整阶梯行的总行数。

---
### 解法1 迭代减
从 $n$ 枚硬币开始，不断减去 $1$ 枚、$2$ 枚……直到第 $i$ 枚时 $n < i$ 为止，于是完整阶梯行的行数为 $i - 1$ 。算法的时间复杂度为 $O(\sqrt{n})$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int arrangeCoins(int n) { 
        int level = 1;
        while (n >= level) n -= (level++); 
        return level - 1;
    }
};
//执行用时：12 ms, 在所有 C++ 提交中击败了30.42% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了72.76% 的用户
```

---
### 解法2 二分求上界
对于 $n$ 个硬币而言，最多凑成的完整阶梯行行数不会超过 $n$ 行，当且仅当 $n = 1$ 时取等。注意，硬币阶梯需要的硬币数量，可以运用等差数列求和公式求出。

假设最多能够凑成的行数为 $x$ ，那么以 $x$ 为分割点的数轴上具有**单调性**和**二段性**——完整行数越大，需要的硬币越多；完整行数越小，需要的硬币越少。使用 $n$ 个硬币如果能够凑成 $x$ 行，则必然也能凑成少于等于 $x$ 行的硬币阶梯，必然不能凑成大于 $x$ 行的硬币阶梯。

于是不断二分答案区间 $[1, n]$ ，以找到最多能够凑成的行数 $x$（即最后一个满足题意的 $mid$ ）——每次求出当前行数下的硬币数量，和 $n$ 比较来缩小答案范围。时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
//以前的代码,找到第一个超出题意的mid,lo=hi=mid,返回lo-1
class Solution {
public:
    int arrangeCoins(int n) {
        if (n <= 1) return n; //特判,注意:n=1不存在第一个满足mid*(mid+1)>2n的mid
        uint64_t l = 0, r = n, mid, n2 = 2ull * n; //找到最后一个满足mid*(mid+1)<=2n的mid
        while (l < r) { //找到第一个满足mid*(mid+1)>2n的mid
            mid = l + (r - l) / 2;  
            if ((mid + 1) * mid > n2) r = mid;
            else l = mid + 1;
        }
        return l - 1;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了81.62% 的用户
//内存消耗：6.3 MB, 在所有 C++ 提交中击败了5.78% 的用户
```
```cpp
//C++ version
class Solution {
public:
    int arrangeCoins(int n) { 
        using ll = long long;
        ll l = 1, r = n;
        while (l < r) {
            ll mid = l + r + 1 >> 1;
            if (mid * (mid + 1) / 2 <= n) l = mid;
            else r = mid - 1;
        }
        return static_cast<int>(l);
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了74.94% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了70.21% 的用户
```

---
### 解法3 数学 
最多有 $x$ 层，每层硬币数从 $1$ 逐个增加到 $x$ ，运用等差数列求和公式有 $x  (x + 1) \le 2 n$ 。接着使用一元二次方程的求根公式，求出满足此式的前提下 $x$ 的最大值为多少：$$x \le \lfloor \frac{-b + \sqrt{\Delta}}{2a}\rfloor =  \lfloor  \frac{-b + \sqrt{b^2-4ac}}{2a} \rfloor = \lfloor \frac{-1+\sqrt{1+8n}}{2} \rfloor$$

```cpp
//C++ version
class Solution {
public:
    int arrangeCoins(int n) {
        return (-1 + sqrt(1 + 8ll * n)) / 2; 
    }
}; 
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了23.51% 的用户
```
 
