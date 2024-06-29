/*
13.17 дһ�������㷨������¶����ָ�����⣺n����Ա��ָ����n��������ʹ��ָ�ɵ� i �������� j ������
�ĺķ��� c(i,j), �ҳ�һ��ָ��ʹ���ܺķ����١��ٶ��ķ��ǷǸ��ģ������� 1 <= i,j <= n, c(i, j) >=0 
*/ 

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
const int MAXN = 30, N = 10;
int cost[MAXN][MAXN]; //�ķѾ��󣬵�i��������j����ĺķ�
int minCost = 0x3fffffff; //��С�ķ�
int assign[MAXN]; //answer[j] = i��ʾ��j����Ѿ���ָ�ɸ���i���� 
int n; //n����Ա��n������ 
int answer[MAXN]; //���乤���ķ��� 

void dfs(int employee, int costSum) {
	if (employee >= n) {
		if (costSum < minCost) {
			minCost = costSum;
			for (int i = 0; i < n; ++i) 
				answer[i] = assign[i]; 
		}
		return;
	} 
	for (int i = 0; i < n; ++i) {
		if (assign[i] != -1) continue;
		int cs = costSum + cost[employee][i];
		if (cs >= minCost) continue; //�����Լ�֦ 
		assign[i] = employee; //ָ�ɵ�i��������employee 
		dfs(employee + 1, cs);
		assign[i] = -1;
	}
}

int main() { 
	cin >> n;
	for (int i = 0; i < n; ++i) 
		for (int j = 0; j < n; ++j)
			cin >> cost[i][j];
	memset(assign, -1, sizeof(assign));
	dfs(0, 0); //�ȴӵ�0���˿�ʼ���ķ���Ϊ0 
	for (int i = 0; i < n; ++i) 
		printf("%d ", answer[i]);
	printf("\n%d", minCost); 
	return 0;	
}
