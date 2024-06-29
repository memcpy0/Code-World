
> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Your task is to calculate <code>a<sup>b</sup></code> mod <code>1337</code> where <code>a</code> is a positive integer and <code>b</code> is an extremely large positive integer given in the form of an array.</p>


<p><strong>Example 1:</strong></p>

```java
Input: a = 2, b = [3]
Output: 8
```

<p><strong>Example 2:</strong></p>

```java
Input: a = 2, b = [1,0]
Output: 1024
```
 
<p><strong>Example 3:</strong></p>

```java
Input: a = 1, b = [4,3,3,8,5,2]
Output: 1
```
<p><strong>Example 4:</strong></p>

```java
Input: a = 2147483647, b = [2,0,0]
Output: 1198
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= a &lt;= 2<sup>31</sup> - 1</code></li>
	<li><code>1 &lt;= b.length &lt;= 2000</code></li>
	<li><code>0 &lt;= b[i] &lt;= 9</code></li>
	<li><code>b</code> doesn't contain leading zeros.</li>
</ul>

题意：你的任务是计算 $a^b$ 对 $1337$ 取模，$a$ 是一个正整数，$b$ 是一个非常大的正整数且会以数组形式给出。

---
### 解法1 高精度 `Python`
由题意可知，我们要计算的是 $a^b \pmod {1337}$ ，其中 $b$ 以数组形式给出，是一个非常大的正整数。乍一看有点麻烦，好在我们有无限精度的Python，以及它的快速幂函数 `pow` ：
```py
# Python version
class Solution:
    def superPow(self, a: int, b: List[int]) -> int:
        return pow(a, int(''.join([str(i) for i in b])), 1337)
# 执行用时：48 ms, 在所有 Python3 提交中击败了99.58% 的用户
# 内存消耗：13.6 MB, 在所有 Python3 提交中击败了61.30% 的用户
```
---
### 解法2 递归+快速幂（可选）
解法1能够通过，但对于锻炼码力毫无用处。我们仔细对比一下[**快速幂**](https://memcpy0.blog.csdn.net/article/details/108410952)，不仅能发现它们的相似之处，还可以学到如何「**快速计算低精度整数的高精度幂**」，而不仅仅是「**快速计算低精度整数的低精度幂**」。

推导可知：
$$a^b = a^{ ( \lfloor \frac{ b} { 10}\rfloor \times 10) + (b \bmod 10)}\\ 
= a^{ \lfloor \frac{ b} { 10}\rfloor \times 10} \times a^{b \bmod 10} \\
= (a^{ \lfloor \frac{ b} { 10}  \rfloor} )^{10} \times a^{b \bmod 10}$$

即，每次我们只需要递归计算 $a^{ \lfloor \frac{ b} { 10}  \rfloor}$ 的值、取其 $10$ 次方，再乘以 $a ^{b \bmod 10}$ 即可。也就是说，每次取 $b$ 数组的最后一位数，作为指数值，即可将问题规模缩小、递归求解（有点秦九韶算法的感觉）。由于真正涉及到幂运算的地方，所用的指数都是一个 $10$ 以内的数字，所以不一定要用快速幂，不过我还是条件反射地写了快速幂。

假设 $b$ 数组的长度为 $n$ ，则使用快速幂的代码，其时间复杂度为 $O(n \log_210)$ ，$\log_2 10$ 的含义是用快速幂计算次方为 $10$ 以内的值；不使用快速幂的代码，其时间复杂度为 $O(10 n)$ 。忽略递归带来的额外空间开销，空间复杂度为 $O(1)$ 。
```java
//Java version
class Solution {
    private int mod = 1337;
    private int fastPow(int a, int b) {
        int ans = 1;
        a %= mod; // a可能比较大,模mod后就<1337,相乘时不会溢出
        while (b != 0) {
            if ((b & 1) != 0) ans = (ans * a) % mod;
            a = (a * a) % mod;
            b >>= 1;
        }
        return ans;
    }
    private int dfs(int a, int[] b, int i) {
        if (i == -1) return 1;
        return (fastPow(dfs(a, b, i - 1), 10) * fastPow(a, b[i])) % mod;
    }
    public int superPow(int a, int[] b) {
        return dfs(a, b, b.length - 1);
    }
}
//执行用时：2 ms, 在所有 Java 提交中击败了91.39% 的用户
//内存消耗：38.7 MB, 在所有 Java 提交中击败了38.05% 的用户
```
---
### 解法3 迭代+快速幂
既然可以递归，当然可以迭代；既然可以二进制划分指数，当然可以十进制划分指数。
```java
//C++ version
class Solution {
private:
    int mod = 1337;
    int fastPow(int a, int b) {
        int ans = 1;
        while (b) {
            if (b & 1) ans = ans * a % mod;
            b >>= 1;
            a = a * a % mod;
        }
        return ans;
    }
public:
    int superPow(int a, vector<int>& b) {
        int ans = 1;
        a %= mod; // a可能比较大,模mod后就<1337,相乘时不会溢出
        for (int i = b.size() - 1; i >= 0; --i) {
            if (b[i] != 0) ans = (ans * fastPow(a, b[i])) % mod;
            a = fastPow(a, 10);
        }
        return ans;
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了84.62% 的用户
//内存消耗：11.3 MB, 在所有 C++ 提交中击败了88.96% 的用户
```

