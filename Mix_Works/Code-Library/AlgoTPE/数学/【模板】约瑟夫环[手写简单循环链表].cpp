#include <iostream>
using namespace std;
//Լɪ�� ѭ������ⷨ
struct node {
	int v;
	node *next;
	node() {}
	node(int d) : v(d), next(NULL) {}
}; 
node *head;
void createdLinkedList(int n) {
	head = new node(1); //û������ͷ��� 
	node *rear = head; //β�巨 
	for (int i = 2; i <= n; ++i) {
		node *temp = new node(i);
		rear->next = temp;
		rear = temp;
	}
	//ͷβ����
	rear->next = head; 
}

int solve(int n, int m) { //��n����, ÿ�ֵ���m���˾�ɾ���ý�� 
 //���ÿ��һ���˾�ɾ��һ��,��ô���һ����Ϊ��; ���ֻ��һ���˵Ļ�Ҳ�� 
	if (m == 1 || n < 2)  
		return n;
	createdLinkedList(n);
	int kth = 1;
	node *cur = head, *pre = NULL; //ǰ�����
	while (cur != pre) { //���ֻʣһ�����, cur==pre 
		//ɾ�����
		if (kth == m) {
			kth = 1;
			pre->next = cur->next;
			cur = pre->next; //cur = cur->next; 
		} else {
			++kth;
			pre = cur;
			cur = cur->next;
		}
	}
	return cur->v; 
} 
 
int main() {
	int n, m;
	cin >> n >> m;
	cout << solve(n, m);
	return 0;
}
