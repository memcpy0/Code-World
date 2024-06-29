**Description**
In the Fibonacci integer sequence, $F_0 = 0, F_1 = 1, and\ F_n = F_{n − 1} + F_{n − 2}\ \ for\ n ≥ 2$. For example, the first ten terms of the Fibonacci sequence are:
```cpp
0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...
```
An alternative formula for the Fibonacci sequence is
![在这里插入图片描述](https://img-blog.csdnimg.cn/2af9461ce39449eebe336be21fce8fbd.png)
Given an integer $n$ , your goal is to compute the last 4 digits of $F_n$ .

**Input**
The input test file will contain multiple test cases. Each test case consists of a single line containing $n$ (where $0 ≤ n ≤ 1,000,000,000$). The end-of-file is denoted by a single line containing the number $−1$ .

**Output**
For each test case, print the last four digits of $F_n$ . If the last four digits of $F_n$ are all zeros, print ‘0’; otherwise, omit any leading zeros (i.e., print $F_n$ mod 10000).

**Sample Input**
```cpp
0
9
999999999
1000000000
-1
```
**Sample Output**
```cpp
0
34
626
6875
```
**Hint**
As a reminder, matrix multiplication is associative, and the product of two 2 × 2 matrices is given by
![在这里插入图片描述](https://img-blog.csdnimg.cn/4a009bd8397042809c36620e395f90ee.png)

Also, note that raising any 2 × 2 matrix to the 0th power gives the identity matrix:![在这里插入图片描述](https://img-blog.csdnimg.cn/51aaf977ed644306bf6a6e225e43c149.png)
**Source**
[Stanford Local 2006](http://poj.org/searchproblem?field=source&key=Stanford+Local+2006)

题意：求出斐波那契数列的第 $n$ 项模 $10000$ 后的值。

---

### 解法 矩阵快速幂

```cpp
#include <cstdio>
#include <cstring>
using namespace std;
const int MAXN = 2;
const int MOD = 10000;
struct Matrix {
	int m[MAXN][MAXN];
	Matrix() { memset(m, 0, sizeof(m)); }
	Matrix(int a1, int a2, int b1, int b2) {
		m[0][0] = a1, m[0][1] = a2, m[1][0] = b1, m[1][1] = b2;
	}
};
Matrix Multi(const Matrix& a, const Matrix& b) {
	Matrix ans;
	for (int i = 0; i < MAXN; ++i)
		for (int j = 0; j < MAXN; ++j)
			for (int k = 0; k < MAXN; ++k)
				ans.m[i][j] = (ans.m[i][j] + a.m[i][k] * b.m[k][j]) % MOD;
	return ans;
}
Matrix FastPow(Matrix a, int n) {
	Matrix ans(1, 0, 0, 1);
	while (n) {
		if (n & 1) ans = Multi(a, ans);
		a = Multi(a, a);
		n >>= 1;
	}
	return ans;
}
int n;
int main() {
	while (~scanf("%d", &n)) {
		if (n == -1) break;
		if (n <= 1) { printf("%d\n", n); continue; }
		Matrix tmp = FastPow(Matrix(1, 1, 1, 0), n);
		printf("%d\n", tmp.m[0][1] % MOD);
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/eb2b312321b04dcc8f096cf9ef298db5.png)

