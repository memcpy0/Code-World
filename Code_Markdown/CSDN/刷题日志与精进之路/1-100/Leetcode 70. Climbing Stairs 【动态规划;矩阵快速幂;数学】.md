> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are climbing a stair case. It takes n steps to reach to the top.

Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

Note: Given n will be a positive integer.

**Example 1:**
```java
Input: 2
Output: 2
Explanation: There are two ways to climb to the top.
1. 1 step + 1 step
2. 2 steps
```
**Example 2:**
```java
Input: 3
Output: 3
Explanation: There are three ways to climb to the top.
1. 1 step + 1 step + 1 step
2. 1 step + 2 steps
3. 2 steps + 1 step
```
题意：爬楼梯，每次走1步或者2步，走上n阶的楼梯，有多少种走法。

---
1. 方法一：暴力。O(2^n^)的时间复杂度，n=44过不去。
```cpp
class Solution {
public:
    int stairs[50] = {0, 1, 2};
    int climbStairs(int n) {
        if (n <= 2) return n;
        else return climbStairs(n - 1) + climbStairs(n - 2); 
    }
};
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200106210255321.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

2. 方法二：记忆化搜索。memorization。因为法一存在很多重复计算，导致时间太长，因此把一些已经求出来的解存进数组，不重复求解。时间空间都是O(n)。

	执行用时 :0 ms, 在所有 C++ 提交中击败了100.00% 的用户
	内存消耗 :8.3 MB, 在所有 C++ 提交中击败了62.09%的用户
	```cpp
	class Solution {
	public:
	    int stairs[50] = {0, 1, 2};
	    int climbStairs(int n) {
	        if (n <= 2) return n;
	        else {
	            if (stairs[n - 1] == 0) stairs[n - 1] = climbStairs(n - 1);
	            if (stairs[n - 2] == 0) stairs[n - 2] = climbStairs(n - 2);
	            return stairs[n - 1] + stairs[n - 2];
	        } 
	    }
	};
	```
3. 方法三：动态规划。这个问题可以被分解为一些包含最优子结构的子问题，即它的最优解可以从其子问题的最优解来有效地构建。第 i 阶可以由以下两种方法得到：

	    在第 (i−1) 阶后向上爬 1 阶。 
	    在第 (i−2) 阶后向上爬 2 阶。

	所以到达第 i 阶的方法总数就是到第 (i−1) 阶和第 (i-2) 阶的方法数之和。令 dp[i] 表示能到达第 iii 阶的方法总数：`dp[i]=dp[i−1]+dp[i−2]`。时间空间复杂度均为O(n)。
	
	执行用时 :4 ms, 在所有 C++ 提交中击败了62.73% 的用户
	内存消耗 :8.4 MB, 在所有 C++ 提交中击败了54.70%的用户
	```cpp
	class Solution {
	public:
	    int climbStairs(int n) {
	        if (n <= 2) return n;
	        int *dp = new int[n + 1];
	        dp[1] = 1;
	        dp[2] = 2;
	        for (int i = 3; i <= n; i++) {
	            dp[i] = dp[i - 1] + dp[i - 2];
	        }
	        return dp[n];
	    }
	};
	```

4. 方法四：斐波那契数列递推。这个题实际上就是斐波拉契数列。只需使用两个变量即可推出答案要求的解。时间O(n)，空间O(1)。 `Fib(n)=Fib(n−1)+Fib(n−2) `。

	现在我们必须找出以 1 和 2 作为第一项和第二项的斐波那契数列中的第 n 个数，也就是说 Fib(1)=1 且 Fib(2)=2。
	
	执行用时 :0 ms, 在所有 C++ 提交中击败了100.00% 的用户
	内存消耗 :8.3 MB, 在所有 C++ 提交中击败了70.59%的用户
	```cpp
	class Solution {
	public:
	    int climbStairs(int n) {
	        if (n <= 2) return n;
	        int first = 1, second = 2;
	        for (int i = 3; i <= n; i++) {
	            int third = first + second;
	            first = second;
	            second = third;
	        }
	        return second;
	    }
	};
	```

5. 方法五：斐波拉契数列求解公式：
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200106212022122.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)$$F_n = 1/\sqrt{5}\left[ \left(\frac{1+\sqrt{5}}{2}\right)^{n} - \left(\frac{1-\sqrt{5}}{2}\right)^{n} \right]  $$ 
$$F_n = 1/\sqrt{5}\left( \left(\frac{1+\sqrt{5}}{2}\right)^{n+1} - \left(\frac{1-\sqrt{5}}{2}\right)^{n+1} \right)  $$ 
时间复杂度为O(log(n))，用在快速幂上面了，空间O(1)。
执行用时 :4 ms, 在所有 C++ 提交中击败了62.73% 的用户
内存消耗 :8.5 MB, 在所有 C++ 提交中击败了32.17%的用户
	```cpp
	class Solution {
	public:
	    int climbStairs(int n) {
	        double sqrt5 = sqrt(5.0);
	        double fibn = pow((1 + sqrt5) / 2, n + 1) - pow((1 - sqrt5) / 2, n + 1);
	        return (int)(fibn / sqrt5);
	    }
	};
	```

 6. 方法六：Binets方法 ：使用矩阵相乘。
 [solutions](https://leetcode-cn.com/problems/climbing-stairs/solution/pa-lou-ti-by-leetcode/)里面分享的方法。

思路和算法

我们用 f(x)f(x)f(x) 表示爬到第 xxx 级台阶的方案数，考虑最后一步可能跨了一级台阶，也可能跨了两级台阶，所以我们可以列出如下式子：

f(x)=f(x−1)+f(x−2)f(x) = f(x - 1) + f(x - 2)
f(x)=f(x−1)+f(x−2)
它意味着爬到第 xxx 级台阶的方案数是爬到第 x−1x - 1x−1 级台阶的方案数和爬到第 x−2x - 2x−2 级台阶的方案数的和。很好理解，因为每次只能爬 111 级或 222 级，所以 f(x)f(x)f(x) 只能从 f(x−1)f(x - 1)f(x−1) 和 f(x−2)f(x - 2)f(x−2) 转移过来，而这里要统计方案总数，我们就需要对这两项的贡献求和。

以上是动态规划的转移方程，下面我们来讨论边界条件。我们是从第 000 级开始爬的，所以从第 000 级爬到第 000 级我们可以看作只有一种方案，即 f(0)=1f(0) = 1f(0)=1；从第 000 级到第 111 级也只有一种方案，即爬一级，f(1)=1f(1) = 1f(1)=1。这两个作为边界条件就可以继续向后推导出第 nnn 级的正确结果。我们不妨写几项来验证一下，根据转移方程得到 f(2)=2f(2) = 2f(2)=2，f(3)=3f(3) = 3f(3)=3，f(4)=5f(4) = 5f(4)=5，……，我们把这些情况都枚举出来，发现计算的结果是正确的。

我们不难通过转移方程和边界条件给出一个时间复杂度和空间复杂度都是 O(n)O(n)O(n) 的实现，但是由于这里的 f(x)f(x)f(x) 只和 f(x−1)f(x - 1)f(x−1) 与 f(x−2)f(x - 2)f(x−2) 有关，所以我们可以用「滚动数组思想」把空间复杂度优化成 O(1)O(1)O(1)。下面的代码中给出的就是这种实现。



代码

C++
Java
TypeScript
Golang
JavaScript
C
class Solution {
public:
    int climbStairs(int n) {
        int p = 0, q = 0, r = 1;
        for (int i = 1; i <= n; ++i) {
            p = q; 
            q = r; 
            r = p + q;
        }
        return r;
    }
};
复杂度分析

时间复杂度：循环执行 nnn 次，每次花费常数的时间代价，故渐进时间复杂度为 O(n)O(n)O(n)。
空间复杂度：这里只用了常数个变量作为辅助空间，故渐进空间复杂度为 O(1)O(1)O(1)。
方法二：矩阵快速幂
思路

以上的方法适用于 nnn 比较小的情况，在 nnn 变大之后，O(n)O(n)O(n) 的时间复杂度会让这个算法看起来有些捉襟见肘。我们可以用「矩阵快速幂」的方法来优化这个过程。

首先我们可以构建这样一个递推关系：

[1110][f(n)f(n−1)]=[f(n)+f(n−1)f(n)]=[f(n+1)f(n)] \left[ \begin{matrix} 1 & 1 \\ 1 & 0 \end{matrix} \right] \left[ \begin{matrix} f(n)\\ f(n - 1) \end{matrix} \right] = \left[ \begin{matrix} f(n) + f(n - 1)\\ f(n) \end{matrix} \right] = \left[ \begin{matrix} f(n + 1)\\ f(n) \end{matrix} \right]
[ 
1
1
​
  
1
0
​
 ][ 
f(n)
f(n−1)
​
 ]=[ 
f(n)+f(n−1)
f(n)
​
 ]=[ 
f(n+1)
f(n)
​
 ]
因此：

[f(n+1)f(n)]=[1110]n[f(1)f(0)] \left[ \begin{matrix} f(n + 1)\\ f(n) \end{matrix} \right] = \left[ \begin{matrix} 1 & 1 \\ 1 & 0 \end{matrix} \right] ^n \left[ \begin{matrix} f(1)\\ f(0) \end{matrix} \right]
[ 
f(n+1)
f(n)
​
 ]=[ 
1
1
​
  
1
0
​
 ] 
n
 [ 
f(1)
f(0)
​
 ]
令：

M=[1110] M = \left[ \begin{matrix} 1 & 1 \\ 1 & 0 \end{matrix} \right]
M=[ 
1
1
​
  
1
0
​
 ]
因此我们只要能快速计算矩阵 MMM 的 nnn 次幂，就可以得到 f(n)f(n)f(n) 的值。如果直接求取 MnM^nM 
n
 ，时间复杂度是 O(n)O(n)O(n) 的，我们可以定义矩阵乘法，然后用快速幂算法来加速这里 MnM^nM 
n
  的求取。

如何想到使用矩阵快速幂？

如果一个问题可与转化为求解一个矩阵的 nnn 次方的形式，那么可以用快速幂来加速计算
如果一个递归式形如 f(n)=∑i=1maif(n−i)f(n) = \sum_{i = 1}^{m} a_i f(n - i)f(n)=∑ 
i=1
m
​
 a 
i
​
 f(n−i)，即齐次线性递推式，我们就可以把数列的递推关系转化为矩阵的递推关系，即构造出一个矩阵的 nnn 次方乘以一个列向量得到一个列向量，这个列向量中包含我们要求的 f(n)f(n)f(n)。一般情况下，形如 f(n)=∑i=1maif(n−i)f(n) = \sum_{i = 1}^{m} a_i f(n - i)f(n)=∑ 
i=1
m
​
 a 
i
​
 f(n−i) 可以构造出这样的 m×mm \times mm×m 的矩阵：
[a1a2a3⋯am100⋯0010⋯0001⋯0⋮⋮⋮⋱⋮000⋯1] \left[ \begin{matrix} a_1 & a_2 & a_3 & \cdots & a_m \\ 1 & 0 & 0 & \cdots & 0 \\ 0 & 1 & 0 & \cdots & 0 \\ 0 & 0 & 1 & \cdots & 0 \\ \vdots & \vdots & \vdots & \ddots & \vdots \\ 0 & 0 & 0 & \cdots & 1 \\ \end{matrix} \right]
⎣
⎡
​
  
a 
1
​
 
1
0
0
⋮
0
​
  
a 
2
​
 
0
1
0
⋮
0
​
  
a 
3
​
 
0
0
1
⋮
0
​
  
⋯
⋯
⋯
⋯
⋱
⋯
​
  
a 
m
​
 
0
0
0
⋮
1
​
  
⎦
⎤
​
 
那么遇到非齐次线性递推我们是不是就束手无策了呢？其实未必。有些时候我们可以把非齐次线性递推转化为齐次
线性递推，比如这样一个递推：
f(x)=(2x−6)c+f(x−1)+f(x−2)+f(x−3)f(x) = (2x-6)c + f(x - 1) + f(x - 2) + f(x - 3)
f(x)=(2x−6)c+f(x−1)+f(x−2)+f(x−3)
我们可以做这样的变换：

f(x)+xc=[f(x−1)+(x−1)c]+[f(x−2)+(x−2)c]+[f(x−3)+(x−3)c]f(x) + xc = [f(x - 1) + (x - 1)c] + [f(x - 2) + (x - 2)c] + [f(x - 3) + (x - 3)c]
f(x)+xc=[f(x−1)+(x−1)c]+[f(x−2)+(x−2)c]+[f(x−3)+(x−3)c]
令 g(x)=f(x)+xcg(x) = f(x) + xcg(x)=f(x)+xc，那么我们又得到了一个齐次线性递：

g(x)=g(x−1)+g(x−2)+g(x−3)g(x) = g(x - 1) + g(x - 2) + g(x - 3)
g(x)=g(x−1)+g(x−2)+g(x−3)
于是就可以使用矩阵快速幂求解了。当然并不是所有非齐次线性都可以化成齐次线性，我们还是要具体问题具体分析。

留两个思考题：

你能把 f(x)=2f(x−1)+3f(x−2)+4cf(x) = 2f(x - 1) + 3f(x - 2) + 4cf(x)=2f(x−1)+3f(x−2)+4c 化成齐次线性递推吗？欢迎大家在评论区留言。
如果一个非齐次线性递推可以转化成齐次线性递推，那么一般方法是什么？这个问题也欢迎大家在评论区总结。
代码

Java
C++
JavaScript
Golang
C
public class Solution {
    public int climbStairs(int n) {
        int[][] q = {{1, 1}, {1, 0}};
        int[][] res = pow(q, n);
        return res[0][0];
    }

    public int[][] pow(int[][] a, int n) {
        int[][] ret = {{1, 0}, {0, 1}};
        while (n > 0) {
            if ((n & 1) == 1) {
                ret = multiply(ret, a);
            }
            n >>= 1;
            a = multiply(a, a);
        }
        return ret;
    }

    public int[][] multiply(int[][] a, int[][] b) {
        int[][] c = new int[2][2];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j];
            }
        }
        return c;
    }
}
复杂度分析

时间复杂度：同快速幂，O(log⁡n)O(\log n)O(logn)。
空间复杂度：O(1)O(1)O(1)。
方法三：通项公式
思路

之前的方法我们已经讨论了 f(n)f(n)f(n) 是齐次线性递推，根据递推方程 f(n)=f(n−1)+f(n−2)f(n) = f(n - 1) + f(n - 2)f(n)=f(n−1)+f(n−2)，我们可以写出这样的特征方程：

x2=x+1x^2 = x + 1
x 
2
 =x+1
求得 x1=1+52x_1 = \frac{1+\sqrt{5}}{2}x 
1
​
 = 
2
1+ 
5
​
 
​
 ，x2=1−52x_2 = \frac{1-\sqrt{5}}{2}x 
2
​
 = 
2
1− 
5
​
 
​
 ，设通解为 f(n)=c1x1n+c2x2nf(n) = c_1 x_1 ^n + c_2 x_2 ^ nf(n)=c 
1
​
 x 
1
n
​
 +c 
2
​
 x 
2
n
​
 ，代入初始条件 f(1)=1f(1) = 1f(1)=1，f(2)=1f(2) = 1f(2)=1，得 c1=15c_1 = \frac{1}{\sqrt{5}}c 
1
​
 = 
5
​
 
1
​
 ，c2=−15c_2 = -\frac{1}{\sqrt{5}}c 
2
​
 =− 
5
​
 
1
​
 ，我们得到了这个递推数列的通项公式：

f(n)=15[(1+52)n−(1−52)n]f(n) = \frac{1}{\sqrt{5}}\left[ \left(\frac{1+\sqrt{5}}{2}\right)^{n} - \left(\frac{1-\sqrt{5}}{2}\right)^{n} \right]
f(n)= 
5
​
 
1
​
 [( 
2
1+ 
5
​
 
​
 ) 
n
 −( 
2
1− 
5
​
 
​
 ) 
n
 ]
接着我们就可以通过这个公式直接求第 nnn 项了。

代码

Java
C++
JavaScript
Golang
C
public class Solution {
    public int climbStairs(int n) {
        double sqrt5 = Math.sqrt(5);
        double fibn = Math.pow((1 + sqrt5) / 2, n + 1) - Math.pow((1 - sqrt5) / 2, n + 1);
        return (int) Math.round(fibn / sqrt5);
    }
}
复杂度分析

代码中使用的 pow\texttt{pow}pow 函数的时空复杂度与 CPU 支持的指令集相关，这里不深入分析。

总结
这里形成的数列正好是斐波那契数列，答案要求的 f(n)f(n)f(n) 即是斐波那契数列的第 nnn 项（下标从 000 开始）。我们来总结一下斐波那契数列第 nnn 项的求解方法：

nnn 比较小的时候，可以直接使用过递归法求解，不做任何记忆化操作，时间复杂度是 O(2n)O(2^n)O(2 
n
 )，存在很多冗余计算。
一般情况下，我们使用「记忆化搜索」或者「迭代」的方法，实现这个转移方程，时间复杂度和空间复杂度都可以做到 O(n)O(n)O(n)。
为了优化空间复杂度，我们可以不用保存 f(x−2)f(x - 2)f(x−2) 之前的项，我们只用三个变量来维护 f(x)f(x)f(x)、f(x−1)f(x - 1)f(x−1) 和 f(x−2)f(x - 2)f(x−2)，你可以理解成是把「滚动数组思想」应用在了动态规划中，也可以理解成是一种递推，这样把空间复杂度优化到了 O(1)O(1)O(1)。
随着 nnn 的不断增大 O(n)O(n)O(n) 可能已经不能满足我们的需要了，我们可以用「矩阵快速幂」的方法把算法加速到 O(log⁡n)O(\log n)O(logn)。
我们也可以把 nnn 代入斐波那契数列的通项公式计算结果，但是如果我们用浮点数计算来实现，可能会产生精度误差。

作者：力扣官方题解
链接：https://leetcode.cn/problems/climbing-stairs/solutions/286022/pa-lou-ti-by-leetcode-solution/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。