#include <bits/stdc++.h>
typedef int ElemType;
typedef struct node {
	ElemType data;
	node *left, *right;
} *BST; //���������� 

node* newNode(ElemType v) {
	node *Node = new node;
	Node->data = v;
	Node->left = Node->right = NULL;
	return Node;
}

/* ָ������, �޷���ֵ����汾 */
void Insert(BST &root, ElemType X) {
	if (root == NULL) { //����, ����λ�� 
		root = newNode(X);
		return; 
	} else {
		if (X > root->data) Insert(root->right, X);
		else if (X < root->data) Insert(root->left, X);
		/* else X is in the tree, do nothing */ 
	} 
}

BST Create(int n) {
	node *root = NULL; //�½������root
	ElemType data;
	for (int i = 0; i < n; i++) {
		scanf("%d", &data); //���뺯����һ��Ҫ����ֵ 
		Insert(root, data);
	} 
	return root;
}

BST MakeEmpty(BST root) {
	if (root) {
		MakeEmpty(root->left);
		MakeEmpty(root->right);
		free(root); //������� 
	} 
	return NULL;  //���ؿ��� 
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

/* ����: ÿ�����ж�����, ��֮�ֱ����׼�����������Ƚ� */
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
		compareTree = MakeEmpty(compareTree); //��� 
	}
	return 0;
} 

/* C���԰汾��һЩ���� */
//node* newNode(ElemType v) {
//	node *Node = (node *)malloc(sizeof(struct node));
//	Node->data = v;
//	Node->left = Node->right = NULL;
//	return Node;
//}
/* ֻ��ָ��, �з���ֵ�Ĳ���汾 */
//BST Insert(BST root, ElementType X) {
//	if(root == NULL){
//		root = newNode(X);
//	} else {
//		if(item > root->Data)
//			root->Right = Insert(root->Right, item);
//		else if(item < root->Data)
//			root->Left = Insert(root->Left, item);
//	} /* rootָ��ÿ�������ĸ����, ���ֻᷢ���仯, ����������Ҫ���� */
//	return root; /* һ��Ҫ����! */
//}
//BST Create(int n) {
//	node *root = NULL; //�½������root
//	ElemType data;
//	for (int i = 0; i < n; i++) {
//		scanf("%d", &data); //���뺯����һ��Ҫ����ֵ 
//		root = Insert(root, data);
//	} 
//	return root;
//}

