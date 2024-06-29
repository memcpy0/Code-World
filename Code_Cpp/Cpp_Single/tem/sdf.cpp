#include <cstdio>
#include <iostream>
#include <cctype>
#include <cmath>
using namespace std; 
int n;
int main() { 
	scanf("%d", &n);
	if (n % 2 == 1) cout << "奇数";
	else cout << "偶数";
	return 0;
}