#include <bits/stdc++.h>
const int maxn = 1010;
const int MINH = -10001;
int H[maxn], size;

void Insert(int x) {
    int i; //将元素x插入优先队列小顶堆 
	for (i = ++size; H[i / 2] > x; i /= 2) 
		H[i] = H[i / 2];
	H[i] = x;
}

void CreateHeap(int N) {
	size = 0;
	int t;
	H[0] = MINH; /* 设置岗哨 */
	for (int i = 0; i < N; i++) {
		scanf("%d", &t);
		Insert(t); 
	}
}
int main() {
    int n, m, k;
    scanf("%d%d", &n, &m);
	CreateHeap(n); //根据输入序列建堆 
	for (int i = 0; i < m; i++) {
		scanf("%d", &k);
		printf("%d", H[k]); //打印到根的路径 
		while (k > 1) {
			k /= 2;
			printf(" %d", H[k]); 
		}
		printf("\n");
	}
    return 0;
}
