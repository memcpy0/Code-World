#include <bits/stdc++.h>
using namespace std;

int main() {
	int N, K, data[10001], queue[10001], idx[10001];
	int front = 1, rear = 0; //队首队尾
	cin >> N >> K; //N大小的数组, K大小的区间
	//1开始 
	for (int i = 1; i <= N; ++i) cin >> data[i];
	for (int i = 1; i <= N; ++i) { //越小数据优先级越高 
		while (front <= rear && data[i] < queue[rear]) --rear; //非空时, 新元素<队尾元素, 剔除队尾元素
		queue[++rear] = data[i];   //前面的一定先进队 
		idx[rear] = i;
		if (idx[front] + K <= i) ++front; //队首元素超出区间, 出队
		if (i >= K) cout << queue[front] << ' ';
	}
	return 0;
}
