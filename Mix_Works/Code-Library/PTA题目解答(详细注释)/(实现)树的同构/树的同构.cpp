#include <bits/stdc++.h>
#define Null -1
const int MaxSize = 10;
typedef int Tree;

typedef struct treeNode {
	char data;
	Tree left, right;
} BTreeNode;
BTreeNode T1[MaxSize], T2[MaxSize];

Tree BuildTree(BTreeNode *T);
bool isMorph(Tree R1, Tree R2) ;
void printTree(BTreeNode *T, Tree R) {
	if (R != Null) {
		printf("%c \n", T[R].data);
		printTree(T, T[R].left);
		printTree(T, T[R].right);
	}
}

Tree BuildTree(BTreeNode *T) { 
	int n, i;  //非链表顺序存储
	char cl, cr;
	Tree root;
	scanf("%d\n", &n); //n非负整数 
	if (n) {
        int checked[n]; 
		for (i = 0; i < n; i++) checked[i] = 0;
		for (i = 0; i < n; i++) {
			scanf("%c %c %c\n", &T[i].data, &cl, &cr);
			if (cl != '-') {
				T[i].left = cl - '0';
				checked[T[i].left] = 1; //表示該结点被指向了 
			} else T[i].left = Null; 
			if (cr != '-') {
				T[i].right = cr - '0';
				checked[T[i].right] = 1; 
			} else T[i].right = Null; 
		}
		for (i = 0; i < n; i++) {//没有被指向的结点就是根结点 
			if (!checked[i]) break;
		}
		root = i;
	} else root = Null;
	return root; //返回根结点的位置 
}

bool isMorph(Tree R1, Tree R2) {
	bool flag;	
	//都空树
	if (R1 == Null && R2 == Null) flag = true; 
	//某一棵为空 
	else if (R1 == Null || R2 == Null) flag = false; 
	else {	
		 /* compare the data of root */
		if (T1[R1].data != T2[R2].data) flag = false;
		else {
		/* compare the left and right subtree */
		    if (isMorph(T1[R1].left, T2[R2].left) && isMorph(T1[R1].right, T2[R2].right))
		   	    flag = true;	
	        else if (isMorph(T1[R1].left, T2[R2].right) && isMorph(T1[R1].right, T2[R2].left))
       	        flag = true;
    		else 
  			    flag = false;
		}
	}
    return flag;
} 

int main() {	
	Tree R1 = BuildTree(T1), R2 = BuildTree(T2); //建二叉树
	if (isMorph(R1, R2)) printf("Yes\n");
	else printf("No\n");
	return 0;
}
