#include <bits/stdc++.h>
using namespace std;
inline int read() {
	int x = 0, sign = 1;
	char ch = getchar();
	while (ch > '9' || ch < '0') {
	    if (ch == '-') sign = -1;
	    ch = getchar();
	}
	while (ch >= '0' && ch <= '9') {
	    x = x * 10 + ch - '0';
		ch = getchar();
	}
	return x * sign;
}
inline void write(int x) {
	if (x == 0) {
	    putchar(48);
		return;
	}
	int len = 0, dg[20];
	while (x > 0) {
		dg[++len] = x % 10;
		x /= 10;
	}
	for (register int i = len; i >= 1; --i)
		putchar(dg[i] + 48);
}

int main() { 

    return 0;
}

