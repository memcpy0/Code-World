> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Given an integer <code>n</code>, return <em><code>true</code> if it is a power of two. Otherwise, return <code>false</code></em>.</p>

<p>An integer <code>n</code> is a power of two, if there exists an integer <code>x</code> such that <code>n == 2<sup>x</sup></code>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: n = 1
Output: true
Explanation: 2^0 = 1
```
<p><strong>Example 2:</strong></p>

```clike
Input: n = 16
Output: true
Explanation: 2^4 = 16
```

<p><strong>Example 3:</strong></p>

```clike
Input: n = 3
Output: false
```
 
<p><strong>Example 4:</strong></p>

```clike
Input: n = 4
Output: true
```
  
<p><strong>Example 5:</strong></p>

```clike
Input: n = 5
Output: false
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>-2<sup>31</sup> &lt;= n &lt;= 2<sup>31</sup> - 1</code></li>
</ul>
 
<strong>Follow up:</strong> Could you solve it without loops/recursion?

题意：给你一个整数 `n` ，请你判断该整数是否是 `2` 的幂次方。如果是，返回 `true` ；否则，返回 `false` 。<strong>进阶是</strong>在不使用循环或者递归来完成本题。


---
这道题类似于[326. Power of Three](https://leetcode-cn.com/problems/power-of-three/)和[342. Power of Four](https://leetcode-cn.com/problems/power-of-four/)，尤其类似**4的幂**的解法。
### 解法1 迭代+试除
朴素解法是不断将 $n$ 对 $2$ 试除，直到 $n$ 不再与 $2$ 呈倍数关系，然后判断 $n$ 是否等于 $2^0 = 1$ 即可。注意，非正整数不是 $2$ 的幂。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfTwo(int n) {
        if (n <= 0) return false;
        while (n % 2 == 0) n /= 2;
        return n == 1;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.6 MB, 在所有 C++ 提交中击败了98.27% 的用户
```
我在[4的幂题解](https://memcpy0.blog.csdn.net/article/details/117408777)中的sx做法，这里也可以使用。将 $n$ 转为浮点数，然后不断除以 $2$ ，最后判断是否等于 $1.0$（不推荐使用） ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfTwo(int n) {
        double tn = n;
        while (tn > 1.0) tn /= 2;
        return tn == 1.0;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了47.41% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了70.78% 的用户
```
---
### 解法2 位操作（1不断左移）
不断将 $1$ 左移 $1$ 位和 $n$ 进行比较，直到大于等于 $n$ 为止。比起使用除法甚至是浮点数除法，这种做法能节省一些常数时间。为了避免溢出报错，这里使用<u>无符号整数类型 `uint32_t`</u>（注意排除非正整数尤其是负整数）或者像[4的幂题解](https://memcpy0.blog.csdn.net/article/details/117408777)一样使用 `long long` 。算法时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfTwo(int n) {
        if (n <= 0) return false;
        uint32_t x = 1;
        while (x < n) x <<= 1;
        return x == n;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了47.41% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了66.92% 的用户
```

---
### 解法3 数学库函数（不推荐使用）
使用数学库函数 `log(), pow()` ，不过这里需要先判断 `n` 是否为正整数，~~而且要处理误差~~（可能是测试样例比[326. Power of Three](https://leetcode-cn.com/problems/power-of-three/)少得多，只有一千多个吧）然后就通过了：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfTwo(int n) { 
        if (n <= 0) return false;
        int k = log(n) / log(2);
        return pow(2, k) == n;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了47.41% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了38.62% 的用户
```
---
### 解法4 打表
最容易想到的一个「不使用循环/递归」的方法就是，进行打表预处理。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
int ans[35] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824};
class Solution { 
public:
    bool isPowerOfTwo(int n) { 
        if (n <= 0) return false;
        for (int i = 0; i < 32; ++i) if (n == ans[i]) return true;
        return false;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了26.88% 的用户
```
---
### 解法5 数学（倍数/约数）
$n$ 的数据类型为 `int` ，由打表可知，`int` 范围内最大的 $2$ 次幂是 $1073741824$ 。如果 $n$ 为 $2$ 的幂，则必然满足 $n * 2^k = 1073741824$ ，即 $n$ 与 $1073741824$ 存在**倍数关系**。因此只需要判定 $n$ 是正整数、且是 $1073741824$ 的约数即可。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ 。

需要注意的是，这种做法不是快速判断 $x$ 的幂次方的通用做法，**当且仅当 $x$ 为质数时可用**。所以无法推广到[342. Power of Four](https://leetcode-cn.com/problems/power-of-four/)。一个简单的示例是，$64$ 是 $4$ 的幂次方，也必然是某个最大的 $4$ 的幂次方数 $z$ 的约数，然而 $32$ 不是 $4$ 的幂次方，但一定是 $64$ 的约数、从而是 $z$ 的约数。
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfTwo(int n) {
        return n > 0 && 1073741824 % n == 0;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了58.76% 的用户
```

---
### 解法6 位操作技巧
利用位操作技巧，我们可以轻松解决这一题。理论上来说，在二进制形式中 $2$ 的幂和 $4$ 的幂一样，都只有一个二进制位 $1$ ，于是可以使用 `n > 0` 判断为正数，用 `!(n & (n - 1))` 判断二进制形式中只出现了一个 $1$ 。算法时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfTwo(int n) {  
        return n > 0 && !(n & (n - 1));
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了26.88% 的用户
```

