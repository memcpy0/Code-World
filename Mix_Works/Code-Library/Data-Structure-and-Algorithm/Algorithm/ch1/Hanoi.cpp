//��1.8
#include <bits/stdc++.h>
using namespace std;
//��ŵ������
void Hanoi(int n, char x, char y, char z) {
    if (n == 1)
        printf("����Ƭ%d��%c�ƶ���%c\n", n, x, z);
    else {
		Hanoi(n - 1, x, z, y);
		printf("����Ƭ%d��%c�ᵽ%c\n", n, x, z);
		Hanoi(n - 1, y, x, z);
    }
}
int main() {
    int n = 4;
    Hanoi(n, 'X', 'Y', 'Z');
    return 0;
}
	
