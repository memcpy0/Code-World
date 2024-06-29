#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
	//��һ�� 
	int fastPow(int a, int n) {
		if (n == 0) return 1;
		int temp = fastPow(a, n / 2);
		if (n & 1) //����
			return temp * temp * a;
		else //ż��
			return temp * temp;
	}
	//�ݹ������
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
	//���mod�ϴ�,Ӧ�ÿ�long long
	const int mod = 200907;
	int fastPow(int a, int n) {
		if (n == 0) return 1;
		int temp = fastPow(a, n / 2) % mod;
		if (n & 1) //����
			return temp * temp % mod * a % mod;
		else	   //ż��
			return temp * temp % mod;
	}
	int fastPow(int a, int n) {
		if (n == 0) return 1; 
		if (n & 1) //����
			return fastPow(a, n - 1) * a % mod;
		else {	   //ż��
			int temp = fastPow(a, n / 2) % mod;
			return temp * temp % mod;
		}
	}
	int fastPow(int a, int n) { 
		int base = a;
		int res = 1;
		while (n) {
			if (n & 1)
				res = (res * base) % mod; //����ȡģ
			base = (base * base) % mod;   //����ȡģ
			n >>= 1;
		}
		return res;
	}
/////////////////////////////////////////////////////////////////// 
    double Power(double base, int exponent) { //exponent����Ϊ���� 
        if (exponent < 0) base = 1.0 / base, exponent = -exponent;
        double ans = 1; //��� 
        while (exponent) {
            if (exponent & 1) ans *= base;
            exponent >>= 1;
            base *= base;
        }
        return ans;
    }
/////////////////////////////////////////////////////////////////// 
	const int MAXN = 2;		//������ĿҪ�������Ľ� 
	const int MOD = 1e4;    //������ĿҪ����ģ
	struct Matrix {
		int m[MAXN][MAXN];
		Matrix() {
			memset(m, 0, sizeof(m));
		}
	}
	Matrix Multi(const Matrix &a, const Matrix &b) { //����˷�
		Matrix res;
		for (int i = 0; i < MAXN; ++i)
			for (int j = 0; j < MAXN; ++j)
				for (int k = 0; k < MAXN; ++k)
					res.m[i][j] = (res.m[i][j] + a.m[i][k] * b.m[k][j]) % MOD; 
	}
	Matrix fastPow(const Matrix &a, int n) {
		Matrix res;
		for (int i = 0; i < MAXN; ++i) //��ʼ��Ϊ��λ����,�൱��int res = 1;
			res.m[i][i] = 1;
		while (n) {
			if (n & 1)
				res = Multi(res, a);
			a = Multi(a, a);
			n >>= 1;
		}
		return res;
	}
	long long mul(long long a, long long b, long long p) { //���ٳ�a * b % p 
		long long ans = 0;
		while (b) {
			if (b & 1) ans = (ans + a) % p;
			a = a * 2 % p;
			b >>= 1;
		}
		return ans;
	}
	long long mul(long long a, long long b, long long p) { //a * b % p
		a %= p, b %= p; //a,bһ������0~p֮��ʱ,�����ޱ�Ҫ
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
[HDU 1061](http://acm.hdu.edu.cn/showproblem.php?pid=1061)  �� $n^n$ ��ĩβ���֣�$n \le 10^9$
[HDU 5392](http://acm.hdu.edu.cn/showproblem.php?pid=5392) ������ȡģ��LCM
[HDU 6030](http://acm.hdu.edu.cn/showproblem.php?pid=6030) �ѵ��ƹ�ϵת��Ϊ����
[HDU 5895](http://acm.hdu.edu.cn/showproblem.php?pid=5895) ���Ѷȵľ��������
[HDU 5564](http://acm.hdu.edu.cn/showproblem.php?pid=5564) ��λDP�����������
[HDU 2243](http://acm.hdu.edu.cn/showproblem.php?pid=2243) AC�Զ��������������
[HDU 3117](http://acm.hdu.edu.cn/showproblem.php?pid=3117) ������һ��һ������������ݵľ�����Ŀ���� `Fibonacci` ���С���� $10^9$ �� `Fibonacci` ������Ϊֱ�ӵ����޷���ɣ��������þ����ʾ `Fibonacci` ���еĵ��ƹ�ϵ��Ȼ��ת��Ϊ���������� $10^9$ ���ݣ�
[POJ 3070](http://poj.org/problem?id=3070)
*/ 
