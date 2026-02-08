> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091402530.png)

给定两个以字符串形式表示的非负整数 `num1` 和 `num2`，返回 `num1` 和 `num2` 的乘积，它们的乘积也表示为字符串形式。

**注意：**不能使用任何内置的 BigInteger 库或直接将输入转换为整数。

**示例 1:**

**输入:** num1 = "2", num2 = "3"
**输出:** "6"

**示例 2:**

**输入:** num1 = "123", num2 = "456"
**输出:** "56088"

**提示：**

- `1 <= num1.length, num2.length <= 200`
- `num1` 和 `num2` 只能由数字组成。
- `num1` 和 `num2` 都不包含任何前导零，除了数字0本身。

这是字符串相乘，前序题目是[415. 字符串相加](https://leetcode-cn.com/problems/add-strings/)

---
方法一：做加法
如果 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  之一是 000，则直接将 000 作为结果返回即可。

如果 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  都不是 000，则可以通过模拟「竖式乘法」的方法计算乘积。从右往左遍历乘数，将乘数的每一位与被乘数相乘得到对应的结果，再将每次得到的结果累加。这道题中，被乘数是 num1\textit{num}_1num 
1
​
 ，乘数是 num2\textit{num}_2num 
2
​
 。

需要注意的是，num2\textit{num}_2num 
2
​
  除了最低位以外，其余的每一位的运算结果都需要补 000。



对每次得到的结果进行累加，可以使用「415. 字符串相加」的做法。

Java
C++
Python3
Golang
C
class Solution {
    public String multiply(String num1, String num2) {
        if (num1.equals("0") || num2.equals("0")) {
            return "0";
        }
        String ans = "0";
        int m = num1.length(), n = num2.length();
        for (int i = n - 1; i >= 0; i--) {
            StringBuffer curr = new StringBuffer();
            int add = 0;
            for (int j = n - 1; j > i; j--) {
                curr.append(0);
            }
            int y = num2.charAt(i) - '0';
            for (int j = m - 1; j >= 0; j--) {
                int x = num1.charAt(j) - '0';
                int product = x * y + add;
                curr.append(product % 10);
                add = product / 10;
            }
            if (add != 0) {
                curr.append(add % 10);
            }
            ans = addStrings(ans, curr.reverse().toString());
        }
        return ans;
    }

    public String addStrings(String num1, String num2) {
        int i = num1.length() - 1, j = num2.length() - 1, add = 0;
        StringBuffer ans = new StringBuffer();
        while (i >= 0 || j >= 0 || add != 0) {
            int x = i >= 0 ? num1.charAt(i) - '0' : 0;
            int y = j >= 0 ? num2.charAt(j) - '0' : 0;
            int result = x + y + add;
            ans.append(result % 10);
            add = result / 10;
            i--;
            j--;
        }
        ans.reverse();
        return ans.toString();
    }
}
复杂度分析

时间复杂度：O(mn+n2)O(mn+n^2)O(mn+n 
2
 )，其中 mmm 和 nnn 分别是 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  的长度。需要从右往左遍历 num2\textit{num}_2num 
2
​
 ，对于 num2\textit{num}_2num 
2
​
  的每一位，都需要和 num1\textit{num}_1num 
1
​
  的每一位计算乘积，因此计算乘积的总次数是 mnmnmn。字符串相加操作共有 nnn 次，相加的字符串长度最长为 m+nm+nm+n，因此字符串相加的时间复杂度是 O(mn+n2)O(mn+n^2)O(mn+n 
2
 )。总时间复杂度是 O(mn+n2)O(mn+n^2)O(mn+n 
2
 )。

空间复杂度：O(m+n)O(m+n)O(m+n)，其中 mmm 和 nnn 分别是 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  的长度。空间复杂度取决于存储中间状态的字符串，由于乘积的最大长度为 m+nm+nm+n，因此存储中间状态的字符串的长度不会超过 m+nm+nm+n。

方法二：做乘法
方法一的做法是从右往左遍历乘数，将乘数的每一位与被乘数相乘得到对应的结果，再将每次得到的结果累加，整个过程中涉及到较多字符串相加的操作。如果使用数组代替字符串存储结果，则可以减少对字符串的操作。

令 mmm 和 nnn 分别表示 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  的长度，并且它们均不为 000，则 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  的乘积的长度为 m+n−1m+n-1m+n−1 或 m+nm+nm+n。简单证明如下：

如果 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  都取最小值，则 num1=10m−1\textit{num}_1=10^{m-1}num 
1
​
 =10 
m−1
 ，num2=10n−1\textit{num}_2=10^{n-1}num 
2
​
 =10 
n−1
 ，num1×num2=10m+n−2\textit{num}_1 \times \textit{num}_2=10^{m+n-2}num 
1
​
 ×num 
2
​
 =10 
m+n−2
 ，乘积的长度为 m+n−1m+n-1m+n−1；

如果 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  都取最大值，则 num1=10m−1\textit{num}_1=10^m-1num 
1
​
 =10 
m
 −1，num2=10n−1\textit{num}_2=10^n-1num 
2
​
 =10 
n
 −1，num1×num2=10m+n−10m−10n+1\textit{num}_1 \times \textit{num}_2=10^{m+n}-10^m-10^n+1num 
1
​
 ×num 
2
​
 =10 
m+n
 −10 
m
 −10 
n
 +1，乘积显然小于 10m+n10^{m+n}10 
m+n
  且大于 10m+n−110^{m+n-1}10 
m+n−1
 ，因此乘积的长度为 m+nm+nm+n。

由于 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  的乘积的最大长度为 m+nm+nm+n，因此创建长度为 m+nm+nm+n 的数组 ansArr\textit{ansArr}ansArr 用于存储乘积。对于任意 0≤i<m0 \le i < m0≤i<m 和 0≤j<n0 \le j < n0≤j<n，num1[i]×num2[j]\textit{num}_1[i] \times \textit{num}_2[j]num 
1
​
 [i]×num 
2
​
 [j] 的结果位于 ansArr[i+j+1]\textit{ansArr}[i+j+1]ansArr[i+j+1]，如果 ansArr[i+j+1]≥10\textit{ansArr}[i+j+1] \ge 10ansArr[i+j+1]≥10，则将进位部分加到 ansArr[i+j]\textit{ansArr}[i+j]ansArr[i+j]。

最后，将数组 ansArr\textit{ansArr}ansArr 转成字符串，如果最高位是 000 则舍弃最高位。


1 / 13
Java
C++
Python3
Golang
C
class Solution {
    public String multiply(String num1, String num2) {
        if (num1.equals("0") || num2.equals("0")) {
            return "0";
        }
        int m = num1.length(), n = num2.length();
        int[] ansArr = new int[m + n];
        for (int i = m - 1; i >= 0; i--) {
            int x = num1.charAt(i) - '0';
            for (int j = n - 1; j >= 0; j--) {
                int y = num2.charAt(j) - '0';
                ansArr[i + j + 1] += x * y;
            }
        }
        for (int i = m + n - 1; i > 0; i--) {
            ansArr[i - 1] += ansArr[i] / 10;
            ansArr[i] %= 10;
        }
        int index = ansArr[0] == 0 ? 1 : 0;
        StringBuffer ans = new StringBuffer();
        while (index < m + n) {
            ans.append(ansArr[index]);
            index++;
        }
        return ans.toString();
    }
}
复杂度分析

时间复杂度：O(mn)O(mn)O(mn)，其中 mmm 和 nnn 分别是 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  的长度。需要计算 num1\textit{num}_1num 
1
​
  的每一位和 num2\textit{num}_2num 
2
​
  的每一位的乘积。

空间复杂度：O(m+n)O(m+n)O(m+n)，其中 mmm 和 nnn 分别是 num1\textit{num}_1num 
1
​
  和 num2\textit{num}_2num 
2
​
  的长度。需要创建一个长度为 m+nm+nm+n 的数组存储乘积。

结语
方法二还可以用另外一种方法改写。我们把两个数相乘看成是两个多项式相乘，因为任何一个数都可以表示成为

∑i=0n−1ai×10i \sum_{i = 0}^{n - 1} a_i \times 10^i
i=0
∑
n−1
​
 a 
i
​
 ×10 
i
 
的形式，也就相当于对多项式

A(x)=∑i=0n−1aixi A(x) = \sum_{i = 0} ^ {n - 1} a_i x^i
A(x)= 
i=0
∑
n−1
​
 a 
i
​
 x 
i
 
在 x=10x = 10x=10 处求值。当两个数 NaN_aN 
a
​
 、NbN_bN 
b
​
  相乘的时候，我们也可以认为这两个数是两个多项式

{A(x)=∑i=0n−1aixiB(x)=∑i=0m−1bixi \left\{ \begin{aligned} & A(x) = \sum_{i = 0} ^ {n - 1} a_i x^i \\ & B(x) = \sum_{i = 0} ^ {m - 1} b_i x^i \end{aligned} \right.
⎩
⎨
⎧
​
  
​
  
A(x)= 
i=0
∑
n−1
​
 a 
i
​
 x 
i
 
B(x)= 
i=0
∑
m−1
​
 b 
i
​
 x 
i
 
​
 
相乘的结果 C(x)=A(x)×B(x)C(x) = A(x) \times B(x)C(x)=A(x)×B(x) 在 x=10x = 10x=10 处求值。我们可以这样表示 C(x)C(x)C(x)：

C(x)=∑i=0n+m−2cixi C(x) = \sum_{i = 0}^{n + m - 2} c_i x^i
C(x)= 
i=0
∑
n+m−2
​
 c 
i
​
 x 
i
 
这里

ci=∑k=0iakbi−k c_i = \sum_{k = 0} ^ {i} a_k b_{i - k}
c 
i
​
 = 
k=0
∑
i
​
 a 
k
​
 b 
i−k
​
 
于是我们就可以顺序求解 cic_ic 
i
​
 ，每次 O(i)O(i)O(i) 地选取下标和为 iii 的一组 (ak,bi−k)(a_k, b_{i - k})(a 
k
​
 ,b 
i−k
​
 )。求到 cic_ic 
i
​
  序列之后，再处理进位即可得到答案。对比这两种做法：

顺序求解 cic_ic 
i
​
  的过程相当于集中计算 cic_ic 
i
​
 
而方法二相当于每对 (ai,bj)(a_i, b_j)(a 
i
​
 ,b 
j
​
 ) 对 ci+jc_{i + j}c 
i+j
​
  算贡献（注意这里的 aia_ia 
i
​
  并不是题目中的 num1[i]{\it num}_1[i]num 
1
​
 [i]，aia_ia 
i
​
  下标越小，代表的位权越小，而 num1[i]{\it num}_1[i]num 
1
​
 [i] 下标越小，代表的位权越大）
它们的本质是一样的，并且时间复杂度都是 O(max⁡{n,m}2)O(\max \{ n, m\} ^2)O(max{n,m} 
2
 )。我们再仔细的观察 cic_ic 
i
​
  的形式：

ci=∑k=0iakbi−k c_i = \sum_{k = 0}^{i} a_k b_{i - k}
c 
i
​
 = 
k=0
∑
i
​
 a 
k
​
 b 
i−k
​
 
它揭示了多项式乘法的另一面：cic_ic 
i
​
  序列其实是 aia_ia 
i
​
  序列和 bib_ib 
i
​
  序列的卷积，即：

c⃗=a⃗∗b⃗ \vec{c} = \vec{a} * \vec{b}
c
 = 
a
 ∗ 
b
 
至此，我们就可以用一种叫做快速傅立叶变换（Fast Fourier Transform，FFT）的方法来加速卷积的计算，使得时间复杂度降低到 O(clog⁡c)O(c \log c)O(clogc)，这里 ccc 是不小于 n+mn + mn+m 的最小的 222 的整数幂。由于这个方法并不在面试的考纲范围内，感兴趣的同学可以自行学习。

作者：力扣官方题解
链接：https://leetcode.cn/problems/multiply-strings/solutions/372098/zi-fu-chuan-xiang-cheng-by-leetcode-solution/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

方法一：普通竖式
思路

竖式运算思想，以 num1 为 123，num2 为 456 为例分析：

{:align=center}

遍历 num2 每一位与 num1 进行相乘，将每一步的结果进行累加。

注意：

num2 除了第一位的其他位与 num1 运算的结果需要 补 0
计算字符串数字累加其实就是 415. 字符串相加
实现

Java
class Solution {
    /**
    * 计算形式
    *    num1
    *  x num2
    *  ------
    *  result
    */
    public String multiply(String num1, String num2) {
        if (num1.equals("0") || num2.equals("0")) {
            return "0";
        }
        // 保存计算结果
        String res = "0";
        
        // num2 逐位与 num1 相乘
        for (int i = num2.length() - 1; i >= 0; i--) {
            int carry = 0;
            // 保存 num2 第i位数字与 num1 相乘的结果
            StringBuilder temp = new StringBuilder();
            // 补 0 
            for (int j = 0; j < num2.length() - 1 - i; j++) {
                temp.append(0);
            }
            int n2 = num2.charAt(i) - '0';
            
            // num2 的第 i 位数字 n2 与 num1 相乘
            for (int j = num1.length() - 1; j >= 0 || carry != 0; j--) {
                int n1 = j < 0 ? 0 : num1.charAt(j) - '0';
                int product = (n1 * n2 + carry) % 10;
                temp.append(product);
                carry = (n1 * n2 + carry) / 10;
            }
            // 将当前结果与新计算的结果求和作为新的结果
            res = addStrings(res, temp.reverse().toString());
        }
        return res;
    }

    /**
     * 对两个字符串数字进行相加，返回字符串形式的和
     */
    public String addStrings(String num1, String num2) {
        StringBuilder builder = new StringBuilder();
        int carry = 0;
        for (int i = num1.length() - 1, j = num2.length() - 1;
             i >= 0 || j >= 0 || carry != 0;
             i--, j--) {
            int x = i < 0 ? 0 : num1.charAt(i) - '0';
            int y = j < 0 ? 0 : num2.charAt(j) - '0';
            int sum = (x + y + carry) % 10;
            builder.append(sum);
            carry = (x + y + carry) / 10;
        }
        return builder.reverse().toString();
    }
}
复杂度分析

时间复杂度：O(M N)。M,NM,NM,N 分别为 num1 和 num2 的长度。
空间复杂度：O(M+N)。用于存储计算结果。
方法二：优化竖式
该算法是通过两数相乘时，乘数某位与被乘数某位相乘，与产生结果的位置的规律来完成。具体规律如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307060838773.png)
乘数 num1 位数为 MMM，被乘数 num2 位数为 NNN， num1 x num2 结果 res 最大总位数为 M+N
num1[i] x num2[j] 的结果为 tmp(位数为两位，"0x""0x""0x", "xy""xy""xy" 的形式)，其第一位位于 res[i+j]，第二位位于 res[i+j+1]。
结合下图更容易理解

{:align=center}

Java
class Solution {
    public String multiply(String num1, String num2) {
        if (num1.equals("0") || num2.equals("0")) {
            return "0";
        }
        int[] res = new int[num1.length() + num2.length()];
        for (int i = num1.length() - 1; i >= 0; i--) {
            int n1 = num1.charAt(i) - '0';
            for (int j = num2.length() - 1; j >= 0; j--) {
                int n2 = num2.charAt(j) - '0';
                int sum = (res[i + j + 1] + n1 * n2);
                res[i + j + 1] = sum % 10;
                res[i + j] += sum / 10;
            }
        }

        StringBuilder result = new StringBuilder();
        for (int i = 0; i < res.length; i++) {
            if (i == 0 && res[i] == 0) continue;
            result.append(res[i]);
        }
        return result.toString();
    }
}
复杂度分析

时间复杂度：O∗(∗MN∗)O*(*M N*)O∗(∗MN∗)。M,NM,NM,N 分别为 num1 和 num2 的长度。
空间复杂度：O∗(M+N)O*(M+N)O∗(M+N)。用于存储计算结果。
算法比较
算法	提交结果	执行用时	内存消耗	语言
普通竖式	通过	48 ms	43.8 MB	Java
优化竖式	通过	4 ms	36.6 MB	Java
虽然两者时间复杂度和空间复杂度相同，但优化竖式执行速度提高很明显，普通竖式耗时主要还是对每步计算的字符串相加这个过程。

作者：breezean
链接：https://leetcode.cn/problems/multiply-strings/solutions/29100/you-hua-ban-shu-shi-da-bai-994-by-breezean/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



---

# 【详细注释】快速傅里叶变换（FFT） C++ 版

该版答案实现了以下FFT的优化手段：

- [x]  使用迭代版本
- [x]  使用位逆序置换
- [x]  使用蝴蝶操作
- [x]  使用平方法以减少一次DFT操作

（由于需要使用vector开辟空间等原因，其实没有**贡献法**速度快（100%），但是思想还是很有意思的，这是需要提前告知各位观众的一点~）

```cpp
template<typename T>
vector<T> & operator *= (vector<T> & A, vector<T> const & B) {
    for(int i = 0; i < A.size(); ++i) A[i] *= B[i]; return A;
}

class Solution {
    int N; // the target FFI array length, which is the pow of 2
    vector<int> rev; // bit-reverse permutation

    void FFT(vector<complex<double>> & A, int inv) {
        for(int i = 1; i < N; ++i) if(i < rev[i]) swap(A[i], A[rev[i]]); // reorder

        for(int epoch = 1; epoch <= __builtin_ctz(N); ++epoch) { // cnt of tail zero = iter times
            const int n = 1 << epoch; // the length of sub array to merge
            complex<double> wn(cos(2 * M_PI / n), sin(2 * M_PI / n * inv));
            for(int i = 0; i < N; i += n) { // iter over sub arrays
                complex<double> w(1, 0), u, v;    // initial w for each sub array
                for(int j = 0; j < n/2; ++j)  // iter within each sub array (half)
                    u = A[i + j      ], 
                    v = A[i + j + n/2] * w,
                        A[i + j      ] = u + v, 
                        A[i + j + n/2] = u - v,
                    w *= wn; // spin the omega
            }
        }
        if(inv == -1) for(auto & e: A) e /= N; // IFFT multiplier
    }
public:
    string multiply(string num1, string num2) {
        if(num1 == "0" || num2 == "0") return "0"; // in case of final ans like "000"
        int N1 = num1.size(), N2 = num2.size(); // highest order = N1+N2-2, min string len = N1+N2-1
        N = 1; while(N < N1 + N2 - 1) N <<= 1; // N >= (N1 - 1) + (N2 - 1) + 1
        vector<complex<double>> A(N, 0); rev.resize(N);
        for(int i = 0; i < N1; ++i) A[i].real(num1[N1 - 1 - i] - '0'); // input with reverse order
        for(int i = 0; i < N2; ++i) A[i].imag(num2[N2 - 1 - i] - '0'); // input with reverse order
        for(int i = 0; i < N; ++i) rev[i] = (rev[i >> 1] >> 1) + ((i & 1) ? (N >> 1) : 0); // bit-reverse

        FFT(A, 1), A *= A, FFT(A, -1); // FFT/IFFT

        string ans(N1 + N2 - 1 , '0');
        int diff = 0, sum;
        for(int i = 0; i < N1 + N2 - 1; ++i) // stream direction: FFT --> , ans <--
            sum = diff + (A[i].imag() + 1e-8) / 2, ans[N1 + N2 - 2 - i] += sum % 10, diff = sum / 10;
        if(diff) ans.insert(0, to_string(diff));
        return ans;
    }
};
```