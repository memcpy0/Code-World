> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Given an integer <code>n</code>, return <em><code>true</code> if it is a power of three. Otherwise, return <code>false</code></em>.</p>

<p>An integer <code>n</code> is a power of three, if there exists an integer <code>x</code> such that <code>n == 3<sup>x</sup></code>.</p>


<p><strong>Example 1:</strong></p>

```clike
Input: n = 27
Output: true
```

</pre><p><strong>Example 2:</strong></p>

```clike
Input: n = 0
Output: false
```

</pre><p><strong>Example 3:</strong></p>

```clike
Input: n = 9
Output: true
```
</pre><p><strong>Example 4:</strong></p>
```clike
Input: n = 45
Output: false
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>-2<sup>31</sup> &lt;= n &lt;= 2<sup>31</sup> - 1</code></li>
</ul> 

<strong>Follow up:</strong> Could you solve it without loops/recursion?

题意：给定一个整数，写一个函数来判断它是否是 `3` 的幂次方。如果是，返回 `true` ；否则，返回 `false` 。<strong>进阶是</strong>在不使用循环或者递归来完成本题。

---

这道题类似于[231. Power of Two](https://leetcode-cn.com/problems/power-of-two/)和[342. Power of Four](https://leetcode-cn.com/problems/power-of-four/)，只是**2的幂**和**4的幂**的解法更加相似。

### 解法1 迭代+试除
朴素解法是不断将 $n$ 对 $3$ 试除，直到 $n$ 不再与 $3$ 呈倍数关系，然后判断 $n$ 是否等于 $3^0 = 1$ 即可。注意，非正整数不是 $3$ 的幂。算法的时间复杂度为 $O(\log_3 n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfThree(int n) {
        if (n <= 0) return false;
        while (n % 3 == 0) n /= 3;
        return n == 1;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了96.39% 的用户
//内存消耗：5.6 MB, 在所有 C++ 提交中击败了94.77% 的用户
```
我在[4的幂题解](https://memcpy0.blog.csdn.net/article/details/117408777)中的sx做法，这里也可以使用。将 $n$ 转为浮点数，然后不断除以 $3$ ，最后判断是否等于 $1.0$（不推荐使用） ：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfThree(int n) {
        double tn = n;
        while (tn > 1.0) tn /= 3;
        return tn == 1.0;
    }
};
//执行用时：16 ms, 在所有 C++ 提交中击败了71.42% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了40.15% 的用户
```
 ---
### 解法2 数学库函数+处理误差（不推荐使用）
使用数学库函数 `log(), round()` ，不过这里需要先判断 `n` 是否为正整数，而且要处理误差（可能是测试样例比[231. Power of Two](https://leetcode-cn.com/problems/power-of-two/)和[342. Power of Four](https://leetcode-cn.com/problems/power-of-four/)多得多，有两万多个吧）：
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfThree(int n) {
        if (n <= 0) return false;  
        double k = log(n) / log(3);
        return fabs(k - round(k)) <= 1e-10;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了96.39% 的用户
//内存消耗：6 MB, 在所有 C++ 提交中击败了6.12% 的用户
```
---
### 解法3 打表
最容易想到的一个「不使用循环/递归」的方法就是，进行打表预处理。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
int ans[25] = {1,3,9,27,81,243,729,2187,6561,19683,59049,177147,531441,1594323,4782969,14348907,43046721,129140163,387420489,1162261467};
class Solution {
public:
    bool isPowerOfThree(int n) {
        if (n <= 0) return false;
        for (int i = 0; i < 20; ++i) if (n == ans[i]) return true;
        return false;
    }
};
//执行用时：16 ms, 在所有 C++ 提交中击败了71.42% 的用户
//内存消耗：5.6 MB, 在所有 C++ 提交中击败了94.77% 的用户
```
---
### 解法4 数学（倍数/约数）
$n$ 的数据类型为 `int` ，由打表可知，`int` 范围内最大的 $3$ 次幂是 $1162261467$ 。如果 $n$ 为 $3$ 的幂，则必然满足 $n * 3^k = 1162261467$ ，即 $n$ 与 $1162261467$ 存在**倍数关系**。因此只需要判定 $n$ 是正整数、且是 $1162261467$ 的约数即可。算法的时间复杂度为 $O(1)$ ，空间复杂度为 $O(1)$ 。

需要注意的是，这种做法不是快速判断 $x$ 的幂次方的通用做法，**当且仅当 $x$ 为质数时可用**。所以无法推广到[342. Power of Four](https://leetcode-cn.com/problems/power-of-four/)。一个简单的示例是，$64$ 是 $4$ 的幂次方，也必然是某个最大的 $4$ 的幂次方数 $z$ 的约数，然而 $32$ 不是 $4$ 的幂次方，但一定是 $64$ 的约数、从而是 $z$ 的约数。
```cpp
//C++ version
class Solution {
public:
    bool isPowerOfThree(int n) {
        return n > 0 && 1162261467 % n == 0;
    }
};
//执行用时：16 ms, 在所有 C++ 提交中击败了71.42% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了58.63% 的用户
```
