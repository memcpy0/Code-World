#include <iostream>
using namespace std;
const int maxn = 100;
struct node {
	int v;
	int next;
} L[maxn];
int n, m, head = 0, idx = 0; //idx�����ַ�±� 
void newNode(int v) {
	L[idx].v = v;
	if (idx != 0) L[idx - 1].next = idx; //ǰ���ָ���µ�һ�� 
	L[idx++].next = head; //�γ�ѭ�� 
} //������ 
//void print() { for (int i = 0; i < n; ++i) cout << L[i].v << ", " << L[i].next << endl; }

int main() {
	cin >> n >> m; 
	for (int i = 0; i < n; ++i) 
		newNode(i + 1);
	//print(); cout << endl; 
	int kth = 1, cur = head, pre = n - 1; //��һ���������һ����� 
	while (cur != pre) { //ѭ������Ľ�������, ֻʣ��һ�����ʱ��ָ���Լ� 
		if (kth++ % m == 0) {  //��kth���� 
			L[pre].next = L[cur].next; //ɾ���������� 
			cur = L[cur].next; //curָ����һ����� 
			//print(); cout << endl; 
		} else {
			pre = cur;
			cur = L[cur].next;
		}
	}
	cout << L[cur].v;
	return 0;
}
