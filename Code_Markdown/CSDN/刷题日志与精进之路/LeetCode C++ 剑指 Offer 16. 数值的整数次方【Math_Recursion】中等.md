<div class="notranslate"><p>实现函数double Power(double base, int exponent)，求base的exponent次方。不得使用库函数，同时不需要考虑大数问题。</p>

 

<p><strong>示例 1:</strong></p>

<pre><strong>输入:</strong> 2.00000, 10
<strong>输出:</strong> 1024.00000
</pre>

<p><strong>示例&nbsp;2:</strong></p>

<pre><strong>输入:</strong> 2.10000, 3
<strong>输出:</strong> 9.26100
</pre>

<p><strong>示例&nbsp;3:</strong></p>

<pre><strong>输入:</strong> 2.00000, -2
<strong>输出:</strong> 0.25000
<strong>解释:</strong> 2<sup>-2</sup> = 1/2<sup>2</sup> = 1/4 = 0.25</pre>

 

<p><strong>说明:</strong></p>

<ul>
	<li>-100.0 &lt;&nbsp;<em>x</em>&nbsp;&lt; 100.0</li>
	<li><em>n</em>&nbsp;是 32 位有符号整数，其数值范围是&nbsp;[−2<sup>31</sup>,&nbsp;2<sup>31&nbsp;</sup>− 1] 。</li>
</ul>


---
### 解法1 递归快速幂
```cpp
class Solution { 
public:
    double myPow(double x, int n) { 
        if (n == 0) return 1; //正数右移最后得到0
        if (n == 1) return x;
        if (n == -1) return 1 / x; //负数右移永远是负数
        double temp = myPow(x, n >> 1);
        if (n & 1) return temp * temp * x;
        else return temp * temp;
    }
};
```
运行结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了14.35% 的用户
```

---
### 解法2 迭代快速幂
```cpp
class Solution { 
public:
    double myPow(double x, int n) { 
        double ans = 1, base = x;
        bool flag = (n >= 0);
        //负数取反,考虑到最小负数,需要先自增,后续再多除一个x
        if (!flag) n = -(++n); //或者使用longlong
        while (n) {
            if (n & 1) ans *= base;
            base *= base;
            n >>= 1; //n必须取正
        }
        return flag ? ans : 1 / ans / x;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了13.86% 的用户
```
