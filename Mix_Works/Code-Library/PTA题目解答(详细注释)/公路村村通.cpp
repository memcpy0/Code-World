#include <bits/stdc++.h> 
/* Kruskal + �ڽӱ� + ����S */ 
using namespace std;
struct node {
	int v, w; //vΪ�ߵ�Ŀ��, wΪ��Ȩ 
	node(int _v, int _w): v(_v), w(_w) { } 
};
const int inf = 1000000000, maxv = 1010; //1000�������� 
int n, m, d[maxv]; //n������m����; �����뼯��S����̾��� 
bool vis[maxv] = {false}; //�������, ��Ƕ�������˼���S
vector<node> Adj[maxv]; //�ڽӱ� 

int prim() { //Ĭ��1��Ϊ��ʼ��, ����MST�ı�Ȩ�� 
	fill(d, d + maxv, inf); //������d���鸳ֵΪinf
	d[1] = 0; //ֻ��1�Ž��ĵ�����S�ľ���Ϊ0, ����ȫ��Ϊinf 
	int ans = 0; //MST�ı�Ȩ��
	for (int i = 0; i < n; i++) { //ѭ��n��
		int u = -1, min = inf; //uʹ��d[u]��С, min�����С��d[u]
		for (int j = 1; j <= n; j++) {
			if (vis[j] == false && d[j] < min) {
				u = j;
				min = d[j];
			}
		} 
		//�Ҳ���С��inf��d[u], ʣ�µĶ���ͼ���S����ͨ
		if (u == -1) return -1;
		vis[u] = true;
		ans += d[u]; //���뼯��S������С�ı߼�����С������ 
	    for (int i = 0; i < Adj[u].size(); i++) { //u�ɴ�v
	    	int v = Adj[u][i].v; //ͨ���ڽӱ�ֱ�ӻ��u�ܴﵽ�Ķ���v 
	    	//vδ����, ��uΪ�н�����ʹ��v�뼯��S����
			if (vis[v] == false && Adj[u][i].w < d[v]) {
				d[v] = Adj[u][i].w; //��G[u][v]��ֵ��d[v] 
			} 
		}
	}
	return ans; //����MST�ı�Ȩ֮�� 
} 

int main() {
	scanf("%d%d", &n, &m);
	int u, v, w; 
	for (int i = 1; i <= m; i++)  {//��ı�Ŵ�1��ʼ  
		scanf("%d%d%d", &u, &v, &w);
		Adj[u].push_back(node(v, w)); //����ͼ 
		Adj[v].push_back(node(u, w));  
	}
	printf("%d\n", prim()); 
	return 0;
}
