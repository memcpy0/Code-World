#include <bits/stdc++.h>
using namespace std; 
/* dijkstra + �ڶ�������· */
const int inf = 1000000000, maxv = 510;
int n, m, s, D; //���и��� ��·���� �����ر�� Ŀ�ĵر��
int G[maxv][maxv], cost[maxv][maxv]; //0 N-1��� 
int c[maxv], d[maxv]; //���־�Ϊ�����Ҳ�����500 
bool vis[maxv] = {false}; 

void Dijkstra(int s) {
	fill(d, d + maxv, inf);  //������d�����ʼ��Ϊinf 
	d[s] = 0; //�������Լ��ľ�����0 
	fill(c, c + maxv, inf);  //������cost�����ʼ����Ϊinf
	c[s] = 0; //��㵽���Լ��Ļ���Ϊ0 
	for (int i = 0; i < n; i++) {
		int u = -1, min = inf;
		for (int j = 0; j < n; j++) { //�ҵ�δ���ʶ�����d[v]��С�� 
			if (vis[j] == false && d[j] < min) {
				u = j;
				min = d[j]; 
			}
		}
		if (u == -1) return; //�����Ķ��������S����ͨ 
		vis[u] = true;
		for (int v = 0; v < n; v++) {
			if (vis[v] == false && G[u][v] != inf) {
				if (d[u] + G[u][v] < d[v]) { //���·���ͻ��� 
					d[v] = d[u] + G[u][v];
					c[v] = c[u] + cost[u][v];
				} else if (d[u] + G[u][v] == d[v] && c[u] + cost[u][v] < c[v]) {
					c[v] = c[u] + cost[u][v]; //��̾�����ͬʱ���ܷ�ʹc[v]���� 
				} 
			} 
		}
	}
}

int main() {
	fill(G[0], G[0] + maxv * maxv, inf); //��ʼ��G 
	fill(cost[0], cost[0] + maxv * maxv, inf); //��ʼ��cost 
	scanf("%d%d%d%d", &n, &m, &s, &D);
	int c1, c2, p, spend; //����1������2�����ٹ�·���ȡ��շѶ�
	for (int i = 0; i < m; i++) { //����1������2�����ٹ�·���ȡ��շѶ�
		scanf("%d%d%d%d", &c1, &c2, &p, &spend);
		G[c1][c2] = G[c2][c1] = p; //��¼��·���� 
		cost[c1][c2] = cost[c2][c1] = spend; //��¼�շѶ� 
	} 
	Dijkstra(s);
    //���·���ĳ��Ⱥ��շ��ܶ�
    printf("%d %d\n", d[D], c[D]);
	return 0;  
} 
