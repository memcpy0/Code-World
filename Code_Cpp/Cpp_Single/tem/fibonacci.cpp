#include <cstdio>
#include <cstring>
using namespace std;
const int MOD = 10000;
const int MAXN = 2;		//根据题目要求定义矩阵的阶
struct Matrix {
	int m[MAXN][MAXN];
	Matrix() {
		memset(m, 0, sizeof(m));
	}
    Matrix(int a1, int a2, int b1, int b2) { 
        m[0][0] = a1, m[0][1] = a2, m[1][0] = b1, m[1][1] = b2;
    }
};
Matrix Multi(const Matrix &a, const Matrix &b) { //矩阵乘法,准确来说是方阵乘法
	Matrix res;
	for (int i = 0; i < MAXN; ++i)
		for (int j = 0; j < MAXN; ++j)
			for (int k = 0; k < MAXN; ++k)
				res.m[i][j] = (res.m[i][j] + a.m[i][k] * b.m[k][j]) % MOD; 
}
Matrix fastPow(const Matrix &a, int n) { //方阵快速幂
	Matrix res;
	for (int i = 0; i < MAXN; ++i) //初始化为单位矩阵,相当于int res = 1;
		res.m[i][i] = 1;
	while (n) {
		if (n & 1)
			res = Multi(res, a);
		a = Multi(a, a);
		n >>= 1;
	}
	return res;
}
int main() {
	while (~scanf("%d", &n)) {
		if (n == -1) break;
		if (n <= 1) { printf("%d\n", n); continue; }
		Matrix E(1, 1, 1, 0), ans(1, 0, 0, 1);
		int x = n;
		printf("%d\n", ans[0][1] % MOD);
	}
	return 0;
}