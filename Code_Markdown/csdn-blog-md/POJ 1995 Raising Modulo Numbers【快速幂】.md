# Description
People are different. Some secretly read magazines full of interesting girls' pictures, others create an A-bomb in their cellar, others like using Windows, and some like difficult mathematical games. Latest marketing research shows, that this market segment was so far underestimated and that there is lack of such games. This kind of game was thus included into the KOKODáKH. The rules follow:

Each player chooses two numbers Ai and Bi and writes them on a slip of paper. Others cannot see the numbers. In a given moment all players show their numbers to the others. The goal is to determine the sum of all expressions AiBi from all players including oneself and determine the remainder after division by a given number M. The winner is the one who first determines the correct result. According to the players' experience it is possible to increase the difficulty by choosing higher numbers.

You should write a program that calculates the result and is able to find out who won the game.

# Input
The input consists of Z assignments. The number of them is given by the single positive integer Z appearing on the first line of input. Then the assignements follow. Each assignement begins with line containing an integer M (1 <= M <= 45000). The sum will be divided by this number. Next line contains number of players H (1 <= H <= 45000). Next exactly H lines follow. On each line, there are exactly two numbers Ai and Bi separated by space. Both numbers cannot be equal zero at the same time.

# Output
For each assingnement there is the only one line of output. On this line, there is a number, the result of expression
$$(A_1^{B_1} + A_2^{B_2} + \dots + A_H^{B^H}) \bmod M$$
# Sample Input

```java
3
16
4
2 3
3 4
4 5
5 6
36123
1
2374859 3029382
17
1
3 18132
```

# Sample Output

```java
2
13195
13
```
# Source
[CTU Open 1999](http://poj.org/searchproblem?field=source&key=CTU+Open+1999)

题意：有 $Z$ 组任务，每组任务先给出 $M, H$ ，然后是 $H$ 组 $A_i, B_i$ ，要求计算 $$A_1^{B_1} + A_2^{B_2} + \dots + A_H^{B^H}) \bmod M$$

---
### 解法 快速幂
从示例中可以看出，$A_i, B_i$ 可能很大，为了快速计算 $A_i^{B_i}$ ，需要用到快速幂。只是没有给出 $A_i, B_i$ 的数据范围，不过从提交情况来看，不会超过32位有符号整型，因此程序中都用 `long long`（用G++、C++提交均可）。
```cpp
#include <cstdio>
#include <iostream>
using namespace std;
long long fastPow(long long a, long long b, long long p) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = (ans * a) % p;
		b >>= 1;
		a = (a * a) % p;
	}
	return ans;
}
int main() {
	long long z, m, h, ai, bi;
	scanf("%lld", &z);
	while (z--) {
		scanf("%lld%lld", &m, &h);
		long long sum = 0;
		for (int i = 0; i < h; ++i) {
			scanf("%lld%lld", &ai, &bi);
			sum = (sum + fastPow(ai, bi, m)) % m;
		}
		printf("%lld\n", sum);
	}
	return 0;	
}
```
不过无语的是，写成下面这样，使用C++提交会WA，使用G++提交则AC：
![在这里插入图片描述](https://img-blog.csdnimg.cn/6a9431b8a2fe430c8f1a16a4e9e477d3.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/ec1f6c5b9a4a4f5c885e4631b98187c6.png)

