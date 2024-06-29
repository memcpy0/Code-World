> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个正整数 `n` ，将其拆分为 `k` 个 **正整数** 的和（ `k >= 2` ），并使这些整数的乘积最大化。

返回 _你可以获得的最大乘积_ 。

**示例 1:**
```js
输入: n = 2
输出: 1
解释: 2 = 1 + 1, 1 × 1 = 1。
```
**示例 2:**
```js
输入: n = 10
输出: 36
解释: 10 = 3 + 3 + 4, 3 × 3 × 4 = 36。
```
**提示:**
- `2 <= n <= 58`

---
求组合数问题公式：$$dp[i] += dp[i-num]$$ $True/False$ 问题公式：$$dp[i] = dp[i] or dp[i-num]$$
最大最小问题公式：$$\begin{aligned}dp[i] = \min(dp[i],\ dp[i-num]+1)\\ 或者 dp[i] = \max(dp[i],\ dp[i-num]+1)\end{aligned}$$
以上三组公式是解决对应问题的**核心公式**。

拿到问题后，套用以下几个步骤，即可得到解法：
1. **分析是否为背包问题**。 背包问题的判定——背包问题具备的特征：给定一个 $target$ ，$target$ 可以是数字也可以是字符串，再给定一个数字或字符串数组，问：能否使用该数组中的元素**做各种排列组合**得到 $target$ 。
2. 是以上三种背包问题中的**哪一种**。如果是求组合数问题，**是否需要考虑元素之间的顺序**。需要考虑顺序有顺序的解法，不需要考虑顺序又有对应的解法。
3. **再分为**是0-1背包问题还是完全背包问题。也就是题目给的 $nums$ 数组中的元素**是否可以重复使用**。 

### 解法1 完全背包
对于正整数 $n$，当 $n\ge 2$ 时，可以拆分成至少两个正整数的和。由于每个正整数对应的最大乘积取决于比它小的正整数对应的最大乘积，因此可以使用动态规划求解。

套用上述步骤：
1. 目标是 $n$ ，元素数组为 $[1\dots n]$ ，使用该数组中的元素进行组合得到 $n$ 
2. 是最大问题，要求出这些元素组合的最大乘积
3. 是完全背包问题，元素可以重复使用

因此，创建数组 $dp$，其中 **$dp[i]$ 表示将正整数 $i$ 拆分成至少两个正整数的和之后，这些正整数的最大乘积**。特别地，$0$ 不是正整数，$1$ 是最小的正整数，$0$ 和 $1$ 都不能拆分，因此特设 $dp[0]=dp[1]=1$ 。
```cpp
class Solution {
public:
    int integerBreak(int n) {
        int dp[n + 1];
        dp[0] = 1;
        for (int i = 1; i < n; ++i) 
            for (int j = i; j <= n; ++j)    
                dp[j] = max(dp[j], dp[j - i] * i);
        return dp[n];
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$
- 空间复杂度：$O(n)$ ，其中 $n$ 是给定的正整数。创建一个数组 $dp$ ，其长度为 $n+1$ 。

---
### 解法2 优化的完全背包+数学
使用上述状态转移方程，计算 $dp[j]$ 时，$i$ 的值遍历了从 $1$ 到 $j-1$ 的所有值，因此总时间复杂度是 $O(n^2)$ 。是否可以降低时间复杂度？

上述状态转移方程包含两项，当 $j$ 固定时，$dp[i]$ 的值由 j×(i−j)j \times (i-j)j×(i−j) 和 j×dp[i−j]j \times \textit{dp}[i-j]j×dp[i−j] 中的较大值决定，因此需要对两项分别考虑。

首先考虑 j×dp[i−j]j \times \textit{dp}[i-j]j×dp[i−j] 这一项。

注意到 $dp$ 的定义，$dp[i]$ 表示将正整数 iii 拆分成至少两个正整数的和之后，这些正整数的最大乘积，因此对于任意 1≤j<i1 \le j < i1≤j<i，有 dp[i]≥j×dp[i−j]\textit{dp}[i] \ge j \times \textit{dp}[i-j]dp[i]≥j×dp[i−j]。

当 jjj 是奇数时，有 j=j−12+j+12j=\dfrac{j-1}{2}+\dfrac{j+1}{2}j= 
2
j−1
​
 + 
2
j+1
​
 ，因此 dp[i]≥j−12×dp[i−j−12]≥j−12×j+12×dp[i−j]\textit{dp}[i] \geq \dfrac{j-1}{2} \times \textit{dp}[i - \dfrac{j-1}{2}] \ge \dfrac{j-1}{2} \times \dfrac{j+1}{2} \times \textit{dp}[i-j]dp[i]≥ 
2
j−1
​
 ×dp[i− 
2
j−1
​
 ]≥ 
2
j−1
​
 × 
2
j+1
​
 ×dp[i−j]。

当 jjj 是偶数时，有 j=j2+j2j=\dfrac{j}{2}+\dfrac{j}{2}j= 
2
j
​
 + 
2
j
​
 ，因此 dp[i]≥j2×dp[i−j2]≥j2×j2×dp[i−j]\textit{dp}[i] \ge \dfrac{j}{2} \times \textit{dp}[i - \dfrac{j}{2}] \ge \dfrac{j}{2} \times \dfrac{j}{2} \times \textit{dp}[i-j]dp[i]≥ 
2
j
​
 ×dp[i− 
2
j
​
 ]≥ 
2
j
​
 × 
2
j
​
 ×dp[i−j]。

如果 j≥4j \ge 4j≥4 且 jjj 是奇数，则 j−12×j+12>j\dfrac{j-1}{2} \times \dfrac{j+1}{2}>j 
2
j−1
​
 × 
2
j+1
​
 >j 恒成立。如果 j≥4j \ge 4j≥4 且 jjj 是偶数，则 j2×j2≥j\dfrac{j}{2} \times \dfrac{j}{2} \ge j 
2
j
​
 × 
2
j
​
 ≥j 恒成立，当且仅当 j=4j=4j=4 时等号成立。

由此可知，如果 j≥4j \ge 4j≥4，则 dp[j]≥j\textit{dp}[j] \ge jdp[j]≥j，当且仅当 j=4j=4j=4 时等号成立，即当 j≥4j \ge 4j≥4 时一定能将 jjj 拆成至少两个正整数的和，这些正整数的乘积大于或等于 jjj。

同时也可以得到，如果 j≥4j \ge 4j≥4，则 dp[i]≥j×dp[i−j]\textit{dp}[i] \ge j \times \textit{dp}[i-j]dp[i]≥j×dp[i−j]，只有当 j=4j=4j=4 时等号可能成立。又由于

dp[i]≥2×dp[i−2]≥2×(2×dp[i−4])=4×dp[i−4]\textit{dp}[i] \ge 2 \times \textit{dp}[i-2] \ge 2 \times (2 \times \textit{dp}[i-4]) = 4 \times \textit{dp}[i-4]
dp[i]≥2×dp[i−2]≥2×(2×dp[i−4])=4×dp[i−4]
因此取 j=2j=2j=2 计算得到的 $dp[i]$ 一定不会小于取 j=4j=4j=4 计算得到的 $dp[i]$。根据上述分析，j≥4j \ge 4j≥4 的情况都是不需要考虑的。

那么 j=1j=1j=1 是否需要考虑？答案是不需要。如果取 j=1j=1j=1，则有 dp[i]≥1×dp[i−1]=dp[i−1]\textit{dp}[i] \ge 1 \times \textit{dp}[i-1]=\textit{dp}[i-1]dp[i]≥1×dp[i−1]=dp[i−1]。当 i≥3i \ge 3i≥3 时，dp[i−1]\textit{dp}[i-1]dp[i−1] 是将正整数 i−1i-1i−1 拆分成至少两个正整数的和之后，这些正整数的最大乘积，在拆分成的正整数中，任选一个数字加 $1$，则拆分成的正整数的和变成 iii，且乘积一定大于 dp[i−1]\textit{dp}[i-1]dp[i−1]，因此必有 dp[i]>dp[i−1]\textit{dp}[i]>\textit{dp}[i-1]dp[i]>dp[i−1]，即当 j=1j=1j=1 时不可能得到最大的 $dp[i]$ 的值。

根据上述分析可知，计算 $dp[i]$ 的值只需要考虑 j=2j=2j=2 和 j=3j=3j=3 的情况，不需要遍历从 $1$ 到 i−1i-1i−1 的所有值。

其次考虑 j×(i−j)j \times (i-j)j×(i−j) 这一项。

根据上述推导可知，如果 j≥4j \ge 4j≥4，则 dp[j]≥j\textit{dp}[j] \ge jdp[j]≥j，当且仅当 j=4j=4j=4 时等号成立。因此在 i−j≥4i-j \ge 4i−j≥4 的情况下，有 dp[i−j]≥i−j\textit{dp}[i-j] \ge i-jdp[i−j]≥i−j，dp[i]≥j×dp[i−j]≥j×(i−j)\textit{dp}[i] \ge j \times \textit{dp}[i-j] \ge j \times (i-j)dp[i]≥j×dp[i−j]≥j×(i−j)，此时计算 $dp[i]$ 的值不需要考虑 j×(i−j)j \times (i-j)j×(i−j) 的值。

如果 i−j<4i-j<4i−j<4，在计算 $dp[i]$ 的值的时候就需要考虑 j×(i−j)j \times (i-j)j×(i−j) 的值。在考虑 j×dp[i−j]j \times \textit{dp}[i-j]j×dp[i−j] 时，根据上述分析，只需要考虑 j=2j=2j=2 和 j=3j=3j=3 的情况。在考虑 j×(i−j)j \times (i-j)j×(i−j) 时，需要考虑 jjj 的哪些值？

如果 j=1j=1j=1，则 1×(i−1)=i−11 \times (i-1)=i-11×(i−1)=i−1，当 i=2i=2i=2 或 i=3i=3i=3 时有 dp[i]=i−1\textit{dp}[i]=i-1dp[i]=i−1，当 i≥4i \ge 4i≥4 时有 dp[i]≥i>i−1\textit{dp}[i] \ge i>i-1dp[i]≥i>i−1，显然当 i≥4i \ge 4i≥4 时取 j=1j=1j=1 不可能得到最大乘积，因此 j=1j=1j=1 时是不需要考虑的。

如果 j≥4j \ge 4j≥4，$dp[i]$ 是否可能等于 j×(i−j)j \times (i-j)j×(i−j)？当 iii 固定时，要使得 j×(i−j)j \times (i-j)j×(i−j) 的值最大，jjj 的值应该取 j=i/2j=i/2j=i/2，这里的 /// 表示整数除法。当 j≥4j \ge 4j≥4 时，若要满足 j=i/2j=i/2j=i/2，则 i≥8i \ge 8i≥8，此时 i−j≥4i-j \ge 4i−j≥4，利用上述结论，dp[i−j]≥i−j\textit{dp}[i-j] \ge i-jdp[i−j]≥i−j，因此 j×dp[i−j]≥j×(i−j)j \times \textit{dp}[i-j] \ge j \times (i-j)j×dp[i−j]≥j×(i−j)。由此可见，当 j≥4j \ge 4j≥4 时，计算 $dp[i]$ 只需要考虑 j×dp[i−j]j \times \textit{dp}[i-j]j×dp[i−j]，不需要考虑 j×(i−j)j \times (i-j)j×(i−j)。

又由于在使用 j×dp[i−j]j \times \textit{dp}[i-j]j×dp[i−j] 计算 $dp[i]$ 时，j=2j=2j=2 和 j=3j=3j=3 的情况一定优于 j≥4j \ge 4j≥4 的情况，因此无论是考虑 j×dp[i−j]j \times \textit{dp}[i-j]j×dp[i−j] 还是考虑 j×(i−j)j \times (i-j)j×(i−j)，都只需要考虑 j=2j=2j=2 和 j=3j=3j=3 的情况。

由此可以对方法一的动态规划进行优化。

边界情况是 n=2n=2n=2，此时唯一的拆分方案是 2=1+12=1+12=1+1，最大乘积是 1×1=11 \times 1=11×1=1。

当 i≥3i \ge 3i≥3 时，状态转移方程如下：

dp[i]=max⁡(2×(i−2),2×dp[i−2],3×(i−3),3×dp[i−3])\textit{dp}[i]=\max(2 \times (i-2), 2 \times \textit{dp}[i-2], 3 \times (i-3), 3 \times \textit{dp}[i-3])
dp[i]=max(2×(i−2),2×dp[i−2],3×(i−3),3×dp[i−3])
Java
C#
C++
Python3
Golang
C
class Solution {
    public int integerBreak(int n) {
        if (n <= 3) {
            return n - 1;
        }
        int[] dp = new int[n + 1];
        dp[2] = 1;
        for (int i = 3; i <= n; i++) {
            dp[i] = Math.max(Math.max(2 * (i - 2), 2 * dp[i - 2]), Math.max(3 * (i - 3), 3 * dp[i - 3]));
        }
        return dp[n];
    }
}
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 $n$ 是给定的正整数。和方法一相比，计算每个整数对应的 $dp$ 的值的时间复杂度降到 O(1)O(1)O(1)，因此总时间复杂度降到 O(n)O(n)O(n)。

空间复杂度：O(n)O(n)O(n)，其中 $n$ 是给定的正整数。创建一个数组 $dp$，其长度为 n+1n+1n+1。

方法三：数学
方法二中利用了数学知识降低时间复杂度。正整数 444 可以拆分成 2+22+22+2，乘积不变（4=2×24=2 \times 24=2×2）。对于大于 444 的正整数，总是存在一种拆分的方案，使得拆分成的两个正整数的乘积大于拆分前的正整数（例如，5=2+35=2+35=2+3，2×3=6>52 \times 3=6>52×3=6>5）。那么，能否利用数学知识在方法二的基础上进一步降低时间复杂度，找到最优的拆分方案呢？

下面给出两种直接得出最优拆分方案的证明方法。

函数极值证明法

显然，如果将给定的正整数拆分成尽可能多的某个特定的正整数，则这些正整数的乘积最大。

定义函数 f(x)f(x)f(x) 表示将给定的正整数 $n$ 拆分成尽可能多的正数 $x$ 的情况下的最大乘积，则可以将 $n$ 分成 nx\dfrac{n}{x} 
x
n
​
  项，此时 f(x)=xnxf(x)=x^{\frac{n}{x}}f(x)=x 
x
n
​
 
 ，目标是求 f(x)f(x)f(x) 的最大值，即

max⁡x(f(x))\mathop{\max}\limits_{x}(f(x))
x
max
​
 (f(x))
可以将 f(x)f(x)f(x) 写成如下形式：

f(x)=xnx=enln⁡xxf(x)=x^{\frac{n}{x}}=e^{\frac{n \ln x}{x}}
f(x)=x 
x
n
​
 
 =e 
x
nlnx
​
 
 
令 g(t)=etg(t)=e^tg(t)=e 
t
 ，h(x)=ln⁡xxh(x)=\dfrac{\ln x}{x}h(x)= 
x
lnx
​
 ，则有 f(x)=g(n⋅h(x))f(x)=g(n \cdot h(x))f(x)=g(n⋅h(x))。由于 g(t)g(t)g(t) 是单调递增的，n>0n>0n>0，因此 h(x)h(x)h(x) 与 f(x)f(x)f(x) 的单调性相同。

计算 h(x)h(x)h(x) 的驻点，即 h′(x)=1−ln⁡xx2=0h'(x)=\dfrac{1- \ln x}{x^2}=0h 
′
 (x)= 
x 
2
 
1−lnx
​
 =0 的点，得到驻点为 x=ex=ex=e。

由于当 0<x<e0<x<e0<x<e 时 h′(x)>0h'(x)>0h 
′
 (x)>0，当 x>ex>ex>e 时 h′(x)<0h'(x)<0h 
′
 (x)<0，因此 x=ex=ex=e 是 h(x)h(x)h(x) 的极大值点，也是 f(x)f(x)f(x) 的极大值点。由于函数 f(x)f(x)f(x) 的定义域连续，因此极大值点唯一，也是最大值点。

因此，当 x=ex=ex=e 时，f(x)f(x)f(x) 取到最大值，max⁡f(x)=f(e)=ene\max f(x)=f(e)=e^{\frac{n}{e}}maxf(x)=f(e)=e 
e
n
​
 
 。

由于 eee 不是整数，因此使用与 eee 最接近的整数作为 $x$ 的值，$x$ 可以是 222 或 333，此时需要比较 f(2)f(2)f(2) 与 f(3)f(3)f(3) 的大小，可以通过计算 f(3)f(2)\dfrac{f(3)}{f(2)} 
f(2)
f(3)
​
  进行比较。

f(3)f(2)=en⋅h(3)en⋅h(2)=en⋅h(3)−n⋅h(2)=en⋅(ln⁡33−ln⁡22)=en6⋅(2ln⁡3−3ln⁡2)=en6⋅(ln⁡9−ln⁡8)\dfrac{f(3)}{f(2)}=\dfrac{e^{n \cdot h(3)}}{e^{n \cdot h(2)}}=e^{n \cdot h(3)-n \cdot h(2)}=e^{n \cdot (\frac{\ln 3}{3} - \frac{\ln 2}{2})}=e^{\frac{n}{6} \cdot (2 \ln 3 - 3 \ln 2)}=e^{\frac{n}{6} \cdot (\ln 9 - \ln 8)}
f(2)
f(3)
​
 = 
e 
n⋅h(2)
 
e 
n⋅h(3)
 
​
 =e 
n⋅h(3)−n⋅h(2)
 =e 
n⋅( 
3
ln3
​
 − 
2
ln2
​
 )
 =e 
6
n
​
 ⋅(2ln3−3ln2)
 =e 
6
n
​
 ⋅(ln9−ln8)
 
由于 ln⁡9>ln⁡8\ln 9 > \ln 8ln9>ln8，因此 f(3)f(2)>1\dfrac{f(3)}{f(2)}>1 
f(2)
f(3)
​
 >1，即 f(3)>f(2)f(3)>f(2)f(3)>f(2)。当 x=3x=3x=3 时，可以得到最大乘积。因此，应该将给定的正整数拆分成尽可能多的 333。

根据 $n$ 除以 333 的余数进行分类讨论：

如果余数为 $0$，即 n=3m(m≥2)n=3m(m \ge 2)n=3m(m≥2)，则将 $n$ 拆分成 mmm 个 333；

如果余数为 $1$，即 n=3m+1(m≥1)n=3m+1(m \ge 1)n=3m+1(m≥1)，由于 2×2>3×12 \times 2 > 3 \times 12×2>3×1，因此将 $n$ 拆分成 m−1m-1m−1 个 333 和 222 个 222；

如果余数为 222，即 n=3m+2(m≥1)n=3m+2(m \ge 1)n=3m+2(m≥1)，则将 $n$ 拆分成 mmm 个 333 和 $1$ 个 222。

上述拆分的适用条件是 n≥4n \ge 4n≥4。如果 n≤3n \le 3n≤3，则上述拆分不适用，需要单独处理。

如果 n=2n=2n=2，则唯一的拆分方案是 2=1+12=1+12=1+1，最大乘积是 1×1=11 \times 1=11×1=1；

如果 n=3n=3n=3，则拆分方案有 3=1+2=1+1+13=1+2=1+1+13=1+2=1+1+1，最大乘积对应方案 3=1+23=1+23=1+2，最大乘积是 1×2=21 \times 2=21×2=2。

这两种情形可以合并为：当 n≤3n \le 3n≤3 时，最大乘积是 n−1n-1n−1。

归纳证明法

第一步：证明最优的拆分方案中不会出现大于 444 的整数。

假设出现了大于 444 的整数 $x$，由于 2(x−2)>x2(x-2) > x2(x−2)>x 在 x>4x > 4x>4 时恒成立，将 $x$ 拆分成 222 和 x−2x-2x−2 可以增大乘积。因此最优的拆分方案中不会出现大于 444 的整数。

第二步：证明最优的拆分方案中可以不出现整数 444。

如果出现了整数 444，我们可以用 2×22 \times 22×2 代替之，乘积不变。

此时，我们可以知道，最优的拆分方案中只会出现 $1$，222 和 333。

第三步：证明当 n≥5n \geq 5n≥5 时，最优的拆分方案中不会出现整数 $1$。

当 n≥5n \geq 5n≥5 时，如果出现了整数 $1$，那么拆分中剩余的数的和为 n−1≥4n-1 \geq 4n−1≥4，对应这至少两个整数。我们将其中任意一个整数 $x$ 加上 $1$，乘积就会增大。因此最优的拆分方案中不会出现整数 $1$。

此时，我们可以知道，当 n≥5n \geq 5n≥5 时，最优的拆分方案中只会出现 222 和 333。

第四步：证明当 n≥5n \geq 5n≥5 时，最优的拆分方案中 222 的个数不会超过 333 个。

如果出现了超过 333 个 222，那么将它们转换成 222 个 333，可以增大乘积，即 3×3>2×2×23 \times 3 > 2 \times 2 \times 23×3>2×2×2。

此时，n≥5n \geq 5n≥5 的最优拆分方案就唯一了。这是因为当最优的拆分方案中 222 的个数分别为 $0$，$1$，222 个时，就对应着 $n$ 除以 333 的余数分别为 $0$，222，$1$ 的情况。因此我们可以得到和「函数极值证明法」相同的分类讨论结果。

当 n=4n = 4n=4 时，4=2×24 = 2 \times 24=2×2 的最优拆分方案也可以放入分类讨论结果；当 2≤n≤32 \leq n \leq 32≤n≤3 时，只有唯一的拆分方案 1×(n−1)1 \times (n - 1)1×(n−1)。

Java
C#
C++
Python3
Golang
C
class Solution {
    public int integerBreak(int n) {
        if (n <= 3) {
            return n - 1;
        }
        int quotient = n / 3;
        int remainder = n % 3;
        if (remainder == 0) {
            return (int) Math.pow(3, quotient);
        } else if (remainder == 1) {
            return (int) Math.pow(3, quotient - 1) * 4;
        } else {
            return (int) Math.pow(3, quotient) * 2;
        }
    }
}
复杂度分析

时间复杂度：O(1)O(1)O(1)。涉及到的操作包括计算商和余数，以及幂次运算，时间复杂度都是常数。

空间复杂度：O(1)O(1)O(1)。只需要使用常数复杂度的额外空间。

作者：力扣官方题解
链接：https://leetcode.cn/problems/integer-break/description/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。