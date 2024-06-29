@[toc]

---
# 1. GCD、LCM
最大公约数GCD和最小公倍数LCM是很常见的知识点，这两个知识点很容易理解，但往往会与其他知识点结合起来出题，有点棘手。

## (1) 最大公约数GCD
整数 `a, b` 的最大公约数记为 `gcd(a, b)` 。使用经典的**欧几里得算法**，用**辗转相除法**求最大公约数：
```cpp
int gcd(int a, int b) {
	return !b ? a : gcd(b, a % b);
}
int gcd(int a, int b) {
	int c;
	while (b) {
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}
```
过程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200912165052891.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

或者使用C++的内置函数 `__gcd` 求GCD：
```cpp
std::__gcd(a, b);
```
时间复杂度差不多是 $O(\log_2n)$ ，非常快。为了证明欧几里得算法的正确性，我们如下思考：
1. 设 $a > b,\ q = a / b,\ m = a \bmod b,\ d_1 = \gcd(a, b),\ d_2  = \gcd(b, m)$ ；
2. $a, b$ 都是 $d_1$ 的倍数；
3. 那么 $qb$ 也是 $d_1$ 的倍数；
4. 那么 $a - qb$ 也是 $d_1$ 的倍数；
5. 由于 $a = qb + m$ ，所以 $m$ 是 $d_1$ 的倍数；
6. 所以，$d_1$ 是 $b, m$ 的公因数；
7. 由于 $d_2 = \gcd(b, m)$ ，所以 $d_1$ 是 $d_2$ 的因数。
8. $b ,m$ 都是 $d_2$ 的倍数；
9. 那么 $qb$ 也是 $d_2$ 的倍数；
10. 那么 $qb + m$ 也是 $d_2$ 的倍数；
11. 由于 $a = qb + m$ ，所以 $a$ 是 $d_2$ 的因数；
12. 所以 $d_2$ 是 $a, b$ 的公因数；
13. 由于 $d_1 = \gcd(a, b)$ ，所以 $d_2$ 是 $d_1$ 的因数。
 14. 现在知道 $d1$ 是 $d_2$ 的因数，$d_2$ 是 $d_1$ 的因数，则有 $d_1 = d_2$ ，即 $\gcd(a, b) = \gcd(b, a \bmod b)$ 。
 15. 于是只要不断往下进行这一个操作，就能够求出两个数的最大公因数。


## (2) 最小公倍数LCM
整数 `a, b` 的最小公倍数为 `lcm(a, b)` ，模板如下：
```cpp
int lcm(int a, int b) {
	return a / gcd(a, b) * b;
}
```

---
# 2. 拓展欧几里得算法

