#include <bits/stdc++.h>
using namespace std;
const int maxv = 1010;
struct node {
	int v, layer; //������, ������ 
	node(int _v, int _l): v(_v), layer(_l) { }
};
vector<node> Adj[maxv]; //����1��N���
bool inq[maxv] = {false};
int nums[maxv] = {0}; //nums[i]��ʾ����i���벻����6�Ľ����, �����Լ� 
int n, m;

void BFS(int u) {
    queue<node> q;
    q.push(node(u, 0)); //����ʼ�����
	inq[u] = true; 
	while (!q.empty()) {
		node top = q.front(); q.pop();
		int v = top.v; //���׶���ı�� 
		if (top.layer <= 6) nums[u]++; 
		else break; //���������˳� 
		for (int i = 0; i < Adj[v].size(); i++) {
			node next = Adj[v][i]; //��v�����ܵ���Ķ���next
			next.layer = top.layer + 1;
			if (inq[next.v] == false) {
				q.push(next); //��k��� 
				inq[next.v] = true; //���v�Ѽ������ 
			}
		}
	} 
}

void BFSTrave() {
    fill(nums, nums + maxv, 0); 
	for (int i = 1; i <= n; i++) {
		fill(inq, inq + maxv, 0); //ÿһ�ν�֮��0 
		BFS(i); //��ÿ����������ý����벻����6�Ľ����ռ��������İٷֱ�
		printf("%d: %.2lf%%\n", i, nums[i] * 100.0 / n);
	} 
}


int main() {
	int u, v;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &u, &v);
		Adj[u].push_back(node(v, 0));
		Adj[v].push_back(node(u, 0)); //����ͼ 
	}
	BFSTrave();
	return 0;
}

