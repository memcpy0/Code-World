#include <iostream>
using namespace std;
int n, m; //�ᱬջ 
int solve(int n, int m) {
	if (n == 1) return n;
	if (m == 1) return n;
	return (solve(n - 1, m) + m - 1) % n + 1;
}
int main() {
	cin >> n >> m;
	cout << solve(n, m);
	return 0;
}
