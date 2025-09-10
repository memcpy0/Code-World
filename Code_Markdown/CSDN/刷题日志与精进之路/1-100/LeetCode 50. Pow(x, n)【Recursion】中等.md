> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091735319.png)

<div class="notranslate"><p>Implement <a href="http://www.cplusplus.com/reference/valarray/pow/">pow(<em>x</em>, <em>n</em>)</a>, which calculates&nbsp;<em>x</em> raised to the power <em>n</em> (i.e. x<sup><span style="font-size:10.8333px">n</span></sup>).</p>

 
<p><strong>Example 1:</strong></p>

<pre><strong>Input:</strong> x = 2.00000, n = 10
<strong>Output:</strong> 1024.00000
</pre>

<p><strong>Example 2:</strong></p>

<pre><strong>Input:</strong> x = 2.10000, n = 3
<strong>Output:</strong> 9.26100
</pre>

<p><strong>Example 3:</strong></p>

<pre><strong>Input:</strong> x = 2.00000, n = -2
<strong>Output:</strong> 0.25000
<strong>Explanation:</strong> 2<sup>-2</sup> = 1/2<sup>2</sup> = 1/4 = 0.25
</pre>

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>-100.0 &lt;&nbsp;x&nbsp;&lt; 100.0</code></li>
	<li><code>-2<sup>31</sup>&nbsp;&lt;= n &lt;=&nbsp;2<sup>31</sup>-1</code></li>
	<li><code>-10<sup>4</sup> &lt;= x<sup>n</sup> &lt;= 10<sup>4</sup></code></li>
</ul>
</div>

题意：实现函数 `double Power(double base, int exponent)` ，求 `base` 的 `exponent` 次方。不得使用库函数，同时不需要考虑大数问题。

---
虽然不用考虑大数问题，但这不是意味着简单递归/迭代乘法，就能够完成这一道题——比如 `1.00000 2147483647` ，顺序相乘绝对会超时。
### 解法1 递归快速幂
```cpp
class Solution { 
public:
    double myPow(double x, int n) { 
        if (n == 0) return 1; //正数右移最后得到0
        if (n == 1) return x;
        if (n == -1) return 1 / x; //负数右移永远是负数,加一个判断
        double temp = myPow(x, n >> 1);
        if (n & 1) return temp * temp * x;
        else return temp * temp;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了28.57% 的用户
```
或者写成：
```cpp
class Solution { 
public:
    double myPow(double x, int n) { 
        if (n == 0) return 1; //正数右移最后得到0
        if (n == 1) return x;
        if (n == -1) return 1 / x; //负数右移永远是负数 
        if (n & 1) return myPow(x, n - 1) * x;
        double temp = myPow(x, n >> 1);
        return temp * temp;
    }
};
```
---
### 解法2 迭代快速幂
```cpp
class Solution { 
public:
    double myPow(double x, int n) { 
        double ans = 1, base = x;
        bool flag = (n >= 0);
        //负数取反,考虑到最小负数,需要先自增,后续再多除一个x
        if (!flag) n = -(++n); //或者使用longlong
        while (n) {
            if (n & 1) ans *= base;
            base *= base;
            n >>= 1; //n必须取正
        }
        return flag ? ans : 1 / ans / x;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了20.39% 的用户
```
或者一开始 `n` 不取正，而是使用 `/ 2` 代替右移，这样**不论正负 `n` 最终都会缩小到 `0`** ：
```cpp
class Solution { 
public:
    double myPow(double x, int n) { 
        double ans = 1, base = x;
        bool flag = (n >= 0);
        while (n) {
            if (n & 1) ans *= base;
            base *= base;
            n /= 2; //换成/2,不用担心负数问题
        }
        return flag ? ans : 1 / ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了8.13% 的用户
```
