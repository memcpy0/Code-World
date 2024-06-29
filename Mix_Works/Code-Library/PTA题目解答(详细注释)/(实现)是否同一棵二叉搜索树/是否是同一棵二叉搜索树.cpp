#include <bits/stdc++.h>
typedef int ElemType;
typedef struct node {
	ElemType data;
	node *left, *right;
} *BST; //二叉搜索树 

node* newNode(ElemType v) {
	node *Node = new node;
	Node->data = v;
	Node->left = Node->right = NULL;
	return Node;
}

/* 指针引用, 无返回值插入版本 */
void Insert(BST &root, ElemType X) {
	if (root == NULL) { //空树, 插入位置 
		root = newNode(X);
		return; 
	} else {
		if (X > root->data) Insert(root->right, X);
		else if (X < root->data) Insert(root->left, X);
		/* else X is in the tree, do nothing */ 
	} 
}

BST Create(int n) {
	node *root = NULL; //新建根结点root
	ElemType data;
	for (int i = 0; i < n; i++) {
		scanf("%d", &data); //插入函数不一定要返回值 
		Insert(root, data);
	} 
	return root;
}

BST MakeEmpty(BST root) {
	if (root) {
		MakeEmpty(root->left);
		MakeEmpty(root->right);
		free(root); //后序清空 
	} 
	return NULL;  //返回空树 
}

bool isEqualTree(BST t1, BST t2) {
	bool flag;
	if (t1 == NULL && t2 == NULL) flag = true;
	else if (t1 == NULL || t2 == NULL) flag = false;
	else { //compare the value of roots and subtrees
		if (t1->data == t2->data && isEqualTree(t1->left, t2->left) && 
		isEqualTree(t1->right, t2->right)) 
		   flag = true;
        else flag = false;
	}   
	return flag;
} 

/* 方法: 每个序列都建树, 将之分别与标准二叉搜索树比较 */
int main() {
	int N, L;
	BST templateTree, compareTree;
	while (scanf("%d", &N) != EOF && N != 0) {
		scanf("%d", &L);
		/* build template tree */
		templateTree = Create(N);
		/* build compare tree and print the result */ 
		while (L--) {
			compareTree = Create(N);
			/* do the compare */
			if (isEqualTree(templateTree, compareTree)) 
				printf("Yes\n");
            else printf("No\n");
		} 
		templateTree = MakeEmpty(templateTree);
		compareTree = MakeEmpty(compareTree); //清空 
	}
	return 0;
} 

/* C语言版本的一些函数 */
//node* newNode(ElemType v) {
//	node *Node = (node *)malloc(sizeof(struct node));
//	Node->data = v;
//	Node->left = Node->right = NULL;
//	return Node;
//}
/* 只用指针, 有返回值的插入版本 */
//BST Insert(BST root, ElementType X) {
//	if(root == NULL){
//		root = newNode(X);
//	} else {
//		if(item > root->Data)
//			root->Right = Insert(root->Right, item);
//		else if(item < root->Data)
//			root->Left = Insert(root->Left, item);
//	} /* root指向每棵子树的根结点, 根又会发生变化, 因此在最后需要返回 */
//	return root; /* 一定要返回! */
//}
//BST Create(int n) {
//	node *root = NULL; //新建根结点root
//	ElemType data;
//	for (int i = 0; i < n; i++) {
//		scanf("%d", &data); //插入函数不一定要返回值 
//		root = Insert(root, data);
//	} 
//	return root;
//}

