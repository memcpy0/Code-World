//Àý2.3
#include <bits/stdc++.h>
using namespace std;
 
int Fib(int n) {
	if (n == 1 || n == 2)
		return 1;
	else
		return Fib(n - 1) + Fib(n - 2);
} 

int main() {
	int n = 5;
	printf("f=%d\n", Fib(n));
    return 0;
}

