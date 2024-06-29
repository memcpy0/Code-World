/*
求 a 的 b 次方对 p 取模的值。
输入格式

三个整数 a,b,p

,在同一行用空格隔开。
输出格式

输出一个整数，表示a^b mod p的值。
数据范围

0≤a,b≤109

1≤p≤109

输入样例：

3 2 7

输出样例：

2
*/
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll a, b, p;
ll fastPow(ll a, ll b, ll p) {
	ll ans = 1;
	while (b) {
		if (b & 1) ans = (ans * a) % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans % p; // 如果b==0,则a^b=a^0=1,1<=p所以要对模p
}
int main() {
	scanf("%lld%lld%lld", &a, &b, &p);
	printf("%lld", fastPow(a, b, p));
	return 0;	
}