#include <bits/stdc++.h>
using namespace std;

int main() {
	int N, K, data[10001], queue[10001], idx[10001];
	int front = 1, rear = 0; //���׶�β
	cin >> N >> K; //N��С������, K��С������
	//1��ʼ 
	for (int i = 1; i <= N; ++i) cin >> data[i];
	for (int i = 1; i <= N; ++i) { //ԽС�������ȼ�Խ�� 
		while (front <= rear && data[i] < queue[rear]) --rear; //�ǿ�ʱ, ��Ԫ��<��βԪ��, �޳���βԪ��
		queue[++rear] = data[i];   //ǰ���һ���Ƚ��� 
		idx[rear] = i;
		if (idx[front] + K <= i) ++front; //����Ԫ�س�������, ����
		if (i >= K) cout << queue[front] << ' ';
	}
	return 0;
}
