#include <bits/stdc++.h>
using namespace std;

/*
A  B   a1 = 6
A  C   a2 = 4
A  D   a3 = 5
B  E   a4 = 1
C  E   a5 = 1
D  H   a6 = 2
E  F   a7 = 9
E  G   a8 = 7
H  I   a9 = 4
F  I   a10= 2
G  I   a11= 4
-------------
9 11
0  1  6
0  2  4
0  3  5
1  4  1
2  4  1
3  7  2
4  5  9
4  6  7
7  8  4
5  8  2
6  8  4
*/
const int MAXN = 101;
struct node {
	int to, w;
};
int M, N; 		//M������0~M-1,N���� 
vector<node> G[MAXN];
int *indegrees, *etv, *ltv;  //�¼������緢��ʱ��,������ʱ�� 
stack<int> topOrder;  //�洢������������� 

bool TopologicalSort() {
	stack<int> vex; 
	for (int i = 0; i < M; ++i) if (!indegrees[i]) vex.push(i); //ѹ������ȵĶ��� 
	while (!vex.empty()) {
		int u = vex.top(); vex.pop();
		topOrder.push(u);	//��u������������  
		for (int i = 0; i < G[u].size(); ++i) {
			int v = G[u][i].to, w = G[u][i].w;  //u��i�ź��������Ϊv 
			if (!(--indegrees[v])) vex.push(v);
			//��etv[u]����u�����к�̽��v 
			if (etv[u] + w > etv[v]) etv[v] = etv[u] + w;
		}
	}
	if (topOrder.size() != M) return false;
	return true;
}

int CriticalPath() { //���عؼ�·���ĳ���  
	etv = (int*)malloc(M * sizeof(int));
	ltv = (int*)malloc(M * sizeof(int));
	//memset(etv, 0, sizeof(etv));
	//memset(ltv, 0, sizeof(ltv));
	fill(etv, etv + M, 0);
	if (TopologicalSort() == false) {
		cout << "����Ĳ��������޻�ͼ!" << endl; 
		return -1; //��DAG,����-1
	}
	//int begin = topOrder[0], end = topOrder[top]; //�õ�Դ��ͻ��
	int maxLength = 0; //etv��������ֵ���ǹؼ�·���ĳ���,Ҳ�ǻ�������/��ٿ�ʼʱ��
	for (int i = 0; i < M; ++i) if (etv[i] > maxLength) maxLength = etv[i]; 
	fill(ltv, ltv + M, maxLength);
	//��topOrder��ջ˳����Ϊ����������,��ltv���� 
	while (!topOrder.empty()) {
		int u = topOrder.top(); topOrder.pop();
		for (int i = 0; i < G[u].size(); ++i) {
			int v = G[u][i].to, w = G[u][i].w; //u�ĺ������v
			//��u�����к������v��ltvֵ����ltv[u] 
			if (ltv[v] - w < ltv[u]) ltv[u] = ltv[v] - w; 
		}
	}
	//�����ڽӱ�����б�,���������翪ʼʱ�����ٿ�ʼʱ��
	int ete, lte;
	for (int u = 0; u < M; ++u) {
		for (int i = 0; i < G[u].size(); ++i) {
			int v = G[u][i].to, w = G[u][i].w;
			ete = etv[u], lte = ltv[v] - w;
			if (ete == lte) cout << char(u + 'A') << " -> " << char(v + 'A') << ", w: " << w << endl;
		}
	}
	return maxLength;
}

int main() {
	cin >> M >> N;  //���붥�����ͱ��� 
	indegrees = (int*)malloc(M * sizeof(int));
	fill(indegrees, indegrees + M, 0);
	for (int i = 0; i < N; ++i) {
		int u, v, w;	//�����<u,v>,ȨֵΪw 
		cin >> u >> v >> w;
		++indegrees[v]; //v�����+1 
		G[u].push_back(node{v, w});
	}
	cout << CriticalPath() << endl;  
	return 0;
}
//��̬�滮��ؼ�·��
int *dp, *choice;
dp = new int[numVertexes];
choice = new int[numVertexes];
fill(choice, choice + numVertexes, -1);
void dynamicProgramming() {
	fill(dp, dp + numVertexes, 0);
	int maxLength = 0, v = -1;
	for (int i = 0; i < numVertexes; ++i) {
		if (dp[i] == 0) dp[i] = DP(i);
		if (dp[i] > maxLength) { v = i, maxLength = dp[i]; }
	}
	printf("%d\n", maxLength);
	printPath(v);
}

int DP(int i) {
	if (dp[i] > 0) return dp[i];
	for (int j = 0; j < numVertexes; ++j) {
		if (G[i][j] != INFINITY) {
			int temp = DP(j) + G[i][j];
			if (temp > dp[i]) {
				dp[i] = temp;
				choice[i] = j;
			}
		}
	}
	return dp[i];
}
void printPath(int i) {
	printf("%c", 'A' + i);
	while (choice[i] != -1) {
		i = choice[i];
		printf("->%c", 'A' + i);
	}
}