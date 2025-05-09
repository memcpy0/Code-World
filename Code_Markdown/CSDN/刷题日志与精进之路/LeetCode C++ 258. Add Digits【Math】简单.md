Given a non-negative integer `num` , repeatedly add all its digits until the result has only one digit.

Example:
```cpp
Input: 38
Output: 2 
Explanation: The process is like: 3 + 8 = 11, 1 + 1 = 2. 
             Since 2 has only one digit, return it.
```

**Follow up:** Could you do it without any loop/recursion in $O(1)$ runtime?

题意：给出一个非负整数 `num` ，将各位上的数字相加，相加的结果作为新的 `num` 。反复这一过程，直到结果为一位数。

---
思路1：直接模拟题意。

代码：效率不高。
```cpp
class Solution {
public:
    int addDigits(int num) {
        if (num < 10) return num;
        while (num >= 10) {
            int t = 0;
            while (num) {
                t += num % 10;
                num /= 10;
            }
            num = t;
        }
        return num;
    }
};
```
---
思路2：**数根**是自然数的一种性质，换句话说，每个自然数都有一个**数根**。 一个自然数在给出的数制下，得到的 `digital root/repeated digital sum` 是个位数，这将经过反复的 `summing digits` 过程，每次迭代的结果作为下次迭代计算的起始，直到得到个位数。
用途
数根可以计算模运算的同余，对于非常大的数字的情况下可以节省很多时间。

数字根可作为一种检验计算正确性的方法。例如，两数字的和的数根等于两数字分别的数根的和。

另外，数根也可以用来判断数字的整除性，如果数根能被3或9整除，则原来的数也能被3或9整除。 

我们正式定义这个问题如下：
1. $n$ 为给定的自然数，对于任意进制 $b > 1$ ，数位和 $\displaystyle F_{b} :\mathbb {N} \rightarrow \mathbb {N}$ 定义如下：$$\displaystyle F_{b}(n) = \sum^{k-1}_{i=0}d_i$$
	
	这里，$k = \lfloor \log_b n\rfloor + 1$ 是数 $n$ 在给定数制 $b$ 下的位数。同时 $n$ 每一位上的数定义如下：$$d_i = \frac {n\ {\bmod \ {b^{i+1}}} - n\ {\bmod \ {b}}^{i}} {b^{i}}$$

	一个自然数 $n$ 是 `digital root` 当且仅当它是 $\displaystyle F_b$ 的一个不动点 `fixed point` ，这意味着 $\displaystyle F_b(n) = n$ 。

	所有的自然数 $n$ 都是 $\displaystyle F_b$ 的 `preperiodic points` 。这是因为如果 $n\ge b$ ，就有：
	$$n = \sum^{k-1}_{i=0} d_ib^i$$ 

	而且因为 $b > 1$ ：$$\displaystyle F_b(n) = \sum^{k-1}_{i=0} d_i < \sum^{k-1}_{i=0} d_ib^i = n$$

	如果 $n < b$ ，显然：$$\displaystyle F_b(n)= n$$
	所以， `digital roots` 是范围为 $0\le n \lt b$ 的自然数 ，对于这些不动点不再需要迭代。

2. 示例：在 `base = 12` 时，对于 $n = 3110$ ：
	$$\begin{aligned}
	&{\displaystyle d_{0}={\frac {3110{\bmod {12^{0+1}}}-3110{\bmod {1}}2^{0}}{12^{0}}}={\frac {3110{\bmod {12}}-3110{\bmod {1}}}{1}}={\frac {2-0}{1}}={\frac {2}{1}}=2} \\
	&{\displaystyle d_{1}={\frac {3110{\bmod {12^{1+1}}}-3110{\bmod {1}}2^{1}}{12^{1}}}={\frac {3110{\bmod {144}}-3110{\bmod {1}}2}{12}}={\frac {86-2}{12}}={\frac {84}{12}}=7} \\
	&{\displaystyle d_{2}={\frac {3110{\bmod {12^{2+1}}}-3110{\bmod {1}}2^{2}}{12^{2}}}={\frac {3110{\bmod {1728}}-3110{\bmod {1}}44}{144}}={\frac {1382-86}{144}}={\frac {1296}{144}}=9} \\
	&{\displaystyle d_{3}={\frac {3110{\bmod {12^{3+1}}}-3110{\bmod {1}}2^{3}}{12^{3}}}={\frac {3110{\bmod {20736}}-3110{\bmod {1}}728}{1728}}={\frac {3110-1382}{1728}}={\frac {1728}{1728}}=1}\\
	&{\displaystyle F_{12}(3110) =\sum _{i=0}^{4-1}d_{i}=2+7+9+1=19}
	\end{aligned} 
	$$
	这个过程表明 $3110$ 在十二进制下是 $1972_{12}$ ，对于 $\displaystyle F_{12}(3110)=19$ 有：
	$$\begin{aligned}
	&{\displaystyle d_{0}={\frac {19{\bmod {12^{0+1}}}-19{\bmod {1}}2^{0}}{12^{0}}}={\frac {19{\bmod {12}}-19{\bmod {1}}}{1}}={\frac {7-0}{1}}={\frac {7}{1}}=7}\\
	&{\displaystyle d_{1}={\frac {19{\bmod {12^{1+1}}}-19{\bmod {1}}2^{1}}{12^{1}}}={\frac {19{\bmod {144}}-19{\bmod {1}}2}{12}}={\frac {19-7}{12}}={\frac {12}{12}}=1}\\
	&{\displaystyle F_{12}(19)=\sum _{i=0}^{2-1}d_{i}=1+7=8}
	\end{aligned}
	$$
	$19$ 在十二进制下是 $17$ ，而且在十二进制下，$8$ 是个位数，即 $\displaystyle F_{12}(8)=8$ 。
3. 然后直接在 $b > 1$ 的数制下定义**数根** ${\displaystyle \operatorname {dr} _{b}:\mathbb {N} \rightarrow \mathbb {N} }$ 。公式如下：
	$${\displaystyle \operatorname {dr} _{b}(n)={\begin{cases}0&{\text{if}}\ n=0,\\b-1&{\text {if}}\ n\neq 0,\ n \equiv 0\ {\bmod \ {b-1}},\\n\ {\rm {mod}}\ (b-1)&{ \text{if}}\ n\not \equiv 0\ {\bmod \ {b-1}}\end{cases}}}$$
	或者用下面的公式：
	$${\displaystyle \operatorname {dr} _{b}(n)={\begin{cases}0&{\text{if}}\ n=0,\\1\ +\ ((n-1)\ {\rm {mod}}\ (b-1))&{\text{if}}\ n\neq 0.\end{cases}}}$$
 因此，数根的值是 $n \bmod b -1$ ，因为 $b \equiv 1\ {\rm {mod}}\ b - 1$ ，所以 $b^k \equiv 1^k \equiv 1\bmod b - 1$ ，因此无论位置如何，$n\bmod b-1$ 的值都和 $ab^2 \equiv ab \equiv a \bmod b-1$ 相同，这就是为什么可以有意义地添加数字的原因。 具体来说，对于三位数的数字 $n = a_1b^2 + a_2b^1 + a_3b^0$ ：$$\displaystyle \operatorname {dr}_{b}(n) \equiv a_1b^2 + a_2b^1 + a_3b^0 \equiv a_1(1) + a2(1)+a3(1) =equiv(a_1+a_a+a_3) \bmod b -1$$


第1页
数字根
给定数字基数中自然数的数字根（也是重复的数字总和）是通过以下方法获得的（单个数字）值
的迭代过程求和位数使用来自前一次迭代的结果来计算一个数字和，在每次迭代。的
过程将一直持续到达到一位数字为止。
正式定义
例
直接公式
同余公式
使用发言权功能
物产
加性持久性
编程实例
在流行文化中
也可以看看
参考文献
外部链接
设为自然数。对于基地
，我们定义数字总和
如下所示：
哪里
是base中数字的位数。
是数字的每个数字的值。如果自然数是的固定点，则自然数是数字根，如果
。
无论底数如何，所有自然数都是的周期前点。这是因为
， 然后
因此
因为
。如果
，然后琐碎地
内容
正式定义
第2页
因此，唯一可能的数字根是自然数
，除了固定点外没有其他循环
的
。
在基数12中， 8是基数10的3110的乘法数字根。
此过程表明，基数12中的3110是1972 。现在
，显示以12为底的 19是17 。由于8是一个以1为底的12位数数字
我们可以直接为基数定义数字根
通过以下方式：
基本公式为：
要么，
在基数10中，对应的序列为（OEIS中的序列A010888）。
数字根是取模的值
因为
因此
所以不管
位置，价值
是一样的
–这就是为什么可以有意义地添加数字的原因。
具体来说，对于三位数的数字
。
例
直接公式
同余公式
第3页
要获得有关其他数字的模数值，可以取加权和，其中第-位数字的权重
对应于的值
模。在以10为底的数字中，这对于2、5和10最简单，因为高数字消失了（从2和5开始）
除以10），这对应于一个熟悉的事实：十进制数相对于2、5和10的可除性可以是
由最后一位数字检查（偶数以0、2、4、6或8结尾）。
还要注意的是模量
：因为
因此
以
交替的数字和产生取模的值
。
它有助于将正整数的数字根视为相对于的最大倍数的位置。
小于
数字本身。例如，在以6为底的数字根为11的情况下，这意味着11是数字之后的第二个数字
。
同样，在基数10中，2035的数字根为1，这意味着
。如果数字产生的数字根
究竟
，则数字是的倍数
。
考虑到这一点的正整数的数字根可以通过使用被定义地板函数的 Ñ
，作为
的数字根源
基数是的数字根之和的数字根
和的数字根。
此属性可以用作一种校验和，以检查总和是否已正确执行。
的数字根源
在基础上是完全相同的数字根
和数字根源
模数
。
的数字根源
基本如下：
非零个位数的乘积的数字根
基数是由韦达广场在
基地。
的数字根源
基数是的数字根的乘积的数字根
和数字根源
。
该添加剂持续数多少次，我们必须总结其位数在其数字根到达。
例如，以10为底的2718的加性余辉为 2：首先我们发现2 + 7 + 1 + 8 = 18，然后发现1 + 8 = 9。
对数字的加法持久性没有限制。（证明：对于给定的数字，
由数字1的重复组成的数字比的数字高1。最小加法持久性0、1，...
以10为底的是：
0、10、19、199、19999999999999999999999，...（OEIS中的序列A006050）
序列中的下一个数字（最小累加持久性数5）是2×10 2×（10 22 − 1）/ 9 − 1（即1后跟
2222222222222222222222 9个）。对于任何固定的底数，数字的总和与它的对数成正比；因此，
加性持久性与迭代对数成正比。[1]
使用发言权功能
物产
加性持久性
第4页
下面的示例实现上面定义中描述的数字总和以搜索数字根和加法器
Python的持久性。
def digit_sum （x： int ，b： int ）-> int ：
总计= 0
当x > 0时：
总数=总数+（x ％ b）
x = x // b
总回报
def digital_root （x： int ，b： int ）-> int ：
看到= 设置（）
而x 没出现：
看过。加（x）
x = digit_sum（x，b）
返回x
def 加元持续性（x： int ，b： int ）-> int ：
看到= 设置（）
而x 没出现：
看过。加（x）
x = digit_sum（x，b）
返回 len （看到）-1
数字词根用于西方命理学，但某些数字被认为具有神秘学意义（例如11和22）却没有
总是完全减少到个位数。
数字根源是视觉小说冒险游戏中的重要机制 九小时，九个人，九扇门。


