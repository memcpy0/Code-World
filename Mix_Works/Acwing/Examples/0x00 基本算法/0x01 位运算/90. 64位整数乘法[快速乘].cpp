/*
求 a 乘 b 对 p

取模的值。
输入格式

第一行输入整数a
，第二行输入整数b，第三行输入整数p

。
输出格式

输出一个整数，表示a*b mod p的值。
数据范围

1≤a,b,p≤1018

输入样例：

3
4
5

输出样例：

2
*/
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll fastMul(ll a, ll b, ll p) {
	ll ans = 0;
	while (b) {
		if (b & 1) ans = (ans + a) % p;
		a = (a + a) % p; // long long可以存储2*10^18,不行还有unsigned long long
		b >>= 1;
	}
	return ans;
}
int main() {
	ll a, b, p;
	scanf("%lld%lld%lld", &a, &b, &p);
	printf("%lld", fastMul(a, b, p));
	return 0;	
}