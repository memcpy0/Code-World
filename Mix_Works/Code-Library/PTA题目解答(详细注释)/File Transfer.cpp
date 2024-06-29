#include <bits/stdc++.h>
const int maxn = 10010;
typedef int ElemType; /* 默认元素可以用非负整数表示 */
typedef int SetName;  /* 默认用根结点的下标作为集合名称 */
typedef ElemType DisjSet[maxn]; //不相交集 
DisjSet conn;
int N;

void Initialize(int N) {
	for (int i = 0; i < N; i++) conn[i] = -1; 
}

SetName Find(ElemType X) { //递归查找和路径压缩 
	if (conn[X] < 0) {
		return X;
	} else {
	    return conn[X] = Find(conn[X]); //追溯父结点的位置		
	}
}

void SetUnion(SetName root1, SetName root2) { //按秩(大小)归并 
	if (conn[root1] < conn[root2]) { //1集合规模大于2集合 
		conn[root1] += conn[root2];
		conn[root2] = root1;
	} else { //2集合规模大于1集合 
		conn[root2] += conn[root1];
		conn[root1] = root2;
	}
}

void InputConnection() {
	ElemType s1, s2;
	scanf("%d%d\n", &s1, &s2); //编号从1到N, 因此减1
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
