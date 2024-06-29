#include <iostream>
using namespace std;
const int maxn = 100;
struct node {
	int v;
	int next;
} L[maxn];
int n, m, head = 0, idx = 0; //idx分配地址下标 
void newNode(int v) {
	L[idx].v = v;
	if (idx != 0) L[idx - 1].next = idx; //前面的指向新的一个 
	L[idx++].next = head; //形成循环 
} //调试用 
//void print() { for (int i = 0; i < n; ++i) cout << L[i].v << ", " << L[i].next << endl; }

int main() {
	cin >> n >> m; 
	for (int i = 0; i < n; ++i) 
		newNode(i + 1);
	//print(); cout << endl; 
	int kth = 1, cur = head, pre = n - 1; //第一个结点和最后一个结点 
	while (cur != pre) { //循环链表的结束条件, 只剩下一个结点时都指向自己 
		if (kth++ % m == 0) {  //用kth计数 
			L[pre].next = L[cur].next; //删除掉这个结点 
			cur = L[cur].next; //cur指向下一个结点 
			//print(); cout << endl; 
		} else {
			pre = cur;
			cur = L[cur].next;
		}
	}
	cout << L[cur].v;
	return 0;
}
