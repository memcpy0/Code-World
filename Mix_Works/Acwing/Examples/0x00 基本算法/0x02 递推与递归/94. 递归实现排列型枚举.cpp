/*
把 1∼n 这 n个整数排成一行后随机打乱顺序，输出所有可能的次序。
输入格式

一个整数 n。
输出格式

按照从小到大的顺序输出所有方案，每行 1个。

首先，同一行相邻两个数用一个空格隔开。
其次，对于两个不同的行，对应下标的数一一比较，字典序较小的排在前面。

数据范围
1≤n≤9

输入样例：
3

输出样例：
1 2 3
1 3 2
2 1 3
2 3 1
3 1 2
3 2 1
*/
#include <bits/stdc++.h>
using namespace std;
int n, state;
vector<int> chosen;
void permutations(int begin, int end) { // [begin, end]
	if (begin > end) {
		for (int i = 0, n = chosen.size(); i < n; ++i)
			printf(" %d" + !i, chosen[i]);
		printf("\n");
		return;
	} 
	for (int i = 1; i <= end; ++i) {
		if ((state >> (i - 1)) & 1) continue;
		state |= (1 << (i - 1));
		chosen.push_back(i);
		permutations(begin + 1, end);
		chosen.pop_back();
		state &= ~(1 << (i - 1));
	}
}

int main() {
	scanf("%d", &n);
	permutations(1, n);
	return 0;
}