> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

有 `buckets` 桶液体，其中 **正好有一桶** 含有毒药，其余装的都是水。它们从外观看起来都一样。为了弄清楚哪只水桶含有毒药，你可以喂一些猪喝，通过观察猪是否会死进行判断。不幸的是，你只有 `minutesToTest` 分钟时间来确定哪桶液体是有毒的。

喂猪的规则如下：
1. 选择若干活猪进行喂养
2. 可以允许小猪同时饮用任意数量的桶中的水，并且该过程不需要时间。
3. 小猪喝完水后，必须有 `minutesToDie` 分钟的冷却时间。在这段时间里，你只能观察，而不允许继续喂猪。
4. 过了 `minutesToDie` 分钟后，所有喝到毒药的猪都会死去，其他所有猪都会活下来。
5. 重复这一过程，直到时间用完。

给你桶的数目 `buckets` ，`minutesToDie` 和 `minutesToTest` ，返回 _在规定时间内判断哪个桶有毒所需的 **最小** 猪数_ 。

**示例 1：**
```js
输入：buckets = 1000, minutesToDie = 15, minutesToTest = 60
输出：5
```
**示例 2：**
```js
输入：buckets = 4, minutesToDie = 15, minutesToTest = 15
输出：2
```
**示例 3：**
```js
输入：buckets = 4, minutesToDie = 15, minutesToTest = 30
输出：2
```
**提示：**
- `1 <= buckets <= 1000`
- `1 <= minutesToDie <= minutesToTest <= 100`

---
### 解法1 动态规划
根据题意，$minutesToDie$ 和 $minutesToTest$ 可以算出最大测试轮数 $\textit{iterations} = \Big\lfloor \dfrac{\textit{minutesToTest}}{\textit{minutesToDie}} \Big\rfloor$ 。

问题的等价描述是：在 $buckets$ 桶液体中恰好有一桶有毒，至少需要多少只小猪才能在 $iterations$ 轮测试中确定有毒的是哪一桶。

这个问题很难直接计算，不过可以从别的角度考虑——用 $f(i, j)$ 表示 **$i$ 只小猪测试 $j$ 轮、最多可在多少桶液体中确定有毒的是哪一桶**。当最大测试轮数确定为 $iterations$ 时，需要计算让 $f(i, iterations) \ge buckets$ 成立的最小的 $i$ 。

考虑边界情况：
- 如果测试轮数是 $0$ ，则不能进行测试。如果小猪数量是 $0$ ，也不能进行测试。此时如果有超过 $1$ 桶液体，则无法确定有毒的是哪一桶，因此，这种情况下**最多只能在 $1$ 桶液体中确定有毒的是这唯一的一桶**。因此对任意 $i$ 都有 $f(i,0) = 1$ ，对任意 $j$ 都有 $f(0, j) = 1$ 。

当 $i, j$ 都大于 $0$ 时，可以通过递推方法计算 $f(i, j)$ 的值。
- 当剩下 $i$ 只小猪和 $j$ 轮测试时，如果在一轮测试后有 $k$ 只小猪存活，则剩下 $k$ 只小猪和 $j - 1$ 轮测试。
- 由于 $k$ 只小猪和 $j - 1$ 轮测试可判断的最大桶数是 $f(k, j - 1)$ ，$i$ 只小猪中有 $k$ 只小猪存活的组合数是 $C(i, k)$ 。特别的 $C(0, 0) = 1$ 。
- 因此剩下 $k$ 只小猪和 $j -1$ 轮测试时，可判断的最大桶数是 $f(k, j - 1) \times C(i, k)$ 。
- 由于 $0\le k \le i$ ，因此 $f(i, j)$ 的计算如下：$$f(i,j) = \sum_{k=0} f(k, j - 1) \times C(i, k)$$


当 i≥1i \ge 1i≥1 时，组合数的计算如下：

C(i,0)=C(i,i)=1C(i, 0) = C(i, i) = 1C(i,0)=C(i,i)=1；

当 0<j<i0 < j < i0<j<i 时，C(i,j)=C(i−1,j−1)+C(i−1,j)C(i, j) = C(i - 1, j - 1) + C(i - 1, j)C(i,j)=C(i−1,j−1)+C(i−1,j)。

当小猪数量等于 buckets−1\textit{buckets} - 1buckets−1 时，可以将 buckets−1\textit{buckets} - 1buckets−1 桶液体分别给每只小猪喝一桶，剩下一桶液体没有小猪喝，如果有一只小猪死了则这只小猪喝的一桶液体有毒，如果小猪都存活则剩下一桶没有小猪喝的液体有毒，此时可以在一轮内确定有毒的是哪一桶。因此最多需要的小猪数量是 buckets−1\textit{buckets} - 1buckets−1，iii 的取值范围是 0≤i<buckets0 \le i < \textit{buckets}0≤i<buckets。

由于最大测试轮数 iterations\textit{iterations}iterations 可以根据 minutesToDie\textit{minutesToDie}minutesToDie 和 minutesToTest\textit{minutesToTest}minutesToTest 计算得到，因此最大测试轮数可以看成已知，任何情况下的测试轮数都不能超过最大测试轮数，jjj 的取值范围是 0≤j≤iterations0 \le j \le \textit{iterations}0≤j≤iterations。

为了计算 fff 的值，一种做法是预先计算组合数，然后计算 fff 的值，但是题目的数据规模是 buckets≤1000\textit{buckets} \le 1000buckets≤1000，如果预先计算所有 0≤j≤i≤buckets0 \le j \le i \le \textit{buckets}0≤j≤i≤buckets 的组合数则可能导致结果溢出。为了避免溢出，可以在计算 fff 的同时计算组合数。

具体做法是，对于 1≤i<buckets1 \le i < \textit{buckets}1≤i<buckets 的每个 iii，首先计算满足 0≤j≤i0 \le j \le i0≤j≤i 的所有组合数 C(i,j)C(i, j)C(i,j)，然后计算所有满足 1≤j≤iterations1 \le j \le \textit{iterations}1≤j≤iterations 的 f(i,j)f(i, j)f(i,j)。计算过程中，找到使得 f(i,iterations)≥bucketsf(i, \textit{iterations}) \ge \textit{buckets}f(i,iterations)≥buckets 成立的最小的 iii 并返回，该返回值即为至少需要的小猪数量。

特别地，当 buckets=1\textit{buckets} = 1buckets=1 时，不需要进行测试即可知道这唯一的一桶液体一定有毒，此时不需要任何小猪，返回 000。

下面用一个例子说明 fff 的计算。假设有 333 只小猪和 444 轮测试，f(3,4)=125f(3, 4) = 125f(3,4)=125，即最多可以在 125125125 桶液体中确定有毒的是哪一桶。

将 125125125 桶液体排成 5×5×55 \times 5 \times 55×5×5 的正方体，每桶液体都可以用唯一的坐标 (x,y,z)(x, y, z)(x,y,z) 表示，其中 xxx、yyy、zzz 都是整数且取值范围都是 [0,4][0, 4][0,4]。

排成棱长为 555 的正方体是因为 444 轮测试对应 555 种状态，前 444 种状态分别是在 444 轮当中的某一轮喝，最后一种状态是不喝。

在第 iii 轮测试中，第 000 只小猪喝 x=ix = ix=i 平面内的全部液体，第 111 只小猪喝 y=iy = iy=i 平面内的全部液体，第 222 只小猪喝 z=iz = iz=i 平面内的全部液体，其中 0≤i<40 \le i < 40≤i<4。

考虑第 000 轮之后存活的小猪数量。

第 000 轮之后没有小猪存活。有毒的液体位于 (0,0,0)(0, 0, 0)(0,0,0)，有毒的液体的可能位置有 f(0,3)×C(3,0)=1f(0, 3) \times C(3, 0) = 1f(0,3)×C(3,0)=1 个。

第 000 轮之后有 111 只小猪存活。假设存活的是第 000 只小猪，则有毒的液体的坐标 (x,y,z)(x, y, z)(x,y,z) 满足 x≠0x \ne 0x

=0、y=0y = 0y=0 且 z=0z = 0z=0，此时 xxx 有 444 种取值，因此有毒的液体的可能位置有 f(1,3)=4f(1, 3) = 4f(1,3)=4 个。

由于有 111 只小猪存活的组合数是 C(3,1)=3C(3, 1) = 3C(3,1)=3，因此有毒的液体的所有可能位置有 f(1,3)×C(3,1)=12f(1, 3) \times C(3, 1) = 12f(1,3)×C(3,1)=12 个。
第 000 轮之后有 222 只小猪存活。假设存活的是第 000 只小猪和第 111 只小猪，则有毒的液体的坐标 (x,y,z)(x, y, z)(x,y,z) 满足 x≠0x \ne 0x

=0、y≠0y \ne 0y

=0 且 z=0z = 0z=0，此时 xxx 和 yyy 各有 444 种取值，因此有毒的液体的可能位置有 f(2,3)=16f(2, 3) = 16f(2,3)=16 个。

由于有 222 只小猪存活的组合数是 C(3,2)=3C(3, 2) = 3C(3,2)=3，因此有毒的液体的所有可能位置有 f(2,3)×C(3,2)=48f(2, 3) \times C(3, 2) = 48f(2,3)×C(3,2)=48 个。
第 000 轮之后有 333 只小猪存活。有毒的液体的坐标 (x,y,z)(x, y, z)(x,y,z) 满足 x≠0x \ne 0x

=0、y≠0y \ne 0y

=0 且 z≠0z \ne 0z

=0，此时 xxx、yyy 和 zzz 各有 444 种取值，因此有毒的液体的可能位置有 f(3,3)×C(3,3)=64f(3, 3) \times C(3, 3) = 64f(3,3)×C(3,3)=64。

因此 f(3,4)=1+12+48+64=125f(3, 4) = 1 + 12 + 48 + 64 = 125f(3,4)=1+12+48+64=125。

Java
C#
C++
Golang
Python3
JavaScript
class Solution {
public:
    int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
        if (buckets == 1) {
            return 0;
        }
        vector<vector<int>> combinations(buckets + 1,vector<int>(buckets + 1));
        combinations[0][0] = 1;
        int iterations = minutesToTest / minutesToDie;
        vector<vector<int>> f(buckets,vector<int>(iterations + 1));
        for (int i = 0; i < buckets; i++) {
            f[i][0] = 1;
        }
        for (int j = 0; j <= iterations; j++) {
            f[0][j] = 1;
        }
        for (int i = 1; i < buckets; i++) {
            combinations[i][0] = 1;
            combinations[i][i] = 1;
            for (int j = 1; j < i; j++) {
                combinations[i][j] = combinations[i - 1][j - 1] + combinations[i - 1][j];
            }
            for (int j = 1; j <= iterations; j++) {
                for (int k = 0; k <= i; k++) {
                    f[i][j] += f[k][j - 1] * combinations[i][i - k];
                }
            }
            if (f[i][iterations] >= buckets) {
                return i;
            }
        }
        return 0;
    }
};
复杂度分析

时间复杂度：O(buckets×(buckets+iterations×buckets))O(\textit{buckets} \times (\textit{buckets} + \textit{iterations}\times\textit{buckets}))O(buckets×(buckets+iterations×buckets))，其中 iterations=⌊minutesToTestminutesToDie⌋\textit{iterations} = \Big\lfloor \dfrac{\textit{minutesToTest}}{\textit{minutesToDie}} \Big\rflooriterations=⌊ 
minutesToDie
minutesToTest
​
 ⌋ 为最大测试轮数。最多需要循环 O(buckets)O(\textit{buckets})O(buckets) 轮，对于每一轮循环，需要 O(buckets)O(\textit{buckets})O(buckets) 的时间计算组合数，以及需要 O(iterations×buckets)O(\textit{iterations}\times\textit{buckets})O(iterations×buckets) 的时间计算 fff 的状态值。

空间复杂度：O(buckets2+buckets×iterations)O(\textit{buckets}^2 + \textit{buckets} \times \textit{iterations})O(buckets 
2
 +buckets×iterations)，其中 iterations=⌊minutesToTestminutesToDie⌋\textit{iterations} = \Big\lfloor \dfrac{\textit{minutesToTest}}{\textit{minutesToDie}} \Big\rflooriterations=⌊ 
minutesToDie
minutesToTest
​
 ⌋ 为最大测试轮数。需要创建二维数组 combinations\textit{combinations}combinations 和 fff。

方法二：数学
方法一的动态规划需要计算 fff 的每个状态，也可以直接推导 fff 的每个元素的表达式。

当最大测试轮数是 111 时，iii 只小猪可以判断的最大桶数是 f(i,1)f(i, 1)f(i,1)。根据递推关系，有

f(i,1)=∑k=0if(k,0)×C(i,k)=∑k=0iC(i,k)=2i\begin{aligned} f(i, 1) &= \sum\limits_{k = 0}^i f(k, 0) \times C(i, k) \\ &= \sum\limits_{k = 0}^i C(i, k) \\ &= 2^i \end{aligned}
f(i,1)
​
  
= 
k=0
∑
i
​
 f(k,0)×C(i,k)
= 
k=0
∑
i
​
 C(i,k)
=2 
i
 
​
 
当最大测试轮数是 222 时，iii 只小猪可以判断的最大桶数是 f(i,2)f(i, 2)f(i,2)。根据递推关系，有

f(i,2)=∑k=0if(k,1)×C(i,k)=∑k=0i2k×C(i,k)=3i\begin{aligned} f(i, 2) &= \sum\limits_{k = 0}^i f(k, 1) \times C(i, k) \\ &= \sum\limits_{k = 0}^i 2^k \times C(i, k) \\ &= 3^i \end{aligned}
f(i,2)
​
  
= 
k=0
∑
i
​
 f(k,1)×C(i,k)
= 
k=0
∑
i
​
 2 
k
 ×C(i,k)
=3 
i
 
​
 
推广到一般情况，当最大测试轮数是 jjj 时，iii 只小猪可以判断的最大桶数是 f(i,j)f(i, j)f(i,j)。根据递推关系，有

f(i,j)=∑k=0if(k,j−1)×C(i,k)=∑k=0ijk×C(i,k)=(j+1)i\begin{aligned} f(i, j) &= \sum\limits_{k = 0}^i f(k, j - 1) \times C(i, k) \\ &= \sum\limits_{k = 0}^i j^k \times C(i, k) \\ &= (j + 1)^i \end{aligned}
f(i,j)
​
  
= 
k=0
∑
i
​
 f(k,j−1)×C(i,k)
= 
k=0
∑
i
​
 j 
k
 ×C(i,k)
=(j+1) 
i
 
​
 
上述结论可以通过二项式定理证明。

当最大测试轮数为 iterations\textit{iterations}iterations 时，需要找到使得 (iterations+1)i≥buckets(\textit{iterations} + 1)^i \ge \textit{buckets}(iterations+1) 
i
 ≥buckets 成立的最小的 iii，即为至少需要的小猪数量。令 states=iterations+1\textit{states} = \textit{iterations} + 1states=iterations+1，则至少需要的小猪数量是 ⌈log⁡statesbuckets⌉\lceil \log_{\textit{states}} \textit{buckets} \rceil⌈log 
states
​
 buckets⌉。

实现方面需要注意浮点数的精度问题。

Java
C#
C++
Golang
Python3
JavaScript
class Solution {
public:
    int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
        int states = minutesToTest / minutesToDie + 1;
        int pigs = ceil(log(buckets) / log(states) - 1e-5);
        return pigs;
    }
};
复杂度分析

时间复杂度：O(1)O(1)O(1)。

空间复杂度：O(1)O(1)O(1)。

数学
我们用实验对象来代指题干的小动物。同时为了方便，我们使用 nnn 代指有多少桶水，ddd 为实验对象的反应时间，ttt 为测试总时间。

根据题意，最大测试次数为 k=⌊td⌋k = \left \lfloor \frac{t}{d} \right \rfloork=⌊ 
d
t
​
 ⌋。

我们可以先考虑 k=1k = 1k=1 的情况，最简单的情况是，我们使用与水同等数量的实验对象数量来进行测试。

此时哪个实验对象有反应，则可以推断出哪一桶水有问题。

但这样的测试方式，每个实验动物承载的信息量是很低的，每个实验对象仅承载了某一桶水是否有问题。

为减少实验对象数量，我们需要增大每个实验对象承载的信息量（让每个实验对象同时测试多桶水），然后从最终所有实验对象的状态（是否有所反应）来反推哪一桶水有问题。

用最小单位表示最大信息量，这引导我们使用「进制表示」相关方式。由于我们只有 111 次测试机会，因此我们可以使用二进制的方式进行测试。

当 k=1k = 1k=1，使用二进制的方式测试哪桶水有问题，我们至少需要 mmm 个实验对象（其中 mmm 为 nnn 的二进制表示的长度），然后让编号为 xxx（0<=x<m0 <= x < m0<=x<m）的实验对象喝掉二进制表示中第 xxx 位为 111 的水。

最终这 mmm 个实验对象会对应一个结果序列：如果编号 x1x_1x 
1
​
  的实验对象没有反应，说明有问题的水的二进制表示中第 x1x_1x 
1
​
  位为 000，如果编号为 x2x_2x 
2
​
  的实验对象有反应，则说明有问题的水的二进制表示中第 x2x_2x 
2
​
  为 111。即根据最终每个实验对象的状态，我们可以完整地反推回有问题的水的编号是多少。

当 k>1k > 1k>1 时，相当于在原问题基础上，多考虑一层「轮数」维度，即不仅考虑某个实验对象是否有所反应，还需要考虑是在哪一轮有所反应。

我们还是使用「进制表示」的方式来最大化每个单位所能承载的最大信息量。

具体的，我们先使用 k+1k + 1k+1 进制对所有水进行编号，此时每桶水都有唯一的进制表示编码。然后我们考虑「什么时候」将水喂给「哪个实验对象」。

其中一种可行的测试方式是：设定需要的实验对象数量 mmm 为 k+1k + 1k+1 进制数的长度，若某桶水的 k+1k + 1k+1 进制中的第 xxx 位为 iii（0<=i<=k0 <= i <= k0<=i<=k），则代表将该水在第 iii 轮喂给编号为 xxx 的实验对象。

同理，利用最终的结果矩阵，我们可以反推回是哪一桶水是有问题的。

上述做法，只是阐述了我们存在这样的可行解，需要证明这样的做法是最优解。

利用 香农熵，我们可以计算明确熵值，公式为：

H(X)=−∑xP(x)log⁡2[P(x)]H(X) = - \sum_{x}^{} P(x) \log_2[P(x)]
H(X)=− 
x
∑
​
 P(x)log 
2
​
 [P(x)]
其中 P(x)P(x)P(x) 代表随机事件 xxx 的发生概率。

对于本题，记随机事件 AAA 为 nnn 桶水中哪一个桶有问题，概率为 1n\frac{1}{n} 
n
1
​
 。

记随机事件 BBB 为在测试轮数为 kkk 时，所有实验对象的最终状态，每个实验对象的状态共有 k+1k + 1k+1 种，即共有 C=(k+1)mC = (k + 1)^mC=(k+1) 
m
  种最终结果，可近似看做等概率 1C\frac{1}{C} 
C
1
​
 。

我们需要求得在满足 H(A)<=H(B)H(A) <= H(B)H(A)<=H(B) 前提下的最小 mmm 值。

代入公式可得：

−(log⁡21n)<=−∑result=0(k+1)m1(k+1)mlog⁡21(k+1)m=mlog⁡2(k+1)-(\log_2{\frac{1}{n}}) <= - \sum_{result = 0}^{(k + 1)^m} \frac{1}{(k + 1)^m} \log_2{\frac{1}{(k + 1)^m}} = m \log_2(k + 1)
−(log 
2
​
  
n
1
​
 )<=− 
result=0
∑
(k+1) 
m
 
​
  
(k+1) 
m
 
1
​
 log 
2
​
  
(k+1) 
m
 
1
​
 =mlog 
2
​
 (k+1)
移项化简得：

log⁡2nlog⁡2(k+1)<=m\frac{\log_2{n}}{\log_2{(k + 1)}} <= m
log 
2
​
 (k+1)
log 
2
​
 n
​
 <=m
代码：

Java
class Solution {
    public int poorPigs(int n, int d, int t) {
        int k = t / d;
        return (int) Math.ceil(Math.log(n) / Math.log(k + 1));
    }
}
时间复杂度：O(1)O(1)O(1)
空间复杂度：O(1)O(1)O(1)


作者：宫水三叶
链接：https://leetcode.cn/problems/poor-pigs/description/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。