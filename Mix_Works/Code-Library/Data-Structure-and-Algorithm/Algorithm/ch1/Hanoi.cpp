//例1.8
#include <bits/stdc++.h>
using namespace std;
//汉诺塔程序
void Hanoi(int n, char x, char y, char z) {
    if (n == 1)
        printf("将盘片%d从%c移动到%c\n", n, x, z);
    else {
		Hanoi(n - 1, x, z, y);
		printf("将盘片%d从%c搬到%c\n", n, x, z);
		Hanoi(n - 1, y, x, z);
    }
}
int main() {
    int n = 4;
    Hanoi(n, 'X', 'Y', 'Z');
    return 0;
}
	
