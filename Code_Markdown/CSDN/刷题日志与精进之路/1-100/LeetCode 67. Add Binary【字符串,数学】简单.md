> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given two binary strings, return their sum (also a binary string).

The input strings are both **non-empty** and contains only characters `1` or `0` .

**Example 1:**
```swift
Input: a = "11", b = "1"
Output: "100"
```
**Example 2:**
```swift
Input: a = "1010", b = "1011"
Output: "10101"
```
**Constraints:**
- Each string consists only of `'0'` or `'1'` characters.
- `1 <= a.length, b.length <= 10^4`
- Each string is either `"0"` or doesn't contain any leading zero.

题意：给出两个二进制字符串，返回它们的和（用二进制表示）。

---
### 思路
和 `LeetCode 2. Add Two Numbers` 完全一致：
```cpp
class Solution {
public:
    string addBinary(string a, string b) {
        string ans;
        int n = a.size(), m = b.size(), i = n - 1, j = m - 1, c = 0;
        while (i >= 0 || j >= 0 || c) {
            if (i >= 0) c += a[i--] - '0';
            if (j >= 0) c += b[j--] - '0';
            ans.push_back('0' + (c & 1));
            c >>= 1;
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了98.07% 的用户
```

考虑一个最朴素的方法：先将 aaa 和 bbb 转化成十进制数，求和后再转化为二进制数。利用 Python 和 Java 自带的高精度运算，我们可以很简单地写出这个程序：

Python3
Java
class Solution:
    def addBinary(self, a, b) -> str:
        return '{0:b}'.format(int(a, 2) + int(b, 2))
如果 aaa 的位数是 nnn，bbb 的位数为 mmm，这个算法的渐进时间复杂度为 O(n+m)O(n + m)O(n+m)。但是这里非常简单的实现基于 Python 和 Java 本身的高精度功能，在其他的语言中可能并不适用，并且在 Java 中：

如果字符串超过 333333 位，不能转化为 Integer
如果字符串超过 656565 位，不能转化为 Long
如果字符串超过 500000001500000001500000001 位，不能转化为 BigInteger
因此，为了适用于长度较大的字符串计算，我们应该使用更加健壮的算法。

方法一：模拟
思路和算法

我们可以借鉴「列竖式」的方法，末尾对齐，逐位相加。在十进制的计算中「逢十进一」，二进制中我们需要「逢二进一」。

具体的，我们可以取 n=max⁡{∣a∣,∣b∣}n = \max\{ |a|, |b| \}n=max{∣a∣,∣b∣}，循环 nnn 次，从最低位开始遍历。我们使用一个变量 carry\rm carrycarry 表示上一个位置的进位，初始值为 000。记当前位置对其的两个位为 aia_ia 
i
​
  和 bib_ib 
i
​
 ，则每一位的答案为 (carry+ai+bi) mod 2({\rm carry} + a_i + b_i) \bmod{2}(carry+a 
i
​
 +b 
i
​
 )mod2，下一位的进位为 ⌊carry+ai+bi2⌋\lfloor \frac{{\rm carry} + a_i + b_i}{2} \rfloor⌊ 
2
carry+a 
i
​
 +b 
i
​
 
​
 ⌋。重复上述步骤，直到数字 aaa 和 bbb 的每一位计算完毕。最后如果 carry\rm carrycarry 的最高位不为 000，则将最高位添加到计算结果的末尾。

注意，为了让各个位置对齐，你可以先反转这个代表二进制数字的字符串，然后低下标对应低位，高下标对应高位。当然你也可以直接把 aaa 和 bbb 中短的那一个补 000 直到和长的那个一样长，然后从高位向低位遍历，对应位置的答案按照顺序存入答案字符串内，最终将答案串反转。这里的代码给出第一种的实现。

代码

Java
C++
Golang
c
class Solution {
    public String addBinary(String a, String b) {
        StringBuffer ans = new StringBuffer();

        int n = Math.max(a.length(), b.length()), carry = 0;
        for (int i = 0; i < n; ++i) {
            carry += i < a.length() ? (a.charAt(a.length() - 1 - i) - '0') : 0;
            carry += i < b.length() ? (b.charAt(b.length() - 1 - i) - '0') : 0;
            ans.append((char) (carry % 2 + '0'));
            carry /= 2;
        }

        if (carry > 0) {
            ans.append('1');
        }
        ans.reverse();

        return ans.toString();
    }
}
复杂度分析

假设 n=max⁡{∣a∣,∣b∣}n = \max\{ |a|, |b| \}n=max{∣a∣,∣b∣}。

时间复杂度：O(n)O(n)O(n)，这里的时间复杂度来源于顺序遍历 aaa 和 bbb。
空间复杂度：O(1)O(1)O(1)，除去答案所占用的空间，这里使用了常数个临时变量。
方法二：位运算
思路和算法

如果不允许使用加减乘除，则可以使用位运算替代上述运算中的一些加减乘除的操作。

如果不了解位运算，可以先了解位运算并尝试练习以下题目：

只出现一次的数字 II
只出现一次的数字 III
数组中两个数的最大异或值
重复的DNA序列
最大单词长度乘积
我们可以设计这样的算法来计算：

把 aaa 和 bbb 转换成整型数字 xxx 和 yyy，在接下来的过程中，xxx 保存结果，yyy 保存进位。
当进位不为 000 时
计算当前 xxx 和 yyy 的无进位相加结果：answer = x ^ y
计算当前 xxx 和 yyy 的进位：carry = (x & y) << 1
完成本次循环，更新 x = answer，y = carry
返回 xxx 的二进制形式
为什么这个方法是可行的呢？在第一轮计算中，answer 的最后一位是 xxx 和 yyy 相加之后的结果，carry 的倒数第二位是 xxx 和 yyy 最后一位相加的进位。接着每一轮中，由于 carry 是由 xxx 和 yyy 按位与并且左移得到的，那么最后会补零，所以在下面计算的过程中后面的数位不受影响，而每一轮都可以得到一个低 iii 位的答案和它向低 i+1i + 1i+1 位的进位，也就模拟了加法的过程。

代码

Python3
class Solution:
    def addBinary(self, a, b) -> str:
        x, y = int(a, 2), int(b, 2)
        while y:
            answer = x ^ y
            carry = (x & y) << 1
            x, y = answer, carry
        return bin(x)[2:]
复杂度分析

时间复杂度：O(∣a∣+∣b∣+X⋅max⁡(∣a∣+∣b∣))O(|a| + |b| + X \cdot \max ({|a| + |b|}))O(∣a∣+∣b∣+X⋅max(∣a∣+∣b∣))，字符串转化成数字需要的时间代价为 O(∣a∣+∣b∣)O(|a| + |b|)O(∣a∣+∣b∣)，计算的时间代价为 O(max⁡{∣a∣,∣b∣})O(\max \{ |a|, |b| \})O(max{∣a∣,∣b∣})，XXX 为位运算所需的时间，因为这里用到了高精度计算，所以位运算的时间不一定为 O(1)O(1)O(1)。
空间复杂度：这里使用了 xxx 和 yyy 来保存 aaa 和 bbb 的整数形式，如果用 Python 实现，这里用到了 Python 的高精度功能，实际的空间代价是 O(∣a∣+∣b∣)O(|a| + |b|)O(∣a∣+∣b∣)。

作者：力扣官方题解
链接：https://leetcode.cn/problems/add-binary/solutions/299667/er-jin-zhi-qiu-he-by-leetcode-solution/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。