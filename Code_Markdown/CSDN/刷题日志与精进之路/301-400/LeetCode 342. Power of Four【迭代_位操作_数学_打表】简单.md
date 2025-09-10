> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Given an integer <code>n</code>, return <em><code>true</code> if it is a power of four. Otherwise, return <code>false</code></em>.</p>

<p>An integer <code>n</code> is a power of four, if there exists an integer <code>x</code> such that <code>n == 4<sup>x</sup></code>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: n = 16
Output: true 
```
<p><strong>Example 2:</strong></p>

```cpp
Input: n = 5
Output: false 
```
<p><strong>Example 3:</strong></p>

```cpp
Input: n = 1
Output: true 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>-2<sup>31</sup> &lt;= n &lt;= 2<sup>31</sup> - 1</code></li>
</ul>
 
<strong>Follow up:</strong> Could you solve it without loops/recursion?

题意：给定一个整数，写一个函数来判断它是否是 4 的幂次方。如果是，返回 <code>true</code> ；否则，返回 <code>false</code> 。整数 <code>n</code> 是 4 的幂次方需满足：存在整数 <code>x</code> 使得 <code>n == 4<sup>x</sup></code>。<strong>进阶是</strong>在不使用循环或者递归来完成本题。

---
这道题类似于[231. Power of Two](https://leetcode-cn.com/problems/power-of-two/)和[326. Power of Three](https://leetcode-cn.com/problems/power-of-three/)，尤其类似**2的幂**的解法。

### 解法1 迭代+试除
朴素解法是不断将 $n$ 对 $4$ 试除，直到 $n$ 不再与 $4$ 呈倍数关系，然后判断 $n$ 是否等于 $4^0 = 1$ 即可。注意，非正整数不是 $4$ 的幂。算法的时间复杂度为 $O(\log_4 n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfFour(int n) {
        if (n <= 0) return false;
        while (n % 4 == 0) n /= 4;
        return n == 1;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了42.18% 的用户
```

本人过去的sx做法是，将 $n$ 转为浮点数，然后不断除以 $4$ ，最后判断是否等于 $1.0$（不推荐使用） ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfFour(int n) { 
        double tn = n;
        while (tn > 1.0) tn /= 4;
        return tn == 1.0; 
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了26.50% 的用户
```

---
### 解法2 位操作（1不断左移）
不断将 $1$ 左移 $2$ 位和 $n$ 进行比较，直到大于等于 $n$ 为止。比起使用除法甚至是浮点数除法，这种做法能节省一些常数时间。为了避免溢出报错（用 `int` 类型）和死循环（用 `uint32_t` 对负整数和较大的正整数），这里使用 `long long` 。算法时间复杂度为 $O(\log_4 n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfFour(int n) { 
        long long x = 1;
        while (x < n) x <<= 2;
        return x == n; 
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.7 MB, 在所有 C++ 提交中击败了88.92% 的用户
```

---
### 解法3 数学库函数（不推荐使用）
使用数学库函数 `log(), pow()` ，不过这里需要先判断 `n` 是否为正整数，~~而且要处理误差~~（可能是测试样例比[326. Power of Three](https://leetcode-cn.com/problems/power-of-three/)少得多，只有一千多个吧）然后就通过了：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfFour(int n) { 
        if (n <= 0) return false;
        int k = log(n) / log(4);
        return pow(4, k) == n;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.1 MB, 在所有 C++ 提交中击败了5.18% 的用户
```
---
### 解法4 打表 
最容易想到的一个「不使用循环/递归」的方法就是，进行打表预处理。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
int ans[20] = {1,4,16,64,256,1024,4096,16384,65536,262144,1048576,4194304,16777216,67108864,268435456,1073741824};
class Solution {
public:
    bool isPowerOfFour(int n) { 
        if (n <= 0) return false;
        for (int i = 0; i < 16; ++i) if (n == ans[i]) return true;
        return false;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了45.34% 的用户
//内存消耗：5.7 MB, 在所有 C++ 提交中击败了92.24% 的用户
```

---
### 解法5 位操作技巧
利用位操作技巧，我们可以轻松解决这一题。理论上来说，在二进制形式中 $4$ 的幂和 $2$ 的幂一样，都只有一个二进制位 $1$（因为 $4$ 的幂同时也是 $2$ 的幂），而且均分布在二进制形式的偶数位上（从 $0$ 开始从右到左计算），于是可以使用 $n > 0$ 判断为正数，用 `!(n & (n - 1))` 判断二进制形式中只出现了一个 $1$ ，用 $0x5 = 0b0101$ 校验偶数位上的 $1$ 。算法时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfFour(int n) { 
        //0b0101 0101 0101 0101 0101 0101 0101 0101
        return n > 0 && !(n & (n - 1)) && (n & 0x55555555);
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.7 MB, 在所有 C++ 提交中击败了75.04% 的用户
```

> 利用 $a^x$ 在 $a$ 进制形式表示中只有唯一一个 $1$ 的特点，可以解决底数任意的这一类型的问题，即判断任意一个整数是否是任意底数 $a$ 的幂次方：
> ```cpp 
> class Solution { 
> public:
>     bool isPowerOfA(int n, int a) {
>         if (n <= 0) return false;
>         int n1 = 0; 
>         do {
>             int t = n % a;
>             n /= a; //求出n的a进制形式
>             if (t > 1) return false;
>             if (t == 1) ++n1;
>         } while (n);
>         return n1 == 1;
>     } 
>  }; 
>   ```
>   其实和迭代+试除没有本质区别，不过这是从进制的角度来理解。

---
### 解法6 位操作+模运算
利用模运算的性质，我们也能解决这一题。设底数为 $a\ (a>2)$ ，于是 $$a^x = \underbrace{a \times a \times \cdots \times a}_{x个a}$$
因此模 $(a - 1)$ 有：
$$a^x \bmod (a-1) = \big[a \bmod (a-1)\big]^x \bmod (a -1) = 1 $$

不过这只是必要条件，不能因此就认为一个数是 $a$ 的幂，比如 $(2a - 1) \bmod (a -1) =1$ ，但是 $2a-1$ 不是 $a$ 的幂。其他限制条件是 `n > 0 && !(n & (n - 1))` 即 $n$ 为 $2$ 的幂（要排除掉 $2$ 的幂）。对应的代码如下：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfFour(int n) {
        if (n <= 0) return false;
        return n > 0 && !(n & (n - 1)) && n % 3 == 1;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.6 MB, 在所有 C++ 提交中击败了93.24% 的用户
```

