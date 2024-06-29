/***************************************************************************************
Root of AVL Tree
An AVL tree is a self-balancing binary search tree. In an AVL tree, the heights of the two 
child subtrees of any node differ by at most one; if at any time they differ by more than one,
rebalancing is done to restore this property. Figures 1-4 illustrate the rotation rules.
Now given a sequence of insertions, you are supposed to tell the root of the resulting AVL tree.
Input Specification:
Each input file contains one test case. For each case, the first line contains a positive integer
N (¡Ü20) which is the total number of keys to be inserted. Then N distinct integer keys are given
in the next line. All the numbers in a line are separated by a space.
Output Specification:
For each test case, print the root of the resulting AVL tree in one line
Sample Input 1:
5
88 70 61 96 120
Sample Output 1:
70
Sample Input 2:
7
88 70 61 96 120 90 65
Sample Output 2:
88
*****************************************************************************************/ 
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

typedef int ElementType;
typedef struct AVLTreeNode *AVLTree;
struct AVLTreeNode{
	ElementType Data;
	struct AVLTreeNode *Left;
	struct AVLTreeNode *Right;
	int Height;
};

int GetHeight(AVLTree T);
AVLTree InsertAVLTree(ElementType data, AVLTree T);
AVLTree SingleLeftRotation(AVLTree T);
AVLTree DoubleLeftRightRotation(AVLTree T);
AVLTree SingleRightRotation(AVLTree T);
AVLTree DoubleRightLeftRotation(AVLTree T);
void InOrderTraversalAVLTree(AVLTree T);

int main()
{
	int num, i;
	ElementType data;
	AVLTree T;
	
	T = NULL;
	scanf("%d", &num);
	for(i = 0; i < num; i++){
		scanf("%d", &data);
		T = InsertAVLTree(data, T);
//		{
//			/*print the current tree*/
//			printf("the current AVLTree is:");
//			InOrderTraversalAVLTree(T);
//			printf("\n");
//			printf("the height of AVLTree is:%d\n", GetHeight(T));	
//		}	
	}
	printf("%d\n", T->Data);

	return 0;
}

AVLTree InsertAVLTree(ElementType data, AVLTree T)
{
	if(T){
		if(data < T->Data) {
			T->Left = InsertAVLTree(data, T->Left);
			if(GetHeight(T->Left) - GetHeight(T->Right) == 2){
				/*need adjust the tree*/
				if(data < T->Left->Data)
					/*do the SingleL rotation*/
					T = SingleLeftRotation(T);
				else if(data > T->Left->Data)
					/*do the DoubleLR rotation*/
					T = DoubleLeftRightRotation(T);
			}
		}
		else if(data > T->Data){
			T->Right = InsertAVLTree(data, T->Right);
			if(GetHeight(T->Right) - GetHeight(T->Left) == 2){
				if(data > T->Right->Data)
					T = SingleRightRotation(T);
				else if(data < T->Right->Data)
					T = DoubleRightLeftRotation(T);
			}
		}/*if data == T->Data then do nothing*/ 
	}
	else { /*The tree is empty, so build a node and return*/
		T = (AVLTree)malloc(sizeof(struct AVLTreeNode));
		T->Data = data;
		T->Left = T->Right = NULL;
		T->Height = 0;
	}
	/*refresh the height of AVLTree*/
	T->Height = GetHeight(T);
	
	return T;
}

int max(int a, int b)
{
	return a>b? a: b;
}

int GetHeight(AVLTree T)
{
	int hl, hr, height;
	if(T){
		hl = GetHeight(T->Left);
		hr = GetHeight(T->Right);
		height = max(hl,hr) + 1;
	}
	else 
		height = 0;

	return height;
}

AVLTree SingleLeftRotation(AVLTree T)
{
	AVLTree bTree;
	bTree = T->Left;
	T->Left = bTree->Right;
	bTree->Right = T;
	bTree->Height = GetHeight(bTree);
	T->Height = GetHeight(T);
	
	return bTree;
}

AVLTree DoubleLeftRightRotation(AVLTree T)
{
	T->Left = SingleRightRotation(T->Left);
	return SingleLeftRotation(T);
}

AVLTree SingleRightRotation(AVLTree T)
{
	AVLTree bTree;
	bTree = T->Right;
	T->Right = bTree->Left;
	bTree->Left = T;
	bTree->Height = GetHeight(bTree);
	T->Height = GetHeight(T);
	
	return bTree;
}

AVLTree DoubleRightLeftRotation(AVLTree T)
{
	T->Right = SingleLeftRotation(T->Right);
	return SingleRightRotation(T);
}

void InOrderTraversalAVLTree(AVLTree T)
{
	if(T){
		InOrderTraversalAVLTree(T->Left);
		printf("%d ", T->Data);
		InOrderTraversalAVLTree(T->Right);
	}
}
