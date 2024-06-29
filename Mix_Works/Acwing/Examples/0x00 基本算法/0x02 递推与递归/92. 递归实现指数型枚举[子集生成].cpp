/*
从 1∼n 这 n个整数中随机选取任意多个，输出所有可能的选择方案。
输入格式

输入一个整数 n。
输出格式

每行输出一种方案。

同一行内的数必须升序排列，相邻两个数用恰好 1个空格隔开。
对于没有选任何数的方案，输出空行。

本题有自定义校验器（SPJ），各行（不同方案）之间的顺序任意。

数据范围
1≤n≤15

输入样例：
3

输出样例：
3
2
2 3
1
1 3
1 2
1 2 3
*/
//迭代
//不考虑生成子集数组：
#include <bits/stdc++.h>
using namespace std;
int n;
void dfs(int n) {
	int tot = 1 << n;
	for (int i = 0; i < tot; ++i) {
		for (int j = 0; j < 32; ++j)
			if ((i >> j) & 1)
				printf("%d ", j + 1);
		printf("\n");
	}
}
int main() {
	scanf("%d", &n);
	dfs(n);	
	return 0;	
}
//普通递归
//考虑生成子集数组：
#include <bits/stdc++.h>
using namespace std;
int n, arr[35], subset[35];
void dfs(int cur, int end, int len) {
	if (cur >= n) {
		for (int i = 0; i < len; ++i)
			printf("%d ", subset[i]);
		printf("\n");
		return;
	}
	dfs(cur + 1, end, len);
	subset[len] = arr[cur];
	dfs(cur + 1, end, len + 1);
}
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) arr[i] = i + 1;
	dfs(0, n, 0);	
	return 0;	
}
//状态压缩递归
//不考虑生成子集数组：
#include <bits/stdc++.h>
using namespace std;
int n;
void dfs(int cur, int state) { // cur是当前枚举到的数,state二进制记录哪些数被选
    if (cur >= n) {
        for (int i = 0; i < n; ++i)
            if ((state >> i) & 1)
                printf("%d ", i + 1);
            printf("\n");
        return;
    }
    dfs (cur + 1, state);  // 不用cur这个数
    dfs (cur + 1, state | (1 << cur)); // 用cur这个数
}
int main() {
	scanf("%d", &n);
	dfs(0, 0);
	return 0;	
}