> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091351535.png)

Given two integers <code>dividend</code> and <code>divisor</code>, divide two integers without using multiplication, division, and mod operator.

Return the quotient after dividing <code>dividend</code> by <code>divisor</code>.

The integer division should truncate toward zero, which means losing its fractional part. For example, <code>truncate(8.345) = 8</code> and <code>truncate(-2.7335) = -2</code>.

<strong>Note: </strong>Assume we are dealing with an environment that could only store integers within the <strong>32-bit</strong> signed integer range: <code>[−2<sup>31</sup>, 2<sup>31</sup> − 1]</code>. For this problem, assume that your function <strong>returns </strong><code>2<sup>31</sup> − 1</code><strong> when the division result overflows</strong>.

<strong>Example 1:</strong>
```cpp
Input: dividend = 10, divisor = 3
Output: 3
Explanation: 10/3 = truncate(3.33333..) = 3.
```
 <strong>Example 2:</strong>
```cpp
Input: dividend = 7, divisor = -3
Output: -2
Explanation: 7/-3 = truncate(-2.33333..) = -2.
```
<strong>Example 3:</strong>
```cpp
Input: dividend = 0, divisor = 1
Output: 0
```
<strong>Example 4:</strong>
```cpp
Input: dividend = 1, divisor = 1
Output: 1
```
<strong>Constraints:</strong>
<ul>
	<li><code>-2<sup>31</sup> &lt;= dividend, divisor &lt;= 2<sup>31</sup> - 1</code></li>
	<li><code>divisor != 0</code></li>
</ul>

题意：给定两个整数，被除数 <code>dividend</code> 和除数 <code>divisor</code>。将两数相除，要求不使用乘法、除法和 mod 运算符。返回被除数 <code>dividend</code> 除以除数 <code>divisor</code> 得到的商。
<p>整数除法的结果应当截去（<code>truncate</code>）其小数部分，例如：<code>truncate(8.345) = 8</code> 以及 <code>truncate(-2.7335) = -2</code></p>
注意，被除数和除数均为 32 位有符号整数。除数不为 0。且假设我们的环境只能存储 32 位有符号整数，其数值范围是 <code>[−2<sup>31</sup>,  2<sup>31 </sup>− 1]</code>。本题中，如果除法结果溢出，则返回 <code>2<sup>31 </sup>− 1</code> 。

---
类似371和面试题17.01.Add Without Plus，用二进制完成加法/这里是除法;解法很多;
### 解法1 

![在这里插入图片描述](https://img-blog.csdnimg.cn/ac27e7427cce4286abb5df8385eb747e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

```cpp
class Solution {
    int INF = Integer.MAX_VALUE;
    public int divide(int _a, int _b) {
        long a = _a, b = _b;
        boolean flag = false;
        if ((a < 0 && b > 0) || (a > 0 && b < 0)) flag = true;
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        long l = 0, r = a;
        while (l < r) {
            long mid = l + r + 1 >> 1;
            if (mul(mid, b) <= a) l = mid;
            else r = mid - 1;
        }
        r = flag ? -r : r;
        if (r > INF || r < -INF - 1) return INF;
        return (int)r;
    }
    long mul(long a, long k) {
        long ans = 0;
        while (k > 0) {
            if ((k & 1) == 1) ans += a;
            k >>= 1;
            a += a;
        }
        return ans;
    }
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/48046e4edaf7400780b1f33356bd008c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
 
```cpp
class Solution {
    int MIN = Integer.MIN_VALUE, MAX = Integer.MAX_VALUE;
    int LIMIT = -1073741824; // MIN 的一半
    public int divide(int a, int b) {
        if (a == MIN && b == -1) return MAX;
        boolean flag = false;
        if ((a > 0 && b < 0) || (a < 0 && b > 0)) flag = true;
        if (a > 0) a = -a;
        if (b > 0) b = -b;
        int ans = 0;
        while (a <= b){
            int c = b, d = -1;
            while (c >= LIMIT && d >= LIMIT && c >= a - c){
                c += c; d += d;
            }
            a -= c;
            ans += d;
        }
        return flag ? ans : -ans;
    }
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/eb6063029fd44444b04361e2812e66fd.png)

由于题目规定了「只能存储 323232 位整数」，本题解的正文部分和代码中都不会使用任何 646464 位整数。诚然，使用 646464 位整数可以极大地方便我们的编码，但这是违反题目规则的。

如果除法结果溢出，那么我们需要返回 231−12^{31} - 12 
31
 −1 作为答案。因此在编码之前，我们可以首先对于溢出或者容易出错的边界情况进行讨论：

当被除数为 323232 位有符号整数的最小值 −231-2^{31}−2 
31
  时：

如果除数为 111，那么我们可以直接返回答案 −231-2^{31}−2 
31
 ；
如果除数为 −1-1−1，那么答案为 2312^{31}2 
31
 ，产生了溢出。此时我们需要返回 231−12^{31} - 12 
31
 −1。
当除数为 323232 位有符号整数的最小值 −231-2^{31}−2 
31
  时：

如果被除数同样为 −231-2^{31}−2 
31
 ，那么我们可以直接返回答案 111；
对于其余的情况，我们返回答案 000。
当被除数为 000 时，我们可以直接返回答案 000。

对于一般的情况，根据除数和被除数的符号，我们需要考虑 444 种不同的可能性。因此，为了方便编码，我们可以将被除数或者除数取相反数，使得它们符号相同。

如果我们将被除数和除数都变为正数，那么可能会导致溢出。例如当被除数为 −231-2^{31}−2 
31
  时，它的相反数 2312^{31}2 
31
  产生了溢出。因此，我们可以考虑将被除数和除数都变为负数，这样就不会有溢出的问题，在编码时只需要考虑 111 种情况了。

如果我们将被除数和除数的其中（恰好）一个变为了正数，那么在返回答案之前，我们需要对答案也取相反数。

方法一：二分查找
思路与算法

根据「前言」部分的讨论，我们记被除数为 XXX，除数为 YYY，并且 XXX 和 YYY 都是负数。我们需要找出 X/YX/YX/Y 的结果 ZZZ。ZZZ 一定是正数或 000。

根据除法以及余数的定义，我们可以将其改成乘法的等价形式，即：

Z×Y≥X>(Z+1)×Y Z \times Y \geq X > (Z+1) \times Y
Z×Y≥X>(Z+1)×Y
因此，我们可以使用二分查找的方法得到 ZZZ，即找出最大的 ZZZ 使得 Z×Y≥XZ \times Y \geq XZ×Y≥X 成立。

由于我们不能使用乘法运算符，因此我们需要使用「快速乘」算法得到 Z×YZ \times YZ×Y 的值。「快速乘」算法与「快速幂」类似，前者通过加法实现乘法，后者通过乘法实现幂运算。「快速幂」算法可以参考「50. Pow(x, n)」的官方题解，「快速乘」算法只需要在「快速幂」算法的基础上，将乘法运算改成加法运算即可。

细节

由于我们只能使用 323232 位整数，因此二分查找中会有很多细节。

首先，二分查找的下界为 111，上界为 231−12^{31} - 12 
31
 −1。唯一可能出现的答案为 2312^{31}2 
31
  的情况已经被我们在「前言」部分进行了特殊处理，因此答案的最大值为 231−12^{31} - 12 
31
 −1。如果二分查找失败，那么答案一定为 000。

在实现「快速乘」时，我们需要使用加法运算，然而较大的 ZZZ 也会导致加法运算溢出。例如我们要判断 A+BA + BA+B 是否小于 CCC 时（其中 A,B,CA, B, CA,B,C 均为负数），A+BA + BA+B 可能会产生溢出，因此我们必须将判断改为 A<C−BA < C - BA<C−B 是否成立。由于任意两个负数的差一定在 [−231+1,231−1][-2^{31} + 1, 2^{31} - 1][−2 
31
 +1,2 
31
 −1] 范围内，这样就不会产生溢出。

读者可以阅读下面的代码和注释，理解如何避免使用乘法和除法，以及正确处理溢出问题。

代码

C++
Java
C#
Python3
Golang
JavaScript
class Solution {
public:
    int divide(int dividend, int divisor) {
        // 考虑被除数为最小值的情况
        if (dividend == INT_MIN) {
            if (divisor == 1) {
                return INT_MIN;
            }
            if (divisor == -1) {
                return INT_MAX;
            }
        }
        // 考虑除数为最小值的情况
        if (divisor == INT_MIN) {
            return dividend == INT_MIN ? 1 : 0;
        }
        // 考虑被除数为 0 的情况
        if (dividend == 0) {
            return 0;
        }
        
        // 一般情况，使用二分查找
        // 将所有的正数取相反数，这样就只需要考虑一种情况
        bool rev = false;
        if (dividend > 0) {
            dividend = -dividend;
            rev = !rev;
        }
        if (divisor > 0) {
            divisor = -divisor;
            rev = !rev;
        }

        // 快速乘
        auto quickAdd = [](int y, int z, int x) {
            // x 和 y 是负数，z 是正数
            // 需要判断 z * y >= x 是否成立
            int result = 0, add = y;
            while (z) {
                if (z & 1) {
                    // 需要保证 result + add >= x
                    if (result < x - add) {
                        return false;
                    }
                    result += add;
                }
                if (z != 1) {
                    // 需要保证 add + add >= x
                    if (add < x - add) {
                        return false;
                    }
                    add += add;
                }
                // 不能使用除法
                z >>= 1;
            }
            return true;
        };
        
        int left = 1, right = INT_MAX, ans = 0;
        while (left <= right) {
            // 注意溢出，并且不能使用除法
            int mid = left + ((right - left) >> 1);
            bool check = quickAdd(divisor, mid, dividend);
            if (check) {
                ans = mid;
                // 注意溢出
                if (mid == INT_MAX) {
                    break;
                }
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }

        return rev ? -ans : ans;
    }
};
复杂度分析

时间复杂度：O(log⁡2C)O(\log^2 C)O(log 
2
 C)，其中 CCC 表示 323232 位整数的范围。二分查找的次数为 O(log⁡C)O(\log C)O(logC)，其中的每一步我们都需要 O(log⁡C)O(\log C)O(logC) 使用「快速乘」算法判断 Z×Y≥XZ \times Y \geq XZ×Y≥X 是否成立，因此总时间复杂度为 O(log⁡2C)O(\log^2 C)O(log 
2
 C)。

空间复杂度：O(1)O(1)O(1)。

方法二：类二分查找
前言

常规意义下的二分查找为：给定区间 [l,r][l, r][l,r]，取该区间的中点 mid=⌊l+r2⌋\textit{mid} = \lfloor \dfrac{l+r}{2} \rfloormid=⌊ 
2
l+r
​
 ⌋，根据 mid\textit{mid}mid 处是否满足某一条件，来决定移动左边界 lll 还是右边界 rrr。

我们也可以考虑另一种二分查找的方法：

记 kkk 为满足 2k≤r−l<2k+12^k \leq r-l < 2^{k+1}2 
k
 ≤r−l<2 
k+1
  的 kkk 值。

二分查找会进行 k+1k+1k+1 次。在第 i (1≤i≤k+1)i ~ (1 \leq i \leq k+1)i (1≤i≤k+1) 次二分查找时，设区间为 [li,ri][l_i, r_i][l 
i
​
 ,r 
i
​
 ]，我们取 mid=li+2k+1−i\textit{mid} = l_i + 2^{k+1-i}mid=l 
i
​
 +2 
k+1−i
 ：

如果 mid\textit{mid}mid 不在 [li,ri][l_i, r_i][l 
i
​
 ,r 
i
​
 ] 的范围内，那么我们直接忽略这次二分查找；

如果 mid\textit{mid}mid 在 [li,ri][l_i, r_i][l 
i
​
 ,r 
i
​
 ] 的范围内，并且 mid\textit{mid}mid 处满足某一条件，我们就将 lil_il 
i
​
  更新为 mid\textit{mid}mid，否则同样忽略这次二分查找。

最终 lil_il 
i
​
  即为二分查找的结果。这样做的正确性在于：

设在常规意义下的二分查找的答案为 ans\textit{ans}ans，记 δ\deltaδ 为 ans\textit{ans}ans 与左边界的差值 ans−l\textit{ans} - lans−l。δ\deltaδ 不会大于 r−lr-lr−l，并且 δ\deltaδ 一定可以用 2k,2k−1,2k−2,⋯ ,21,202^k, 2^{k-1}, 2^{k-2}, \cdots, 2^1, 2^02 
k
 ,2 
k−1
 ,2 
k−2
 ,⋯,2 
1
 ,2 
0
  中的若干个元素之和表示（考虑 δ\deltaδ 的二进制表示的意义即可）。因此上述二分查找是正确的。

思路与算法

基于上述的二分查找的方法，我们可以设计出如下的算法：

我们首先不断地将 YYY 乘以 222（通过加法运算实现），并将这些结果放入数组中，其中数组的第 iii 项就等于 Y×2iY \times 2^iY×2 
i
 。这一过程直到 YYY 的两倍严格小于 XXX 为止。

我们对数组进行逆序遍历。当遍历到第 iii 项时，如果其大于等于 XXX，我们就将答案增加 2i2^i2 
i
 ，并且将 XXX 中减去这一项的值。

本质上，上述的逆序遍历就模拟了二分查找的过程。

代码

C++
Java
C#
Python3
Golang
JavaScript
class Solution {
public:
    int divide(int dividend, int divisor) {
        // 考虑被除数为最小值的情况
        if (dividend == INT_MIN) {
            if (divisor == 1) {
                return INT_MIN;
            }
            if (divisor == -1) {
                return INT_MAX;
            }
        }
        // 考虑除数为最小值的情况
        if (divisor == INT_MIN) {
            return dividend == INT_MIN ? 1 : 0;
        }
        // 考虑被除数为 0 的情况
        if (dividend == 0) {
            return 0;
        }
        
        // 一般情况，使用类二分查找
        // 将所有的正数取相反数，这样就只需要考虑一种情况
        bool rev = false;
        if (dividend > 0) {
            dividend = -dividend;
            rev = !rev;
        }
        if (divisor > 0) {
            divisor = -divisor;
            rev = !rev;
        }

        vector<int> candidates = {divisor};
        // 注意溢出
        while (candidates.back() >= dividend - candidates.back()) {
            candidates.push_back(candidates.back() + candidates.back());
        }
        int ans = 0;
        for (int i = candidates.size() - 1; i >= 0; --i) {
            if (candidates[i] >= dividend) {
                ans += (1 << i);
                dividend -= candidates[i];
            }
        }

        return rev ? -ans : ans;
    }
};
复杂度分析

时间复杂度：O(log⁡C)O(\log C)O(logC)，即为二分查找需要的时间。方法二时间复杂度优于方法一的原因是：方法一的每一步二分查找都需要重新计算 Z×YZ \times YZ×Y 的值，需要 O(log⁡C)O(\log C)O(logC) 的时间复杂度；而方法二的每一步的权重都是 222 的幂，在二分查找开始前就都是已知的值，因此我们可以在 O(log⁡C)O(\log C)O(logC) 的时间内，一次性将它们全部预处理出来。

空间复杂度：O(log⁡C)O(\log C)O(logC)，即为需要存储的 Y×2iY \times 2^iY×2 
i
  的数量。



递归，不是简简单单么，c++十行代码

```kotlin
class Solution {
public:
    int divide(int dividend, int divisor) {
        if(dividend==INT_MIN&&divisor==-1) return INT_MAX;
        if(dividend==INT_MIN&&divisor==1) return INT_MIN;
        if(dividend>0) return -divide(-dividend, divisor);
        if(divisor>0) return -divide(dividend, -divisor);
        if(dividend>divisor) return 0;
        int res = 1, tmp = divisor;
        while((dividend-divisor)<=divisor)
        {
            res+=res;
            divisor+=divisor;
        }
        return res+divide(dividend-divisor, tmp);
    }
};
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307040112645.png)

```cpp
class Solution {
public:
    int divide(int dividend, int divisor) {
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        if (dividend == INT_MIN && divisor == 1) return INT_MIN;
        if (dividend == INT_MAX && divisor == -1) return -INT_MAX;
        if (dividend == INT_MAX && divisor == 1) return INT_MAX;
        if (divisor == INT_MIN) return dividend == INT_MIN ? 1 : 0;
        if (dividend == 0) return 0;
        if (dividend == divisor) return 1;
        if (dividend == -divisor) return -1;
        
        bool sign = false; // 正
        if (dividend > 0) {
            dividend = -dividend;
            sign = !sign;
        }
        if (divisor > 0) {
            divisor = -divisor;
            sign = !sign;
        }
        if (dividend > divisor) return 0;

        vector<int> tmp;
        tmp.push_back(divisor);
        while (divisor >= dividend - divisor) { 
            divisor += divisor;
            tmp.push_back(divisor);
        }
        int ans = 0;
        for (int n = tmp.size(), i = n - 1; i >= 0; --i) {
            if (tmp[i] >= dividend) {
                ans += (1 << i);
                dividend -= tmp[i];
            }
        }
        return sign ? -ans : ans;
    }
};
```
