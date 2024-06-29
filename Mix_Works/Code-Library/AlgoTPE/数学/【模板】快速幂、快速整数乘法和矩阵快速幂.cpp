#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
	//好一点 
	int fastPow(int a, int n) {
		if (n == 0) return 1;
		int temp = fastPow(a, n / 2);
		if (n & 1) //奇数
			return temp * temp * a;
		else //偶数
			return temp * temp;
	}
	//递归快速幂
	int qpow(int a, int n) {
	    if (n == 0)
	        return 1;
	    else if (n % 2 == 1)
	        return qpow(a, n - 1) * a;
	    else {
	        int temp = qpow(a, n / 2);
	        return temp * temp;
	    }
	}
/////////////////////////////////////////////////////////////////// 
	//如果mod较大,应该开long long
	const int mod = 200907;
	int fastPow(int a, int n) {
		if (n == 0) return 1;
		int temp = fastPow(a, n / 2) % mod;
		if (n & 1) //奇数
			return temp * temp % mod * a % mod;
		else	   //偶数
			return temp * temp % mod;
	}
	int fastPow(int a, int n) {
		if (n == 0) return 1; 
		if (n & 1) //奇数
			return fastPow(a, n - 1) * a % mod;
		else {	   //偶数
			int temp = fastPow(a, n / 2) % mod;
			return temp * temp % mod;
		}
	}
	int fastPow(int a, int n) { 
		int base = a;
		int res = 1;
		while (n) {
			if (n & 1)
				res = (res * base) % mod; //步步取模
			base = (base * base) % mod;   //步步取模
			n >>= 1;
		}
		return res;
	}
/////////////////////////////////////////////////////////////////// 
    double Power(double base, int exponent) { //exponent可能为负数 
        if (exponent < 0) base = 1.0 / base, exponent = -exponent;
        double ans = 1; //结果 
        while (exponent) {
            if (exponent & 1) ans *= base;
            exponent >>= 1;
            base *= base;
        }
        return ans;
    }
/////////////////////////////////////////////////////////////////// 
	const int MAXN = 2;		//根据题目要求定义矩阵的阶 
	const int MOD = 1e4;    //根据题目要求定义模
	struct Matrix {
		int m[MAXN][MAXN];
		Matrix() {
			memset(m, 0, sizeof(m));
		}
	}
	Matrix Multi(const Matrix &a, const Matrix &b) { //矩阵乘法
		Matrix res;
		for (int i = 0; i < MAXN; ++i)
			for (int j = 0; j < MAXN; ++j)
				for (int k = 0; k < MAXN; ++k)
					res.m[i][j] = (res.m[i][j] + a.m[i][k] * b.m[k][j]) % MOD; 
	}
	Matrix fastPow(const Matrix &a, int n) {
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
	long long mul(long long a, long long b, long long p) { //快速乘a * b % p 
		long long ans = 0;
		while (b) {
			if (b & 1) ans = (ans + a) % p;
			a = a * 2 % p;
			b >>= 1;
		}
		return ans;
	}
	long long mul(long long a, long long b, long long p) { //a * b % p
		a %= p, b %= p; //a,b一定属于0~p之间时,此行无必要
		long long c = (long double)a * b / p;
		long long ans = a * b - c * p;
		if (ans < 0) ans += p;
		else if (ans >= p) ans -= p;
		return ans;
	}
};

int main() {
	Solution s;
	cout << s.Power(2, -3);
	return 0;
}
/*
[HDU 2817](http://acm.hdu.edu.cn/showproblem.php?pid=2817)
[HDU 1061](http://acm.hdu.edu.cn/showproblem.php?pid=1061)  求 $n^n$ 的末尾数字，$n \le 10^9$
[HDU 5392](http://acm.hdu.edu.cn/showproblem.php?pid=5392) 快速幂取模、LCM
[HDU 6030](http://acm.hdu.edu.cn/showproblem.php?pid=6030) 把递推关系转换为矩阵
[HDU 5895](http://acm.hdu.edu.cn/showproblem.php?pid=5895) 有难度的矩阵快速幂
[HDU 5564](http://acm.hdu.edu.cn/showproblem.php?pid=5564) 数位DP、矩阵快速幂
[HDU 2243](http://acm.hdu.edu.cn/showproblem.php?pid=2243) AC自动机、矩阵快速幂
[HDU 3117](http://acm.hdu.edu.cn/showproblem.php?pid=3117) 和下面一题一样，矩阵快速幂的经典题目，算 `Fibonacci` 数列。求第 $10^9$ 个 `Fibonacci` 数，因为直接递推无法完成，所以先用矩阵表示 `Fibonacci` 数列的递推关系，然后转换为求这个矩阵的 $10^9$ 次幂；
[POJ 3070](http://poj.org/problem?id=3070)
*/ 
