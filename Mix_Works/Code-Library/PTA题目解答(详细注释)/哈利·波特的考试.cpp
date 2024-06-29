#include <bits/stdc++.h>
using namespace std;
const int maxv = 110, inf = 1000000000;
int G[maxv][maxv];
int n, m;
void floyd() {
	for (int k = 1; k <= n; k++) {
		for (int i = 1; i <= n; i++) {
			if (G[i][k] != inf) {
				for (int j = 1; j <= n; j++) {
					if (G[k][j] != inf && G[i][k] + G[k][j] < G[i][j]) {
						G[i][j] = G[i][k] + G[k][j];
					}
				}
			}
		}
	} 
}

int main() {
	int u, v, w;
	scanf("%d%d", &n, &m);
	fill(G[0], G[0] + maxv * maxv, inf);
	for (int i = 1; i <= n; i++) 
		G[i][i] = 0; //自己到自己为0, 无负环 
	for (int i = 0; i < m; i++) {
		scanf("%d%d%d", &u, &v, &w);
		G[u][v] = G[v][u] = w;
	}
	floyd();
    int mindist = inf, animal = 0;
    //检查每一行里面最大的数字 
	for (int i = 1; i <= n; i++) {
		int max = -1;
		for (int j = 1; j <= n; j++) 
			if (G[i][j] > max) 
				max = G[i][j];
		if (max == inf) {
			printf("0\n");
			return 0;
		}
		if (mindist > max) {
			mindist = max; //找到最长距离最小的动物
			animal = i; 
		}
	} 
	printf("%d %d\n", len[0].index, len[0].maxlen);
	return 0;
} 
