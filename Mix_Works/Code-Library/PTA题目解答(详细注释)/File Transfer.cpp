#include <bits/stdc++.h>
const int maxn = 10010;
typedef int ElemType; /* Ĭ��Ԫ�ؿ����÷Ǹ�������ʾ */
typedef int SetName;  /* Ĭ���ø������±���Ϊ�������� */
typedef ElemType DisjSet[maxn]; //���ཻ�� 
DisjSet conn;
int N;

void Initialize(int N) {
	for (int i = 0; i < N; i++) conn[i] = -1; 
}

SetName Find(ElemType X) { //�ݹ���Һ�·��ѹ�� 
	if (conn[X] < 0) {
		return X;
	} else {
	    return conn[X] = Find(conn[X]); //׷�ݸ�����λ��		
	}
}

void SetUnion(SetName root1, SetName root2) { //����(��С)�鲢 
	if (conn[root1] < conn[root2]) { //1���Ϲ�ģ����2���� 
		conn[root1] += conn[root2];
		conn[root2] = root1;
	} else { //2���Ϲ�ģ����1���� 
		conn[root2] += conn[root1];
		conn[root1] = root2;
	}
}

void InputConnection() {
	ElemType s1, s2;
	scanf("%d%d\n", &s1, &s2); //��Ŵ�1��N, ��˼�1
	SetName r1 = Find(s1 - 1), r2 = Find(s2 - 1);
	if (r1 != r2) {
		SetUnion(r1, r2); 
	}	
}

void CheckConnection() {
	ElemType s1, s2;
	scanf("%d%d\n", &s1, &s2);
	SetName r1 = Find(s1 - 1), r2 = Find(s2 - 1);
	if (r1 == r2) printf("yes\n");
	else printf("no\n");
} 

void CheckNetwork() {
	int i, counter = 0;
	for (int i = 0; i < N; i++) {
		if (conn[i] < 0) counter++;
	}
	if (counter == 1) printf("The network is connected.\n");
	else printf("There are %d components.\n", counter);
}

int main() {
	scanf("%d\n", &N);
	Initialize(N);
	char c;
	do {
	   	scanf("%c", &c);
		switch (c) {
			case 'I': InputConnection(); break;
			case 'C': CheckConnection(); break;
			case 'S': CheckNetwork(); break;
		}
	} while (c != 'S');
	return 0;
} 
