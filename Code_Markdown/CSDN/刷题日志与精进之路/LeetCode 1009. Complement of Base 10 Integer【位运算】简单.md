> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。



Every non-negative integer `N` has a binary representation.  For example, `5` can be represented as `"101"` in binary, `11` as `"1011"` in binary, and so on.  Note that except for `N = 0` , there are no leading zeroes in any binary representation.

The complement of a binary representation is the number in binary you get when changing every `1` to a `0` and `0` to a `1` .  For example, the complement of `"101"` in binary is `"010"` in binary.

For a given number `N` in base-10, return the complement of it's binary representation as a base-10 integer.

**Example 1:**

```swift
Input: 5
Output: 2
Explanation: 5 is "101" in binary, with complement "010" in binary, which is 2 in base-10.
```

**Example 2:**

```swift
Input: 7
Output: 0
Explanation: 7 is "111" in binary, with complement "000" in binary, which is 0 in base-10.
```

**Example 3:**

```swift
Input: 10
Output: 5
Explanation: 10 is "1010" in binary, with complement "0101" in binary, which is 5 in base-10.
```

 

**Note:**
- `0 <= N < 10^9`
 -  This question is the same as 476: `https://leetcode.com/problems/number-complement/`

题意：给出一个十进制非负整数 `N`，返回其二进制表示形式的反码所对应的十进制整数。

---
本题只是要注意 $0$ 的补数为 $1$ ，其他和476题完全一致。

### 解法1 模拟（遍历）
题目中给定的整数在 $32$ 位带符号整数的范围内。只是要注意，这里的补数和我们一般认为的不同，不是直接对原串按位取反，而是在二进制表示范围内取反，即二进制形式**不包含前导零位**。比如 $10 = (1010)_2$ ，范围内按位取反为 $(0101)_2 = 5$ ；$2147483647$ 取反后为 $0$ 。

一个最简单的做法是，先对 $n$ 「从高位到低位」检查，找到最高位 $1$ 的位置 $p$ ，然后再对 $n$ 「从低位到高位」进行遍历，将低位到 $p$ 位的比特位逐位取反。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int bitwiseComplement(int n) {
        if (n == 0) return 1;
        int p = -1;
        for (int i = 31; i >= 0; --i) {
            if ((n >> i) & 1) {
                p = i; // 找到最高位1的位置
                break;
            }
        }
        int ans = 0;
        for (int i = 0; i < p; ++i) 
            if (!((n >> i) & 1)) ans |= (1 << i);
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了27.29% 的用户
```
---
### 解法2 遍历+补数原理
观察可知，$n+ n_{补}$ 的二进制形式正好为 $111\dots1$ ，且位数与 $n$ 二进制下相同，则有 $n+ n_{补} = 2^k - 1$ ，其中 $k$ 取使得 $2^k > n$ 的最小正整数。我们通过遍历找到 $k$ ，然后计算 $2^k - 1 - n$ 得到答案。**注意 $2^k$ 可能超出有符号整型的范围**，需要使用无符号整型 `unsigned int` 。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int bitwiseComplement(int n) {
        if (n == 0) return 1;
        for (int k = 0; k < 32; ++k) {
            unsigned int j = 1 << k;
            if (j > n) return j - 1 - n;
        }
        return 0;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了47.32% 的用户
```
由于 $2^k - 1$ 是一个和 $n$ 二进制形式（本题限定）等长的全 $1$ 二进制数，我们也可以用另一个数算出 $2^k - 1$ ，然后将其与 $n$ 相异或，**即等同于对 $n$ 按位取反**。计算这一全 $1$ 二进制的做法有许多种。最简单最容易理解的做法如下：
```cpp
//C++ version
class Solution {
public:
    int bitwiseComplement(int n) {
        if (n == 0) return 1;
        int temp = n, allOnes = 0;
        while (temp > 0) {
            temp >>= 1;
            allOnes = (allOnes << 1) | 1; 
        }
        return n ^ allOnes;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了14.68% 的用户
```
当然，上面两种做法没有本质区别，都是利用**补数原理**，不断遍历/右移，以找到第一个满足 $2^k > n$ 的 $k$ / 计算和 $n$ 等长的全 $1$ 二进制数。

---
### 解法3 补数原理（优化）
假设 $n$ 二进制表示中最高位 $1$ 的位置为 $p$ ，我们完全可以不断使用 `lowbit` 操作，去掉 $n$ 二进制表示的 $1$ ，直到得到最高位 $1$（其余位为 $0$ ）代表的值 $x$ ，$(x << 1) - 1$ 即可得到和 $n$ 等长的二进制数。由此观之，解法一还绕了一点弯路。
```cpp
//C++ version
class Solution {
public:
    int bitwiseComplement(int n) {
        if (n == 0) return 1;
        int x = 0;
        for (int i = n; i; i -= i & -i) x = i;
        return n ^ (((unsigned)x << 1) - 1);
    }
}; 
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了78.24% 的用户
```
或者参考JDK源码中HashMap获得哈希容量、并使其为大于 $n$ 的最小2幂次的方法：
```cpp
//C++ version
class Solution {
public:
    int bitwiseComplement(int n) {
        if (n == 0) return 1;
        int num = n;
        num |= num >> 1;
        num |= num >> 2;
        num |= num >> 4;
        num |= num >> 8;
        num |= num >> 16;
        return n ^ num;
    }
}; 
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了48.71% 的用户
```
