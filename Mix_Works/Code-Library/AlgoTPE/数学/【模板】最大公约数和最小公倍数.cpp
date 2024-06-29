#include <bits/stdc++.h>
using namespace std;

inline long long gcd(int a, int b) {
	return !b ? a : gcd(b, a % b);
}

int gcd(int a, int b) {
	int c;
	while (b) {
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}
int lcm(int a, int b) {
	return a / gcd(a, b) * b;
} 
int main() { 

    return 0;
}

