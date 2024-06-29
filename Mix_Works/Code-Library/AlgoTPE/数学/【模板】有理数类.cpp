#include <bits/stdc++.h>
using namespace std;
using ll = long long;
class rational { 
private:
    ll u, d;
    ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
    ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }
    void reduce(ll& u, ll& d) {
        ll g = gcd(abs(u), abs(d)); //可能有负数
        u /= g, d /= g;
    }
public:
    rational(ll a, ll b) : u(a), d(b) { reduce(u, d); }
    rational operator+(const rational& t) {
		ll m = lcm(d, t.d);
		return rational(u * (m / d) + t.u * (m / t.d), m);
	}
	rational operator-(const rational& t) {
		return *this + rational(-t.u, t.d);
	}
	rational operator*(const rational& t) {
		return rational(u * t.u, d * t.d);
	}
	rational operator/(const rational& t) {
		if (t.u == 0) return rational(1, 0); //inf,分母为0 
		if (t.u < 0) return *this * rational(-t.d, -t.u); //分母<0则翻转分子分母的符号,使得分母为正 
		return *this * rational(t.d, t.u);
	}
    friend ostream& operator<<(ostream& os, const rational& r);
};
ostream& operator<<(ostream& os, const rational& r) {
	if (r.d == 0) { os << "Inf"; return os; } //分母为0,则是除法 
	//if (r.u == 0) { os << 0; return os; } //分子为0,输出0 
	if (r.u < 0) os << "("; //负数需要( 
	if (r.d == 1) os << r.u; //分母为1,输出分子; 分子为0,化简得到分母为1,也一样输出分子
	else if (abs(r.u) > r.d) os << r.u / r.d << " " << abs(r.u) % r.d << "/" << r.d; //假分数
	else os << r.u << "/" << r.d; //真分数 
	if (r.u < 0) os << ")";	//负数需要) 
	return os;
} 
int main() {
    ll a1, b1, a2, b2;
    scanf("%lld/%lld %lld/%lld", &a1, &b1, &a2, &b2);
    rational a(a1, b1), b(a2, b2);
	cout << a << " + " << b << " = " << (a + b) << endl;
	cout << a << " - " << b << " = " << (a - b) << endl;
	cout << a << " * " << b << " = " << (a * b) << endl;
	cout << a << " / " << b << " = " << (a / b) << endl;
    return 0;
}

//分子和分母全是整型范围内的整数,负号只可能出现在分子前,分母不为0
//输出的每个有理数必须是该有理数的最简形式k a/b,其中k是整数部分,a/b是最简分数部分
//若为负数,则须加括号
//若除法分母为0,则输出Inf
//若分数的分子为0,输出0,注意对0的加减乘除
//若分母为1,输出分子
