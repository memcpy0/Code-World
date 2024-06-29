> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091337312.png)
给你一个 32 位的有符号整数 `x` ，返回将 `x` 中的数字部分反转后的结果。

如果反转后整数超过 32 位的有符号整数的范围 $[−2^{31},  2^{31} − 1]$ ，就返回 0。

**假设环境不允许存储 64 位整数（有符号或无符号）。**

**示例 1：**
```java
输入：x = 123
输出：321
```
**示例 2：**
```java
输入：x = -123
输出：-321
```
**示例 3：**
```java
输入：x = 120
输出：21
```
**示例 4：**
```java
输入：x = 0
输出：0
```
**提示：**
- $-2^{31} \le x \le 2^{31} - 1$
 
---
### 解法1 转换为long long
这里有点坑，首先表示 `int` 范围最大值和最小值有很多种方法：
- 比如直接 `2e31-1` 和 `-2e31` ，但是如果使用 `if (ans > 2e31 - 1 || ans < -2e31) return 0;` ，而原来的数是 `1547498789` (转变成 `9878947451`) 这种的话就会出错。我也不太清楚，可能是因为这样写是**双精度浮点型字面值**，需要`(ans > (int)2e31 - 1 || ans < (int)-2e31)` 才行。
- 可以写成 `0x7fffffff` 和 `0x80000000` 十六进制整型字面值，但比较时候都需要转换为 `int` 。
- 还可以使用 `limits.h` 中的 `INT_MAX` 和 `INT_MIN` 宏。或者直接使用 `2147483647` 和 `-2147483648` 的十进制整型字面值。
```cpp
class Solution {
public:
    int reverse(int x) {
        long long ans = 0; 
        while (x) { 
            ans = ans * 10 + x % 10;
            x /= 10;
        } 
        return ans > INT_MAX || ans < INT_MIN ? 0 : ans;
    }
};
```
运行效率如下：
```css
执行用时: 4 ms, 在所有 cpp 提交中击败了84.04% 的用户
内存消耗: 8.1 MB, 在所有 cpp 提交中击败了92.59%的用户
```
---
### 解法二 哨兵+数学
由于环境不允许存储 64 位整数（有符号或无符号），我们需要想别的方法——**设置一个哨兵**。此哨兵非彼哨兵，指的是一个值标记，**当越过这个值的时候，再继续翻转下去就会溢出**。

首先，我们可以知道原数 `x` 无法取到比 `INT_MAX, INT_MIN` 更大或小的数 ，因此不存在翻转后得到 `2147483648` 这样的数的可能。翻转后，能取到的最大的数是 `2147483641` ，原数 `x` 是 `1463847412` ；最小的数是 `-2147483641` ，原数是 `-1463847412` 。

于是，**只要当前的数 `ans > INT_MAX / 10` 或者 `ans < INT_MIN / 10` ，继续翻转下去，就必然溢出**，此时返回 `0` 即可。
```cpp
class Solution {
public:
    int reverse(int x) {
        int ans = 0; 
        while (x) {
            if (ans > 214748364 || ans < -214748364) return 0;
            ans = ans * 10 + x % 10;
            x /= 10;
        }
        return ans;
    }
};
```
运行效率如下：
```css
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.8 MB, 在所有 C++ 提交中击败了68.94% 的用户
```
方法一：数学
思路

记 rev\textit{rev}rev 为翻转后的数字，为完成翻转，我们可以重复「弹出」xxx 的末尾数字，将其「推入」rev\textit{rev}rev 的末尾，直至 xxx 为 000。

要在没有辅助栈或数组的帮助下「弹出」和「推入」数字，我们可以使用如下数学方法：

// 弹出 x 的末尾数字 digit
digit = x % 10
x /= 10

// 将数字 digit 推入 rev 末尾
rev = rev * 10 + digit
题目需要判断反转后的数字是否超过 323232 位有符号整数的范围 [−231,231−1][-2^{31},2^{31}-1][−2 
31
 ,2 
31
 −1]，例如 x=2123456789x=2123456789x=2123456789 反转后的 rev=9876543212>231−1=2147483647\textit{rev}=9876543212>2^{31}-1=2147483647rev=9876543212>2 
31
 −1=2147483647，超过了 323232 位有符号整数的范围。

因此我们需要在「推入」数字之前，判断是否满足

−231≤rev⋅10+digit≤231−1-2^{31}\le\textit{rev}\cdot10+\textit{digit}\le2^{31}-1
−2 
31
 ≤rev⋅10+digit≤2 
31
 −1
若该不等式不成立则返回 000。

但是题目要求不允许使用 646464 位整数，即运算过程中的数字必须在 323232 位有符号整数的范围内，因此我们不能直接按照上述式子计算，需要另寻他路。

考虑 x>0x>0x>0 的情况，记 INT_MAX=231−1=2147483647\textit{INT\_MAX}=2^{31}-1=2147483647INT_MAX=2 
31
 −1=2147483647，由于

INT_MAX=⌊INT_MAX10⌋⋅10+(INT_MAX mod 10)=⌊INT_MAX10⌋⋅10+7 \begin{aligned} \textit{INT\_MAX}&=\lfloor\dfrac{\textit{INT\_MAX}}{10}\rfloor\cdot10+(\textit{INT\_MAX}\bmod10)\\ &=\lfloor\dfrac{\textit{INT\_MAX}}{10}\rfloor\cdot10+7 \end{aligned}
INT_MAX
​
  
=⌊ 
10
INT_MAX
​
 ⌋⋅10+(INT_MAXmod10)
=⌊ 
10
INT_MAX
​
 ⌋⋅10+7
​
 
则不等式

rev⋅10+digit≤INT_MAX\textit{rev}\cdot10+\textit{digit}\le\textit{INT\_MAX}
rev⋅10+digit≤INT_MAX
等价于

rev⋅10+digit≤⌊INT_MAX10⌋⋅10+7\textit{rev}\cdot10+\textit{digit}\le\lfloor\dfrac{\textit{INT\_MAX}}{10}\rfloor\cdot10+7
rev⋅10+digit≤⌊ 
10
INT_MAX
​
 ⌋⋅10+7
移项得

(rev−⌊INT_MAX10⌋)⋅10≤7−digit(\textit{rev}-\lfloor\dfrac{\textit{INT\_MAX}}{10}\rfloor)\cdot10\le7-\textit{digit}
(rev−⌊ 
10
INT_MAX
​
 ⌋)⋅10≤7−digit
讨论该不等式成立的条件：

若 rev>⌊INT_MAX10⌋\textit{rev}>\lfloor\cfrac{\textit{INT\_MAX}}{10}\rfloorrev>⌊ 
10
INT_MAX
​
 ⌋，由于 digit≥0\textit{digit}\ge0digit≥0，不等式不成立。 若 rev=⌊INT_MAX10⌋\textit{rev}=\lfloor\cfrac{\textit{INT\_MAX}}{10}\rfloorrev=⌊ 
10
INT_MAX
​
 ⌋，当且仅当 digit≤7\textit{digit}\le7digit≤7 时，不等式成立。 若 rev<⌊INT_MAX10⌋\textit{rev}<\lfloor\cfrac{\textit{INT\_MAX}}{10}\rfloorrev<⌊ 
10
INT_MAX
​
 ⌋，由于 digit≤9\textit{digit}\le9digit≤9，不等式成立。

注意到当 rev=⌊INT_MAX10⌋\textit{rev}=\lfloor\cfrac{\textit{INT\_MAX}}{10}\rfloorrev=⌊ 
10
INT_MAX
​
 ⌋ 时若还能推入数字，则说明 xxx 的位数与 INT_MAX\textit{INT\_MAX}INT_MAX 的位数相同，且要推入的数字 digit\textit{digit}digit 为 xxx 的最高位。由于 xxx 不超过 INT_MAX\textit{INT\_MAX}INT_MAX，因此 digit\textit{digit}digit 不会超过 INT_MAX\textit{INT\_MAX}INT_MAX 的最高位，即 digit≤2\textit{digit}\le2digit≤2。所以实际上当 rev=⌊INT_MAX10⌋\textit{rev}=\lfloor\cfrac{\textit{INT\_MAX}}{10}\rfloorrev=⌊ 
10
INT_MAX
​
 ⌋ 时不等式必定成立。

因此判定条件可简化为：当且仅当 rev≤⌊INT_MAX10⌋\textit{rev}\le\lfloor\cfrac{\textit{INT\_MAX}}{10}\rfloorrev≤⌊ 
10
INT_MAX
​
 ⌋ 时，不等式成立。

x<0x<0x<0 的情况类似，留给读者自证，此处不再赘述。

综上所述，判断不等式

−231≤rev⋅10+digit≤231−1-2^{31}\le\textit{rev}\cdot10+\textit{digit}\le2^{31}-1
−2 
31
 ≤rev⋅10+digit≤2 
31
 −1
是否成立，可改为判断不等式

⌈−23110⌉≤rev≤⌊231−110⌋\lceil\cfrac{-2^{31}}{10}\rceil\le\textit{rev}\le\lfloor\dfrac{2^{31}-1}{10}\rfloor
⌈ 
10
−2 
31
 
​
 ⌉≤rev≤⌊ 
10
2 
31
 −1
​
 ⌋
是否成立，若不成立则返回 000。
 

作者：力扣官方题解
链接：https://leetcode.cn/problems/reverse-integer/solutions/755611/zheng-shu-fan-zhuan-by-leetcode-solution-bccn/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。