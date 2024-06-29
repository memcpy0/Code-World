> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>The Tribonacci sequence T<sub>n</sub> is defined as follows:&nbsp;</p>

<p>T<sub>0</sub> = 0, T<sub>1</sub> = 1, T<sub>2</sub> = 1, and T<sub>n+3</sub> = T<sub>n</sub> + T<sub>n+1</sub> + T<sub>n+2</sub> for n &gt;= 0.</p>

<p>Given <code>n</code>, return the value of T<sub>n</sub>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: n = 4
Output: 4
Explanation:
T_3 = 0 + 1 + 1 = 2
T_4 = 1 + 1 + 2 = 4
```
<p><strong>Example 2:</strong></p>

```cpp
Input: n = 25
Output: 1389537
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= n &lt;= 37</code></li>
	<li>The answer is guaranteed to fit within a 32-bit integer, ie. <code>answer &lt;= 2^31 - 1</code>.</li>
</ul>


题意：给你整数 $n$ ，请返回第 $n$ 个泰波那契数 $T_n$ 的值。

---
### 解法1 递推（动态规划）
转移方程都给出来了，完全可以通过递推解决本题。时间复杂度 $O(n)$ ，空间复杂度 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    int tribonacci(int n) {
        if (n <= 1) return n;
        if (n == 2) return 1;
        int a = 0, b = 1, c = 1;
        for (int i = 3; i <= n; ++i) {
            int d = a + b + c;
            a = b;
            b = c;
            c = d;
        }
        return c;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6 MB, 在所有 C++ 提交中击败了20.16% 的用户
```
---
### 解法2 递归+记忆化（动态规划）
既然能用「递推」形式实现动态规划，自然也能用「递归」的形式实现。只是为了避免重复计算，需要加入「记忆化」功能，即利用「某个值 `x` 在不同的调用间，可能作为中间结果被重复计算，并且计算结果 `tribonacci(x)` 完全确定」这一特性，使用 `static` 缓存器，让所有计算过的结果在全部样例中共享。时间复杂度 $O(n)$ ，空间复杂度 $O(n)$ ：
```cpp
//C++ version
static int cache[40] = {0};
class Solution {
public:
    int tribonacci(int n) {
        if (n <= 1) return n;
        if (n == 2) return 1;
        if (cache[n]) return cache[n];
        cache[n] = tribonacci(n - 1) + tribonacci(n - 2) + tribonacci(n - 3);
        return cache[n];
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了90.99% 的用户
```
---
### 解法3 打表
由于数据范围很小，可以预先打表，然后直接返回。时间复杂度 $O(1)$ ，空间复杂度 $O(n)$ ：
```cpp
//C++ version
static int ans[40] = {0, 1, 1, 2, 4, 7, 13, 24, 44, 81, 149, 274, 504, 927, 1705, 3136, 5768, 10609, 19513, 35890, 66012, 121415, 223317, 410744, 755476, 1389537, 2555757, 4700770, 8646064, 15902591, 29249425, 53798080, 98950096, 181997601, 334745777, 615693474, 1132436852, 2082876103};
class Solution {
public:
    int tribonacci(int n) {
        return ans[n];
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了78.26% 的用户
```
---
### 解法4 矩阵快速幂
**对于此类数列递推问题，均可使用矩阵快速幂加速**。一般性的做法如下所述。

首先要将答案依赖的多个状态表示为列向量。本题中要求的是 $f(n)$ ，它依赖于 $f(n - 1),\ f(n - 2), f(n - 3)$ ：
$$\begin{bmatrix}
 \  f(n - 1)\ \\
 \  f(n-2)\ \\
 \ f(n - 3)\ \\
\end{bmatrix}$$

然后，列出目标值 $f(n)$ 所在矩阵：
$$\begin{bmatrix}
\ f(n)\ \\
\ f(n - 1)\ \\
\ f(n - 2)\ \\
\end{bmatrix}$$

接着，利用转移方程对目标矩阵的元素进行展开：
$$
\begin{bmatrix}
\ f(n)\ \\
\ f(n - 1)\ \\
\ f(n - 2)\ \\
\end{bmatrix}
= \begin{bmatrix}
\ f(n - 1) * 1 + f(n-2) * 1 + f(n - 3) * 1\ \\
\ f(n - 1) * 1 + f(n - 2) * 0 + f(n - 3) * 0\ \\
\ f(n - 1) * 0 + f(n - 2) * 1 + f(n- 3) * 0\ \\
\end{bmatrix}
$$

根据矩阵乘法，可以得到：
$$
\begin{bmatrix}
\ f(n)\ \\
\ f(n - 1)\ \\
\ f(n - 2)\ \\
\end{bmatrix}
= \begin{bmatrix}
\ 1\quad 1 \quad 1\ \\
\ 1\quad 0 \quad 0\ \\
\ 0 \quad 1 \quad 0\ \\
\end{bmatrix} *
\begin{bmatrix}
\ f(n - 1)\ \\
\ f(n - 2)\ \\
\ f(n - 3)\ \\
\end{bmatrix}
$$

现在令：
$$
E =  \begin{bmatrix}
\ 1\quad 1 \quad 1\ \\
\ 1\quad 0 \quad 0\ \\
\ 0 \quad 1 \quad 0\ \\
\end{bmatrix}$$

开始时只有 $a = \begin{bmatrix}\ f(2)\ \\ \ f(1)\ \\ \ f(0) \ \\ \end{bmatrix}$ ，根据递推式和矩阵乘法的结合性，可以得到：
$$
\begin{bmatrix}
\ f(n)\ \\
\ f(n - 1)\ \\
\ f(n - 2)\ \\
\end{bmatrix}
= \overbrace{E * E *  \dots * E}^{n-2个E} * \begin{bmatrix}\ f(2)\ \\ \ f(1)\ \\ \ f(0) \ \\ \end{bmatrix} \\
= E^{n - 2} * \begin{bmatrix}\ f(2)\ \\ \ f(1)\ \\ \ f(0) \ \\ \end{bmatrix}
$$

然后套用「快速幂」计算 $E^{n-2} * a$ ，从而得到结果矩阵。最后取出目标元素并返回。算法的时间复杂度为 $O(\log n)$ ，空间复杂度为 $O(1)$ ：
```cpp
//C++ version
class Solution {
private:
    using Matrix = vector<vector<long long>>;
    Matrix multi(const Matrix& a, const Matrix& b) {
        int r = a.size(), c = b[0].size(), z = b.size();
        Matrix ans(r, vector<long long>(c));
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                for (int k = 0; k < z; ++k)
                    ans[i][j] += a[i][k] * b[k][j];
        return ans;
    }
public:
    int tribonacci(int n) {
        if (n <= 1) return n;
        if (n == 2) return 1;
        Matrix E = {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}}, ans = {{1}, {1}, {0}};
        int x = n - 2;
        while (x) {
            if (x & 1) ans = multi(E, ans);
            E = multi(E, E);
            x >>= 1;
        }
        return (int)ans[0][0];
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.1 MB, 在所有 C++ 提交中击败了5.93% 的用户
```




---
### 解法5 数学（生成函数）

