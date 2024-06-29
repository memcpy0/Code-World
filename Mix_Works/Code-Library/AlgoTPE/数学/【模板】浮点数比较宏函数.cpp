//计算机中的浮点数都有精度限制，并不总是精确的，甚至经过大量计算后也可能出现精度损失。**这就会给比较运算带来极大困扰**（尤其是在计算几何的题目中）。为此，我们总是需要引入一个极小的误差数 `eps` 来进行修正，经验表明 `eps` 取 $10^{-8}$ 是一个比较合适的数字。
//
//---
//# 1. 等于
//如下图，如果一个数 `a` 落在了 `[b - eps, b + eps]` 这一区间内，我们就应当判断 `a == b` 成立：
//```cpp
//									  |		 a		|
//								______|______|______|______
//									b-eps    b	  b+eps
//```
//宏代码如下，将 `a` 和 `b` 相减，如果差的绝对值小于 `eps` 则返回 `true` ，否则返回 `false` 。有了等于，使用不等于就很简单了——调用 `!equ(a, b)` 即可。这样我们就可以在程序中对浮点数判等了：
//```cpp
//const double eps = 1e-8;
//#define equ(a, b) ((fabs((a) - (b))) <= (eps))
//```
//示例代码如下：
//```cpp
//#include <stdio.h>
//#include <math.h> 
//const double eps = 1e-8;
//#define equ(a, b) ((fabs((a) - (b))) <= (eps))
//int main() {
//	double a = 1.2223;
//	printf(equ(a, 1.2223) ? "true" : "false");
//	return 0;
//}
//```
//结果为 `true` 。
//
//# 2. 小于
//如下图，如果一个数 `a` 在 `eps` 的扰动范围之外小于 `b` ，就应当判断 `a < b` 。因此，只有小于 `b - eps` 的数才能判断为小于 `b` 。
//```cpp
//								    a |		  		|
//								______|______|______|______
//									b-eps    b	  b+eps
//```
//宏代码如下，将 `a` 和 `b` 相减，如果差小于 `-eps` 则有 `a < b - eps` ，于是返回 `true` ，否则返回 `false` 。
//```cpp
//#define less(a, b) (((a) - (b)) < (-eps))
//```
//示例代码如下：
//```cpp
//#include <stdio.h>
//#include <math.h> 
//const double eps = 1e-8; 
//#define less(a, b) (((a) - (b)) < (-eps))
//int main() {
//	double a = 1.2223;
//	printf(less(a, 1.2233) ? "true" : "false");
//	return 0;
//}
//```
//结果为 `true` 。
//
//# 3. 小于等于
//如下图，小于等于可以理解为小于和等于的结合，因此只要小于等于 `b + eps` 的数，都可以判断为小于等于 `b` 。
//```cpp
//								    a |		 a 		|
//								______|______|______|______
//									b-eps    b	  b+eps
//```
//宏代码如下，将 `a` 和 `b` 相减，如果差小于等于 `eps` 则有 `a <= b + eps` ，于是返回 `true` ，否则返回 `false` 。
//```cpp
//#define lessEqu(a, b) (((a) - (b)) <= (eps))
//```
//
//# 4. 大于
//如下图，如果一个数 `a` 在 `eps` 的扰动范围之外大于 `b` ，就应当判断 `a > b` 。因此，只有大于 `b + eps` 的数才能判断为大于 `b` 。
//```cpp
//								      |		  		| a
//								______|______|______|______
//									b-eps    b	  b+eps
//```
//宏代码如下，将 `a` 和 `b` 相减，如果差大于 `eps` 则有 `a > b + eps` ，于是返回 `true` ，否则返回 `false` 。
//```cpp
//#define greater(a, b) (((a) - (b)) > (eps))
//```
//示例代码如下：
//```cpp
//#include <stdio.h>
//#include <math.h> 
//const double eps = 1e-8; 
//#define greater(a, b) (((a) - (b)) > (eps))
//int main() {
//	double a = 1.2223;
//	printf(greater(a, 1.2221) ? "true" : "false");
//	return 0;
//}
//```
//结果为 `true` 。
//
//# 5. 大于等于
//如下图，大于等于可以理解为大于和等于的结合，因此只要大于等于 `b - eps` 的数，都可以判断为大于等于 `b` 。
//```cpp
//								      |		 a 		| a
//								______|______|______|______
//									b-eps    b	  b+eps
//```
//宏代码如下，将 `a` 和 `b` 相减，如果差大于等于 `-eps` 则有 `a >= b - eps` ，于是返回 `true` ，否则返回 `false` 。
//```cpp
//#define greaterEqu(a, b) (((a) - (b)) >= (-eps))
//```
//
//
//# 6. 用小于实现其他运算符
//```cpp
//const double eps = 1e-8;
//#define equ(a, b) ((fabs((a) - (b))) <= (eps)) 
//#define less(a, b) (((a) - (b)) < (-eps)) 
//#define lessEqu(a, b) (((a) - (b)) <= (eps)) 
//#define greater(a, b) (((a) - (b)) > (eps)) 
//#define greaterEqu(a, b) (((a) - (b)) >= (-eps))
//```
//其实我们不必推导和列出上面全部代码，只需要记住并复用 `less(a, b)` 就可以实现其他所有运算符：
//```cpp
//const double eps = 1e-8;
//#define less(a, b) (((a) - (b)) < (-eps)) 
//#define equ(a, b) (!less((a), (b)) && !less((b), (a)))
//#define lessEqu(a, b) (less((a), (b)) || equ((a), (b)))
//#define greater(a, b) (!lessEqu((a), (b)))
//#define greaterEqu(a, b) (!less((a), (b)))
//```
