#include <bits/stdc++.h>
using namespace std; 
#define Null -1
typedef int Tree;
const int maxn = 10;

typedef struct BTreeNode {
    int data;
    Tree left, right; 
} BTreeNode;
BTreeNode T[maxn];

Tree CreateTree(BTreeNode *T) {
	Tree root;
	int n;
	scanf("%d\n", &n);
	if (n) {
		char cl, cr;
		int checked[n]; //表示结点是否有父结点 
		for (int i = 0; i < n; i++) checked[i] = 0;
		for (int i = 0; i < n; i++) {
			T[i].data = i; //树结点的编号0到N-1 
			scanf("%c %c\n", &cl, &cr);
			if (cl != '-') {
			   T[i].left = cl - '0';
			   checked[T[i].left] = 1; 
			} else T[i].left = Null;
			if (cr != '-') {
			   T[i].right = cr - '0';
			   checked[T[i].right] = 1; 
			} else T[i].right = Null;
		}
		int j = 0;
		while (j < n && checked[j] != 0) j++;
		root = j; //找到根结点 
	} else root = -1;
	return root; 
}

void ListLeaves(Tree root) { //层序遍历 
	queue<int> q; //队列里面存储树结点编号 
	q.push(root); //根结点入队 
	vector<int> vi; //存储叶子结点编号 
	while (!q.empty()) {
		Tree now = q.front(); //取队首元素
		q.pop(); 
		if (T[now].left == Null && T[now].right == Null) {
			vi.push_back(T[now].data);  
		} 
		if (T[now].left != Null) q.push(T[now].left); //左子树非空 
		if (T[now].right != Null) q.push(T[now].right); //右子树非空 
	}
	for (int i = 0; i < vi.size(); i++) {
		if (i > 0) printf(" ");
		printf("%d", vi[i]);
	}
}

int main() {
	Tree root = CreateTree(T);
	ListLeaves(root);
	return 0;
}
