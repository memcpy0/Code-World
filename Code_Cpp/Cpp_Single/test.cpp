#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

int main() {
	int q, t, s;
	cin >> q >> t >> s;
	int carry = 0;
	int ans = 0;
	int res ;
	while (t > 0 || s > 0) {
		res = t % 10 + s % 10 + carry;
		t /= 10;
		s /= 10;
		int temp = q % 10;
		q /= 10;
		if (res >= temp) {
			carry = res / temp;
			res = res % temp;
		} else carry = 0;
		//cout <<t << " " << s << " " << endl;
		ans = ans * 10 + res;
		cout << ans << endl;
	}
	cout <<ans;
	return 0;
}
