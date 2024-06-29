> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Given two integers representing the <code>numerator</code> and <code>denominator</code> of a fraction, return <em>the fraction in string format</em>.</p>

<p>If the fractional part is repeating, enclose the repeating part in parentheses.</p>

<p>If multiple answers are possible, return <strong>any of them</strong>.</p>

<p>It is <strong>guaranteed</strong> that the length of the answer string is less than <code>10<sup>4</sup></code> for all the given inputs.</p>

 
<p><strong>Example 1:</strong></p>

```java
Input: numerator = 1, denominator = 2
Output: "0.5"
```
<p><strong>Example 2:</strong></p>

```java
Input: numerator = 2, denominator = 1
Output: "2"
```
<p><strong>Example 3:</strong></p>

```java
Input: numerator = 2, denominator = 3
Output: "0.(6)"
```
<p><strong>Example 4:</strong></p>

```java
Input: numerator = 4, denominator = 333
Output: "0.(012)"
```
<p><strong>Example 5:</strong></p>

```java
Input: numerator = 1, denominator = 5
Output: "0.2"
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>-2<sup>31</sup> &lt;=&nbsp;numerator, denominator &lt;= 2<sup>31</sup> - 1</code></li>
	<li><code>denominator != 0</code></li>
</ul>

题意：给定两个整数，分别表示分数的分子 `numerator` 和分母 `denominator` ，以 **字符串形式返回小数** 。如果小数部分为循环小数，则将循环的部分括在括号内。如果存在多个答案，只需返回 **任意一个** 。对于所有给定的输入，**保证** 答案字符串的长度小于 $10^4$ 。

 
---
### 解法 哈希表+字符串
题目给的测试用例还太少，这里给出更多的测试用例：
```cpp
// 整数和非负小数
//// 整数
2 1
-4 2
4 -2
-2147483648 -1

//// 有限小数
////// 分子绝对值小于分母绝对值
1 2
1 5
////// 分子绝对值大于分母绝对值
5 2
-9 -4

//// 无限循环小数
////// 分子绝对值小于分母绝对值
2 3
4 333
1 214748364
////// 分子绝对值大于分母绝对值
-21474832 -337
//---------------------------
// 负小数
//// 分子为负数

////// 有限小数
//////// 分子绝对值小于分母绝对值
-1 2
-1 5
//////// 分子绝对值大于分母绝对值
-5 2
-9 4

////// 无限循环小数
//////// 分子绝对值小于分母绝对值
-2 3
-4 333
-1 214748364
//////// 分子绝对值大于分母绝对值
-21474832 337

// 分母为负数
...
```
这一题应用到的数学知识很简单，就是**竖式除法**（见[维基百科Long division](https://en.wikipedia.org/wiki/Long_division)）。在所有除法问题中，**被除数** `dividend` 被**除数** `divisor` 除 `divide` ，产生的整数结果叫做**商** `quotient` ，剩下无法被除尽的部分叫做**余数** `remainder` 。

不过，除法的意义不仅在于此。更重要的是，**两个数相除的结果，要么是「有限小数」、要么是「无限循环小数」**，即是**有理数**（其定义即**两整数之比**，分子为 `numerator` ，分母为 `denominator` ），而不可能是「**无限不循环小数**」即**无理数**。

然后，回忆一下如何手动计算两数相除。不难发现，结果的整数部分为 $\big\lfloor \dfrac{\textrm{dividend}}{\textrm{divisor}} \big\rfloor$ ，即被除数和除数的商；小数部分则麻烦许多——其开始于整数部分的余数，我们将它作为新的被除数，然后不断对当前的余数（即被除数）补零，再重新计算余数（即被除数）和除数的新余数，其商作为小数位的结果……如此往复，递归进行。
<img src="https://img-blog.csdnimg.cn/9116733e39eb447fa47e0f8b35894eb8.png#pic_center" width="25%">

由于一直对余数补零，往后的过程就完全取决于当前余数是多少：
- **一旦余数开始重复，则被除的结果（商和余数）也会开始重复**，说明该小数是「**无限循环小数**」。
- 如果某次除法操作后余数为零，则说明该小数是「**有限小数**」。

此外，题意要求我们给出「无限循环小数」的「**循环节**」，也提醒我们——可以用哈希表记录**某个余数最早出现在哪个位置**。一旦出现相同余数，则「出现位置」和「当前位置」之间的数字，就是该小数的循环部分。

有些细节不能忽略：
- 如果除数能整除被除数，则结果为整数，直接返回即可；
- 如果两个数有一个是负数，则结果为负数，要在答案头部先添加一个负号 `-` ;
- 开始计算前，注意将两个数都转为非负整数，方便计算；
- 两个数的范围为 `int` ，但计算结果可能溢出，比如分子为 $-2147483648$ 、分母为 $-1$ ，结果为 $2^{31}$ ，超出32位有符号整型的范围。因此，最好在方法的开始位置，将两个入参转为 `long long` 类型。

讲到这里，可能还是有疑问——为什么我们敢肯定「余数一定会重复或为零」？或者说「为什么结果不是无限不循环小数」？显而易见，**余数的取值范围是有限集合**  $[0, \textrm{divisor})$ ，所以一直往下计算，根据「**抽屉原理**」，最终结果要么是出现相同余数，要么是余数为零，除法运算结束。

代码实现如下。**时间和空间复杂度都取决于最终答案的长度**，即整体复杂度为 $O(M)$ ，$M$ 为题目规定的最大长度。
```cpp
//C++ version
class Solution {
public:
    using ll = long long;
    string fractionToDecimal(int numerator , int denominator) {
        ll n = numerator, d = denominator, idx = 0; // 防止溢出
        if (n % d == 0) return to_string(n / d); // 如果能够整除,直接返回计算结果,可能为负数
        string ans, tmp;
        if (n * d < 0) ans.push_back('-'); // 如果其一为负数,先追加负号
        n = abs(n), d = abs(d); // 都化为非负整数,方便计算
        ans += to_string(n / d) + "."; // 计算整数部分,并将余数赋值给n
        n %= d; // 此时0<n<d
        unordered_map<ll, ll> rec;
        do { 
            rec[n] = idx++; // 记录当前余数对应的(小数点后第几位)位置,可能从该位开始循环
            n *= 10; // 在余数后补零
            tmp.push_back('0' + n / d); // n/d可能为零或其他数位
            n %= d;
        } while (n && rec.find(n) == rec.end()); // 当余数n不等于0、且n未出现过时,继续循环
        if (n) { // 如果当前余数之前出现过
            int pos = rec[n];
            for (int i = 0, n = tmp.size(); i < n; ++i) {
                if (i == pos) ans.push_back('(');
                ans.push_back(tmp[i]);
            }
            ans.push_back(')');
        } else ans += tmp;
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.2 MB, 在所有 C++ 提交中击败了66.76% 的用户
```
