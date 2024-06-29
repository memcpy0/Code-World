/*
假设现在有两个自然数 A 和 B，S 是 AB 的所有约数之和。

请你求出 Smod9901 的值是多少。

输入格式
在一行中输入用空格隔开的两个整数 A和 B。

输出格式
输出一个整数，代表 Smod9901 的值。

数据范围
0≤A,B≤5×10^7

输入样例：
2 3

输出样例：
15

注意: A和 B 不会同时为 0。
*/
#include <bits/stdc++.h>
using namespace std;
const int mod = 9901;
const int maxn = 5e7 + 100;
bitset<maxn> np; // is not prime
vector<int> primes;
int a, b;
void getPrimes(int a) {
	np[0] = np[1] = 1;
	int sqr = sqrt(a);
	for (int i = 2; i <= sqr; ++i) {
		if (!np[i]) // i是质数
			for (long long j = i * i; j <= a; j += i) 
				np[j] = 1;
	}
	for (int i = 2; i <= sqr; ++i) 
		if (!np[i] && a % i == 0) // i是质数
			primes.push_back(i);
	if (!np[a]) primes.push_back(a);
}
vector<pair<int, int>> pf;
void getPrimeFactors(int a) {
	int ta = a;
	for (int i = 0, n = primes.size(); i < n; ++i) {
		int tmp = primes[i], cnt = 0;
		while (ta % tmp == 0) {
			++cnt;
			ta /= tmp;
		}
		if (cnt) pf.push_back(make_pair(tmp, cnt));
		if (ta == 1) break;
	}
}
int fastPow(int p, int c) {
	int ans = 1;
	while (c) {
		if (c & 1) ans = ((long long)ans * p) % mod;
		p = ((long long)p * p) % mod;
		c >>= 1;
	}
	return ans;
}
// 求(1+pi^1+pi^2+...+pi^bci)
int quickSeqSum(int pi, int bci) {
	if (bci == 0) return 1;
	if (bci == 1) return (1 + pi) % mod;
	if (bci & 1)
		return (1 + fastPow(pi, (bci + 1) / 2)) * 
		       quickSeqSum(pi, (bci - 1) / 2) % mod;
	return (
		(1 + fastPow(pi, bci / 2)) * quickSeqSum(pi, bci / 2 - 1) % mod + fastPow(pi, bci)
	) % mod; 
}
int factorSum(int a, int b) {
	int ans = 1;
	for (int i = 0, n = pf.size(); i < n; ++i) {
		ans = (ans * quickSeqSum(pf[i].first, b * pf[i].second)) % mod;
	}
	return ans;
}
int main() {
	scanf("%d%d", &a, &b);
	if (a <= 1) {
		printf("%d", a);
		return 0;
	}
	if (b == 0) {
		printf("1");
		return 0;
	}
	getPrimes(a); // a>1,则primes非空
	getPrimeFactors(a);
	printf("%d", factorSum(a, b)); // 计算a^b的约数之和 % 9901
	return 0;    
}