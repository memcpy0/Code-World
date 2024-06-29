#include <iostream>
using namespace std;
//约瑟夫环 循环链表解法
struct node {
	int v;
	node *next;
	node() {}
	node(int d) : v(d), next(NULL) {}
}; 
node *head;
void createdLinkedList(int n) {
	head = new node(1); //没有虚拟头结点 
	node *rear = head; //尾插法 
	for (int i = 2; i <= n; ++i) {
		node *temp = new node(i);
		rear->next = temp;
		rear = temp;
	}
	//头尾串联
	rear->next = head; 
}

int solve(int n, int m) { //有n个人, 每轮到第m个人就删除该结点 
 //如果每数一个人就删除一个,那么最后一个人为答案; 如果只有一个人的话也是 
	if (m == 1 || n < 2)  
		return n;
	createdLinkedList(n);
	int kth = 1;
	node *cur = head, *pre = NULL; //前驱结点
	while (cur != pre) { //最后只剩一个结点, cur==pre 
		//删除结点
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
