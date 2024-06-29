> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。



<p>The <strong>complement</strong> of an integer is the integer you get when you flip all the <code>0</code>'s to <code>1</code>'s and all the <code>1</code>'s to <code>0</code>'s in its binary representation. You could assume <b>no leading zero</b> bit in the integer’s binary representation. </p>
<p>For example, The integer <code>5</code> is <code>"101"</code> in binary and its <strong>complement</strong> is <code>"010"</code> which is the integer <code>2</code>. Given an integer <code>num</code>, return <em>its complement</em>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: num = 5
Output: 2
Explanation: The binary representation of 5 is 101 (no leading zero bits), and its complement is 010. So you need to output 2.
```
<p><strong>Example 2:</strong></p>

```cpp
Input: num = 1
Output: 0
Explanation: The binary representation of 1 is 1 (no leading zero bits), and its complement is 0. So you need to output 0.
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= num &lt; 2<sup>31</sup></code></li>
</ul>

 
<p><strong>Note:</strong> This question is the same as 1009: <a href="https://leetcode.com/problems/complement-of-base-10-integer/">https://leetcode.com/problems/complement-of-base-10-integer/</a></p> 

题意：给定一个**正整数**，输出它的补数。补数是对该数的二进制表示取反。

---
### 解法1 模拟+遍历（绕弯路了）
题目中给定的整数在 $32$ 位带符号整数的范围内。只是要注意，这里的补数和我们一般认为的不同，不是直接对原串按位取反，而是在二进制表示范围内取反，即二进制形式**不包含前导零位**。比如 $10 = (1010)_2$ ，范围内按位取反为 $(0101)_2 = 5$ ；$2147483647$ 取反后为 $0$ 。

一个最简单的做法是，先对 $num$ 「从高位到低位」检查，找到最高位 $1$ 的位置 $p$ ，然后再对 $num$ 「从低位到高位」进行遍历，将低位到 $p$ 位的比特位逐位取反。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int findComplement(int num) {
        int p = -1;
        for (int i = 31; i >= 0; --i) {
            if ((num >> i) & 1) {
                p = i;
                break;
            }
        }
        int ans = 0;
        for (int i = 0; i < p; ++i) 
            if (!((num >> i) & 1)) ans |= (1 << i);
        return ans; 
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了38.18% 的用户
```

---
### 解法2 遍历+补数原理
观察可知，$num + num_{补}$ 的二进制形式正好为 $111\dots1$ ，且位数与 $num$ 二进制下相同，则有 $num + num_{补} = 2^k - 1$ ，其中 $k$ 取使得 $2^k > num$ 的最小正整数。我们通过遍历找到 $k$ ，然后计算 $2^k - 1 - num$ 得到答案。**注意 $2^k$ 可能超出有符号整型的范围**，需要使用无符号整型 `unsigned int` 。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int findComplement(int num) {
        for (int k = 0; k < 32; ++k) {
            unsigned int n = 1 << k;
            if (n > num) return n - 1 - num;
        }
        return 0;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了47.32% 的用户
```
由于 $2^k - 1$ 是一个和 $num$ 二进制形式（本题限定）等长的全 $1$ 二进制数，我们也可以用另一个数算出 $2^k - 1$ ，然后将其与 $num$ 相异或，**即等同于对 $num$ 按位取反**。计算这一全 $1$ 二进制的做法有许多种。最简单最容易理解的做法如下：
```cpp
//C++ version
class Solution {
public:
    int findComplement(int num) {
        int temp = num, allOnes = 0;
        while (temp > 0) {
            temp >>= 1;
            allOnes = (allOnes << 1) | 1; 
        }
        return num ^ allOnes;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了32.47% 的用户
```
当然，上面两种做法没有本质区别，都是利用**补数原理**，不断遍历/右移，以找到第一个满足 $2^k > num$ 的 $k$ / 计算和 $num$ 等长的全 $1$ 二进制数。

---
### 解法3 补数原理（优化）
假设 $num$ 二进制表示中最高位 $1$ 的位置为 $p$ ，我们完全可以不断使用 `lowbit` 操作，去掉 $num$ 二进制表示的 $1$ ，直到得到最高位 $1$（其余位为 $0$ ）代表的值 $x$ ，$(x << 1) - 1$ 即可得到和 $num$ 等长的二进制数。由此观之，解法一还绕了一点弯路。
```cpp
//C++ version
class Solution {
public:
    int findComplement(int num) {
        int x = 0;
        for (int i = num; i; i -= i & -i) x = i;
        return num ^ (((unsigned)x << 1) - 1);
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.7 MB, 在所有 C++ 提交中击败了91.76% 的用户
```
或者参考JDK源码中HashMap获得哈希容量、并使其为大于 $num$ 的最小2幂次的方法：
```cpp
//C++ version
class Solution {
public:
    int findComplement(int num) {
        int n = num;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16; 
        return n ^ num;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了33.98% 的用户
```
