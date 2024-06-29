#include <cstdio>
#include <queue>
#include <vector> 
using namespace std;
/* �ڽӾ���汾 */
const int maxv = 50; 
const int inf = 1000;
int nv, ne, G[maxv][maxv];
bool vis[maxv] = {false}, inq[maxv] = {false};

void CreateGraph() {
	int u, v;
	for (int i = 0; i < nv; i++) 
		for (int j = 0; j < nv; j++) 
			G[i][j] = inf;
	for (int i = 0; i < ne; i++) {
		scanf("%d%d", &u, &v); //0��N-1��� 
		G[u][v] = 1; //����ͼ 
		G[v][u] = 1; 
	}
}

vector<int> comp;
void DFS(int u) {
	vis[u] = true;
	comp.push_back(u);
	for (int v = 0; v < nv; v++) {
		if (vis[v] == false && G[u][v] != inf) {
			DFS(v);
		}
	} 
}
void DFSTrave() {
	for (int i = 0; i < nv; i++) {
		if (vis[i] == false) {
			comp.clear(); 
			DFS(i);
			printf("{");
			for (int j = 0; j < comp.size(); j++) { 
				printf(" %d", comp[j]);
			} 
			printf(" }\n");
		}
	}
}

void BFS(int u) {
	queue<int> q;
	q.push(u);
	inq[u] = true; //u�Ѿ��������
	while (!q.empty()) {
		int u = q.front(); q.pop();
	    comp.push_back(u);
		for (int v = 0; v < nv; v++) { //���u���ڽӵ�vδ����������� 
			if (inq[v] == false && G[u][v] != inf) {
				q.push(v); 
				inq[v] = true; //���v�Ѿ���������� 
			}
		}
	} 
}
void BFSTrave() {
	for (int u = 0; u < nv; u++) {
		if (inq[u] == false) { //uδ����������� 
			comp.clear(); 
			BFS(u); //����u�ڵ���ͨ�� 
			printf("{");
			for (int j = 0; j < comp.size(); j++) { 
				printf(" %d", comp[j]);
			} 
			printf(" }\n");
		}
	} 
}

int main() {
	scanf("%d%d", &nv, &ne);
	CreateGraph();
	DFSTrave();
	BFSTrave();
	return 0;
}
