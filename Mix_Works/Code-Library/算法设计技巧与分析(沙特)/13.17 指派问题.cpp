/*
13.17 写一个回溯算法求解如下定义的指派问题：n个雇员被指派做n件工作，使得指派第 i 个人做第 j 件工作
的耗费是 c(i,j), 找出一种指派使得总耗费最少。假定耗费是非负的，即对于 1 <= i,j <= n, c(i, j) >=0 
*/ 

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
const int MAXN = 30, N = 10;
int cost[MAXN][MAXN]; //耗费矩阵，第i个人做第j项工作的耗费
int minCost = 0x3fffffff; //最小耗费
int assign[MAXN]; //answer[j] = i表示第j项工作已经被指派给第i个人 
int n; //n个雇员，n件工作 
int answer[MAXN]; //分配工作的方案 

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
		if (cs >= minCost) continue; //最优性剪枝 
		assign[i] = employee; //指派第i件工作给employee 
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
	dfs(0, 0); //先从第0个人开始，耗费先为0 
	for (int i = 0; i < n; ++i) 
		printf("%d ", answer[i]);
	printf("\n%d", minCost); 
	return 0;	
}
